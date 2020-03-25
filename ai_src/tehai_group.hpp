#pragma once

#include "../share/include.hpp"
#include "agari.hpp"
#include "tehai_ana.hpp"
#include "tehai_change.hpp"

#include "tehai_state2.hpp"

const int CAL_NUM_THREAD = NPROCS;

const int MAX_CANDIDATES_NUM = 200000;
const int MAX_TA_NUM_PER_THREAD = 100000;
const int BUF_FOR_REACH_PER_THREAD = 10000;
const int MAX_EDGE_NUM_PER_THREAD = 100000 * 64;
const int MAX_TSUMO_EDGE_NUM_PER_THREAD = 100000 * 16;
const int MAX_TA_NUM_PER_GROUP = 128;
const int BUF_FOR_REACH_PER_GROUP = 10;

const int MAX_TSUMO_NUM = 20;
// ツモ数は2でないが、メモリ的には2あれば十分なのでこのようになった。十分でないときは要修正

class Tehai_Location {
	public:
	Tehai_Location();
	Tehai_Location(const int first, const int second);
	uint32_t data;

	int get_first() const;
	int get_second() const;
};

int loc_intvec(const std::vector<int>& vec, const int num);
Fuuro_Type fuuro_action_type_to_fuuro_type(const int fuuro_action_type);

class Tehai_Group{
	public:
	Tehai_Group();
	void reset();

	Tehai_Change tehai_change;
	boost::container::static_vector<Tehai_Location, MAX_TA_NUM_PER_GROUP> ta_loc;

	void set_tav_init(
		const Bit_Hai_Num& tehai_bit_original,
		const Tehai_State2& ts,
		const int fuuro_cand_hai,
		boost::unordered_map<Tehai_State2, int>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);
	
	void add_fuuro_child(
		Tehai_Analyzer_Basic& tehai_analyzer,
		int fuuro_cand[38], const int fuuro_cand_copy[38], int fuuro_num_begin, int fuuro_num_end, int fuuro_must, int kan_cand[38],
		boost::unordered_map<Tehai_State2,int>& ts_map,
		//std::unordered_map<Tehai_State, int, Tehai_State_Hash>& ts_map,
		const int fuuro_type, int hai0, int hai1, int hai2, int hai3,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);
	void add_fuuro(
		Tehai_Analyzer_Basic tehai_analyzer,
		int fuuro_cand[38], const int fuuro_cand_copy[38], int fuuro_num_begin, int fuuro_num_end, int fuuro_must, int kan_cand[38],
		boost::unordered_map<Tehai_State2, int>& ts_map,
		//std::unordered_map<Tehai_State, int, Tehai_State_Hash>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);

	void add_ankan(
		const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);

	void add_daiminkan(
		const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);

	void add_kakan(
		const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);

	void analyze_all_tenpai(
		const int my_pid, const Game_State& game_state,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav
	);

	void add_reach(
		boost::unordered_map<Tehai_State2, int>& ts_map,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		const int thread_num
	);

	void analyze_all_agari(
		const int my_pid, const Game_State& game_state,
		std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
		boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>& agari_graph,
		//boost::container::static_vector<Agari_Basic, 10>* graph_agariv_cn
		std::array<std::array<std::array<int, 3>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& agari_graph_loc,
		const int thread_num
	);
};
