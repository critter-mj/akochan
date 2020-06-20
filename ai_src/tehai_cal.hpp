#pragma once

#include "../share/include.hpp"

#include "tehai_group.hpp"
#include "tehai_action.hpp"
#include "tehai_cal_work.hpp"
#include "betaori.hpp"
#include "tehai_cal_supl.hpp"
#include "tactics.hpp"

class Tehai_Calculator{
	public:
	Tehai_Calculator();
	Tehai_Calculator(int my_pid_in);

	void reset(int my_pid_in, bool aka_flag_in);

	//int group_size[MAX_CANDIDATES_NUM];
	//std::pair<int, int> location[MAX_CANDIDATES_NUM][MAX_GROUP_NUM];
	
	int in_flag[38];
	int out_flag[38];
	int effective_flag[38];
	std::vector<Tehai_Group> candidates_sub;

	boost::unordered_set<Tehai_Change> tcs;
	//std::unordered_set<Tehai_Change, Tehai_Change_Hash, Tehai_Change_EqualTo> tcs;
	boost::unordered_set<Tehai_Change> tcs_sub;

	int my_pid;
	bool aka_flag;
	int tehai_all_num;
	Bit_Hai_Num bit_hai_num;
	int in0num;
	int in0num_sub;
	int fuuro_cand_hai;
	int cnm_restrict_flag;
	std::vector<int> cnm_restriction;

	double rn_para;

	void set_inout_flag(Tehai_Analyzer ta_in);
	void get_effective(const Game_State& game_state, Tehai_Analyzer tehai_analyzer);

	const Tehai_Analyzer_Basic& get_const_ta(const Tehai_Location loc) const;
	const Tehai_Analyzer_Basic& get_const_ta_cgn(const int cn, const int gn) const;
	Tehai_Analyzer_Basic& get_ref_ta_cgn(const int cn, const int gn);

	size_t candidates_size() const;
	size_t group_size(const int cn) const;

	double get_agari_prob(const int cn, const int gn, const int tn) const;
	std::array<double, 5> get_agari_han_prob(const int cn, const int gn, const int tn) const;
	double get_agari_exp(const int cn, const int gn, const int tn) const;
	double get_tenpai_prob(const int cn, const int gn, const int tn) const;
	double get_ten_exp(const int cn, const int gn, const int tn) const;
	double get_ori_exp(const int cn, const int gn, const int tn) const;

	// merge_candidatesは今は使っていないが、将来的にcandidates列挙に時間がかかる場合に利用する想定
	void merge_candidates_child(
		std::vector<Tehai_Group>& cand1,
		boost::unordered_set<Tehai_Change>& tcs1,
		std::vector<Tehai_Group>& cand2
	);

	void merge_candidates(
		std::vector<std::vector<Tehai_Group>>& candvv,
		std::vector<boost::unordered_set<Tehai_Change>>& tcsv
	);

	void set_tsumo_node_1out(
		const Tehai_Analyzer& ta_in
	);

	void set_tsumo_node_1in_1out(
		const Game_State& game_state,
		const Tehai_Analyzer_Basic& ta_in,
		Tehai_Change& tehai_change_tmp_new
	);

	void set_tsumo_node_titoi(
		Tehai_Analyzer ta_in,
		int in_num_end
	);

	void set_tsumo_edge(
		Tehai_Change tehai_change_tmp, const int thread_num
	);

	void set_tsumo_edge2(
		//Tehai_Change& tehai_change_tmp
		Tehai_Change tehai_change_tmp, const int thread_num
	);

	void add_fuuro_node(
		const Game_State& game_state,
		int cn_start, int cn_end, int fuuro_num_begin, int fuuro_num_end, int must_effective_flag, int fuuro_must,
		boost::unordered_map<Tehai_State2, int>* ts_maps
		//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps
	);

	void set_candidates3_sub(Tehai_Analyzer tehai_analyzer, const int change_num);
	void set_candidates3_single_thread(const Game_State& game_state, Tehai_Analyzer tehai_analyzer, Tehai_Analyzer tehai_analyzer_af, const int cn_max, const Tactics& tactics);
	void set_candidates3_multi_thread(const Moves& game_record, const Game_State& game_state, const std::array<bool, 38> sute_flag, Tehai_Analyzer tehai_analyzer, const Tactics& tactics);

	void set_cand_graph_sub_child(
		int cn1, int gn1, int cn2, int hai_in, int hai_out, Action_Type action_id, int hai0, int hai1, int hai2,
		boost::unordered_map<Tehai_State2, int>* ts_maps,
		//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps
		const int thread_num
	);
	void set_cand_graph_sub(
		const int cn1, const std::vector<std::array<int, 3>>& tsumo_edge_array,
		boost::unordered_map<Tehai_State2, int>* ts_maps,
		//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps
		const int thread_num
	);

	int get_max_group_size();
	int get_max_edge_size();
	int get_sub_num_all();

	void set_agari_shanten_num(const Game_State& game_state);
	int get_fuuro_agari_shanten_num(const Game_State& game_state, const Hai_Array& current_tehai, const bool chi_flag);

	void set_agari_exp(
		const Game_State& game_state,
    	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
	);

	void calc_agari_prob(
		const int tsumo_num, const double exp_min, const Game_State& game_state,
    	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp	
	);

	void calc_DP(
		const int act_num, const int tsumo_num, const double exp_other, const double exp_other_ar, const double exp_other_kan, const std::array<float, 4>& tenpai_prob_now,
		const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_value,
		const int ori_choice_mode,
		const double reach_regression_coeff,
		double my_tenpai_prob,
		double ryuukyoku_prob_now,
		const Hai_Array& hai_visible_kind, const Game_State& game_state,
    	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp,
		const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& exp_ryuukyoku,
		const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& exp_ryuukyoku_ar,
		const Tactics& tactics
	);
};
