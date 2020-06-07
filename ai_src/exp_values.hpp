#pragma once

#include "../share/include.hpp"
#include "mjutil.hpp"
#include "betaori.hpp"
//#include "cal_result.hpp"

float get_ryuukyoku_pattern_prob(const std::array<int, 4>& flag, const std::array<float, 4>& keiten_prob);
int cal_tsumo_num_exp(const int my_pid, const Game_State& game_state, const int dahai_inc, const std::array<float, 4> tenpai_prob); // 残り行動回数推定。

float cal_my_agari_prob(const int my_pid, const Moves& game_record, const float agari_prob_sol, const Game_State& game_state, const int dahai_inc, const std::array<float, 4>& tenpai_prob); // アガリ方策1人麻雀結果からのアガリ確率推定。
float cal_my_agari_value(const float agari_prob_sol, const float value_sol, const float value_not_agari); // アガリ方策1人麻雀からのアガリ価値推定。

float cal_ryuukyoku_prob(const int my_pid, const Game_State& game_state, const float my_agari_prob, const std::array<float, 4>& tenpai_prob, const int dahai_inc); // 自分がアガリできないときに流局する確率
float cal_my_keiten_prob(const int my_pid, const Game_State& game_state,  const int dahai_inc, const float keiten_prob_sol); // 流局するときにテンパイしている確率
float cal_other_keiten_prob(const int my_pid, const int target_pid, const Game_State& game_state,  const int dahai_inc, const float current_tenpai_prob);
std::array<float, 4> cal_keiten_prob(const int my_pid, const std::array<bool, 4>& reach_flag, const float keiten_prob_sol, const std::array<float, 4>& tenpai_prob);
float cal_ryuukyoku_value(const std::array<float, 4>& keiten_prob, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp);
float cal_noten_ryuukyoku_value(const int my_pid, const std::array<float, 4>& keiten_prob, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp);

std::array<std::array<float, 4>, 4> cal_target_prob(const int my_pid, const std::array<float, 4>& risk_array, const Game_State& game_state, const int dahai_inc, const int fuuro_num_inc); // あるプレイヤがアガリをする時、誰からアガリをするかの推定
std::array<std::array<float, 4>, 4> cal_target_prob_other(const int my_pid); // あるプレイヤがアガリをする時、誰からアガリをするかの推定(自分は関与しない想定)

float cal_target_value_child (
	const std::array<std::array<float, 12>, 14>& kyoku_end_pt_exp, const std::array<std::array<float, 12>, 14>& han_prob
);

std::array<std::array<float, 4>, 4> cal_target_value(
	const int my_pid, const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp, const float my_agari_value,
	const std::array<std::array<std::array<float, 12>, 14>, 4>& tsumo_prob, const std::array<std::array<std::array<float, 12>, 14>, 4>& ron_prob
); // あるプレイヤが誰かからアガリをするときの価値

std::array<float, 5> cal_kyoku_result_prob(const int my_pid, const Game_State& game_state, const int dahai_inc, const float my_agari_prob, const std::array<float, 4>& tenpai_prob, const float ryuukyoku_prob); // 各プレイヤのアガリ率推定

std::array<float, 5> cal_kyoku_result_value(
	const std::array<std::array<float, 4>, 4>& target_prob, const std::array<std::array<float, 4>, 4>& target_value, const float ryuukyoku_value
);

std::array<float, 4> cal_risk_array(const int my_pid, const Game_State& game_state, const int dahai_inc, const Hai_Array& tehai, const std::array<float, 4> tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob);

float cal_exp(
	const int my_pid, const Moves& game_record, const Game_State& game_state, const Hai_Array& result_tehai, const float agari_prob_sol, const float value_sol, const float value_not_agari, const float keiten_prob_sol,
	const std::array<float, 4>& tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, const std::array<std::array<float, 100>, 4>& tsumo_prob, const std::array<std::array<float, 100>, 4>& ron_prob,
	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp,
	const int dahai_inc, const int fuuro_inc
);

float cal_passive_ryuukyoku_value(
	const int my_pid, const Game_State& game_state, const std::array<float, 4>& tenpai_prob, 
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp
);

float cal_other_end_value(
	const int my_pid, const Game_State& game_state, const std::array<float, 4>& tenpai_prob,
	const std::array<std::array<float, 100>, 4>& tsumo_prob, const std::array<std::array<float, 100>, 4>& ron_prob,
	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
);

float cal_betaori_exp(
	const int my_pid, const Game_State& game_state,
	const Hai_Array& tehai_tmp, const std::array<float, 38>& betaori_houjuu_hai_prob, const std::array<float, 38>& total_houjuu_hai_value,
	const float not_agari_value, const float other_end_value, const float passive_ryuukyoku_value, const float passive_ryuukyoku_prob,
	const int tsumo_num_exp, const Tactics& tactics
);
