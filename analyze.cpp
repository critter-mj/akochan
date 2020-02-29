#include "analyze.hpp"

std::vector<json11::Json> full_analyze(const Moves& game_record, const int my_pid) {
    std::vector<json11::Json> ret;
    Moves current_game_record;
    for (int i = 0; i < game_record.size(); i++) {
        const json11::Json action_json = game_record[i];
        current_game_record.push_back(action_json);
        json11::Json::object obj = action_json.object_items(); 
        if (action_json["type"] == "start_game") {
            // nothing to do.
            // mjaiのログにtonpu tonnanの表示は無いのでチェックしない。
        } else if ((action_json["type"] == "dahai" && action_json["actor"] != my_pid) || 
                   (action_json["type"] == "tsumo" && action_json["actor"] == my_pid)
        ) {
            std::array<std::vector<Moves>, 4> all_legal_moves = get_all_legal_moves(current_game_record);
            if (all_legal_moves[my_pid].size() >= 2) {
                int check_pos = 1;
                while (game_record[i + check_pos]["type"] == "dora" || game_record[i + check_pos]["type"] == "reach_accepted") {
                    check_pos++; //カンドラがめくられたときならスキップ
                }
                if ((game_record[i + check_pos]["type"] == "pon" || game_record[i + check_pos]["type"] == "hora") &&
                    game_record[i + check_pos]["actor"].int_value() != my_pid)
                {
                    continue; //邪魔ポン、ロンの時はスキップ
                } else if (game_record[i + check_pos]["type"] == "ankan" || game_record[i + check_pos]["type"] == "kakan" || game_record[i + check_pos]["type"] == "daiminkan") {
                    continue;		//行った行動がカンならスキップ
                } else if (game_record[i + check_pos]["type"] == "hora" && (game_record[i - check_pos]["type"] == "ankan" || game_record[i - check_pos]["type"] == "kakan" || game_record[i - check_pos]["type"] == "daiminkan" || game_record[i-check_pos]["type"]=="dora")) {
                    continue;	//嶺上牌であがりならばスキップ
                } else {		//エラーチェック
                    const std::vector<std::pair<Moves, float>> moves_score = calc_moves_score(current_game_record, my_pid);
                    if (moves_score.size() < 2) {
                        continue; // AIがルールベース
                    }
                    for (const std::pair<Moves, float>& pair : moves_score) {
                        bool is_different = false;
                        for (int j = 0; j < pair.first.size(); j++) {
                            if (pair.first[j]["type"] == "none") {
                                if (game_record[i + check_pos + j]["type"] != "tsumo" &&
                                    !game_record[i + check_pos + j]["actor"].is_null() &&
                                    game_record[i + check_pos + j]["actor"].int_value() == my_pid
                                ) {
                                    is_different = true;
                                }
                            } else if (pair.first[j]["actor"] == game_record[i + check_pos + j]["actor"]) {
                                if (pair.first[j]["type"] == "hora") {
                                    if (game_record[i + check_pos + j]["type"] != "hora") {
                                        is_different = true;
                                    }
                                } else if (pair.first[j]["type"] == "dahai") {
                                    if (game_record[i + check_pos + j]["type"] != "dahai" ||
                                        (pair.first[j]["pai"] != game_record[i + check_pos + j]["pai"])
                                    ) {
                                        is_different = true;
                                    }
                                } else {
                                    // 副露
                                    if (pair.first[j]["type"] != game_record[i + check_pos + j]["type"]) {
                                        is_different = true;
                                    } else if (pair.first[j]["consumed"].array_items().size() != game_record[i + check_pos + j]["consumed"].array_items().size()) {
                                        is_different = true;
                                    } else {
                                        for (int k = 0; k < pair.first[j]["consumed"].array_items().size(); k++) {
                                            if (pair.first[j]["consumed"][k] != game_record[i + check_pos + j]["consumed"][k]) {
                                                is_different = true;
                                            }
                                        }
                                    }
                                }
                            } else {
                                is_different = true;
                            }
                        }
                        if (!is_different) {
                            // moves actually done in log
                            obj["err"] = moves_score[0].second - pair.second;
                            std::cout << "---" << std::endl;
                            for (int j = 0; j < pair.first.size(); j++) {
                                std::cout << pair.first[j].dump() << std::endl;
                            }
                            std::cout << "err:" << moves_score[0].second - pair.second << std::endl;
                            std::cout << "---" << std::endl;
                            break;
                        }
                    }
                }
            }
        }
        ret.push_back(json11::Json(obj));
    }
    return ret;
}
