#include "make_move.hpp"

json11::Json make_start_game() {
    json11::Json::object move;
    move["type"] = "start_game";
    move["kyoku_first"] = 4;
    move["aka_flag"] = true;
    // name
    return json11::Json(move);
}

json11::Json make_start_game_with_initial_condition(const int kyoku, const std::array<int, 4>& scores) {
    json11::Json::object move, initial_condition;
    move["type"] = "start_game";
    initial_condition["kyoku"] = kyoku;
    initial_condition["scores"] = json11::Json(scores);
    move["first_kyoku"] = initial_condition;
    return json11::Json(move);
}

json11::Json make_start_kyoku(const int bakaze, const int kyoku, const int honba, const int kyotaku, const std::array<int, 4>& scores) {
    json11::Json::object move;
    move["type"] = "start_kyoku";
    move["bakaze"] = kaze_int_to_str(bakaze);
    move["kyoku"] = kyoku;
    move["honba"] = honba;
    move["kyotaku"] = kyotaku;
    move["scores"] = json11::Json(scores);
    return json11::Json(move);
}

json11::Json make_start_kyoku(const int bakaze, const int kyoku, const int honba, const int kyotaku, const int oya,
                              const std::array<std::array<int, 13>, 4>& tehais, const std::array<int, 4>& scores, const std::vector<int>& haiyama)
{
    json11::Json::object move;
    move["type"] = "start_kyoku";
    move["bakaze"] = kaze_int_to_str(bakaze);
    move["kyoku"] = kyoku;
    move["honba"] = honba;
    move["kyotaku"] = kyotaku;
    move["oya"] = oya;

    json11::Json::array haipai_arrays;
	for (int pid = 0; pid < 4; pid++) {
		json11::Json::array haipai_one_player;
		for (int i = 0; i < 13; i++) {
			haipai_one_player.push_back(hai_int_to_str(tehais[pid][i]));
		}
		haipai_arrays.push_back(haipai_one_player);
	}
	move["tehais"] = haipai_arrays;
    move["scores"] = json11::Json(scores);
    json11::Json::array haiyama_arrays;
    for (const int hai : haiyama) {
        haiyama_arrays.push_back(hai_int_to_str(hai));
    }
    move["haiyama"] = json11::Json(haiyama_arrays);
    return json11::Json(move);
}

json11::Json make_none(const int actor) {
    json11::Json::object move;
    move["type"] = "none";
    move["actor"] = actor;
    return json11::Json(move);
}

json11::Json make_tsumo(const int actor, const int hai) {
    json11::Json::object move;
    move["type"] = "tsumo";
    move["actor"] = actor;
    move["pai"] = hai_int_to_str(hai);
    return json11::Json(move);
}

json11::Json make_dahai(const int actor, const int hai, const bool tsumogiri) {
    json11::Json::object move;
    move["type"] = "dahai";
    move["actor"] = actor;
    move["pai"] = hai_int_to_str(hai);
    move["tsumogiri"] = tsumogiri;
    return json11::Json(move);
}

json11::Json make_pon(const int actor, const int target, const int hai, const std::vector<int> consumed) {
    json11::Json::object move;
    move["type"] = "pon";
    move["actor"] = actor;
    move["target"] = target;
    move["pai"] = hai_int_to_str(hai);
    move["consumed"] = json11::Json::array({hai_int_to_str(consumed[0]), hai_int_to_str(consumed[1])});
    return json11::Json(move);
}

json11::Json make_pon_default(const int actor, const int target, const int hai) {
    return make_pon(actor, target, hai, std::vector<int>({haikind(hai), haikind(hai)}));
}

json11::Json make_pon_aka(const int actor, const int target, const int hai) {
    assert(hai % 10 == 5 && hai < 30);
    return make_pon(actor, target, hai, std::vector<int>({haikind(hai), haikind(hai)+5}));
}

json11::Json make_daiminkan(const int actor, const int target, const int hai, const std::vector<int> consumed) {
    json11::Json::object move;
    move["type"] = "daiminkan";
    move["actor"] = actor;
    move["target"] = target;
    move["pai"] = hai_int_to_str(hai);
    move["consumed"] = json11::Json::array({hai_int_to_str(consumed[0]), hai_int_to_str(consumed[1]), hai_int_to_str(consumed[2])});
    return json11::Json(move);
}

json11::Json make_daiminkan_default(const int actor, const int target, const int hai) {
    return make_daiminkan(actor, target, hai, std::vector<int>({haikind(hai), haikind(hai), haikind(hai)}));
}

json11::Json make_daiminkan_aka(const int actor, const int target, const int hai) {
    assert(hai % 10 == 5 && hai < 30);
    return make_daiminkan(actor, target, hai, std::vector<int>({haikind(hai), haikind(hai), haikind(hai) + 5}));
}

json11::Json make_ankan(const int actor, const std::vector<int> consumed) {
    json11::Json::object move;
    move["type"] = "ankan";
    move["actor"] = actor;
    move["consumed"] = json11::Json::array({hai_int_to_str(consumed[0]), hai_int_to_str(consumed[1]), hai_int_to_str(consumed[2]), hai_int_to_str(consumed[3])});
    return json11::Json(move);
}

json11::Json make_ankan_default(const int actor, const int hai) {
    return make_ankan(actor, std::vector<int>({haikind(hai), haikind(hai), haikind(hai), haikind(hai)}));
}

json11::Json make_ankan_aka(const int actor, const int hai) {
    assert(hai % 10 == 5 && hai < 30);
    return make_ankan(actor, std::vector<int>({haikind(hai), haikind(hai), haikind(hai), haikind(hai) + 5}));
}

json11::Json make_kakan(const int actor, const int hai, const std::vector<int> consumed) {
    json11::Json::object move;
    move["type"] = "kakan";
    move["actor"] = actor;
    move["pai"] = hai_int_to_str(hai);
    move["consumed"] = json11::Json::array({hai_int_to_str(consumed[0]), hai_int_to_str(consumed[1]), hai_int_to_str(consumed[2])});
    return json11::Json(move);
}

json11::Json make_kakan_default(const int actor, const int hai) {
    return make_kakan(actor, hai, std::vector<int>({haikind(hai), haikind(hai), haikind(hai)}));
}

json11::Json make_kakan_aka(const int actor, const int hai) {
    assert(hai % 10 == 5 && hai < 30);
    return make_kakan(actor, hai, std::vector<int>({haikind(hai), haikind(hai), haikind(hai) + 5}));
}

json11::Json make_chi(const int actor, const int target, const int hai, const std::vector<int> consumed) {
    json11::Json::object move;
    move["type"] = "chi";
    move["actor"] = actor;
    move["target"] = target;
    move["pai"] = hai_int_to_str(hai);
    move["consumed"] = json11::Json::array({hai_int_to_str(consumed[0]), hai_int_to_str(consumed[1])});
    return json11::Json(move);
}

json11::Json make_chi_low_default(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(haikind(hai) % 10 <= 7);
    return make_chi(actor, target, hai, std::vector<int>({haikind(hai) + 1, haikind(hai) + 2}));
}

json11::Json make_chi_low_aka(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(haikind(hai) % 10 == 3 || haikind(hai) % 10 == 4);
    if (haikind(hai) % 10 == 3) {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) + 1, haikind(hai) + 7}));
    } else {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) + 6, haikind(hai) + 2}));
    }
}

json11::Json make_chi_middle_default(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(2 <= haikind(hai) % 10 && haikind(hai) % 10 <= 8);
    return make_chi(actor, target, hai, std::vector<int>({haikind(hai) - 1, haikind(hai) + 1}));
}

json11::Json make_chi_middle_aka(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(haikind(hai) % 10 == 4 || haikind(hai) % 10 == 6);
    if (haikind(hai) % 10 == 4) {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) - 1, haikind(hai) + 6}));
    } else {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) + 4, haikind(hai) + 1}));
    }
}

json11::Json make_chi_high_default(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(3 <= haikind(hai) % 10);
    return make_chi(actor, target, hai, std::vector<int>({haikind(hai) - 2, haikind(hai) - 1}));
}

json11::Json make_chi_high_aka(const int actor, const int target, const int hai) {
    assert(haikind(hai) < 30);
    assert(haikind(hai) % 10 == 6 || haikind(hai) % 10 == 7);
    if (haikind(hai) % 10 == 6) {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) - 2, haikind(hai) + 4}));
    } else {
        return make_chi(actor, target, hai, std::vector<int>({haikind(hai) + 3, haikind(hai) - 1}));
    }
}

json11::Json make_hora(const int actor, const int target, const int hai_hora) {
    json11::Json::object move;
    move["type"] = "hora";
    move["actor"] = actor;
    move["target"] = target;
    move["pai"] = hai_int_to_str(hai_hora);
    return json11::Json(move);
}

json11::Json make_hora(const int actor, const int target, const int hai_hora, const Hai_Array& tehai, const int han, const std::array<int, 4>& scores, const json11::Json& hora_info) {
    json11::Json::object move;
    move["type"] = "hora";
    move["actor"] = actor;
    move["target"] = target;
    move["pai"] = hai_int_to_str(hai_hora);
    move["hora_tehais"] = hai_array_to_json(tehai);
    move["fan"] = han;
    move["scores"] = json11::Json(scores);
    move["info"] = hora_info;

    return json11::Json(move);
}


json11::Json make_ryukyoku_fanpai(const std::array<bool, 4>& tenpai, const std::array<json11::Json, 4>& tehais, const std::array<int, 4>& scores) {
    json11::Json::object move;
    move["type"] = "ryukyoku";
    move["reason"] = "fanpai";
    move["tenpais"] = json11::Json(tenpai);
    move["tehais"] = json11::Json(tehais);
    move["scores"] = json11::Json(scores);
    return json11::Json(move);
}

json11::Json make_kyushukyuhai(const int actor) {
    json11::Json::object move;
    move["type"] = "ryukyoku";
    move["reason"] = "kyushukyuhai";
    move["actor"] = actor;
    return json11::Json(move);
}

json11::Json make_kyushukyuhai(const int actor, const Hai_Array& tehai, const std::array<int, 4>& scores) {
    json11::Json::object move;
    move["type"] = "ryukyoku";
    move["reason"] = "kyushukyuhai";
    move["actor"] = actor;
    json11::Json::array tehais;
    for (int pid = 0; pid < 4; pid++) {
        if (pid == actor) {
            tehais.push_back(hai_array_to_json(tehai));
        } else {
            json11::Json::array tehai_tmp;
            for (int i = 0; i < 13; i++) {
                tehai_tmp.push_back("?");
            }
            tehais.push_back(tehai_tmp);
        }
    }
    move["tehais"] = json11::Json(tehais);
    move["scores"] = json11::Json(scores);
    return json11::Json(move);
}

json11::Json make_end_kyoku() {
    json11::Json::object move;
    move["type"] = "end_kyoku";
    return json11::Json(move);
}

json11::Json make_end_game(const std::array<int, 4>& scores) {
    json11::Json::object move;
    move["type"] = "end_game";
    move["scores"] = json11::Json(scores);
    return json11::Json(move);
}

bool is_valid_start_kyoku(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "start_kyoku") { return false; }
    // to do.
    return true;
}

bool is_valid_dahai(const json11::Json& action_json) {
    return action_json["type"].string_value() == "dahai";
}

bool is_valid_chi(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "chi") { return false; }
    if (action_json["actor"].int_value() != (action_json["target"].int_value() + 1) % 4) { return false; }
    if (action_json["consumed"].array_items().size() != 2) { return false; }
    const int hai = hai_str_to_int(action_json["pai"].string_value());
    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    return (haikind(consumed0) == haikind(hai) + 1 && haikind(consumed1) == haikind(hai) + 2) ||
           (haikind(consumed0) == haikind(hai) - 1 && haikind(consumed1) == haikind(hai) + 1) ||
           (haikind(consumed0) == haikind(hai) - 2 && haikind(consumed1) == haikind(hai) - 1);
}

bool is_valid_pon(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "pon") { return false; }
    if (action_json["actor"].int_value() == action_json["target"].int_value()) { return false; }
    if (action_json["consumed"].array_items().size() != 2) { return false; }
    const int hai = hai_str_to_int(action_json["pai"].string_value());
    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    return haikind(consumed0) == haikind(hai) && haikind(consumed1) == haikind(hai);
}

bool is_valid_daiminkan(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "daiminkan") { return false; }
    if (action_json["actor"].int_value() == action_json["target"].int_value()) { return false; }
    if (action_json["consumed"].array_items().size() != 3) { return false; }
    const int hai = hai_str_to_int(action_json["pai"].string_value());
    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    const int consumed2 = hai_str_to_int(action_json["consumed"][2].string_value());
    return haikind(consumed0) == haikind(hai) && haikind(consumed1) == haikind(hai) && haikind(consumed2) == haikind(hai);
}

bool is_valid_ankan(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "ankan") { return false; }
    if (action_json["consumed"].array_items().size() != 4) { return false; }
    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    const int consumed2 = hai_str_to_int(action_json["consumed"][2].string_value());
    const int consumed3 = hai_str_to_int(action_json["consumed"][3].string_value());
    return haikind(consumed0) == haikind(consumed1) && haikind(consumed0) == haikind(consumed2) && haikind(consumed0) == haikind(consumed3);
}

bool is_valid_kakan(const json11::Json& action_json) {
    if (action_json["type"].string_value() != "kakan") { return false; }
    if (action_json["consumed"].array_items().size() != 3) { return false; }
    const int hai = hai_str_to_int(action_json["pai"].string_value());
    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    const int consumed2 = hai_str_to_int(action_json["consumed"][2].string_value());
    return haikind(consumed0) == haikind(hai) && haikind(consumed1) == haikind(hai) && haikind(consumed2) == haikind(hai);
}

bool is_valid_hora(const json11::Json& action_json) {
    return action_json["type"].string_value() == "hora";
}

bool is_valid_pon_and_dahai(const Moves& moves) {
    if (moves.size() != 2) { return false; }
    if (!is_valid_pon(moves[0])) { return false; }
    if (!is_valid_dahai(moves[1])) { return false; }
    if (moves[0]["actor"].int_value() != moves[1]["actor"].int_value()) { return false; }
    return haikind(hai_str_to_int(moves[0]["pai"].string_value())) != haikind(hai_str_to_int(moves[1]["pai"].string_value()));
}

bool is_valid_chi_and_dahai(const Moves& moves) {
    if (moves.size() != 2) { return false; }
    if (!is_valid_chi(moves[0])) { return false; }
    if (!is_valid_dahai(moves[1])) { return false; }
    if (moves[0]["actor"].int_value() != moves[1]["actor"].int_value()) { return false; }
    const int hai = hai_str_to_int(moves[0]["pai"].string_value());
    const int consumed0 = hai_str_to_int(moves[0]["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(moves[0]["consumed"][1].string_value());
    const int dahai = hai_str_to_int(moves[1]["pai"].string_value());
    if (haikind(hai) == haikind(dahai)) { return false; }
    if (haikind(hai) < haikind(consumed0) && haikind(hai) < haikind(consumed1) && haikind(dahai) == haikind(hai) + 2) { return false; }
    if (haikind(hai) > haikind(consumed0) && haikind(hai) > haikind(consumed1) && haikind(dahai) == haikind(hai) - 2) { return false; }
    return true;
}

bool is_valid_kyushukyuhai(const json11::Json& action_json) {
    return action_json["type"].string_value() == "ryukyoku" && action_json["reason"].string_value() == "kyushukyuhai"; 
}

bool is_valid_game_record(const Moves& game_record) {
    if (game_record.size() < 2) { return false; }
    for (int i = 0; i < game_record.size(); i++) {
        if (i == 0 && game_record[i]["type"].string_value() != "start_game") { return false; }
        if (i == 1 && !is_valid_start_kyoku(game_record[i])) { return false; }
    }
    return true;
}

bool is_legal_dahai(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (action_json["type"].string_value() != "dahai") { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() == "tsumo") {
        const int actor = action_json["actor"].int_value();
        if (actor != last_action["actor"].int_value()) { return false; }
        // チE��刁E��は常に可能、E
        if (action_json["tsumogiri"].bool_value()) {
            return action_json["pai"].string_value() == last_action["pai"].string_value();
        }

        Hai_Array tehai_prev = game_state.player_state[actor].tehai;
        tehai_prev[hai_str_to_int(last_action["pai"].string_value())]--;
        return tehai_prev[hai_str_to_int(action_json["pai"].string_value())] > 0;
    }
    return false;
    // to do リーチ、副露後�Eも�Eなど、E
}

bool is_legal_chi(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_chi(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() != "dahai") { return false; }
    if (last_action["actor"].int_value() != action_json["target"].int_value()) { return false; }
    if (last_action["pai"].int_value() != action_json["pai"].int_value()) { return false; }

    const int actor = action_json["actor"].int_value();
    if (count_tsumo_num_all(game_record) == 84) { return false; }

    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    return game_state.player_state[actor].tehai[consumed0] > 0 && game_state.player_state[actor].tehai[consumed1] > 0;
}

bool is_legal_pon(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_pon(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() != "dahai") { return false; }
    if (last_action["actor"].int_value() != action_json["target"].int_value()) { return false; }
    if (last_action["pai"].int_value() != action_json["pai"].int_value()) { return false; }

    const int actor = action_json["actor"].int_value();
    if (count_tsumo_num_all(game_record) == 84) { return false; }

    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    if (consumed0 == consumed1) {
        return game_state.player_state[actor].tehai[consumed0] > 1;
    } else {
        return game_state.player_state[actor].tehai[consumed0] > 0 && game_state.player_state[actor].tehai[consumed1] > 0;
    }
}

bool is_legal_daiminkan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_daiminkan(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() != "dahai") { return false; }
    if (last_action["actor"].int_value() != action_json["target"].int_value()) { return false; }
    if (last_action["pai"].int_value() != action_json["pai"].int_value()) { return false; }

    const int actor = action_json["actor"].int_value();
    if (count_tsumo_num_all(game_record) == 84) { return false; }

    const int consumed0 = hai_str_to_int(action_json["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(action_json["consumed"][1].string_value());
    const int consumed2 = hai_str_to_int(action_json["consumed"][2].string_value());
    if (consumed1 == consumed0 && consumed2 == consumed0) {
        return game_state.player_state[actor].tehai[consumed0] > 2;
    } else {
        return game_state.player_state[actor].tehai[haikind(consumed0)] > 1 && game_state.player_state[actor].tehai[haikind(consumed0) + 5] > 0;
    }
}

bool is_legal_ankan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_ankan(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() == "tsumo") {
        const int actor = action_json["actor"].int_value();
        if (actor != last_action["actor"].int_value()) { return false; }
        if (count_tsumo_num_all(game_record) == 84) { return false; } // ハイチE��はアンカンできなぁE��E
        // to do リーチ時のアンカン条件

        Hai_Array tehai = game_state.player_state[actor].tehai;
        tehai[hai_str_to_int(action_json["consumed"][0].string_value())]--;
        tehai[hai_str_to_int(action_json["consumed"][1].string_value())]--;
        tehai[hai_str_to_int(action_json["consumed"][2].string_value())]--;
        tehai[hai_str_to_int(action_json["consumed"][3].string_value())]--;

        return 0 <= tehai[hai_str_to_int(action_json["consumed"][0].string_value())] &&
               0 <= tehai[hai_str_to_int(action_json["consumed"][1].string_value())] &&
               0 <= tehai[hai_str_to_int(action_json["consumed"][2].string_value())] &&
               0 <= tehai[hai_str_to_int(action_json["consumed"][3].string_value())];
    }
    return false;
}

bool is_legal_kakan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_kakan(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() == "tsumo") {
        const int actor = action_json["actor"].int_value();
        if (actor != last_action["actor"].int_value()) { return false; }
        if (count_tsumo_num_all(game_record) == 84) { return false; } // ハイチE��はカカンできなぁE��E

        const int hai = hai_str_to_int(action_json["pai"].string_value());
        Fuuro_Vector fuuro = game_state.player_state[actor].fuuro;
        for (const auto& elem : fuuro) {
            if (elem.type == FT_PON && haikind(elem.hai) == haikind(hai)) {
                std::vector<int> pon_hai_vec, consumed_vec;
                pon_hai_vec.push_back(elem.hai);
                pon_hai_vec.push_back(elem.consumed[0]);
                pon_hai_vec.push_back(elem.consumed[1]);
                std::sort(pon_hai_vec.begin(), pon_hai_vec.end());
                consumed_vec.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
                consumed_vec.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
                consumed_vec.push_back(hai_str_to_int(action_json["consumed"][2].string_value()));
                std::sort(consumed_vec.begin(), consumed_vec.end());
                return pon_hai_vec[0] == consumed_vec[0] &&
                       pon_hai_vec[1] == consumed_vec[1] &&
                       pon_hai_vec[2] == consumed_vec[2] &&
                       game_state.player_state[actor].tehai[hai] > 0;
            }
        }
    }
    return false;
}

bool is_legal_hora(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_hora(action_json)) { return false; }
    const int actor = action_json["actor"].int_value();
    const int hai = hai_str_to_int(action_json["pai"].string_value());
    const json11::Json& last_action = game_record[game_record.size() - 1];
    int han_add = 0;
    if (count_tsumo_num_all(game_record) == 84) {
        han_add++;
    }
    if (last_action["type"].string_value() == "tsumo") {
        if (last_action["actor"].int_value() != actor) { return false; }
        if (hai_str_to_int(last_action["pai"].string_value()) != hai) { return false; }
        Hai_Array tehai = game_state.player_state[actor].tehai;
        tehai[hai]--;
        const Tenpai_Info tenpai_info = cal_tenpai_info(
            game_state.bakaze, game_state.player_state[actor].jikaze, tehai, game_state.player_state[actor].fuuro
        );
        for (int i = 0; i < tenpai_info.agari_vec.size(); i++) {
            if (haikind(hai) == tenpai_info.agari_vec[i].hai &&
                8 <= han_add + tenpai_info.agari_vec[i].result_tsumo.calc_point()
            ) {
                return true;
            }
        }
    } else if (last_action["type"].string_value() == "dahai" || last_action["type"].string_value() == "kakan") {
        if (last_action["actor"].int_value() == actor) { return false; }
        if (hai_str_to_int(last_action["pai"].string_value()) != hai) { return false; }
        if (last_action["type"].string_value() == "kakan") {
            han_add++;
        }
        const Tenpai_Info tenpai_info = cal_tenpai_info(
            game_state.bakaze, game_state.player_state[actor].jikaze, game_state.player_state[actor].tehai, game_state.player_state[actor].fuuro
        );
        for (int i = 0; i < tenpai_info.agari_vec.size(); i++) {
            if (haikind(hai) == tenpai_info.agari_vec[i].hai &&
                8 <= han_add + tenpai_info.agari_vec[i].result_ron.calc_point()
            ) {
                std::array<bool, 38> furiten_flags = get_furiten_flags(game_record, game_state, actor, true);
                for (int j = 0; j < tenpai_info.agari_vec.size(); j++) {
                    if (furiten_flags[tenpai_info.agari_vec[j].hai]) {
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool is_legal_pon_and_dahai(const Moves& game_record, const Game_State& game_state, const Moves& moves) {
    if (!is_valid_pon_and_dahai(moves)) { return false; }
    if (!is_legal_pon(game_record, game_state, moves[0])) { return false; }

    const int consumed0 = hai_str_to_int(moves[0]["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(moves[0]["consumed"][1].string_value());
    const int dahai = hai_str_to_int(moves[1]["pai"].string_value());
    Hai_Array tehai = game_state.player_state[moves[0]["actor"].int_value()].tehai;
    tehai[consumed0]--;
    tehai[consumed1]--;
    tehai[dahai]--;
    return 0 <= tehai[consumed0] && 0 <= tehai[consumed1] && 0 <= tehai[dahai];
}

bool is_legal_chi_and_dahai(const Moves& game_record, const Game_State& game_state, const Moves& moves) {
    if (!is_valid_chi_and_dahai(moves)) { return false; }
    if (!is_legal_chi(game_record, game_state, moves[0])) { return false; }

    const int consumed0 = hai_str_to_int(moves[0]["consumed"][0].string_value());
    const int consumed1 = hai_str_to_int(moves[0]["consumed"][1].string_value());
    const int dahai = hai_str_to_int(moves[1]["pai"].string_value());
    Hai_Array tehai = game_state.player_state[moves[0]["actor"].int_value()].tehai;
    tehai[consumed0]--;
    tehai[consumed1]--;
    tehai[dahai]--;
    return 0 <= tehai[consumed0] && 0 <= tehai[consumed1] && 0 <= tehai[dahai];
}

bool is_legal_kyushukyuhai(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json) {
    if (!is_valid_kyushukyuhai(action_json)) { return false; }

    const json11::Json& last_action = game_record[game_record.size() - 1];
    if (last_action["type"].string_value() == "tsumo") {
        const int actor = action_json["actor"].int_value();
        if (actor != last_action["actor"].int_value()) { return false; }
        if (game_state.player_state[actor].kawa.size() > 0) { return false; }
        for (int pid = 0; pid < 4; pid++) {
            if (game_state.player_state[pid].fuuro.size() > 0) { return false; }
        }
        if (9 <= count_yaochu_kind(game_state.player_state[actor].tehai)) {
            return true;
        }
    }
    return false;
}

bool is_legal_none(const Moves& game_record, const json11::Json& action_json) {
    if (action_json["type"].string_value() != "none") { return false; }

    const json11::Json& last_action = game_record.back();
    const int last_actor = last_action["actor"].int_value();
    const int actor = action_json["actor"].int_value();
    if (last_actor == actor) {
        if (last_action["type"].string_value() == "tsumo" ||
            last_action["type"].string_value() == "chi" ||
            last_action["type"].string_value() == "pon"
        ) {
            return false;
        }
    }
    return true;
}

bool is_legal_single_move(const Moves& game_record, const json11::Json& action_json) {
    const Game_State game_state = get_game_state(game_record);
    if      (action_json["type"].string_value() == "none")         { return is_legal_none(game_record, action_json); }
    else if (action_json["type"].string_value() == "dahai")        { return is_legal_dahai(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "chi")          { return is_legal_chi(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "pon")          { return is_legal_pon(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "daiminkan")    { return is_legal_daiminkan(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "ankan")        { return is_legal_ankan(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "kakan")        { return is_legal_kakan(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "hora")         { return is_legal_hora(game_record, game_state, action_json); }
    else if (action_json["type"].string_value() == "ryukyoku" && action_json["reason"].string_value() == "kyushukyuhai") { return is_legal_kyushukyuhai(game_record, game_state, action_json); }
    else { return false; }
}

bool is_legal_dahai_after_fuuro(const Moves& game_record, const json11::Json& action_json) {
    assert(0 < game_record.size());
    Moves gr = game_record;
    Moves moves;
    moves.push_back(gr.back());
    moves.push_back(action_json);
    gr.pop_back();
    const Game_State game_state = get_game_state(gr);
    if (moves[0]["type"] == "chi") { 
        return is_legal_chi_and_dahai(gr, game_state, moves);
    } else if (moves[0]["type"] == "pon") {
        return is_legal_pon_and_dahai(gr, game_state, moves);
    } else {
        return false;
    }
}

std::vector<Moves> get_legal_tsumogiri_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "tsumo") {
        return result;
    }
    Moves moves;
    moves.push_back(make_dahai(action_json["actor"].int_value(), hai_str_to_int(action_json["pai"].string_value()), true));
    result.push_back(moves);
    return result;
}

std::vector<Moves> get_legal_tedashi_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "tsumo") {
        return result;
    }

    Game_State game_state = get_game_state(game_record);
    const int actor = action_json["actor"].int_value();
    for (int hai = 1; hai < 38; hai++) {
        Moves moves;
        json11::Json tedashi_move = make_dahai(actor, hai, false);
        if (is_legal_dahai(game_record, game_state, tedashi_move)) {
            moves.push_back(tedashi_move);
            result.push_back(moves);
        }
    }
    return result;
}

std::vector<Moves> get_legal_pon_dahai_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "dahai") {
        return result;
    }
    Game_State game_state = get_game_state(game_record);
    const int target = action_json["actor"].int_value();
    const int hai_target = hai_str_to_int(action_json["pai"].string_value());
    for (int pid = 0; pid < 4; pid++) {
        if (pid != target) {
            json11::Json pon_move = make_pon_default(pid, target, hai_target);
            for (int hai = 1; hai < 38; hai++) {
                Moves moves;
                json11::Json tedashi_move = make_dahai(pid, hai, false);
                moves.push_back(pon_move);
                moves.push_back(tedashi_move);
                if (is_legal_pon_and_dahai(game_record, game_state, moves)) {
                    result.push_back(moves);
                }
            }
            if (hai_target % 10 == 5 && hai_target < 30) {
                pon_move = make_pon_aka(pid, target, hai_target);
                for (int hai = 1; hai < 38; hai++) {
                    Moves moves;
                    json11::Json tedashi_move = make_dahai(pid, hai, false);
                    moves.push_back(pon_move);
                    moves.push_back(tedashi_move);
                    if (is_legal_pon_and_dahai(game_record, game_state, moves)) {
                        result.push_back(moves);
                    }
                }
            }
        }
    }
    return result;
}

std::vector<Moves> get_legal_chi_dahai_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "dahai") {
        return result;
    }
    Game_State game_state = get_game_state(game_record);
    const int target = action_json["actor"].int_value();
    const int hai_target = hai_str_to_int(action_json["pai"].string_value());
    if (hai_target > 30) { return result; }
    const int pid = (target + 1) % 4;

    json11::Json chi_move;
    if (haikind(hai_target) % 10 <= 7) {
        chi_move = make_chi_low_default(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }

    if (haikind(hai_target) % 10 == 3 || haikind(hai_target) % 10 == 4) {
        chi_move = make_chi_low_aka(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }

    if (2 <= haikind(hai_target) % 10 && haikind(hai_target) % 10 <= 8) {
        chi_move = make_chi_middle_default(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }

    if (haikind(hai_target) % 10 == 4 || haikind(hai_target) % 10 == 6) {
        chi_move = make_chi_middle_aka(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }

    if (3 <= haikind(hai_target) % 10) {
        chi_move = make_chi_high_default(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }

    if (haikind(hai_target) % 10 == 6 || haikind(hai_target) % 10 == 7) {
        chi_move = make_chi_high_aka(pid, target, hai_target);
        for (int hai = 1; hai < 38; hai++) {
            Moves moves;
            json11::Json tedashi_move = make_dahai(pid, hai, false);
            moves.push_back(chi_move);
            moves.push_back(tedashi_move);
            if (is_legal_chi_and_dahai(game_record, game_state, moves)) {
                result.push_back(moves);
            }
        }
    }
    return result;
}

std::vector<Moves> get_legal_daiminkan_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "dahai") {
        return result;
    }
    Game_State game_state = get_game_state(game_record);
    const int target = action_json["actor"].int_value();
    const int hai_target = hai_str_to_int(action_json["pai"].string_value());
    for (int pid = 0; pid < 4; pid++) {
        if (pid != target) {
            json11::Json daiminkan_move = make_daiminkan_default(pid, target, hai_target);
            if (is_legal_daiminkan(game_record, game_state, daiminkan_move)) {
                Moves moves;
                moves.push_back(daiminkan_move);
                result.push_back(moves);
            }
            if (hai_target % 10 == 5 && hai_target < 30) {
                daiminkan_move = make_daiminkan_aka(pid, target, hai_target);
                if (is_legal_daiminkan(game_record, game_state, daiminkan_move)) {
                    Moves moves;
                    moves.push_back(daiminkan_move);
                    result.push_back(moves);
                }
            }
        }
    }
    return result;
}

std::vector<Moves> get_legal_ankan_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "tsumo") {
        return result;
    }

    Game_State game_state = get_game_state(game_record);
    const int actor = action_json["actor"].int_value();
    for (int hai = 1; hai < 38; hai++) {
        if (hai % 10 != 0) {
            json11::Json ankan_move = make_ankan_default(actor, hai);
            if (is_legal_ankan(game_record, game_state, ankan_move)) {
                Moves moves;
                moves.push_back(ankan_move);
                result.push_back(moves);
            }
        }
        if (hai % 10 == 5 && hai < 30) {
            json11::Json ankan_move = make_ankan_aka(actor, hai);
            if (is_legal_ankan(game_record, game_state, ankan_move)) {
                Moves moves;
                moves.push_back(ankan_move);
                result.push_back(moves);
            }
        }
    }
    return result;
}

std::vector<Moves> get_legal_kakan_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "tsumo") {
        return result;
    }

    Game_State game_state = get_game_state(game_record);
    const int actor = action_json["actor"].int_value();
    for (int hai = 1; hai < 38; hai++) {
        json11::Json kakan_move = make_kakan_default(actor, hai);
        if (is_legal_kakan(game_record, game_state, kakan_move)) {
            Moves moves;
            moves.push_back(kakan_move);
            result.push_back(moves);
        }
        if (hai % 10 == 5 && hai < 30) {
            kakan_move = make_kakan_aka(actor, hai);
            if (is_legal_kakan(game_record, game_state, kakan_move)) {
                Moves moves;
                moves.push_back(kakan_move);
                result.push_back(moves);
            }
        }
    }
    return result;
}

std::vector<Moves> get_legal_ron_move(const Moves& game_record) {
    std::vector<Moves> result;
    const json11::Json& action_json = game_record[game_record.size() - 1];
    if (action_json["type"].string_value() != "dahai" && action_json["type"].string_value() != "kakan") {
        return result;
    }

    Game_State game_state = get_game_state(game_record);
    const int target = action_json["actor"].int_value();
    const int dahai = hai_str_to_int(action_json["pai"].string_value());
    for (int pid = 0; pid < 4; pid++) {
        if (pid != target) {
            Moves moves;
            json11::Json ron_move = make_hora(pid, target, dahai);
            if (is_legal_hora(game_record, game_state, ron_move)) {
                moves.push_back(ron_move);
                result.push_back(moves);
            }
        }
    }
    return result;
}

std::array<std::vector<Moves>, 4> get_all_legal_moves(const Moves& game_record) {
    std::array<std::vector<Moves>, 4> all_legal_moves;
    std::vector<Moves> moves_vec = get_legal_tsumogiri_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_tedashi_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_pon_dahai_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_chi_dahai_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_daiminkan_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_ankan_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_kakan_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    moves_vec = get_legal_ron_move(game_record);
    for (int i = 0; i < moves_vec.size(); i++) {
        all_legal_moves[moves_vec[i][0]["actor"].int_value()].push_back(moves_vec[i]);
    }

    if (game_record[game_record.size()-1]["type"].string_value() == "dahai") {
        for (int pid = 0; pid < 4; pid++) {
            if (all_legal_moves[pid].size() > 0) {
                all_legal_moves[pid].push_back({make_none(pid)});
            }
        }
    }
    return all_legal_moves;
}

std::vector<json11::Json> get_all_legal_single_action(const Moves& game_record) {
    std::array<std::vector<Moves>, 4> all_legal_moves = get_all_legal_moves(game_record);
    std::vector<json11::Json> ret;
    for (int pid = 0; pid < 4; pid++) {
        for (Moves moves : all_legal_moves[pid]) {
            if (ret.size() == 0 || moves[0] != ret.back()) {
                ret.push_back(moves[0]);
            }
        }
    }
    return ret;
}