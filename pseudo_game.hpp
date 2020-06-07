#pragma once

#include "share/include.hpp"
#include "share/types.hpp"
#include "share/agari_ten.hpp"
#include "share/make_move.hpp"

class PG_Kyoku_Init {
    public:
    int bakaze;
    int kyoku;
    int kyotaku;
    int honba;
    std::array<int, 4> scores;
    PG_Kyoku_Init();
};

class PG_Kyoku_Result {
    public:
    int hora_player; // 流局は4
    int target_player;
    int han, fu;
    std::array<bool, 4> tenpai;
    PG_Kyoku_Result();
};

PG_Kyoku_Init get_next_kyoku_init(const PG_Kyoku_Init& kyoku_init, const PG_Kyoku_Result& kyoku_result);

PG_Kyoku_Result make_pg_kyoku_result_hora(const int hora_player, const int target_player, const int han, const int fuidx);
PG_Kyoku_Result make_pg_kyoku_result_ryukyoku(const std::array<bool, 4>& tenpai);

std::vector<float> read_acc_vec(const std::string& file_name);
int random_int_from_acc_vec(const std::vector<float>& acc_vec);
int random_int_uniform(const int size);

class Pseudo_Game_Generator {
    public:
    std::string game_rule;
    std::vector<float> result_type_prob_acc; // 0 ツモアガリ、1 ロンアガリ、2 流局
    std::vector<float> tsumo_han_prob_acc, tsumo_fu_prob_acc, ron_han_prob_acc, ron_fu_prob_acc;
    std::vector<float> ryukyoku_player_prob_acc; // 0 リーチ、1 テンパイ not リーチ、2 ノーテン
    Pseudo_Game_Generator();
    void read_parameter(const std::string& dir_name);
    PG_Kyoku_Result generate_kyoku();
    std::vector<PG_Kyoku_Init> generate_game();
    void generate_game_and_dump(const std::string& file_name);
};
