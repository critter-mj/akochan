#pragma once

#include "../share/include.hpp"


#include "../share/calc_shanten.hpp"
#include "combination.hpp"
#include "tehai_group.hpp"
#include "tehai_action.hpp"
#include "tehai_cal_work.hpp"
#include "betaori.hpp"

int mod2(const int x);

bool same_element_exist(const std::vector<int>& vec1, const std::vector<int>& vec2);

Hai_Array cal_using_array(const Hai_Array& tehai, const Fuuro_Vector& fuuro);

void set_tsumo_node_exec(
	Tehai_Change& tehai_change_new,
	bool is_in0,
    boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
    int& in0num
);

void set_tsumo_node_child(
	const Bit_Hai_Num& bit_hai_num,
	const int tehai_all_num,
	std::vector<int>& hai_in_pattern_nip,
	std::vector<int>& remain,
	int nin,
    boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
	int& in0num
);

void set_tsumo_node_mentu(
	const Bit_Hai_Num& bit_hai_num,
	const int tehai_all_num,
	Tehai_Analyzer ta_in,
    const int fuuro_cand_hai,
	const int in_num_end,
    boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
	int& in0num
);

void set_tenpai_prob_other(const int my_pid, const Game_State& game_state, const int tsumo_num, const std::array<float, 4>& tenpai_prob_now, double **tenpai_prob_other, double **reach_tenpai_prob_other);

float cal_other_end_prob_ako(const bool my_reach, const int act_num1, const int act_num2, const float other_end_prob_input);
float cal_tenpai_after_prob_ako(const bool my_reach, const int act_num1, const int act_num2, const float tenpai_prob_now);
void set_other_end_prob(const int my_pid, const int tsumo_num, const int act_num, const std::array<float, 4>& tenpai_prob_now, double *other_end_prob, double *reach_other_end_prob, const Game_State& game_state);

void set_exp_ryuukyoku_DP(
    const int my_pid, const int reach_regression_mode,
    const double * const * tenpai_prob_other, const double * const * reach_tenpai_prob_other,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp_ar,
    double exp_ryuukyoku[2], double& exp_ryuukyoku_ar
);

double cal_ori_exp_ako(
	const int my_pid, const int cn, const int gn, const int tn, const float wa[3], Tehai_Calculator_Work& tehai_calculator_work,
	double **houjuu_p_hai, double **houjuu_e_hai, double exp_other, const int tsumo_num, const int ori_choice_mode, double ryuukyoku_prob, double noten_ryuukyoku_value,
	const Hai_Array tehai_kind, const Fuuro_Vector fuuro_kind
);

double cal_ori_exp(
    const int my_pid, const int cn, const int gn, const int tn, Tehai_Calculator_Work& tehai_calculator_work,
    double **houjuu_p_hai, double **houjuu_e_hai, double exp_other, const int tsumo_num, const int ori_choice_mode, double ryuukyoku_prob, double noten_ryuukyoku_value,
    const Hai_Array tehai_kind, const Fuuro_Vector fuuro_kind, const Game_State& game_state
);

float get_coeff_for_ron_DP_ako(
    const int hai, const int my_pid, const Tehai_Analyzer_Basic& ta,
    const int pon_ron_flag[38], const int ron_flag[38], 
    const double my_tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, double** tenpai_prob_other, const int tn,
    const float reach_ron_ratio_para[5], const float not_reach_ron_ratio_para[5], const float dama_ron_ratio[6]
);

float get_coeff_for_ron_DP(
    const int hai, const int reach_regression_mode, const double reach_regression_coeff,
    const int my_pid, const Tehai_Analyzer_Basic& ta,
    const int pon_ron_flag[38], const int ron_flag[38], 
    const double my_tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, double** tenpai_prob_other, const int tn,
    const float dama_ron_ratio[6], const Game_State& game_state
);

void exec_calc_DP(
    const double reach_regression_coeff, const int ori_choice_mode,
    const int my_pid, const int tsumo_num,
    Tehai_Calculator_Work& tehai_calculator_work,
    const double ryuukyoku_prob_now, const double exp_ryuukyoku[2], const double exp_ryuukyoku_ar,
    const double exp_ryuukyoku_if_fuuro[2], const bool is_last_mode,
    double **houjuu_p_hai, double **reach_houjuu_p_hai, double **houjuu_e_hai, double **reach_houjuu_e_hai,
    double *other_end_prob, double *reach_other_end_prob, const double exp_other, const double exp_other_ar, const double exp_other_kan,
    const double my_tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, double** tenpai_prob_other,
    const Game_State& game_state, const Tactics& tactics
);