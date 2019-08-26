#pragma once

#include "../share/include.hpp"
#include "tehai_group.hpp"
#include "tehai_action.hpp"

class Tehai_Calculator_Work {
	public:
	Tehai_Calculator_Work();

	boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM> candidates_work;
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> cal_tav_work;

	std::array<boost::container::static_vector<Tsumo_Edge, MAX_TSUMO_EDGE_NUM_PER_THREAD>, CAL_NUM_THREAD> tsumo_edge_work;
	std::array<boost::container::static_vector<Tehai_Action, MAX_EDGE_NUM_PER_THREAD>, CAL_NUM_THREAD> cand_graph_sub_tsumo_work;
	std::array<boost::container::static_vector<Tehai_Action, MAX_EDGE_NUM_PER_THREAD>, CAL_NUM_THREAD> cand_graph_sub_fuuro_work;
	std::array<boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_graph_work;

	std::array<std::array<std::array<int, 3>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> cand_graph_sub_tsumo_loc;
	std::array<std::array<std::array<int, 3>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> cand_graph_sub_fuuro_loc;
	std::array<std::array<std::array<int, 3>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_graph_loc;

	//std::array<std::array<std::array<float, 38>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> tsumo_best_tmp;
	//std::array<std::array<std::array<float, 38>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> fuuro_best_tmp;

	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_prob_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_exp_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_prob_to_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_exp_to_work;

	std::array<std::array<std::array<std::array<double, 5>, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_han_prob_work;
	std::array<std::array<std::array<std::array<double, 5>, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> agari_han_prob_to_work;

	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> tenpai_prob_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> tenpai_prob_to_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> ten_exp_work;
	std::array<std::array<std::array<double, 2>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> ten_exp_to_work;

	//std::array<std::array<std::array<double, MAX_TSUMO_NUM>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> ori_exp_work;
	std::array<std::array<double, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD> ori_exp_work;

    const std::array<int, 3>& get_const_tsumo_edge_loc(const int cn, const int gn) const;
    const std::array<int, 3>& get_const_fuuro_edge_loc(const int cn, const int gn) const;
	const std::array<int, 3>& get_const_agari_loc(const int cn, const int gn) const;
};