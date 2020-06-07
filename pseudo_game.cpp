#include "pseudo_game.hpp"

PG_Kyoku_Init::PG_Kyoku_Init(){}
PG_Kyoku_Result::PG_Kyoku_Result(){}

PG_Kyoku_Init get_next_kyoku_init(const PG_Kyoku_Init& kyoku_init, const PG_Kyoku_Result& kyoku_result, const std::string& game_rule) {
    PG_Kyoku_Init next_kyoku_init = kyoku_init;
    const int oya = kyoku_init.kyoku - 1;
    
    std::array<int, 4> ten_move;
    if (kyoku_result.hora_player < 4) {
        ten_move = ten_move_hora(kyoku_result.hora_player, kyoku_result.target_player, kyoku_result.han);
        next_kyoku_init.kyotaku = 0;
    } else {
        ten_move = ten_move_ryukyoku(kyoku_result.tenpai);
    }
    for (int pid = 0; pid < 4; pid++) {
        next_kyoku_init.scores[pid] += ten_move[pid];
    }
    if (is_tobi_any(next_kyoku_init.scores)) {
        next_kyoku_init.bakaze = -1;
        next_kyoku_init.kyoku = -1;
        next_kyoku_init.honba = 0;
    } else if (kyoku_result.hora_player == oya ||
              (kyoku_result.hora_player == 4 && kyoku_result.tenpai[oya])
    ) {
        if ((is_last_kyoku(kyoku_init.bakaze, kyoku_init.kyoku, game_rule) && next_kyoku_init.scores[oya] >= 30000 && has_highest_score(next_kyoku_init.scores, oya)) ||
            (is_definite_last_kyoku(kyoku_init.bakaze, kyoku_init.kyoku, game_rule) && has_highest_score(next_kyoku_init.scores, oya))
        ) {
            next_kyoku_init.bakaze = -1;
            next_kyoku_init.kyoku = -1;
            next_kyoku_init.honba = 0;
        } else {
            next_kyoku_init.honba++;
        }
    } else {
        if ((is_last_kyoku(kyoku_init.bakaze, kyoku_init.kyoku, game_rule) && is_30000_any(next_kyoku_init.scores)) ||
            is_definite_last_kyoku(kyoku_init.bakaze, kyoku_init.kyoku, game_rule)
        ) {
            next_kyoku_init.bakaze = -1;
            next_kyoku_init.kyoku = -1;
            next_kyoku_init.honba = 0;
        } else {
            next_kyoku_init.bakaze = kyoku_init.kyoku == 4 ? kyoku_init.bakaze + 1 : kyoku_init.bakaze;
            next_kyoku_init.kyoku  = kyoku_init.kyoku == 4 ? 1 : kyoku_init.kyoku + 1;
            next_kyoku_init.honba  = kyoku_result.hora_player == 4 ? next_kyoku_init.honba + 1 : 0;
        }
    }
    return next_kyoku_init;
}

PG_Kyoku_Result make_pg_kyoku_result_hora(const int hora_player, const int target_player, const int han, const int fuidx) {
    PG_Kyoku_Result kyoku_result;
    kyoku_result.hora_player = hora_player;
    kyoku_result.target_player = target_player;
    kyoku_result.han = han;
    if (fuidx == 1) {
		kyoku_result.fu = 20;
	} else if (fuidx == 2) {
		kyoku_result.fu = 25;
	} else {
		kyoku_result.fu = fuidx * 10;
	}
    return kyoku_result;
    // リーチ状況に関しては考慮しないことにする。
}

PG_Kyoku_Result make_pg_kyoku_result_ryukyoku(const std::array<bool, 4>& tenpai) {
    PG_Kyoku_Result kyoku_result;
    kyoku_result.hora_player = 4;
    kyoku_result.tenpai = tenpai;
    return kyoku_result;
}

std::vector<float> read_acc_vec(const std::string& file_name) {
	FILE *fp;
	fp = fopen(file_name.c_str(), "r");
	assert_with_out(fp != NULL, "read_acc_vec file_not_found:" + file_name);

    std::vector<float> acc;
	double tmpd;
	while(fscanf(fp, "%lf", &tmpd) != EOF){
        if (acc.size() == 0) {
            acc.push_back(tmpd);
        } else {
            acc.push_back(acc[acc.size() - 1] + tmpd);
        }
        assert_with_out(acc[acc.size() - 1] < 1.0001, "read_acc value error1");
	}
    fclose(fp);
    for (int i = 0; i < acc.size(); i++) {
        acc[i] = std::min(acc[i], (float)1.0);
    }
    assert_with_out(0.9999 < acc[acc.size() - 1], "read_acc value error2:" + file_name + " " + std::to_string(acc[acc.size() - 1]));
	return acc;
}

int random_int_from_acc_vec(const std::vector<float>& acc_vec) {
    const float rvalue = (float)std::rand() / RAND_MAX; // 乱数に関して精度はそれほど重要でないと思われる。
    for (int i = 0; i < acc_vec.size(); i++) {
        if (rvalue < acc_vec[i]) {
            return i;
        }
    }
    return acc_vec.size() - 1;
}

int random_int_uniform(const int size) {
    std::vector<float> acc_vec;
    for (int i = 0; i < size; i++) {
        acc_vec.push_back((float)(i + 1)/size);
    }
    return random_int_from_acc_vec(acc_vec);
}

Pseudo_Game_Generator::Pseudo_Game_Generator(){
    result_type_prob_acc.resize(3); ryukyoku_player_prob_acc.resize(3);
    tsumo_han_prob_acc.resize(14); ron_han_prob_acc.resize(14);
    tsumo_fu_prob_acc.resize(12); ron_fu_prob_acc.resize(12);
    std::fill(result_type_prob_acc.begin(), result_type_prob_acc.end(), 0.0);
    std::fill(tsumo_han_prob_acc.begin(), tsumo_han_prob_acc.end(), 0.0);
    std::fill(tsumo_fu_prob_acc.begin(), tsumo_fu_prob_acc.end(), 0.0);
    std::fill(ron_han_prob_acc.begin(), ron_han_prob_acc.end(), 0.0);
    std::fill(ron_fu_prob_acc.begin(), ron_fu_prob_acc.end(), 0.0);
    std::fill(ryukyoku_player_prob_acc.begin(), ryukyoku_player_prob_acc.end(), 0.0);
}

void Pseudo_Game_Generator::read_parameter(const std::string& dir_name) {
    result_type_prob_acc = read_acc_vec(dir_name + "/prob_kyoku_result_type.txt");
    tsumo_han_prob_acc = read_acc_vec(dir_name + "/prob_tsumo_han.txt");
    ron_han_prob_acc = read_acc_vec(dir_name + "/prob_ron_han.txt");
    tsumo_fu_prob_acc = read_acc_vec(dir_name + "/prob_tsumo_fu.txt");
    ron_fu_prob_acc = read_acc_vec(dir_name + "/prob_ron_fu.txt");
    ryukyoku_player_prob_acc = read_acc_vec(dir_name + "/prob_ryukyoku_player_type.txt");
    assert_with_out(result_type_prob_acc.size() == 3, "pseudo_game_generator read_parameter_error result_type_prob");
    assert_with_out(tsumo_han_prob_acc.size() == 14, "pseudo_game_generator read_parameter_error tsumo_han_prob");
    assert_with_out(ron_han_prob_acc.size() == 14, "pseudo_game_generator read_parameter_error ron_han_prob");
    assert_with_out(tsumo_fu_prob_acc.size() == 12, "pseudo_game_generator read_parameter_error tsumo_fu_prob");
    assert_with_out(ron_fu_prob_acc.size() == 12, "pseudo_game_generator read_parameter_error ron_fu_prob");
    assert_with_out(ryukyoku_player_prob_acc.size() == 3, "pseudo_game_generator read_parameter_error ryukyoku_player_prob");
}

PG_Kyoku_Result Pseudo_Game_Generator::generate_kyoku() {
    const int result_type = random_int_from_acc_vec(result_type_prob_acc);
    if (result_type == 0) {
        const int hora_player = random_int_uniform(4);
        const int han_tsumo = random_int_from_acc_vec(tsumo_han_prob_acc);
        const int fuidx_tsumo = random_int_from_acc_vec(tsumo_fu_prob_acc);
        return make_pg_kyoku_result_hora(hora_player, hora_player, han_tsumo, fuidx_tsumo);
    } else if (result_type == 1) {
        const int hora_player = random_int_uniform(4);
        const int target_player = (hora_player + 1 + random_int_uniform(3)) % 4;
        const int han_ron = random_int_from_acc_vec(ron_han_prob_acc);
        const int fuidx_ron = random_int_from_acc_vec(ron_fu_prob_acc);
        return make_pg_kyoku_result_hora(hora_player, target_player, han_ron, fuidx_ron);
    } else if (result_type == 2) {
        std::array<bool, 4> tenpai;
        for (int pid = 0; pid < 4; pid++) {
            const int ryukyoku_player_type = random_int_from_acc_vec(ryukyoku_player_prob_acc);
            if (ryukyoku_player_type == 0) {
                tenpai[pid] = true;
            } else if (ryukyoku_player_type == 1) {
                tenpai[pid] = true;
            } else if (ryukyoku_player_type == 2) {
                tenpai[pid] = false;
            } else { assert_with_out(false, "PG_Kyoku_Generator ryukyoku_player_type error!"); }
        }
        return make_pg_kyoku_result_ryukyoku(tenpai);
    } else { assert_with_out(false, "PG_Kyoku_Generator result_type error!"); return PG_Kyoku_Result(); }
}

std::vector<PG_Kyoku_Init> Pseudo_Game_Generator::generate_game() {
    std::vector<PG_Kyoku_Init> game_info;
    PG_Kyoku_Init kyoku_init;
    kyoku_init.bakaze = 0;
    kyoku_init.kyoku = 1;
    kyoku_init.kyotaku = 0;
    kyoku_init.honba = 0;
    std::fill(kyoku_init.scores.begin(), kyoku_init.scores.end(), 25000);
    while (kyoku_init.bakaze != -1) {
        game_info.push_back(kyoku_init);
        kyoku_init = get_next_kyoku_init(kyoku_init, generate_kyoku(), game_rule);
    }
    std::cout << "end_game:" << kyoku_init.scores[0] << " " << kyoku_init.scores[1] << " " << kyoku_init.scores[2] << " " << kyoku_init.scores[3] << std::endl;
    assert_with_out(kyoku_init.bakaze == -1 && kyoku_init.kyoku == -1, "Pseudo_Game_Generator generate_game error!");
    game_info.push_back(kyoku_init);
    return game_info;
}

void Pseudo_Game_Generator::generate_game_and_dump(const std::string& file_name) {
    std::vector<PG_Kyoku_Init> game_info = generate_game();
    std::ofstream ofs(file_name);
    for (int i = 0; i < game_info.size(); i++) {
        if (i < game_info.size() - 1) {
            ofs << make_start_kyoku(game_info[i].bakaze, game_info[i].kyoku, game_info[i].honba, game_info[i].kyotaku, game_info[i].scores).dump() << std::endl;
        } else {
            ofs << make_end_game(game_info[i].scores).dump() << std::endl;
        }
    }
    ofs.close();
}
