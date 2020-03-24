#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"

int factorial (int x);
int combination (int n, int k);
float my_logit(const float x);

void read_parameters(float* w, const int NPara, const std::string& file_name);
float logistic(const float* const w, const float* const x, const int DVec);
void MC_logistic(const float* const w, const float* const x, float* p, const int DVec, const int NClass);
void MC_logistic_mod(const float* const w, const float* const x, float* p, const int DVec, const int NClass);

bool is_aka_hai(const int hai);

int mod_pid(const int kyoku, const int oya, const int pid);

int get_other_reach_declared_num(const int my_pid, const Game_State& game_state);
int get_other_fuuro_num_max(const int my_pid, const Game_State& game_state);

std::array<bool, 4> get_reach_declared_array(const Game_State& game_state);
Hai_Array get_hai_visible_all(const Game_State& game_state);
int get_nokori_hai_num(const int my_pid, const Game_State& game_state);
Hai_Array get_hai_visible_wo_tehai(const int my_pid, const Game_State& game_state);
Hai_Array get_hai_visible_me(const int my_pid, const Game_State& game_state);

Hai_Array sum_hai_array(const Hai_Array& hai_array1, const Hai_Array& hai_array2);
Hai_Array cal_nokori_kind_array(const Hai_Array& visible_kind);
std::array<bool, 38> get_sute_kind_flag(const Kawa& kawa);
std::array<bool, 38> get_minogashi_flag(const Moves& game_record, const int target);
std::array<bool, 38> get_minogashi_ar_flag(const Moves& game_record, const Game_State& game_state, const int target);
std::array<bool, 38> get_sute_before_reach_flag(const Kawa& kawa);
int count_tedashi_num(const Kawa& kawa);
std::array<std::array<bool, 38>, 38> get_tas_flag(const Game_State& game_state, const int target);
std::array<std::array<std::array<bool, 38>, 7>, 3> get_tas_ryoukei_flag(const Game_State& game_state, const int target);
std::array<std::array<std::array<bool, 38>, 9>, 3> get_tas_kanchan_flag(const Game_State& game_state, const int target);
std::array<std::array<std::array<bool, 38>, 2>, 3> get_tas_penchan_flag(const Game_State& game_state, const int target);

int hai_dora_han(const std::vector<int>& dora_markerv, int hai);
void han_prob_shift(std::array<float, 14>& han_prob, const int shift_num);
void hanfu_prob_han_shift(std::array<std::array<float, 12>, 14>& hanfu_prob, const int shift_num);
void hanfu_prob_han_shift_with_prob(std::array<std::array<float, 12>, 14>& hanfu_prob, const std::array<float, 14>& shift_prob);

std::array<std::array<std::array<float, 12>, 14>, 4> cal_hanfu_prob_kan(
    const std::array<std::array<std::array<float, 12>, 14>, 4>& hanfu_prob,
    const std::array<float, 14>& shift_prob
);