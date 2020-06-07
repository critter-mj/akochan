#include "mjai_manager.hpp"

Game_Settings::Game_Settings() {
    player_id = -1;
    chicha = 0;
}

Moves get_masked_log(Moves game_record, const int pid) {
    // 他家のツモ牌を伏せたログを取得
    for (int i = 0; i < game_record.size(); i++) {
        if (game_record[i]["type"].string_value() == "tsumo" &&
            game_record[i]["actor"].int_value() != pid
        ) {
            game_record[i] = json11::Json::object {
                                    { "type", "tsumo" },
                                    { "actor", game_record[i]["actor"].int_value() },
                                    { "pai", "?"}
                                };
        }
    }
    return game_record;
}

void add_start_game(Moves& game_record) {
    assert(game_record.size() == 0);
    game_record.push_back(make_start_game());
}

void prepare_haiyama(std::vector<int>& haiyama) {
    haiyama.clear();
    const int haiyama_size = 136;
    for (int i = 0;i < haiyama_size; i++) {
		haiyama.push_back(i);
	}
	std::random_shuffle(haiyama.begin(), haiyama.end());
	for (int i = 0;i < haiyama_size; i++) {
		haiyama[i] = get_hai38(haiyama[i]);
	}
}

std::array<std::array<int, 13>, 4> get_haipai(const std::vector<int>& haiyama, const int oya) {
    std::array<std::array<int, 13>, 4> haipai_tmp, haipai;
    for (int i = 0;i <13*4; i++) {
		int pid = i % 4;
		haipai_tmp[pid][i/4] = haiyama[i];
	}
    for (int pid = 0; pid < 4; pid++) {
        haipai[pid] = haipai_tmp[(4 + pid - oya) % 4];
    }
    return haipai;
}

void add_first_kyoku(Moves& game_record, std::vector<int>& haiyama, const int chicha, const json11::Json& request) {
    assert(0 < game_record.size());
    const json11::Json& prev = game_record[game_record.size() - 1];
    assert(prev["type"].string_value() == "start_game");
    if (request["haiyama"].is_null()) {
        prepare_haiyama(haiyama);
    }
    const std::array<std::array<int, 13>, 4> haipai = get_haipai(haiyama, chicha);
    if (!prev["first_kyoku"].is_null()) {
        std::array<int, 4> scores;
        assert(!prev["first_kyoku"]["kyoku"].is_null());
        assert(prev["first_kyoku"]["scores"].array_items().size() == 4);
        int first_kyoku = prev["first_kyoku"]["kyoku"].int_value();
        for (int i = 0; i < 4; i++) {
            scores[i] = prev["first_kyoku"]["scores"][i].int_value();
        }
        game_record.push_back(make_start_kyoku(0, first_kyoku, 0, 0, chicha, haipai, scores, haiyama));
    } else {
        game_record.push_back(make_start_kyoku(0, 1, 0, 0, chicha, haipai, {25000, 25000, 25000, 25000}, haiyama));
    }
}

void add_next_kyoku_or_end_game(Moves& game_record, std::vector<int>& haiyama, const json11::Json& request) {
    assert(0 < game_record.size());
    const json11::Json& prev = game_record[game_record.size() - 1];
    assert(prev["type"].string_value() == "hora" || prev["type"].string_value() == "ryukyoku");
    std::pair<int, int> next_bakaze_kyoku = cal_next_bakaze_kyoku(game_record);
    std::array<int, 4> scores;
    for (int pid = 0; pid < 4; pid++) {
        scores[pid] = prev["scores"].array_items()[pid].int_value();
    }
    if (next_bakaze_kyoku.first == -1) {
        game_record.push_back(make_end_kyoku());
        game_record.push_back(make_end_game(scores));
    } else {
        const int next_honba = cal_next_honba(game_record);
        const int next_oya = cal_next_oya(game_record);
        game_record.push_back(make_end_kyoku());
        if (request["haiyama"].is_null()) {
            prepare_haiyama(haiyama);
        }
        const std::array<std::array<int, 13>, 4> haipai = get_haipai(haiyama, next_oya);

        game_record.push_back(make_start_kyoku(
            next_bakaze_kyoku.first, next_bakaze_kyoku.second,
            next_honba, get_kyotaku(game_record), next_oya,
            haipai, scores, haiyama
        ));
    }
}

void add_tsumo(const std::vector<int>& haiyama, Moves& game_record, const int pid) {
    // game_record.push_back(make_tsumo(pid, haiyama[13*4 + count_tsumo_num_all(game_record)]));
    game_record.push_back(make_tsumo(pid, haiyama[13*4 + count_tsumo_num(game_record).first]));
}

void add_rinshan_tsumo(const std::vector<int>& haiyama, Moves& game_record, const int pid) {
    game_record.push_back(make_tsumo(pid, haiyama[haiyama.size() - 1 - count_tsumo_num(game_record).second]));
    // アナログな麻雀とリンシャン牌の取り方が違うが面倒なのでよいことにする。
}

void add_after_ankan(const std::vector<int>& haiyama, Moves& game_record, const int pid) {
    Game_State game_state = get_game_state(game_record);
    add_rinshan_tsumo(haiyama, game_record, pid);
}

void add_ryukyoku_fanpai(Moves& game_record) {
    std::array<bool, 4> tenpai_flag = {false, false, false, false};
    std::array<json11::Json, 4> tehais;
    Game_State game_state = get_game_state(game_record);
    for (int pid = 0; pid < 4; pid++) {
        Hai_Array tehai = game_state.player_state[pid].tehai;
        const Tenpai_Info tenpai_info = cal_tenpai_info(
            game_state.bakaze, game_state.player_state[pid].jikaze, tehai, game_state.player_state[pid].fuuro
        );
        if (tenpai_info.shanten_num() == 0) {
            tenpai_flag[pid] = true;
            tehais[pid] = hai_array_to_json(tehai);
        } else {
            json11::Json::array t;
            for (int hai = 0; hai < 38; hai++) {
                for (int i = 0; i < tehai[hai]; i++) {
                    t.push_back("?");
                }
            }
            tehais[pid] = json11::Json(t);
        }
    }
    std::array<int, 4> deltas = ten_move_ryukyoku(tenpai_flag);
    std::array<int, 4> scores;
    for (int pid = 0; pid < 4; pid++) {
        scores[pid] = game_state.player_state[pid].score + deltas[pid];
    }
    game_record.push_back(make_ryukyoku_fanpai(tenpai_flag, tehais, scores));
}

void add_move_after_dahai(const std::vector<int>& haiyama, Moves& game_record, const std::array<Moves, 4>& candidate_moves) {
    const json11::Json& current_move = game_record[game_record.size() - 1];
    assert(current_move["type"].string_value() == "dahai" || current_move["type"].string_value() == "kakan");
    const int target = current_move["actor"].int_value();
    bool ron_flag = false;
    for (int pid_add = 1; pid_add <= 3; pid_add++) {
        const int actor = (target + pid_add) % 4;
        if (candidate_moves[actor][0]["type"].string_value() == "hora") {
            assert(actor == candidate_moves[actor][0]["actor"].int_value());
            assert(target == candidate_moves[actor][0]["target"].int_value());
            // legal_check;
            const int hai = hai_str_to_int(candidate_moves[actor][0]["pai"].string_value());
            const Game_State game_state = get_game_state(game_record);
            // legal_check;
            Hai_Array tehai = game_state.player_state[actor].tehai;
            const Tenpai_Info tenpai_info = cal_tenpai_info(
                game_state.bakaze, game_state.player_state[actor].jikaze, tehai, game_state.player_state[actor].fuuro
            );
            int agari_id = -1;
            int agari_ten = 0;
            int han_add = 0;

            if (current_move["type"].string_value() == "kakan") {
                han_add++; // 槍槓
            }
            // ハイテイ、
            tehai[hai]++;

            for (int i = 0; i < tenpai_info.agari_vec.size(); i++) {
                if (haikind(hai) == tenpai_info.agari_vec[i].hai &&
                    han_add + tenpai_info.agari_vec[i].han_ron > agari_ten
                ) {
                    agari_ten = han_add + tenpai_info.agari_vec[i].han_ron;
                    agari_id = i;
                }
            }
            const int han = han_add + tenpai_info.agari_vec[agari_id].han_ron;
            std::array<int, 4> ten_move = ten_move_hora(actor, target, han);

            std::array<int, 4> scores;
            for (int pid = 0; pid < 4; pid++) {
                scores[pid] = game_state.player_state[pid].score + ten_move[pid];
            }
            tehai[hai]--;
            game_record.push_back(make_hora(actor, target, hai, tehai, han, scores));
            ron_flag = true;
        }
    }
    if (ron_flag) { return; }
    if (count_tsumo_num_all(game_record) == 70) {
        add_ryukyoku_fanpai(game_record);
        return;
    } else if (current_move["type"].string_value() == "kakan") {
        // 最後の牌はカカンできないため、count_tsumo_num_all(game_record) == 70にはならない。
        add_rinshan_tsumo(haiyama, game_record, target);
        return;
    }
    for (int pid_add = 1; pid_add <= 3; pid_add++) {
        const int actor = (target + pid_add) % 4;
        if (candidate_moves[actor][0]["type"].string_value() == "pon") {
            // legal_check;
            for (const json11::Json& action : candidate_moves[actor]) {
                game_record.push_back(action);
            }
            return;
        } else if (candidate_moves[actor][0]["type"].string_value() == "daiminkan") {
            // legal_check;
            game_record.push_back(candidate_moves[actor][0]);
            return;
        }
    }
    for (int pid_add = 1; pid_add <= 1; pid_add++) {
        const int actor = (target + pid_add) % 4;
        if (candidate_moves[actor][0]["type"].string_value() == "chi") {
            // legal_check;
            for (const json11::Json& action : candidate_moves[actor]) {
                game_record.push_back(action);
            }
            return;
        }
    }
    add_tsumo(haiyama, game_record, (target + 1) % 4);
}

void add_move_after_tsumo(const std::vector<int>& haiyama, Moves& game_record, const std::array<Moves, 4>& candidate_moves) {
    assert(game_record[game_record.size()-1]["type"].string_value() == "tsumo");
    const Moves& moves = candidate_moves[game_record[game_record.size()-1]["actor"].int_value()];
    if (moves[0]["type"].string_value() == "hora") {
        const int actor = moves[0]["actor"].int_value();
        const int hai = hai_str_to_int(moves[0]["pai"].string_value());
        const Game_State game_state = get_game_state(game_record);
        // legal_check;
        Hai_Array tehai = game_state.player_state[actor].tehai;

        tehai[hai]--;
        const Tenpai_Info tenpai_info = cal_tenpai_info(
            game_state.bakaze, game_state.player_state[actor].jikaze, tehai, game_state.player_state[actor].fuuro
        );
        int agari_id = -1;
        int agari_ten = 0;
        int han_add = 0;

        // ハイテイ、
        tehai[hai]++;

        for (int i = 0; i < tenpai_info.agari_vec.size(); i++) {
            if (haikind(hai) == tenpai_info.agari_vec[i].hai &&
                han_add + tenpai_info.agari_vec[i].han_tsumo > agari_ten
            ) {
                agari_ten = han_add + tenpai_info.agari_vec[i].han_tsumo;
                agari_id = i;
            }
        }
        const int han = han_add + tenpai_info.agari_vec[agari_id].han_tsumo;
        std::array<int, 4> ten_move = ten_move_hora(actor, actor, han);
        std::array<int, 4> scores;
        for (int pid = 0; pid < 4; pid++) {
            scores[pid] = game_state.player_state[pid].score + ten_move[pid];
        }
        tehai[hai]--;
        game_record.push_back(make_hora(actor, actor, hai, tehai, han, scores));
    } else if (moves[0]["type"].string_value() == "ryukyoku" && moves[0]["reason"].string_value() == "kyushukyuhai") {
        const int actor = moves[0]["actor"].int_value();
        const Game_State game_state = get_game_state(game_record);
        std::array<int, 4> scores;
        for (int pid = 0; pid < 4; pid++) {
            scores[pid] = game_state.player_state[pid].score;
        }
        game_record.push_back(make_kyushukyuhai(actor, game_state.player_state[actor].tehai, scores));
    } else {
        for (int i = 0; i < moves.size(); i++) {
            game_record.push_back(moves[i]);
        }
    }
}

void add_move_after_tsumo_or_dahai(const std::vector<int>& haiyama, Moves& game_record, const std::array<Moves, 4>& candidate_moves) {
    const json11::Json& current_move = game_record[game_record.size() - 1];
    if (current_move["type"] == "tsumo") {
        add_move_after_tsumo(haiyama, game_record, candidate_moves);
    } else if (current_move["type"] == "dahai" || current_move["type"] == "kakan") {
        add_move_after_dahai(haiyama, game_record, candidate_moves);
    } else {
        assert(false);
    }
}

Moves ai_assign(const Moves& game_record, const int player_id) {
    return ai(game_record, player_id, false);
    //return ai_interface(game_record, player_id);
    //if (player_id == 0 || player_id == 2) {
    //    return ai_interface(game_record, player_id);
    //} else {
    //    return ai_201808(game_record, player_id);
    //}
}

std::array<Moves, 4> require_moves_after_dahai(const Moves& game_record, const int player_id, const json11::Json& request) {
    std::array<std::vector<Moves>, 4> all_legal_moves = get_all_legal_moves(game_record);
    std::array<Moves, 4> result;
    for (int pid = 0; pid < 4; pid++) {
        if (all_legal_moves[pid].size() > 0) {
            if (pid != player_id) {
                //result[pid] = ai_interface(game_record, pid);
                //result[pid] = ai_tsumogiri(game_record, pid);
                result[pid] = ai_assign(game_record, pid);
            } else {
                result[pid].push_back(request);
            }
        } else {
            Moves moves;
            moves.push_back(make_none(pid));
            result[pid] = moves;
        }
    }
    return result;
}

std::array<Moves, 4> require_moves_after_tsumo(const Moves& game_record, const int player_id, const json11::Json& request) {
    std::array<Moves, 4> result;
    const int actor = game_record[game_record.size()-1]["actor"].int_value();
    if (actor != player_id) {
        //result[actor] = ai_interface(game_record, actor);
        //result[actor] = ai_tsumogiri(game_record, actor);
        result[actor] = ai_assign(game_record, actor);
    } else {
        result[actor].push_back(request);
    }
    return result;
}

std::array<Moves, 4> require_moves_after_tsumo_or_dahai(const Moves& game_record, const int player_id, const json11::Json& request) {
    const json11::Json& current_move = game_record[game_record.size() - 1];
    if (current_move["type"] == "tsumo") {
        return require_moves_after_tsumo(game_record, player_id, request);
    } else if (current_move["type"] == "dahai" || current_move["type"] == "kakan") {
        return require_moves_after_dahai(game_record, player_id, request);
    } else {
        assert_with_out(false, "require_moves_after_tsumo_or_dahai error");
        std::array<Moves, 4> result;
        return result;
    }
}

void proceed_game(std::vector<int>& haiyama, Moves& game_record, const int chicha, const int player_id, const json11::Json& request) {
    if (game_record.size() == 0) {
        add_start_game(game_record);
    } else {
        const json11::Json& current_move = game_record[game_record.size() - 1];
        if (player_id == -1) {
            std::cout << game_record.size() - 1 << " " << current_move.dump() << std::endl;
        }
        if (current_move["type"] == "start_game") {
            add_first_kyoku(game_record, haiyama, chicha, request);
            assert_with_out(haiyama.size() == 136, "proceed_game_error: haiyama.size() != 136");
        } else if (current_move["type"] == "hora" || current_move["type"] == "ryukyoku") {
            add_next_kyoku_or_end_game(game_record, haiyama, request);
            assert_with_out(haiyama.size() == 136, "proceed_game_error: haiyama.size() != 136");
        } else if (current_move["type"] == "start_kyoku") {
            add_tsumo(haiyama, game_record, get_oya(game_record));
        } else if (current_move["type"] == "tsumo" || current_move["type"] == "dahai" || current_move["type"] == "kakan") {
            if (!request["type"].is_null() && request["type"] != "pass" && !is_legal_single_move(game_record, request)) {
                return;
            }
            std::array<Moves, 4> candidate_moves = require_moves_after_tsumo_or_dahai(game_record, player_id, request);
            add_move_after_tsumo_or_dahai(haiyama, game_record, candidate_moves);
        } else if (current_move["type"] == "ankan") {
            add_after_ankan(haiyama, game_record, current_move["actor"].int_value());
        } else if (current_move["type"] == "daiminkan") {
            add_rinshan_tsumo(haiyama, game_record, current_move["actor"].int_value());
        } else if (current_move["type"] == "reach" || current_move["type"] == "chi" || current_move["type"] == "pon") {
            assert_with_out(current_move["actor"] == player_id, "game_server_error");
            if (is_legal_dahai_after_reach_or_fuuro(game_record, request)) {
                game_record.push_back(request);
            }
        }
    }
}

void game_loop(std::vector<int>& haiyama, Moves& game_record, const int chicha, const int player_id) {
    while (game_record.size() == 0 || game_record.back()["type"] != "end_game") {
        proceed_game(haiyama, game_record, chicha, player_id, {});
    }
}

json11::Json game_server(Moves& game_record, const json11::Json& request) {
    json11::Json::object ret;
    size_t prev_size = game_record.size();
    const int chicha = !request["chicha"].is_null() ? request["chicha"].int_value() : -1;
    const int my_pid = 0;
    std::vector<int> haiyama;
    if (!request["haiyama"].is_null()) {
        for (const auto& hai : request["haiyama"].array_items()) {
            haiyama.push_back(hai_str_to_int(hai.string_value()));
        }
    } else {
        for (int i = int(game_record.size()) - 1; 0 <= i; i--) {
            const json11::Json& action_json = game_record[i];
            if (action_json["type"] == "start_kyoku") {
                assert_with_out(!action_json["haiyama"].is_null(), "game_server_error: haiyama is null");
                for (const auto& hai : action_json["haiyama"].array_items()) {
                    haiyama.push_back(hai_str_to_int(hai.string_value()));
                }
                break;
            }
        }
    }
    proceed_game(haiyama, game_record, chicha, my_pid, request);
    assert_with_out(0 < game_record.size(), "game_server_error: game_record.size() == 0");
    json11::Json::array new_moves;
    for (size_t i = prev_size; i < game_record.size(); i++) {
        new_moves.push_back(game_record[i]);
    }
    ret["new_moves"] = new_moves;

    const json11::Json& last_action = game_record.back();
    if (prev_size < game_record.size()) {
        if (last_action["type"] == "reach" || last_action["type"] == "pon" || last_action["type"] == "chi") {
            assert_with_out(last_action["actor"] == my_pid, "game_server_error: last_action is unexpected"); // プレイヤーのリーチ、副露が受理された場合のみの想定。
            ret["msg_type"] = "update_and_dahai";
        } else {
            const std::vector<Moves> legal_moves = get_all_legal_moves(game_record)[my_pid];
            if (0 < legal_moves.size()) {
                ret["msg_type"] = last_action["type"] == "tsumo" ? "update_and_dahai" : "update_and_fuuro";
                ret["legal_moves"] = legal_moves;
            } else {
                ret["msg_type"] = "update";
            }
        }
    } else {
        // クライアントが合法でないmoveを送信した場合のみの想定
        // assert(0 < get_all_legal_moves(game_record)[player_id].size()); 副露した牌を捨てた場合などは、get_all_legal_movesが0になる。
        if (last_action["type"] == "reach" || last_action["type"] == "pon" || last_action["type"] == "chi" ||
            (last_action["type"] == "tsumo" && last_action["actor"] == my_pid)
        ) {
            ret["msg_type"] = "dahai_again";
        } else {
            ret["msg_type"] = "fuuro_again";
        }
    }
    return json11::Json(ret);
}
