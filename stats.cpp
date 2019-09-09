#include "stats.hpp"

void show_stats_mjai(const std::string& dir_name, const std::string& player_name_prefix) {
    std::string ls_command = "ls -1 ./" + dir_name + "/*.mjson > tmp.txt"; // 指定したディレクトリ内のmjsonファイルを一旦tmp.txtに書き出す。
    system(ls_command.c_str());

    std::ifstream ifs_tmptxt("tmp.txt");
    assert_with_out(!ifs_tmptxt.fail(), "tmp.txt not generated");
    std::string file_path;
    int result_num[4] = {};
    int kyoku_num = 0;
    int hora_num = 0;
    int reach_num = 0;
    int houjuu_num = 0;
    int fuuro1_num = 0;
    int fuuro_num = 0;
    while (getline(ifs_tmptxt, file_path)) {
        std::ifstream ifs(file_path);
        std::string str;
        std::string err;
        assert_with_out(!ifs.fail(), "file_not_found:" + file_path);
        const int oya_first = 0; // mjaiでは常に0番プレイヤが起家
        int my_pid = -1; // 注目するプレイヤの番号。（現状 1vs3 の1のプレイヤの統計を取る仕様）

        int fuuro_flag[4] = {};
        while (getline(ifs, str)) {
            json11::Json action_json = json11::Json::parse(str, err);
            if (action_json["type"].string_value() == "start_game") {
                for (int j = 0; j < action_json["names"].array_items().size(); j++) {
                    if (str_starts_with(action_json["names"][j].string_value(), player_name_prefix)) {
                        my_pid = j;
                        break;
                    }
                }
                assert_with_out(my_pid != -1, "player_not_found in " + file_path);
            } else if (action_json["type"].string_value() == "start_kyoku") {
                kyoku_num++;
                for (int i = 0; i < 4; i++) {
                    fuuro_flag[i] = 0;
                }
            } else if (action_json["type"].string_value() == "reach") {
                if (action_json["actor"].int_value() == my_pid) {
                    reach_num++;
                }
            } else if (action_json["type"].string_value() == "chi" || action_json["type"].string_value() == "pon") {
                if (action_json["actor"].int_value() == my_pid) {
                    fuuro_num++;
                }
                fuuro_flag[action_json["actor"].int_value()] = 1;
            } else if (action_json["type"].string_value() == "hora") {
                if (action_json["actor"].int_value() == my_pid) {
                    hora_num++;
                } 
                if (action_json["actor"].int_value() != action_json["target"].int_value() &&
                    action_json["target"].int_value() == my_pid
                ) {
                    houjuu_num++;
                }
            } else if (action_json["type"].string_value() == "end_kyoku") {
                fuuro1_num += fuuro_flag[my_pid] ? 1 : 0;
            } else if (action_json["type"].string_value() == "end_game") {
                std::vector<Player_Result> results;
                for (int j = 0; j < action_json["scores"].array_items().size(); j++ ) {
                    results.push_back(Player_Result(j, action_json["scores"].array_items()[j].int_value(), (4 + j - oya_first)%4 ));
                }
                std::sort(results.begin(), results.end());
                for (int j = 0; j < results.size(); j++) {
                    if (results[j].pid == my_pid) {
                        result_num[j]++;
                    }
                }
                break;
            }
        }
    }
    std::cout << result_num[0] << " " << result_num[1] << " " << result_num[2] << " " << result_num[3] << " " << std::endl;
    double jun_average = result_num[0] + result_num[1]*2.0 + result_num[2]*3.0 + result_num[3]*4.0;
    jun_average = jun_average/(result_num[0] + result_num[1] + result_num[2] + result_num[3]);
    std::cout << "average_rank:" << jun_average << std::endl;
    std::cout << "hora_prob:" << (double)hora_num / kyoku_num << std::endl;
    std::cout << "reach_prob:" << (double)reach_num / kyoku_num << std::endl;
    std::cout << "houjuu_prob:" << (double)houjuu_num / kyoku_num << std::endl;
    std::cout << "fuuro_prob:" << (double)fuuro1_num / kyoku_num << std::endl;
    std::cout << "fuuro_num:" << (double)fuuro_num / kyoku_num << std::endl;

    //system("rm ./tmp.txt");
}

void show_stats(const std::string& dir_name, const int chicha_num) {
    int result_num[4][4] = {};
    int kyoku_num = 0;
    int reach_num[4] = {};
    int fuuro_num[4] = {};
    int fuuro_prob[4] = {};
    int fuuro_flag[4] = {};
    int hora_num[4] = {};
    int houjuu_num[4] = {};
    bool file_not_found = false;
    for (int rand_seed = 1000; rand_seed < 2000; rand_seed++) {
        for (int oya_first = 0; oya_first < chicha_num; oya_first++) {
            std::ifstream ifs(dir_name + "haifu_log_" + std::to_string(rand_seed) + "_" + std::to_string(oya_first) + ".json");
            std::string str;
            std::string err;
            if (ifs.fail()) {
                file_not_found = true;
                break;
            }
            while (getline(ifs, str)) {
                json11::Json action_json = json11::Json::parse(str, err);
                if (action_json["type"].string_value() == "start_kyoku") {
                    kyoku_num++;
                    for (int i = 0; i < 4; i++) {
                        fuuro_flag[i] = 0;
                    }
                } else if (action_json["type"].string_value() == "reach") {
                    reach_num[action_json["actor"].int_value()]++;
                } else if (action_json["type"].string_value() == "chi" || action_json["type"].string_value() == "pon") {
                    fuuro_num[action_json["actor"].int_value()]++;
                    fuuro_flag[action_json["actor"].int_value()] = 1;
                } else if (action_json["type"].string_value() == "hora") {
                    hora_num[action_json["actor"].int_value()]++;
                    if (action_json["actor"].int_value() != action_json["target"].int_value()) {
                        houjuu_num[action_json["target"].int_value()]++;
                    }
                } else if (action_json["type"].string_value() == "end_kyoku") {
                    for (int i = 0; i < 4; i++) {
                        fuuro_prob[i] += fuuro_flag[i];
                    }
                } else if (action_json["type"].string_value() == "end_game") {
                    std::vector<Player_Result> results;
                    for (int j = 0; j < action_json["scores"].array_items().size(); j++ ) {
                        results.push_back(Player_Result(j, action_json["scores"].array_items()[j].int_value(), (4 + j - oya_first)%4 ));
                    }
                    std::sort(results.begin(), results.end());
                    for (int j = 0; j < results.size(); j++) {
                        result_num[results[j].pid][j]++;
                    }
                    break;
                }
            }
        }
        if (file_not_found) { break; }
    }
    std::cout << std::endl;
    for (int i = 0; i < 4; i++) {
        // 雑な出力
        std::cout << result_num[i][0] << " " << result_num[i][1] << " " << result_num[i][2] << " " << result_num[i][3] << " ";
        double jun_average = result_num[i][0] + result_num[i][1]*2.0 + result_num[i][2]*3.0 + result_num[i][3]*4.0;
        jun_average = jun_average/(result_num[i][0] + result_num[i][1] + result_num[i][2] + result_num[i][3]);
        std::cout << jun_average << " " << double(reach_num[i])/kyoku_num << " " << double(fuuro_prob[i])/kyoku_num << " " << double(fuuro_num[i])/kyoku_num << " ";
        std::cout << double(hora_num[i])/kyoku_num << " " << double(houjuu_num[i])/kyoku_num << std::endl;
    }
    std::cout << std::endl;
    // きれいな出力
    const int game_num = result_num[0][0] + result_num[0][1] + result_num[0][2] + result_num[0][3];
    std::cout << "average_rank:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << (result_num[pid][0] + result_num[pid][1]*2.0 + result_num[pid][2]*3.0 + result_num[pid][3]*4.0) / game_num;
    }
    std::cout << std::endl;

    for (int i = 0; i < 4; i++) {
        std::cout << "rank" + std::to_string(i+1) + "_prob:";
        for (int pid = 0; pid < 4; pid++) {
            std::cout << " " << float(result_num[pid][i]) / game_num;
        }
        std::cout << std::endl;
    }

    std::cout << "hora_prob:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << float(hora_num[pid])/kyoku_num;
    }
    std::cout << std::endl;

    std::cout << "houjuu_prob:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << float(houjuu_num[pid])/kyoku_num;
    }
    std::cout << std::endl;

    std::cout << "reach_prob:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << float(reach_num[pid])/kyoku_num;
    }
    std::cout << std::endl;

    std::cout << "fuuro_prob:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << float(fuuro_prob[pid])/kyoku_num;
    }
    std::cout << std::endl;

    std::cout << "fuuro_num:";
    for (int pid = 0; pid < 4; pid++) {
        std::cout << " " << float(fuuro_num[pid])/kyoku_num;
    }
    std::cout << std::endl;
}
