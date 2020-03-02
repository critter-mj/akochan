#pragma once

#include "../share/include.hpp"

#include "mjutil.hpp"

std::array<std::array<float, 4>, 4> calc_jun_prob_end(const std::array<int, 4>& ten, const int oya_first);
std::array<float, 24> infer_game_result_prob_ako(const std::array<int, 4>& score, const int kyoku_idx);
std::array<int, 16> score_list_to_4onehot(const std::array<int, 4>& score);

bool chan_end_oya(const std::array<int, 4>& ten, const int oyaid);
bool chan_end_oya_w4(const std::array<int, 4>& ten, const int oyaid);
std::array<std::array<float, 4>, 4> calc_jun_prob(const int kyoku, const std::array<int,4>& ten, const int oya, const bool is_renchan, const json11::Json& tactics_json);
