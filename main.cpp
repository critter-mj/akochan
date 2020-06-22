#include "main.hpp"

Game_Settings game_settings;

void usage() {
  printf("Usage:\n");
  printf("  > system.exe test [[<seed_init>] <seed_end>]\n");
  printf("  > system.exe initial_condition_match [[<seed_init>] <seed_end>]\n");
  printf("     seed_init: default 1000\n");
  printf("     seed_end: default 1001\n");
  printf("  > system.exe check\n");
  printf("  > system.exe mjai_log <file> <id>\n");
  printf("     -- run AI at the final state specified by file, from the point of view specified by id --\n");
  printf("     file: path of logfile\n");
  printf("     id: 0,1,2,3\n");
  printf("  > system.exe mjai_log <file> <id> <line_index>\n");
  printf("     -- run AI at the state specified by file and line_index(0-indexed), from the point of view specified by id --\n");
  printf("     file: path of logfile\n");
  printf("     id: 0,1,2,3\n");
  printf("     line_index: 0,1...\n");
  printf("  > system.exe full_analyze <file> <id>\n");
  printf("     file: path of logfile\n");
  printf("     id: 0,1,2,3\n");
  printf("  > system.exe para_check\n");
  printf("  > system.exe stats <dir_name>\n");
  printf("     dir_name: relative path of directory contains haifu_log json files\n");
  printf("  > system.exe stats_mjai [<dir_name> [player_name_prefix]\n");
  printf("     dir_name: default \"\"\n");
  printf("     player_name_prefix: default \"Akochan\"\n");
  printf("  > system.exe mjai_client\n");
  printf("  > system.exe pipe <file> <id>\n");
  printf("     file: path of tactics json\n");
  printf("     id: 0,1,2,3\n");
  printf("  > system.exe pipe_detailed <file> <id>\n");
  printf("     file: path of tactics json\n");
  printf("     id: 0,1,2,3\n");
}

int main(int argc,char* argv[]) {
    if (argc >= 2 && strcmp(argv[1], "test") == 0) {
        const json11::Json& setup_match_json = load_json_from_file("setup_match.json");
        const std::string match_dir_name = setup_match_json["result_dir"].string_value();
        make_dir(match_dir_name);
        dump_json_to_file(setup_match_json, match_dir_name + "/setup_match.json");
        set_tactics(setup_match_json);
        assert(setup_match_json["chicha"].is_array());
        Moves game_record;
        std::vector<int> haiyama;
        int seed_init = 1000;
        int seed_end = 1001;
        if (argc == 4) {
            seed_init = atoi(argv[2]);
            seed_end = atoi(argv[3]);
        }
        for (int seed = seed_init; seed < seed_end; seed++) {
            for (auto chicha : setup_match_json["chicha"].array_items()) {
                std::srand(seed);
                game_record.clear();
                haiyama.clear();
                game_settings.chicha = chicha.int_value();
                assert(is_valid_player(game_settings.chicha));
                game_settings.player_id = -1;
                game_loop(haiyama, game_record, game_settings.chicha, game_settings.player_id);
                dump_json_vec_to_file(game_record, match_dir_name + "/haifu_log_" + std::to_string(seed) + "_" + std::to_string(game_settings.chicha) + ".json");
            }
        }
        return 0;
    } else if (argc >= 2 && strcmp(argv[1], "initial_condition_match") == 0) {
        const json11::Json& setup_match_json = load_json_from_file("setup_match.json");
        const std::string match_dir_name = setup_match_json["result_dir"].string_value();
        make_dir(match_dir_name);
        dump_json_to_file(setup_match_json, match_dir_name + "/setup_match.json");
        set_tactics(setup_match_json);
        const std::string initial_dir_name = setup_match_json["initial_dir"].string_value();
        int kyoku_init = setup_match_json["kyoku_init"].int_value();
        Moves game_record;
        std::vector<int> haiyama;
        int seed_init = 1000;
        int seed_end = 1001;
        if (argc == 4) {
            seed_init = atoi(argv[2]);
            seed_end = atoi(argv[3]);
        }
        for (int seed = seed_init; seed < seed_end; seed++) {
            const int chicha = 0;
            std::srand(seed);
            game_record.clear();
            haiyama.clear();
            game_settings.chicha = chicha;
            assert(is_valid_player(game_settings.chicha));
            game_settings.player_id = -1;

            const Moves initial_game_record = load_json_vec_from_file(initial_dir_name + "/haifu_log_" + std::to_string(seed) + "_0.json");
            for (const auto move : initial_game_record) {
                if (move["kyoku"].int_value() == kyoku_init) {
                    assert(move["scores"].array_items().size() == 4);
                    std::array<int, 4> scores;
                    for (int i = 0; i < 4; i++) {
                        scores[i] = move["scores"][i].int_value();
                    }
                    game_record.push_back(make_start_game_with_initial_condition(kyoku_init, scores));
                    break;
                }
            }

            game_loop(haiyama, game_record, game_settings.chicha, game_settings.player_id);
            dump_json_vec_to_file(game_record, match_dir_name + "/haifu_log_" + std::to_string(seed) + "_" + std::to_string(game_settings.chicha) + ".json");
        }
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "game_server") == 0) {
        const json11::Json& setup_mjai_json = load_json_from_file("setup_mjai.json");
        set_tactics_one(setup_mjai_json);
        const std::string input_str = argv[2];
        //const std::string input_str = R"({"record":[{"aka_flag":true,"kyoku_first":4,"type":"start_game"},{"bakaze":"E","dora_marker":"2p","haiyama":["6s","2m","F","2m","4s","8s","8p","1s","2s","4p","9s","4p","P","1p","6s","6p","C","8m","1p","8p","3p","3p","9m","7s","S","F","5p","5m","7p","3m","7s","7m","F","5m","3s","9s","5p","8s","4m","P","5pr","2p","F","8p","3s","1s","C","6s","3m","S","1m","4p","E","4m","2m","1s","2p","8m","3m","1m","N","7p","6m","7p","9m","9p","W","S","2s","2s","2s","E","3p","1s","W","S","4p","3s","7m","4m","9p","8p","1p","7s","3s","W","2p","1m","3m","1p","6m","9m","4m","9s","5s","6m","9p","7m","9s","E","9p","7s","1m","N","5sr","5p","C","5s","6p","5m","N","6s","N","2m","6m","6p","8s","8m","P","P","4s","C","7p","4s","3p","8m","4s","6p","9m","E","2p","W","5mr","5s","8s","7m"],"honba":0,"kyoku":1,"kyotaku":0,"oya":0,"scores":[25000,25000,25000,25000],"tehais":[["6s","4s","2s","P","C","3p","S","7p","F","5p","5pr","3s","3m"],["2m","8s","4p","1p","8m","3p","F","3m","5m","8s","2p","1s","S"],["F","8p","9s","6s","1p","9m","5p","7s","3s","4m","F","C","1m"],["2m","1s","4p","6p","8p","7s","5m","7m","9s","P","8p","6s","4p"]],"type":"start_kyoku"},{"actor":0,"pai":"E","type":"tsumo"},{"actor":0,"pai":"E","tsumogiri":true,"type":"dahai"},{"actor":1,"pai":"4m","type":"tsumo"},{"actor":1,"pai":"S","tsumogiri":false,"type":"dahai"},{"actor":2,"pai":"2m","type":"tsumo"},{"actor":2,"pai":"9m","tsumogiri":false,"type":"dahai"},{"actor":3,"pai":"1s","type":"tsumo"},{"actor":3,"pai":"9s","tsumogiri":false,"type":"dahai"},{"actor":0,"pai":"2p","type":"tsumo"},{"actor":0,"pai":"S","tsumogiri":false,"type":"dahai"},{"actor":1,"pai":"8m","type":"tsumo"},{"actor":1,"pai":"1s","tsumogiri":false,"type":"dahai"},{"actor":2,"pai":"3m","type":"tsumo"},{"actor":2,"pai":"1p","tsumogiri":false,"type":"dahai"},{"actor":3,"pai":"1m","type":"tsumo"},{"actor":3,"pai":"P","tsumogiri":false,"type":"dahai"},{"actor":0,"pai":"N","type":"tsumo"},{"actor":0,"pai":"N","tsumogiri":true,"type":"dahai"},{"actor":1,"pai":"7p","type":"tsumo"},{"actor":1,"pai":"F","tsumogiri":false,"type":"dahai"},{"actor":2,"pai":"6m","type":"tsumo"},{"actor":2,"pai":"9s","tsumogiri":false,"type":"dahai"},{"actor":3,"pai":"7p","type":"tsumo"},{"actor":3,"pai":"1s","tsumogiri":false,"type":"dahai"}],"request":{"type":"chi","actor":0,"target":3,"pai":"1s","consumed":["2s","3s"]}})";               
        std::string err;
        const json11::Json input_json = json11::Json::parse(input_str, err);
        assert_with_out(!input_json["record"].is_null(), "game_server input error: record is null");
        Moves game_record;
        for (const json11::Json& action : input_json["record"].array_items()) {
            game_record.push_back(action);
        }
        assert_with_out(!input_json["request"].is_null(), "game_server input error: request is null");
        std::cout << game_server(game_record, input_json["request"]).dump() << std::endl;
    } else if (argc == 3 && strcmp(argv[1], "legal_action") == 0) {
        const std::string input_str = argv[2];
        //const std::string input_str = R"({"record":[{"type":"start_game","kyoku_first":0,"aka_flag":true,"names":["garucia","\\u30d5\\u30fc\\u30ac","\\u677e\\u5cf6","CLS"]},{"type":"start_kyoku","bakaze":"E","dora_marker":"7s","kyoku":1,"honba":0,"kyotaku":0,"oya":0,"scores":[25000,25000,25000,25000],"tehais":[["1m","2m","3m","6m","7m","7m","1p","4p","7p","8p","8p","6s","W"],["1m","2m","4m","5m","5m","8m","9m","2p","3s","5s","N","P","F"],["2m","6m","6m","3p","5p","6p","8s","9s","9s","9s","E","E","C"],["2m","3m","8m","3p","4p","6p","1s","5sr","7s","E","P","F","C"]]},{"type":"tsumo","actor":0,"pai":"1s"}]})";               
        std::string err;
        const json11::Json input_json = json11::Json::parse(input_str, err);
        assert_with_out(!input_json["record"].is_null(), "legal_action input error: record is null");
        Moves game_record;
        for (const json11::Json& action : input_json["record"].array_items()) {
            game_record.push_back(action);
        }
        std::vector<json11::Json> all_legal_single_action = get_all_legal_single_action(game_record);
        std::cout << json_vec_to_str(all_legal_single_action) << std::endl;
    } else if (argc == 3 && strcmp(argv[1], "legal_action_log_all") == 0) {
        std::string file_name = argv[2];
        const Moves game_record = load_json_vec_from_file(file_name);
        Moves current_game_record;
        std::vector<std::vector<json11::Json>> legal_actions;
        for (const json11::Json& action : game_record) {
            current_game_record.push_back(action);
            legal_actions.push_back(get_all_legal_single_action(current_game_record));
        }
        std::cout << json_vec2d_to_str(legal_actions) << std::endl;
    } else if (argc == 2 && strcmp(argv[1], "check") == 0) {
        //field_vis.set_haifu_log_from_file("haifu_log.json");
        const json11::Json& setup_match_json = load_json_from_file("setup_match.json");
        set_tactics(setup_match_json);
        Moves game_record;
        std::ifstream ifs("haifu_log.json");
        std::string str;
        if (ifs.fail()) {
            std::cerr << "失敗" << std::endl;
            return -1;
        }
        while (getline(ifs, str)) {
            std::string err;
            json11::Json action_json = json11::Json::parse(str, err);
            game_record.push_back(action_json);
        }
        std::vector<int> haiyama;
        proceed_game(haiyama, game_record, 0, -1, {});
        return 0;
    } else if (4 <= argc && strcmp(argv[1], "mjai_log") == 0) {
        const json11::Json& setup_mjai_json = load_json_from_file("setup_mjai.json");
        set_tactics_one(setup_mjai_json);
        std::string file_name = argv[2];
        int id = std::atoi(argv[3]);
        const int length = (5 <= argc) ? std::atoi(argv[4]) + 1 : -1;
        const Moves game_record = load_game_record_from_file(file_name, length);
        for (const auto& action : game_record) {
            std::cout << action.dump() << std::endl;
        }
        std::cout << "calculating review" << std::endl;
        auto review = ai_review(game_record, id);
        std::cout << review.dump() << std::endl;
        return 0;
    } else if (argc == 4 && strcmp(argv[1], "pipe") == 0) {
        const json11::Json& tactics = load_json_from_file(argv[2]);
        set_tactics_one(tactics);

        std::vector<json11::Json> game_record;
        int id = std::atoi(argv[3]);

        std::string str;
        std::string err;

        while (std::getline(std::cin, str)) {
            auto receive = json11::Json::parse(str, err);
            auto type = receive["type"];
            if (type == "error") {
                continue;
            }

            if (type == "start_kyoku") {
                // leaving start_game only, clear all the others
                game_record = std::vector<json11::Json>(game_record.begin(), game_record.begin() + 1);
            }

            game_record.push_back(receive);

            if (!receive["can_act"].is_null() && !receive["can_act"].bool_value()) {
                continue;
            }

            if (receive["actor"].is_null()) {
                continue;
            }
            auto actor = receive["actor"].int_value();

            if ((actor == id && type == "tsumo") ||
                (actor != id && type == "dahai")) {
                auto best_moves = ai(game_record, id, false);

                std::cout << json11::Json(best_moves).dump() << std::endl;
            }
        }
    } else if (argc == 4 && strcmp(argv[1], "pipe_detailed") == 0) {
        const json11::Json& tactics = load_json_from_file(argv[2]);
        set_tactics_one(tactics);

        std::vector<json11::Json> game_record;
        int id = std::atoi(argv[3]);

        std::string str;
        std::string err;

        while (std::getline(std::cin, str)) {
            auto receive = json11::Json::parse(str, err);
            auto type = receive["type"];
            if (type == "error") {
                continue;
            }

            if (type == "start_kyoku") {
                // leaving start_game only, clear all the others
                game_record = std::vector<json11::Json>(game_record.begin(), game_record.begin() + 1);
            }

            game_record.push_back(receive);

            if (receive["actor"].is_null()) {
                continue;
            }
            auto actor = receive["actor"].int_value();

            if ((actor == id && type == "tsumo") ||
                (actor != id && type == "dahai")) {
                auto best_moves = ai_review(game_record, id);
                std::cout << best_moves.dump() << std::endl;
            }
        }
    } else if (argc == 4 && strcmp(argv[1], "full_analyze") == 0) {
        const json11::Json& setup_mjai_json = load_json_from_file("setup_mjai.json");
        set_tactics_one(setup_mjai_json);
        std::string file_name = argv[2];
        const Moves game_record = load_json_vec_from_file(file_name);
        int id = std::atoi(argv[3]);
        std::vector<std::string> sp = str_split(file_name, '.');
        const std::vector<json11::Json> ret = full_analyze(game_record, id);
        dump_json_vec_to_file(ret, sp[0] + "_full_analyze." + sp[1]);
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "para_check") == 0) {
        #pragma omp parallel 
        { 
            printf("Hello World from %d\n", omp_get_thread_num()); 
        }
    } else if (argc == 3 && strcmp(argv[1], "stats") == 0) {
        std::string dir_name = argv[2];
        show_stats(dir_name);
        return 0;
    } else if (argc >= 2 && strcmp(argv[1], "stats_mjai") == 0) {
        std::string dir_name = argc >= 3 ? argv[2] : "";
        std::string player_name_prefix = argc >= 4 ? argv[3] : "Akochan";
        show_stats_mjai(dir_name, player_name_prefix);
        return 0;
    } else if (argc >= 2 && strcmp(argv[1], "mjai_client") == 0) {
        std::string mjai_json_name = argc >= 4 ? std::string(argv[3]) : "setup_mjai.json"; // mjaiを起動する際、jsonを指定しないとargv[3]に別の文字列が入り、jsonが指定されないので、起動時にshellの方でjsonを指定するべき。
        const json11::Json& setup_mjai_json = load_json_from_file(mjai_json_name);
        set_tactics_one(setup_mjai_json);
        const int port = (argc >= 3) ? std::atoi(argv[2]) : 11600;
        TcpClient tcp_client("127.0.0.1", port);
        int id = -1;
        MJAI_Interface mjai_interface;

        std::ofstream ofs_log;
        Moves best_moves;
        if (argc > 2)
            ofs_log.open(argv[2]);
    
        while(1) {
            std::string err;
            json11::Json recv = json11::Json::parse(tcp_client.ReadOneLine(), err);
            json11::Json response;
            std::string type = recv["type"].string_value();
      
            if (type == "start_game") {
                mjai_interface.push_start_game(recv, 4);
            } else {
                mjai_interface.push(recv);
            }

            if (argc > 2)
                ofs_log << recv.dump() << std::endl;
      
            if (type == "hello") {
                response = json11::Json::object {
                    {"type", "join"},
                    {"name", "Akochan"},
                    {"room", "default"},
                };
            } else if (type == "start_game") {
                id = recv["id"].int_value();
                response = json11::Json::object{
                      {"type", "none"},
                };
            } else if (type == "start_kyoku") {
                response = json11::Json::object{{"type", "none"}};
            } else if (type == "end_game") {
                for(const auto &json : mjai_interface.game_record)
                    std::cout << json.dump() << std::endl;
                break;
            } else if (type == "tsumo" && recv["actor"].int_value() == id) {
                best_moves = mjai_interface.get_best_move(id, false);
                std::cout << "best_moves for tsumo: " << best_moves[0]["type"].string_value() << std::endl;
                response = best_moves[0];
            } else if (type == "dahai" &&
                recv["actor"].int_value() != id &&
                recv["possible_actions"].array_items().size() != 0){
                best_moves = mjai_interface.get_best_move(id, false);
                response = best_moves[0];
            } else if ((type == "pon" || type == "chi") && recv["actor"] == id){
                response = best_moves[1];
            } else if (type == "error") {
                std::cerr << "recieved an error: " << recv.dump() << std::endl;
            } else if (type == "hora" || type == "ryukyoku") {
                response = json11::Json::object{{"type", "none"}};
            } else {
                response = json11::Json::object{{"type", "none"}};
            }
            std::cout << "response: " << response.dump() << std::endl;
            tcp_client.SendCommand(response.dump() + "\n");
        }
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "unit_test") == 0) {
        //exec_unit_test();
        ai_test();
    } else {    
        usage();
    }

    return 0;
}
