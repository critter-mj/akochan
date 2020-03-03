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
  printf("     file: path of logfile\n");
  printf("     id: 0,1,2,3\n");
  printf("  > system.exe full_analyze <file> <id>\n");
  printf("     file: path of logfile\n");
  printf("     id: 0,1,2,3\n");
  printf("  > system.exe para_check\n");
  printf("  > system.exe stats [<dir_name> [<chicha_num>]]\n");
  printf("     dir_name: default \"\"\n");
  printf("     chicha_num: default 2\n");
  printf("  > system.exe stats_mjai [<dir_name> [player_name_prefix]\n");
  printf("     dir_name: default \"\"\n");
  printf("     player_name_prefix: default \"Akochan\"\n");
  printf("  > system.exe mjai_client\n");
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
                Game_Phase game_phase = GP_AI;
                game_settings.chicha = chicha.int_value();
                assert(is_valid_player(game_settings.chicha));
                game_settings.player_id = -1;
                do_game_one_player(haiyama, game_record, game_settings.chicha, game_settings.player_id, {}, game_phase);
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
            Game_Phase game_phase = GP_AI;
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

            do_game_one_player(haiyama, game_record, game_settings.chicha, game_settings.player_id, {}, game_phase);
            dump_json_vec_to_file(game_record, match_dir_name + "/haifu_log_" + std::to_string(seed) + "_" + std::to_string(game_settings.chicha) + ".json");
        }
        return 0;
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
        Game_Phase game_phase = GP_AI;
        proceed_game(haiyama, game_record, 0, -1, {}, game_phase);
        return 0;
    } else if (argc == 4 && strcmp(argv[1], "mjai_log") == 0) {
        const json11::Json& setup_mjai_json = load_json_from_file("setup_mjai.json");
        set_tactics_one(setup_mjai_json);
        std::string file_name = argv[2];
        const Moves game_record = load_json_vec_from_file(file_name);
        int id = std::atoi(argv[3]);
        for (const auto& action : game_record) {
            std::cout << action.dump() << std::endl;
        }
        std::cout << "calculating best_moves" << std::endl;
        auto best_moves = ai(game_record, id, false);
        for(const auto &json : best_moves) {
            std::cout << json.dump() << std::endl;
        }
        return 0;
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
    } else if (argc >= 2 && strcmp(argv[1], "stats") == 0) {
        std::string dir_name = argc >= 3 ? argv[2] : "";
        const int chicha_num = argc >= 4 ? atoi(argv[3]) : 2;
        if (dir_name != "") { dir_name += "/"; }
        show_stats(dir_name, chicha_num);
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
        int id;
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
                mjai_interface.push_start_game(recv, 4, true);
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
            } else if (type == "reach" && recv["actor"] == id) {
                response = best_moves[1];
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
    } else {
        usage();
    }

    return 0;
}
