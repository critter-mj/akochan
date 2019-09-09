#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "mjutil.hpp"

int yakuhai_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int yakuhai, const Game_State& game_state);
int honitsu_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
int tanyao_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
int chanta_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);
int toitoi_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);
int sanshikidoukou_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);
int sanshiki_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);
int ittsuu_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);

double calc_yaku_dist(const int my_pid, const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state);