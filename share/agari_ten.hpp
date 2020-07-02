#pragma once

#include "include.hpp"

static const int HORA_BASE_POINT = 8;

int ron_agari(const int han, const int fu, const bool oya_flag);
int tsumo_agari_loss(const int han, const int fu, const bool oya_flag);
int tsumo_agari(const int han, const int fu, const bool oya_flag);
std::array<int, 4> ten_move_hora(const int who, const int from_who, const int han);
std::array<int, 4> ten_move_ryukyoku(const std::array<bool, 4>& tenpai_flag);