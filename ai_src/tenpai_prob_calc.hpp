#pragma once

#include "../share/types.hpp"
#include "mjutil.hpp"

float infer_tenpai_prob_ako_old(const Kawa& kawa, const int fuuro_num);
float infer_tenpai_prob_ako(const Moves& game_record, const Game_State& game_state, const int target);

bool somete_possible(const Fuuro_Vector& fuuro, const Color_Type color);
std::array<int, 4> get_somete_feature(const Kawa& kawa, const Color_Type color);
std::array<int, 2> get_somete_tenpai_post_feature(const Kawa& kawa, const Color_Type color);
float cal_somete_ako(const Game_State& game_state, const int target, const Color_Type color);
float cal_somete_tenpai_post(const Game_State& game_state, const int target, const Color_Type color);