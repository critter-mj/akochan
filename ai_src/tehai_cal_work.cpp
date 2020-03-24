#include "tehai_cal_work.hpp"

Tehai_Calculator_Work::Tehai_Calculator_Work() {
	candidates_work.clear();
}

const std::array<int, 3>& Tehai_Calculator_Work::get_const_tsumo_edge_loc(const int cn, const int gn) const {
    const Tehai_Location& ta_loc = candidates_work[cn].ta_loc[gn];
	return cand_graph_sub_tsumo_loc[ta_loc.get_first()][ta_loc.get_second()];
}

const std::array<int, 3>& Tehai_Calculator_Work::get_const_fuuro_edge_loc(const int cn, const int gn) const {
    const Tehai_Location& ta_loc = candidates_work[cn].ta_loc[gn];
	return cand_graph_sub_fuuro_loc[ta_loc.get_first()][ta_loc.get_second()];
}

const std::array<int, 3>& Tehai_Calculator_Work::get_const_agari_loc(const int cn, const int gn) const {
	const Tehai_Location& ta_loc = candidates_work[cn].ta_loc[gn];
	return agari_graph_loc[ta_loc.get_first()][ta_loc.get_second()];
}