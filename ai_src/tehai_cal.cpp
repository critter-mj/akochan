#include "tehai_cal.hpp"

extern const bool out_console;
extern std::mutex mtx;

extern Tehai_Calculator_Work tehai_calculator_work;

Tehai_Calculator::Tehai_Calculator(){
	reset(0, true);
}

Tehai_Calculator::Tehai_Calculator(int my_pid_in){
	reset(my_pid_in, true);
}

void Tehai_Calculator::reset(int my_pid_in, bool aka_flag_in){
	my_pid = my_pid_in;
	aka_flag = aka_flag_in;
	for(int hai=0;hai<38;hai++){
		in_flag[hai] = 0;
		out_flag[hai] = 0;
		effective_flag[hai] = 0;
	}
	tehai_all_num = 0;
	fuuro_cand_hai = 0;
	//fuuro_num_max = 0;
	cnm_restrict_flag = 0;
	rn_para = 1.0;
	cnm_restriction.erase(cnm_restriction.begin(), cnm_restriction.end());
	tehai_calculator_work.candidates_work.clear();
	candidates_sub.clear();
	tcs.clear();
	tcs_sub.clear();

	for (int i = 0; i < CAL_NUM_THREAD; ++i) {
		tehai_calculator_work.cal_tav_work[i].clear();
		tehai_calculator_work.agari_graph_work[i].clear();
	}
}

const Tehai_Analyzer_Basic& Tehai_Calculator::get_const_ta(const Tehai_Location loc) const {
	return tehai_calculator_work.cal_tav_work[loc.get_first()][loc.get_second()];
}

const Tehai_Analyzer_Basic& Tehai_Calculator::get_const_ta_cgn(const int cn, const int gn) const {
	return get_const_ta(tehai_calculator_work.candidates_work[cn].ta_loc[gn]);
}

Tehai_Analyzer_Basic& Tehai_Calculator::get_ref_ta_cgn(const int cn, const int gn) {
	return tehai_calculator_work.cal_tav_work[tehai_calculator_work.candidates_work[cn].ta_loc[gn].get_first()][tehai_calculator_work.candidates_work[cn].ta_loc[gn].get_second()];
}

size_t Tehai_Calculator::candidates_size() const {
	return tehai_calculator_work.candidates_work.size();
}

size_t Tehai_Calculator::group_size(const int cn) const {
	return tehai_calculator_work.candidates_work[cn].ta_loc.size();
}

double Tehai_Calculator::get_agari_prob(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.agari_prob_work[loc.get_first()][loc.get_second()][mod2(tn)];
}

std::array<double, 5> Tehai_Calculator::get_agari_han_prob(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.agari_han_prob_work[loc.get_first()][loc.get_second()][mod2(tn)];
}

double Tehai_Calculator::get_agari_exp(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.agari_exp_work[loc.get_first()][loc.get_second()][mod2(tn)];
}

double Tehai_Calculator::get_tenpai_prob(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.tenpai_prob_work[loc.get_first()][loc.get_second()][mod2(tn)];
}

double Tehai_Calculator::get_ten_exp(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.ten_exp_work[loc.get_first()][loc.get_second()][mod2(tn)];
}

double Tehai_Calculator::get_ori_exp(const int cn, const int gn, const int tn) const {
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	return tehai_calculator_work.ori_exp_work[loc.get_first()][loc.get_second()];
	// この関数はselectorでtn = tsumo_num_DPの時にしか呼ばれていない。exec_calc_DPを実行すると、最終的にはtn = tsumo_num_DPのものが代入される。
}

void Tehai_Calculator::get_effective(const Game_State& game_state, Tehai_Analyzer tehai_analyzer){
	Tehai_Analyzer ta_tmp = tehai_analyzer;
	ta_tmp.pattern_flag = 0;

	if (out_console) {
		std::cout << "shanten_num:" << tehai_analyzer.get_shanten_num() << " mentu_shanten_num:" << tehai_analyzer.get_mentu_shanten_num() << " titoi_shanten_num:" << tehai_analyzer.get_titoi_shanten_num() << std::endl;
	}
	if (tehai_analyzer.get_shanten_num() > 0) {
		if (out_console) {
			std::cout << "effective:";
		}
		for(int hai=1;hai<38;hai++){
			if (hai%10 != 0) {
				ta_tmp.add_hai(hai);
				ta_tmp.analyze_tenpai(my_pid, game_state);
				if (ta_tmp.get_shanten_num() < tehai_analyzer.get_shanten_num() ) {
					effective_flag[hai] = 1;
					if (out_console) { std::cout << " " << hai; }// << ":" << ta_tmp.get_shanten_num();
					if (hai < 30 && hai%10 == 5) {
						effective_flag[hai+5] = 1;
					}
				}
				ta_tmp.delete_hai(hai);
			}
		}
		if(fuuro_cand_hai!=0){
			effective_flag[fuuro_cand_hai] = 1;
		}
		if (out_console) { std::cout << std::endl; }
	}
}

void Tehai_Calculator::set_inout_flag(Tehai_Analyzer ta_in){
	if(ta_in.get_tehai_num() + ta_in.get_fuuro_num()*3==14){
		for(int hai=0;hai<38;hai++){
			if(hai%10 != 0 && ta_in.count_hai(hai)>0){	
				out_flag[hai] = 1;
			}
		}
	}else{
		for(int hai=1;hai<38;hai++){
			if(hai%10!=0){
				in_flag[hai] = 1;
			}
			if(hai%10 != 0 && ta_in.count_hai(hai)>0){
				out_flag[hai] = 1;
			}
		}
	}

	for(int sn=0;sn<9;sn++){
		/*
		for(int ntp=0;ntp<ta_in.pattern_vec[sn].size();ntp++){
			for(int nip=0;nip<ta_in.pattern_vec[sn][ntp].hai_in_pattern.size();nip++){
				for(int nin=0;nin<ta_in.pattern_vec[sn][ntp].hai_in_pattern[nip].size();nin++){
					in_flag[ta_in.pattern_vec[sn][ntp].hai_in_pattern[nip][nin]] = 1;
				}
			}
			for(int nout=0;nout<ta_in.pattern_vec[sn][ntp].remain.size();nout++){
				out_flag[ta_in.pattern_vec[sn][ntp].remain[nout]] = 1;
			}
		}
		*/
		for (int niop = 0; niop < ta_in.inout_pattern_vec[sn].size(); niop++) {
			for (int nin = 0; nin < ta_in.inout_pattern_vec[sn][niop].hai_in_pattern.size(); nin++) {
				in_flag[ta_in.inout_pattern_vec[sn][niop].hai_in_pattern[nin]] = 1;
			}
			for (int nout = 0; nout < ta_in.inout_pattern_vec[sn][niop].hai_out_pattern.size(); nout++) {
				out_flag[ta_in.inout_pattern_vec[sn][niop].hai_out_pattern[nout]] = 1;
			}
		}
	}
	for(int sn=0;sn<7;sn++){
		for(int ntp=0;ntp<ta_in.pattern_titoi_vec[sn].size();ntp++){
			for(int nip=0;nip<ta_in.pattern_titoi_vec[sn][ntp].hai_in_pattern.size();nip++){
				for(int nin=0;nin<ta_in.pattern_titoi_vec[sn][ntp].hai_in_pattern[nip].size();nin++){
					in_flag[ta_in.pattern_titoi_vec[sn][ntp].hai_in_pattern[nip][nin]] = 1;
				}
			}
			for(int nout=0;nout<ta_in.pattern_titoi_vec[sn][ntp].remain.size();nout++){
				out_flag[ta_in.pattern_titoi_vec[sn][ntp].remain[nout]] = 1;
			}
		}
	}

	for (int sn = 0; sn < 7; sn++) {
		for (int ntp = 0; ntp < ta_in.pattern_honors_and_knitted_vec[sn].size(); ntp++) {
			for (int nip = 0; nip < ta_in.pattern_honors_and_knitted_vec[sn][ntp].hai_in_pattern.size(); nip++) {
				for (int nin = 0; nin < ta_in.pattern_honors_and_knitted_vec[sn][ntp].hai_in_pattern[nip].size(); nin++) {
					in_flag[ta_in.pattern_honors_and_knitted_vec[sn][ntp].hai_in_pattern[nip][nin]] = 1;
				}
			}
			for (int nout = 0; nout < ta_in.pattern_honors_and_knitted_vec[sn][ntp].remain.size(); nout++) {
				out_flag[ta_in.pattern_honors_and_knitted_vec[sn][ntp].remain[nout]] = 1;
			}
		}
	}
}

void Tehai_Calculator::merge_candidates_child(
	std::vector<Tehai_Group>& cand1,
	boost::unordered_set<Tehai_Change>& tcs1,
	std::vector<Tehai_Group>& cand2
){
	std::pair< boost::unordered_set<Tehai_Change>::iterator, bool> ret;
	for(int i = 0; i < cand2.size(); i++) {
		cand2[i].tehai_change.id = cand1.size();
		ret = tcs1.insert(cand2[i].tehai_change);
		if (ret.second) {
			cand1.push_back(cand2[i]);
		}
	}
}

void Tehai_Calculator::merge_candidates(
	std::vector<std::vector<Tehai_Group>>& candvv,
	std::vector<boost::unordered_set<Tehai_Change>>& tcsv
){
	int step = 1;
	while(step < candvv.size()) {
		for(int i = 0; i < candvv.size(); i += step*2) {
			if (i+step < candvv.size()) {
				merge_candidates_child(candvv[i], tcsv[i], candvv[i+step]);
			}
		}
		step = step*2;
	}
}

void Tehai_Calculator::set_tsumo_node_1out(
	const Tehai_Analyzer& ta_in
){
	Tehai_Change tehai_change_tmp_new;
	tehai_change_tmp_new.tehai_base = bit_hai_num;
	for(int hai=0;hai<38;hai++){
		if (hai%10 != 0 && ta_in.count_hai(hai)) {
			tehai_change_tmp_new.tehai_base.delete_hai(hai);
			set_tsumo_node_exec(tehai_change_tmp_new, true, tehai_calculator_work.candidates_work, tcs, in0num);
			tehai_change_tmp_new.tehai_base.add_hai(hai);
		}
	}
}

void Tehai_Calculator::set_tsumo_node_1in_1out(
	const Game_State& game_state,
	const Tehai_Analyzer_Basic& ta_in,
	Tehai_Change& tehai_change_tmp_new
){
	set_tsumo_node_exec(tehai_change_tmp_new, true, tehai_calculator_work.candidates_work, tcs, in0num);
	for(int hai_in=0;hai_in<38;hai_in++){
		if(hai_in%10!=0 && ta_in.using_haikind_num(hai_in)<4){
			tehai_change_tmp_new.tehai_base.add_hai(hai_in);
			for(int hai_out=0;hai_out<38;hai_out++){
				if(hai_out%10 != 0 && ta_in.count_hai(hai_out)) {
					tehai_change_tmp_new.tehai_base.delete_hai(hai_out);
					set_tsumo_node_exec(tehai_change_tmp_new, false, tehai_calculator_work.candidates_work, tcs, in0num);
					tehai_change_tmp_new.tehai_base.add_hai(hai_out);
				}
			}
			tehai_change_tmp_new.tehai_base.delete_hai(hai_in);
		}
	}
}

void Tehai_Calculator::set_tsumo_node_titoi(
	Tehai_Analyzer ta_in,
	int in_num_end
){
	for(int nin=1;nin<=in_num_end;nin++){
		for(int ntp=0;ntp<ta_in.pattern_titoi_vec[in_num_end].size();ntp++){
			for(int nip=0;nip<ta_in.pattern_titoi_vec[in_num_end][ntp].hai_in_pattern.size();nip++){
				set_tsumo_node_child(bit_hai_num, tehai_all_num,
					ta_in.pattern_titoi_vec[in_num_end][ntp].hai_in_pattern[nip], ta_in.pattern_titoi_vec[in_num_end][ntp].remain,
					nin, tehai_calculator_work.candidates_work, tcs, in0num);
			}
		}
	}
}

void Tehai_Calculator::set_tsumo_node_honors_and_knitted(Tehai_Analyzer ta_in, int in_num_end) {
	for (int nin = 1; nin <= in_num_end; nin++) {
		for (int ntp = 0; ntp < ta_in.pattern_honors_and_knitted_vec[in_num_end].size(); ntp++) {
			for (int nip = 0; nip < ta_in.pattern_honors_and_knitted_vec[in_num_end][ntp].hai_in_pattern.size(); nip++) {
				set_tsumo_node_child(bit_hai_num, tehai_all_num,
					ta_in.pattern_honors_and_knitted_vec[in_num_end][ntp].hai_in_pattern[nip], ta_in.pattern_honors_and_knitted_vec[in_num_end][ntp].remain,
					nin, tehai_calculator_work.candidates_work, tcs, in0num);
			}
		}
	}
}

void Tehai_Calculator::set_tsumo_edge(
	//Tehai_Change& tehai_change_tmp
	Tehai_Change tehai_change_tmp,
	const int thread_num
){
	for(int ho=0;ho<38;ho++){
		if(ho%10 != 0 && tehai_change_tmp.tehai_base.count_hai(ho)>0){
			tehai_change_tmp.tehai_base.delete_hai(ho);
			for(int hi=ho+1;hi<38;hi++){
				if(hi%10 != 0) {
					// 下の条件は、根節点から入手し得ない牌で、現在の手牌を生成する際に出ていきもしなかった牌は現在の手牌から入手し得ない
					//（入手しても入手した先が探索手牌集合には含まれない）ということを表している。
					if(in_flag[hi]==1 || bit_hai_num.count_hai(hi) - tehai_change_tmp.tehai_base.count_hai(hi)){
						tehai_change_tmp.tehai_base.add_hai(hi);
						boost::unordered_set<Tehai_Change>::iterator itr;
						itr = tcs.find(tehai_change_tmp);
						if(itr!=tcs.end()){
							if (tehai_calculator_work.tsumo_edge_work[thread_num].size() < MAX_TSUMO_EDGE_NUM_PER_THREAD) {
								tehai_calculator_work.tsumo_edge_work[thread_num].push_back(Tsumo_Edge(tehai_change_tmp.id, hi, ho, (*itr).id));
								// ここでは片方のみ登録。（hi,hoを逆にしたものはあとで考慮。）
							} else {
								// 何かアラートを出すべき？
							}
						}
						tehai_change_tmp.tehai_base.delete_hai(hi);
					}
				}
			}
			tehai_change_tmp.tehai_base.add_hai(ho);
		}
	}
}

// 今のところ使っていない。将来的に、以前の手番で構成したDAGを使って、効率的に今の手番で必要なDAGを構成する場合に使う予定。
void Tehai_Calculator::set_tsumo_edge2(
	//Tehai_Change& tehai_change_tmp
	Tehai_Change tehai_change_tmp,
	const int thread_num
){
	for(int ho=0;ho<38;ho++){
		if(ho%10 != 0 && tehai_change_tmp.tehai_base.count_hai(ho)>0){
			tehai_change_tmp.tehai_base.delete_hai(ho);
			for(int hi=0;hi<38;hi++){
				if(hi%10 != 0 && hi != ho) {
					tehai_change_tmp.tehai_base.add_hai(hi);
					boost::unordered_set<Tehai_Change>::iterator itr;
					itr = tcs.find(tehai_change_tmp);
					if(itr!=tcs.end()){
						if (tehai_calculator_work.tsumo_edge_work[thread_num].size() < MAX_TSUMO_EDGE_NUM_PER_THREAD) {
							tehai_calculator_work.tsumo_edge_work[thread_num].push_back(Tsumo_Edge(tehai_change_tmp.id, hi, ho, (*itr).id));
							// ここでは片方のみ登録。（hi,hoを逆にしたものはあとで考慮。）
						} else {
							// 何かアラートを出すべき？
						}
					}
					tehai_change_tmp.tehai_base.delete_hai(hi);
				}
			}
			tehai_change_tmp.tehai_base.add_hai(ho);
		}
	}
}

void Tehai_Calculator::add_fuuro_node(
	const Game_State& game_state,
	int cn_start, int cn_end, int fuuro_num_begin, int fuuro_num_end, int must_effective_flag, int fuuro_must,
	boost::unordered_map<Tehai_State2, int>* ts_maps
	//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps
){
	int kan_cand[38] = {};
	for(int hai=1;hai<38;hai++){
		if(game_state.player_state[my_pid].tehai[hai]==4 || (hai<30 && hai%10==5 && game_state.player_state[my_pid].tehai[hai]==3 && game_state.player_state[my_pid].tehai[hai+5]==1) ){
			kan_cand[hai] = 1;
		}
	}
	if(fuuro_must!=0){
		kan_cand[haikind(fuuro_must)] = 1;
	}
	for (int fn = 0; fn < game_state.player_state[my_pid].fuuro.size(); fn++) {
		if (game_state.player_state[my_pid].fuuro[fn].type == FT_PON) {
			int pon_hai = haikind(game_state.player_state[my_pid].fuuro[fn].hai);
			if (game_state.player_state[my_pid].tehai[pon_hai]==1 || pon_hai<30 && pon_hai%10==5 && game_state.player_state[my_pid].tehai[pon_hai+5]==1){
				kan_cand[pon_hai] = 1;
			}
		}
	}
	#pragma omp parallel for
	for(int cn=cn_start;cn<cn_end;cn++){
		int fuuro_cand[38] = {};
		for (int hai=0; hai<38; hai++) {
			if (hai%10 != 0) {
				fuuro_cand[hai] = std::max(tehai_calculator_work.candidates_work[cn].tehai_change.tehai_base.count_hai(hai)-bit_hai_num.count_hai(hai), 0);
			}
		}
		if(must_effective_flag==1){
			for(int hai=1;hai<38;hai++){
				fuuro_cand[hai] *= effective_flag[hai];
			}
		}
		if(fuuro_must!=0){
			fuuro_cand[fuuro_must]++;
		}
		if (fuuro_cand_hai != 0 && fuuro_cand_hai%10 == 0) {
			fuuro_cand[fuuro_cand_hai] = 1;
		}

		int fuuro_cand_copy[38] = {};
		for (int hai=0; hai<38; hai++) {
			fuuro_cand_copy[hai] = fuuro_cand[hai];
		}
		int tav_size_tmp = group_size(cn);
		for(int gn=0;gn<tav_size_tmp;gn++){
			tehai_calculator_work.candidates_work[cn].add_fuuro(
				get_ref_ta_cgn(cn, gn), fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand,
				ts_maps[cn], tehai_calculator_work.cal_tav_work, omp_get_thread_num());
		}
	}
}

void Tehai_Calculator::set_cand_graph_sub_child(
	int cn1, int gn1, int cn2, int hai_in, int hai_out, Action_Type action_id, int hai0, int hai1, int hai2,
	boost::unordered_map<Tehai_State2, int>* ts_maps,
	//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps
	const int thread_num
){
	// 麻雀のルールで禁止されている手を行わないようにするための処理 // 中国麻雀ではない？

	// 赤牌を含むフーロを行い、赤牌を打牌する手を行わないようにするための処理（本来ここに来る前にはじかれるべきだと思われる）
	if(hai_out%10==0 && hai_out>0 && action_id!=0){
		if(hai0==hai_out || hai1==hai_out || hai2==hai_out){
			assert(action_id==2 || action_id==11 || action_id==12 || action_id==13);
			return;
		}
	}

	Tehai_State2 tehai_state = get_const_ta_cgn(cn1, gn1).tehai_state;
	boost::unordered_map<Tehai_State2, int>::iterator itr;

	if(action_id!=AT_DAIMINKAN){
		if(hai_in%10==0 && hai_in>0){
			tehai_state.set_aka_inside((hai_in-1)/10,1);
		}
		if(hai_out%10==0 && hai_out>0){
			tehai_state.set_aka_inside((hai_out-1)/10,0);
		}
	}
	if(action_id==AT_KAKAN){
		tehai_state.delete_pon(haikind(hai_out));
		tehai_state.add_minkan(haikind(hai_out));
		if(hai_out%10==0 && hai_out>0){
			tehai_state.set_aka_inside((hai_out-1)/10,0);
			tehai_state.set_aka_outside((hai_out-1)/10,1);
		}
	}else if(action_id==AT_DAIMINKAN){
		tehai_state.add_minkan(haikind(hai_in));
		if(hai_in%10==0 || hai0%10==0 || hai1%10==0 || hai2%10==0){
			tehai_state.set_aka_inside((hai_in-1)/10,0);
			tehai_state.set_aka_outside((hai_in-1)/10,1);
		}
	}else if(action_id!=0){
		tehai_state.add_one_fuuro(fuuro_action_type_to_fuuro_type((int)action_id), std::min({haikind(hai0), haikind(hai1), haikind(hai2)}));
		if(hai0%10==0 || hai1%10==0 || hai2%10==0){
			tehai_state.set_aka_inside((hai0-1)/10,0);
			tehai_state.set_aka_outside((hai0-1)/10,1);
		}
	}
	if(action_id==AT_ANKAN){
		if(hai0%10==0){
			tehai_state.set_aka_inside((hai0-1)/10,0);
			tehai_state.set_aka_outside((hai0-1)/10,1);
		}
	}
	itr = ts_maps[cn2].find(tehai_state);
	if(itr != ts_maps[cn2].end()){
		Tehai_Action tehai_action;
		tehai_action.hai = hai_in;
		tehai_action.hai_out = hai_out;
		tehai_action.dst_group = cn2;
		tehai_action.action_type = action_id;
		tehai_action.dst_group_sub = itr->second;
		if(tehai_action.action_type==AT_TSUMO || tehai_action.action_type==AT_ANKAN || tehai_action.action_type==AT_KAKAN){
			if (tehai_calculator_work.cand_graph_sub_tsumo_work[thread_num].size() < MAX_EDGE_NUM_PER_THREAD) {
				tehai_calculator_work.cand_graph_sub_tsumo_work[thread_num].push_back(tehai_action);
			} else {
				// 何かアラートを出すべき？
			}
		}else{
			if (tehai_calculator_work.cand_graph_sub_fuuro_work[thread_num].size() < MAX_EDGE_NUM_PER_THREAD) {
				tehai_calculator_work.cand_graph_sub_fuuro_work[thread_num].push_back(tehai_action);
			} else {
				// 何かアラートを出すべき？
			}
		}
	}
}

void Tehai_Calculator::set_cand_graph_sub(const int cn1, const std::vector<std::array<int, 3>>& tsumo_edge_array, boost::unordered_map<Tehai_State2, int>* ts_maps, const int thread_num){
	for (int gn1 = 0; gn1 < tehai_calculator_work.candidates_work[cn1].ta_loc.size(); gn1++ ) {
		const Tehai_Location& loc_tmp = tehai_calculator_work.candidates_work[cn1].ta_loc[gn1];
		const int loc_tmp_first = loc_tmp.get_first();
		const int loc_tmp_second = loc_tmp.get_second();
		tehai_calculator_work.cand_graph_sub_tsumo_loc[loc_tmp_first][loc_tmp_second][0] = thread_num;
		tehai_calculator_work.cand_graph_sub_tsumo_loc[loc_tmp_first][loc_tmp_second][1] = tehai_calculator_work.cand_graph_sub_tsumo_work[thread_num].size();
		tehai_calculator_work.cand_graph_sub_fuuro_loc[loc_tmp_first][loc_tmp_second][0] = thread_num;
		tehai_calculator_work.cand_graph_sub_fuuro_loc[loc_tmp_first][loc_tmp_second][1] = tehai_calculator_work.cand_graph_sub_fuuro_work[thread_num].size();
		for (int i = 0; i < tsumo_edge_array.size(); i++) {
			const int hai = tsumo_edge_array[i][0];
			const int hai_out = tsumo_edge_array[i][1];
			const int cn2 = tsumo_edge_array[i][2];
			int hai_out_tmp = hai_out;
			int aka_choice;
			if(hai_out<30 && hai_out%10==5){
				if(get_const_ta_cgn(cn1, gn1).count_hai(hai_out+5)==0){
					aka_choice = 1;
				}else if(get_const_ta_cgn(cn1, gn1).count_hai(hai_out)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai_out+5)>0){
					aka_choice = 2;
				}else if(get_const_ta_cgn(cn1, gn1).count_hai(hai_out+5)>0){
					aka_choice = 1;
					hai_out_tmp += 5;
				}else{
					continue;
				}
			}else{
				aka_choice = 1;
			}
			for (int j = 0; j < aka_choice; j++) {
				hai_out_tmp += 5*j;

				if(get_const_ta_cgn(cn1, gn1).count_hai(hai_out_tmp)==0){
					continue;
				}

				set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_TSUMO, 0, 0, 0, ts_maps, thread_num);

				if(get_const_ta_cgn(cn1, gn1).count_hai(hai)>=2){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_PON, hai, hai, hai, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai)>=1 && get_const_ta_cgn(cn1, gn1).count_hai(hai+5)==1){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_PON, hai, hai, hai+5, ts_maps, thread_num);
				}
				//if(hai!=0 && hai%10==0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-5)>=2){
					//set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, 2, hai, hai-5, hai-5, -1, ts_maps);
				//}
				if(hai%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai)>=2 && fuuro_cand_hai==hai+5){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai+5, hai_out_tmp, AT_PON, hai+5, hai, hai, ts_maps, thread_num);
				}

				if(hai<30 && hai%10<=7 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+2)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_LOW, hai, hai+1, hai+2, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==3 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+7)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_LOW, hai, hai+1, hai+7, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==4 && get_const_ta_cgn(cn1, gn1).count_hai(hai+6)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+2)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_LOW, hai, hai+6, hai+2, ts_maps, thread_num);
				}
				//if(hai!=0 && hai%10==0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-4)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-3)>0){
					//set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, 11, hai, hai-4, hai-3, ts_maps);
				//}
				if(hai%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+2)>0 && fuuro_cand_hai==hai+5){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai+5, hai_out_tmp, AT_CHI_LOW, hai+5, hai+1, hai+2, ts_maps, thread_num);
				}

				if(hai<30 && 2<=hai%10 && hai%10<=8 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_MIDDLE, hai, hai-1, hai+1, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==4 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+6)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_MIDDLE, hai, hai-1, hai+6, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==6 && get_const_ta_cgn(cn1, gn1).count_hai(hai+4)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_MIDDLE, hai, hai+4, hai+1, ts_maps, thread_num);
				}
				//if(hai!=0 && hai%10==0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-6)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-4)>0){
					//set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, 12, hai, hai-6, hai-4, ts_maps);
				//}
				if(hai%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+1)>0 && fuuro_cand_hai==hai+5){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai+5, hai_out_tmp, AT_CHI_MIDDLE, hai+5, hai-1, hai+1, ts_maps, thread_num);
				}

				if(hai<30 && 3<=hai%10 && get_const_ta_cgn(cn1, gn1).count_hai(hai-2)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_HIGH, hai, hai-2, hai-1, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==6 && get_const_ta_cgn(cn1, gn1).count_hai(hai-2)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai+4)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_HIGH, hai, hai-2, hai+4, ts_maps, thread_num);
				}
				if(hai<30 && hai%10==7 && get_const_ta_cgn(cn1, gn1).count_hai(hai+3)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_CHI_HIGH, hai, hai+3, hai-1, ts_maps, thread_num);
				}
				//if(hai!=0 && hai%10==0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-7)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-6)>0){
					//set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, 13, hai, hai-7, hai-6, -1, ts_maps);
				//}
				if(hai%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai-2)>0 && get_const_ta_cgn(cn1, gn1).count_hai(hai-1)>0 && fuuro_cand_hai==hai+5){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai+5, hai_out_tmp, AT_CHI_HIGH, hai+5, hai-2, hai-1, ts_maps, thread_num);
				}

				if(get_const_ta_cgn(cn1, gn1).count_hai(hai_out_tmp)==4){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_ANKAN, hai_out_tmp, hai_out_tmp, hai_out_tmp, ts_maps, thread_num);
				}else if(hai_out_tmp<30 && hai_out_tmp%10==5 && get_const_ta_cgn(cn1, gn1).count_hai(hai_out_tmp)==3 && get_const_ta_cgn(cn1, gn1).count_hai(hai_out_tmp+5)==1){
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_ANKAN, hai_out_tmp, hai_out_tmp, hai_out_tmp+5, ts_maps, thread_num);
				}

				if(get_const_ta_cgn(cn1, gn1).tehai_state.get_pon_num(haikind(hai_out_tmp))) {
					set_cand_graph_sub_child(cn1, gn1, cn2, hai, hai_out_tmp, AT_KAKAN, 0, 0, 0, ts_maps, thread_num);
				}
			}
		}

		for(int hai=1;hai<38;hai++){
			if(hai%10 != 0) {
				if(aka_flag && hai<30 && hai%10==5){
					if((get_const_ta_cgn(cn1, gn1).count_hai(hai)==2 && get_const_ta_cgn(cn1, gn1).count_hai(hai+5)==1) || get_const_ta_cgn(cn1, gn1).count_hai(hai)==3){
						set_cand_graph_sub_child(cn1, gn1, cn1, hai, hai, AT_ANKAN, hai, hai, hai+5, ts_maps, thread_num);
					}
				}else{
					if(get_const_ta_cgn(cn1, gn1).count_hai(hai)==3){
						set_cand_graph_sub_child(cn1, gn1, cn1, hai, hai, AT_ANKAN, hai, hai, hai, ts_maps, thread_num);
						set_cand_graph_sub_child(cn1, gn1, cn1, hai, 0, AT_DAIMINKAN, hai, hai, hai, ts_maps, thread_num);
					}
				}
				if(get_const_ta_cgn(cn1, gn1).tehai_state.get_pon_num(hai) == 1) {
					set_cand_graph_sub_child(cn1, gn1, cn1, hai, hai, AT_KAKAN, 0, 0, 0, ts_maps, thread_num);
				}
			}
		}
		tehai_calculator_work.cand_graph_sub_tsumo_loc[loc_tmp_first][loc_tmp_second][2] = tehai_calculator_work.cand_graph_sub_tsumo_work[thread_num].size();
		tehai_calculator_work.cand_graph_sub_fuuro_loc[loc_tmp_first][loc_tmp_second][2] = tehai_calculator_work.cand_graph_sub_fuuro_work[thread_num].size();
	}
}

/*
// いずれ、交換枚数が大きいが重要な手を列挙するために利用する予定
void Tehai_Calculator::set_candidates3_sub(Tehai_Analyzer tehai_analyzer, const int change_num) {
	//tehai_all_num = tehai_analyzer.tehai_num + tehai_analyzer.fuuro_num*3; // 並列で実際に使う時に、この記述がsingle_threadのところと問題ないか要確認
	int tehai_kcp[38];
	get_tehai_kcp_ta(tehai_analyzer, tehai_kcp);
	bit_hai_num.set_from_array38(tehai_kcp);

	Tehai_Change tehai_change_tmp;
	tehai_change_tmp.tehai_base = bit_hai_num;
	tehai_change_tmp.id = 0;
	in0num_sub = 0;

	if (tehai_analyzer.get_shanten_num() != -1) {
		set_tsumo_node_mentu(bit_hai_num, tehai_all_num, tehai_analyzer, fuuro_cand_hai, change_num,
			candidates_sub, tcs_sub, in0num_sub);
	}
}
*/

void Tehai_Calculator::set_candidates3_single_thread(
	const Game_State& game_state, Tehai_Analyzer tehai_analyzer, Tehai_Analyzer tehai_analyzer_af, const int cn_max, const Tactics& tactics
) {
	//tehai_all_num = tehai_analyzer.tehai_num + tehai_analyzer.fuuro_num*3;
	int tehai_kcp[38];
	get_tehai_kcp_ta(tehai_analyzer, tehai_kcp);
	bit_hai_num.set_from_array38(tehai_kcp);

	Tehai_Change tehai_change_tmp;
	tehai_change_tmp.tehai_base = bit_hai_num;
	tehai_change_tmp.id = 0;
	in0num = 0;

	clock_t check0 = clock();

	//cnm_restrict_flag = 1;
	//cnm_restriction.push_back(28);

	if(tehai_analyzer.get_tehai_num() + tehai_analyzer.get_fuuro_num()*3==14 && fuuro_cand_hai==0){
		set_tsumo_node_1out(tehai_analyzer);
	}else if(tehai_analyzer.get_tehai_num() + tehai_analyzer.get_fuuro_num()*3==13){
		set_tsumo_node_1in_1out(game_state, tehai_analyzer, tehai_change_tmp);
	}

	if (tehai_analyzer.get_shanten_num() != -1) {
		int mentu_node_flag = 0;
		for (int sn = 0; sn <= cn_max + tactics.cn_max_addition; sn++) {
			if (sn <= cn_max) {
				if (tactics.tsumo_enumerate_always == -1 ||
					candidates_size() < tactics.tsumo_enumerate_always ||
					(mentu_node_flag == 0 && sn < 5)
				){
					int size_tmp = candidates_size();
					set_tsumo_node_mentu(bit_hai_num, tehai_all_num, tehai_analyzer, fuuro_cand_hai, sn,
						tehai_calculator_work.candidates_work, tcs, in0num);
					if(candidates_size()>size_tmp && mentu_node_flag==0){
						mentu_node_flag = 1;
					}
					if(sn<=tehai_analyzer.get_titoi_shanten_num()){
						set_tsumo_node_titoi(tehai_analyzer, sn);
					}
					if (sn <= tehai_analyzer.get_honors_and_knitted_shanten_num()) {
						set_tsumo_node_honors_and_knitted(tehai_analyzer, sn);
					}
					if (fuuro_cand_hai!=0 &&
						(tactics.tsumo_enumerate_fuuro_restriction == -1 || candidates_size() < tactics.tsumo_enumerate_fuuro_restriction)
					){
						set_tsumo_node_mentu(bit_hai_num, tehai_all_num, tehai_analyzer_af, fuuro_cand_hai, sn,
							tehai_calculator_work.candidates_work, tcs, in0num);
					}
				} else if (candidates_size() < tactics.tsumo_enumerate_restriction) {
					std::sort(tehai_analyzer.inout_pattern_vec[sn].begin(), tehai_analyzer.inout_pattern_vec[sn].end());
					std::reverse(tehai_analyzer.inout_pattern_vec[sn].begin(), tehai_analyzer.inout_pattern_vec[sn].end());
					for(int nip=0;nip<tehai_analyzer.inout_pattern_vec[sn].size();nip++){
						for(int nin=1;nin<=sn;nin++){
							set_tsumo_node_child(bit_hai_num, tehai_all_num,
								tehai_analyzer.inout_pattern_vec[sn][nip].hai_in_pattern, tehai_analyzer.inout_pattern_vec[sn][nip].hai_out_pattern,
								nin, tehai_calculator_work.candidates_work, tcs, in0num);
						}
						if (candidates_size() >= tactics.tsumo_enumerate_restriction) {
							break;
						}
					}
				}
			} else {
				if (candidates_size() < tactics.tsumo_enumerate_additional_maximum) {
					std::sort(tehai_analyzer.inout_pattern_vec[sn].begin(), tehai_analyzer.inout_pattern_vec[sn].end());
					std::reverse(tehai_analyzer.inout_pattern_vec[sn].begin(), tehai_analyzer.inout_pattern_vec[sn].end());
					for(int nip=0;nip<tehai_analyzer.inout_pattern_vec[sn].size();nip++){
						if (candidates_size() < tactics.tsumo_enumerate_additional_minimum ||
							tehai_analyzer.inout_pattern_vec[sn][nip].priority > tactics.tsumo_enumerate_additional_priority
						) {
							for(int nin=1;nin<=sn;nin++){
								set_tsumo_node_child(bit_hai_num, tehai_all_num,
									tehai_analyzer.inout_pattern_vec[sn][nip].hai_in_pattern, tehai_analyzer.inout_pattern_vec[sn][nip].hai_out_pattern,
									nin, tehai_calculator_work.candidates_work, tcs, in0num);
							}
						}
						if (candidates_size() >= tactics.tsumo_enumerate_additional_maximum) {
							break;
						}
					}
				}
			}
			if (out_console) {
				std::cout << sn << " " << cn_max << " " << candidates_size() << " " << mentu_node_flag << std::endl;
			}
		}
	}else{
		//set_tsumo_node_mawashi_routine(tehai_analyzer, 0);
		//set_tsumo_node_mawashi_routine(tehai_analyzer, 1);
	}


	clock_t check1 = clock();

	if (out_console) {
		std::cout << "time graph_node:" << (double)(check1 - check0)/CLOCKS_PER_SEC << std::endl;
	}

	set_inout_flag(tehai_analyzer);
	if(fuuro_cand_hai!=0){
		set_inout_flag(tehai_analyzer_af);
	}

}

void Tehai_Calculator::set_candidates3_multi_thread(const Moves& game_record, const Game_State& game_state, const std::array<bool, 38> sute_flag, Tehai_Analyzer tehai_analyzer, const Tactics& tactics){
	boost::unordered_map<Tehai_State2, int>* ts_maps;
	//std::unordered_map<Tehai_State, int, Tehai_State_Hash>* ts_maps;
	ts_maps = new boost::unordered_map<Tehai_State2, int>[candidates_size()];
	//ts_maps = new std::unordered_map<Tehai_State, int, Tehai_State_Hash>[candidates_size()];

	clock_t check1 = clock();
	const int basic_num = 0;

	#pragma omp parallel
	#pragma omp for
	for(int cn=0;cn<candidates_size();cn++){
		tehai_calculator_work.candidates_work[cn].set_tav_init(
			bit_hai_num, tehai_analyzer.tehai_state, fuuro_cand_hai,
			ts_maps[cn], tehai_calculator_work.cal_tav_work, omp_get_thread_num()
		);
	}

	clock_t check2 = clock();
	const int enumerate_restriction = fuuro_cand_hai == 0 ? tactics.enumerate_restriction : tactics.enumerate_restriction_fp;
	if (enumerate_restriction == -1) {
		add_fuuro_node(game_state, 0, candidates_size(), 1, tactics.fuuro_num_max, 0, 0, ts_maps);
	} else if (candidates_size() < enumerate_restriction) {
		for(int fn=1;fn<=3;fn++){
			if (fuuro_cand_hai!=0 && get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, 0, basic_num, 1, fn, 1, fuuro_cand_hai, ts_maps);
			}
			if (get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, 0, basic_num, 1, fn, 1, 0, ts_maps);
			}
			if (get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, 0, basic_num, 1, fn, 0, 0, ts_maps);
			}
			if (fuuro_cand_hai != 0 && get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, basic_num, candidates_size(), 1, fn, 1, fuuro_cand_hai, ts_maps);
			}
			if (get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, basic_num, candidates_size(), 1, fn, 1, 0, ts_maps);
			}
			if (get_sub_num_all() < enumerate_restriction) {
				add_fuuro_node(game_state, basic_num, candidates_size(), 1, fn, 0, 0, ts_maps);
			}
			if (out_console) { std::cout << "fuuro_node:" << fn << " " << get_sub_num_all() << std::endl; }
		}
	}

	if (tactics.consider_kan) {

		// 暗槓の追加をadd_fuuro_nodeを後に持ってくると、fuuro_maxより大きい回数のフーロ回数を持つ手牌が現れるが、実装都合でこのようにする。
		int ankan_cand[38] = {};
		for (int hai = 1; hai < 38; hai++) {
			if (hai % 10 != 0) {
				if (game_state.player_state[my_pid].tehai[hai] == 4 || (hai<30 && hai%10==5 && game_state.player_state[my_pid].tehai[hai] == 3 && game_state.player_state[my_pid].tehai[hai+5]==1)) {
					ankan_cand[hai] = 1;
				}
			}
		}
		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates_size();cn++) {
			tehai_calculator_work.candidates_work[cn].add_ankan(ankan_cand, ts_maps[cn], tehai_calculator_work.cal_tav_work, omp_get_thread_num());
		}

		int kakan_cand[38] = {};
		for (int fn = 0; fn < game_state.player_state[my_pid].fuuro.size(); fn++) {
			if (game_state.player_state[my_pid].fuuro[fn].type == FT_PON) {
				int pon_hai = haikind(game_state.player_state[my_pid].fuuro[fn].hai);
				if (game_state.player_state[my_pid].tehai[pon_hai] == 1 || pon_hai < 30 && pon_hai % 10 == 5 && game_state.player_state[my_pid].tehai[pon_hai+5] == 1) {
					kakan_cand[pon_hai] = 1;
				}
			}
		}
		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates_size();cn++) {
			tehai_calculator_work.candidates_work[cn].add_kakan(kakan_cand, ts_maps[cn], tehai_calculator_work.cal_tav_work, omp_get_thread_num());
		}

		int daiminkan_cand[38] = {};
		daiminkan_cand[haikind(fuuro_cand_hai)] = 1;
		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates_size();cn++) {
			tehai_calculator_work.candidates_work[cn].add_daiminkan(daiminkan_cand, ts_maps[cn], tehai_calculator_work.cal_tav_work, omp_get_thread_num());
		}

	}	

	#pragma omp parallel
	#pragma omp for
	for(int cn=0;cn<candidates_size();cn++) {
		tehai_calculator_work.candidates_work[cn].analyze_all_tenpai(my_pid, game_state, tehai_calculator_work.cal_tav_work);
	}

	/*
	for (int cn = 0; cn < candidates_size(); cn++) {
		assert(candidates[cn].tav.size() == group_size(cn));
		for (int gn = 0; gn < candidates[cn].tav.size(); gn++) {
			assert(candidates[cn].tav[gn] == get_const_ta_cgn(cn, gn));
		}
	}
	*/

	//graph_agariv = new boost::container::static_vector<Agari_Basic, 10>*[candidates_size()];
	//for(int cn1=0;cn1<candidates_size();cn1++){
	//	graph_agariv[cn1] = new boost::container::static_vector<Agari_Basic, 10>[group_size(cn1)];
	//}

	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		tehai_calculator_work.agari_graph_work[i].clear();
	}

	#pragma omp parallel
	#pragma omp for
	for(int cn=0;cn<candidates_size();cn++) {
		tehai_calculator_work.candidates_work[cn].analyze_all_agari(
			my_pid, game_state, tehai_calculator_work.cal_tav_work,
			tehai_calculator_work.agari_graph_work[omp_get_thread_num()],
			//graph_agariv[cn]
			tehai_calculator_work.agari_graph_loc,
			omp_get_thread_num()
		);
	}

	if (out_console) {
		std::cout << "sub_num_all:" << get_sub_num_all() << std::endl;
	}

	clock_t check3 = clock();

	//set_inout_flag(tehai_analyzer);
	//if(fuuro_cand_hai!=0){
		//set_inout_flag(tehai_analyzer_af);
	//}

	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		tehai_calculator_work.tsumo_edge_work[i].clear();
	}

	#pragma omp parallel
	#pragma omp for
	for(int cn=0;cn<candidates_size();cn++){
		set_tsumo_edge(tehai_calculator_work.candidates_work[cn].tehai_change, omp_get_thread_num());
	}

	clock_t check4 = clock();

	//for(int i=0;i<2;i++){
	//	cand_graph_sub[i] = new std::vector<Tehai_Action>*[candidates_size()];
	//	for(int cn1=0;cn1<candidates_size();cn1++){
	//		cand_graph_sub[i][cn1] = new std::vector<Tehai_Action>[group_size(cn1)];	
	//	}
	//}

	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		tehai_calculator_work.cand_graph_sub_tsumo_work[i].clear();
		tehai_calculator_work.cand_graph_sub_fuuro_work[i].clear();
	}


	std::array< std::array<int, 38>, 38>* graph_edge_new;
	// MAX_CANDIDATES_NUMの静的確保を行うと止まってしまう。（candidates_sizeが大きい時にちゃんと確保できているのか少し疑問。）
	graph_edge_new = new std::array< std::array<int, 38>, 38>[candidates_size()];
	#pragma omp parallel
	#pragma omp for
	for (int cn = 0; cn < candidates_size(); cn++) {
		for (int hi = 0; hi < 38; hi++) {
			for (int ho = 0; ho < 38; ho++) {
				graph_edge_new[cn][hi][ho] = -1;
			}
		}
	}

	#pragma omp parallel
	#pragma omp for
	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		for (int j = 0; j < tehai_calculator_work.tsumo_edge_work[i].size(); j++) {
			const Tsumo_Edge& tsumo_edge = tehai_calculator_work.tsumo_edge_work[i][j];
			graph_edge_new[tsumo_edge.src_group][tsumo_edge.hai_in][tsumo_edge.hai_out] = tsumo_edge.dst_group;
			graph_edge_new[tsumo_edge.dst_group][tsumo_edge.hai_out][tsumo_edge.hai_in] = tsumo_edge.src_group;
		}
	}

	int tsumo_edge_num_all = 0;
	for(int cn=0;cn<candidates_size();cn++){
		for(int hai=0;hai<38;hai++){
			for(int ho=0;ho<38;ho++){
				if(graph_edge_new[cn][hai][ho]!=-1){
					tsumo_edge_num_all++;
				}
			}
		}
	}
	if (out_console) {
		std::cout << "tsumo_edge_num_all:" << tsumo_edge_num_all << std::endl;
	}

	#pragma omp parallel
	#pragma omp for
	for(int cn1=0;cn1<candidates_size();cn1++){
		std::vector<std::array<int, 3>> tsumo_edge_array;
		for(int hai=1;hai<38;hai++){
			for(int ho=0;ho<38;ho++){
				if(graph_edge_new[cn1][hai][ho]!=-1){
					tsumo_edge_array.push_back(std::array<int, 3>({hai, ho, graph_edge_new[cn1][hai][ho]}));
				}
			}
		}
		set_cand_graph_sub(cn1, tsumo_edge_array, ts_maps, omp_get_thread_num());
	}

	int action_num_all_work[2] = {};
	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		action_num_all_work[0] += tehai_calculator_work.cand_graph_sub_tsumo_work[i].size();
		action_num_all_work[1] += tehai_calculator_work.cand_graph_sub_fuuro_work[i].size();
	}

	if (out_console) {
		std::cout << "action_num_all:" << action_num_all_work[0] << " " << action_num_all_work[1] << std::endl;
	}

	for(int cn=0;cn<candidates_size();cn++){
		ts_maps[cn].clear();
	}
	delete[] ts_maps;
	delete[] graph_edge_new;

	clock_t check5 = clock();
	if (out_console) {
		std::cout << "time";
		std::cout << " no_fuuro_analyze:" << (double)(check2 - check1)/CLOCKS_PER_SEC;
		std::cout << " add_fuuro:" << (double)(check3 - check2)/CLOCKS_PER_SEC << std::endl;
		std::cout << "graph_edge:" << (double)(check4 - check3)/CLOCKS_PER_SEC;
		std::cout << " graph_edge_sub:" << (double)(check5 - check4)/CLOCKS_PER_SEC << std::endl;
	}
}

int Tehai_Calculator::get_max_group_size() {
	int result = 0;
	for(int cn=0;cn<candidates_size();cn++){
		if (result < group_size(cn)) {
			result = group_size(cn);
		}
	}
	return result;
}

int Tehai_Calculator::get_max_edge_size() {
	int result = 0;
	for (int cn = 0; cn < candidates_size(); cn++) {
		for (int gn = 0; gn < group_size(cn); gn++) {
			const Tehai_Location& ta_loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
			const int ta_loc_first = ta_loc.get_first();
			const int ta_loc_second = ta_loc.get_second();
			const std::array<int, 3>& edge_tsumo_loc = tehai_calculator_work.cand_graph_sub_tsumo_loc[ta_loc_first][ta_loc_second];
			if (result < edge_tsumo_loc[2] - edge_tsumo_loc[1]) {
				result = edge_tsumo_loc[2] - edge_tsumo_loc[1];
			}
			const std::array<int, 3>& edge_fuuro_loc = tehai_calculator_work.cand_graph_sub_fuuro_loc[ta_loc_first][ta_loc_second];
			if (result < edge_fuuro_loc[2] - edge_fuuro_loc[1]) {
				result = edge_fuuro_loc[2] - edge_fuuro_loc[1];
			}
		}
	}
	return result;
}

int Tehai_Calculator::get_sub_num_all() {
	int result = 0;
	for (int cn = 0; cn < candidates_size(); cn++) {
		result += group_size(cn);
	}

	int result2 = 0;
	for (int i = 0; i < CAL_NUM_THREAD; i++) {
		result2 += tehai_calculator_work.cal_tav_work[i].size();
	}
	assert(result == result2);
	return result;
}

void Tehai_Calculator::set_agari_shanten_num(const Game_State& game_state) {
	for(int cn=0;cn<candidates_size();cn++){
		for(int gn=0;gn<group_size(cn);gn++){
			const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
			for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
				const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
				if (agari.get_ten_tsumo(my_pid, game_state) > 0) {
					get_ref_ta_cgn(cn, gn).set_agari_shanten_num(0);
					break;
				}
			}
		}
	}
	for(int sn=1;sn<=4;sn++){
		for(int cn=0;cn<candidates_size();cn++){
			for(int gn=0;gn<group_size(cn);gn++){
				if(get_ref_ta_cgn(cn, gn).get_agari_shanten_num() > sn){
					const std::array<int, 3>& tsumo_edge_loc = tehai_calculator_work.get_const_tsumo_edge_loc(cn, gn);
					//for(int acn=0;acn<cand_graph_sub[0][cn][gn].size();acn++){
					for (int acn = tsumo_edge_loc[1]; acn < tsumo_edge_loc[2]; acn++) {
						const Tehai_Action& tsumo_edge = tehai_calculator_work.cand_graph_sub_tsumo_work[tsumo_edge_loc[0]][acn];
						if(tsumo_edge.action_type==AT_TSUMO){
							if(get_ref_ta_cgn(tsumo_edge.dst_group, tsumo_edge.dst_group_sub).get_agari_shanten_num() == sn-1){
								get_ref_ta_cgn(cn, gn).set_agari_shanten_num(sn);
								break;
							}
						}
					}
				}
			}
		}
	}
}

int Tehai_Calculator::get_fuuro_agari_shanten_num(const Game_State& game_state, const Hai_Array& current_tehai, const bool chi_flag) {
	int fuuro_agari_shanten_num = 8;
	if (fuuro_cand_hai != 0) {
		set_agari_shanten_num(game_state);
		int cn_fuuro_neg = 0;
		int gn_fuuro_neg = 0;
		for (int gn = 0; gn < group_size(cn_fuuro_neg); gn++) {
			if (is_same_tehai_ta(current_tehai, get_const_ta_cgn(cn_fuuro_neg, gn))) {
				gn_fuuro_neg = gn;
				break;
			}
		}
		if (out_console) { std::cout << "agari_shanten_num:" << get_const_ta_cgn(cn_fuuro_neg, gn_fuuro_neg).get_agari_shanten_num() << std::endl; }

		const std::array<int, 3>& fuuro_edge_loc = tehai_calculator_work.get_const_fuuro_edge_loc(cn_fuuro_neg, gn_fuuro_neg);
		for (int acn = fuuro_edge_loc[1]; acn < fuuro_edge_loc[2]; acn++) {
			const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_fuuro_work[fuuro_edge_loc[0]][acn];
			//ac_tmp.out_info();
			if (ac_tmp.hai != fuuro_cand_hai) {
				continue;
			} else if (!chi_flag && is_chi(ac_tmp.action_type)) {
				continue;
			} else {
				if (get_const_ta_cgn(ac_tmp.dst_group, ac_tmp.dst_group_sub).get_agari_shanten_num() < fuuro_agari_shanten_num) {
					fuuro_agari_shanten_num = get_const_ta_cgn(ac_tmp.dst_group, ac_tmp.dst_group_sub).get_agari_shanten_num();
				}
			}
		}
	}
	if (out_console) { std::cout << "fuuro_agari_shanten_num:" << fuuro_agari_shanten_num << std::endl; }
	return fuuro_agari_shanten_num;
}

void Tehai_Calculator::set_agari_exp(
	const Game_State& game_state,
    const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
) {
	const Hai_Array hai_visible_kind = haikind(get_hai_visible_me(my_pid, game_state)); // ako ではこのようになっている。
	#pragma omp parallel
	#pragma omp for
	for(int cn=0;cn<candidates_size();cn++){
		for(int gn=0;gn<group_size(cn);gn++){
			const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
			for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
				Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
				const std::array<float, 2> ten_exp = agari.get_ten_exp(my_pid, get_const_ta_cgn(cn, gn).tehai_bit, get_const_ta_cgn(cn, gn).tehai_state, hai_visible_kind, game_state, kyoku_end_pt_exp);
				agari.tsumo_exp = ten_exp[0];
				agari.ron_exp = ten_exp[1];
				// 後退解析時は赤ドラを考えない。赤ドラを考える場合、この部分は修正する必要がある。
			}
		}
	}
}

void Tehai_Calculator::calc_agari_prob(
	const int tsumo_num, const double exp_min, const Game_State& game_state,
    const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
) {
	clock_t check0 = clock();
	const int nokori_hai_num = get_nokori_hai_num(my_pid, game_state);
	const Hai_Array hai_visible = haikind(get_hai_visible_wo_tehai(my_pid, game_state));
	const Hai_Array using_haikind_array = haikind(using_hai_array(game_state.player_state[my_pid].tehai, game_state.player_state[my_pid].fuuro));
	set_agari_exp(game_state, kyoku_end_pt_exp);
	clock_t check1 = clock();

	//for(int mode=0;mode<3;mode++){
		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates_size();cn++){
			for(int gn=0;gn<group_size(cn);gn++){
				const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
				const int loc_first = loc.get_first();
				const int loc_second = loc.get_second();
				tehai_calculator_work.agari_prob_work[loc_first][loc_second][0] = 0.0;
				tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][0] = 0.0;
				tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][0] = 1.0*get_const_ta_cgn(cn, gn).get_tenpai_flag();
				tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][0] = 1.0*get_const_ta_cgn(cn, gn).get_tenpai_flag();
				
				tehai_calculator_work.ten_exp_work[loc_first][loc_second][0] = exp_min;
				tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][0] = exp_min;

				for(int han=0;han<5;han++){
					tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][0][han] = 0.0;
					tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][0][han] = 0.0;
				}
			}
		}
		for(int tn=1;tn<=tsumo_num;tn++){
			const int mod2tn = mod2(tn);
			const int mod2tn_prev = mod2(tn-1);
			#pragma omp parallel
			#pragma omp for
			for(int cn=0;cn<candidates_size();cn++){
				for(int gn=0;gn<group_size(cn);gn++){
					const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
					const int loc_first = loc.get_first();
					const int loc_second = loc.get_second();
					tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn_prev];
					tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev];
					tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev];
					for(int han=0;han<5;han++){
						tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][mod2tn][han] = tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][mod2tn_prev][han];
					}

					double prob_tmpbest[38], han_prob_tmpbest[5][38], tenpai_tmpbest[38], exp_tmpbest[38];
					int hai_effective_flag[38];
					for(int hai=0;hai<38;hai++){
						hai_effective_flag[hai] = 0;
						prob_tmpbest[hai] = tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn_prev];
						tenpai_tmpbest[hai] = tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev];
						exp_tmpbest[hai] = tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev];

						for(int han=0;han<5;han++){
							han_prob_tmpbest[han][hai] = tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][mod2tn_prev][han];
						}
					}

					const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
					for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
						const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
						if (agari.get_ten_tsumo(my_pid, game_state) > 0) {
							if(agari.tsumo_exp > exp_tmpbest[agari.agari_info.get_hai()]){
								hai_effective_flag[agari.agari_info.get_hai()] = 1;
								prob_tmpbest[agari.agari_info.get_hai()] = 1.0;
								exp_tmpbest[agari.agari_info.get_hai()] = agari.tsumo_exp;

								for(int han=0;han<5;han++){
									han_prob_tmpbest[han][agari.agari_info.get_hai()] = 0.0;
								}
								int agari_han = agari.agari_info.get_han_tsumo();
								han_prob_tmpbest[std::min(4,agari_han)][agari.agari_info.get_hai()] = 1.0;
								// 裏ドラについて考えるべきかもしれない。（そもそもhan_prob_tmpbestは使われていないが。）
							}
						}
					}

					const std::array<int, 3>& tsumo_edge_loc = tehai_calculator_work.get_const_tsumo_edge_loc(cn, gn);
					for (int acn = tsumo_edge_loc[1]; acn < tsumo_edge_loc[2]; acn++) {
						const Tehai_Action& tsumo_edge = tehai_calculator_work.cand_graph_sub_tsumo_work[tsumo_edge_loc[0]][acn];
						const int dst_group = tsumo_edge.dst_group;
						const int dst_group_sub = tsumo_edge.dst_group_sub;
						const int hai = tsumo_edge.hai;
						const Tehai_Location& dst_loc = tehai_calculator_work.candidates_work[dst_group].ta_loc[dst_group_sub];
						const int dst_loc_first = dst_loc.get_first();
						const int dst_loc_second = dst_loc.get_second();
						if(tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev] > tenpai_tmpbest[hai]){
							hai_effective_flag[hai] = 1;
							tenpai_tmpbest[hai] = tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
						}
						//if(agari_prob[dst_group][dst_group_sub][tn-1] > prob_tmpbest[hai]){
						if(tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] > exp_tmpbest[hai]){
							if(get_const_ta_cgn(dst_group, dst_group_sub).get_neg_flag()==0){
								hai_effective_flag[hai] = 1;
								prob_tmpbest[hai] = tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								exp_tmpbest[hai] = tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];

								for(int han=0;han<5;han++){
									han_prob_tmpbest[han][hai] = tehai_calculator_work.agari_han_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev][han];
								}
							}
						}
					}
					for(int hai=0;hai<38;hai++){
						if(hai%10 != 0 && hai_effective_flag[hai]==1){
							double nokori_tmp = 4.0 - get_const_ta_cgn(cn, gn).using_haikind_num(hai) - hai_visible[hai];
							if (using_haikind_array[hai] - get_const_ta_cgn(cn, gn).using_haikind_num(hai) > 0){
								nokori_tmp -= using_haikind_array[hai] - get_const_ta_cgn(cn, gn).using_haikind_num(hai);
							}
							if(tn!=tsumo_num){
								nokori_tmp *= rn_para;
							}
							if(tenpai_tmpbest[hai]>tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev]){
								tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] += (tenpai_tmpbest[hai] - tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn));
							}
							//if(prob_tmpbest[hai] > agari_prob[cn][gn][tn-1]){
							if(exp_tmpbest[hai]>tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev]){
								tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] += (prob_tmpbest[hai] - tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn_prev])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn));
								tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] += (exp_tmpbest[hai] - tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn));

								for(int han=0;han<5;han++){
									tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][mod2tn][han] += (han_prob_tmpbest[han][hai] - tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][mod2tn_prev][han])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn));
								}
							}
						}
					}
				}
			}

			#pragma omp parallel
			#pragma omp for
			for(int cn=0;cn<candidates_size();cn++){
				for(int gn=0;gn<group_size(cn);gn++){
					const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
					const int loc_first = loc.get_first();
					const int loc_second = loc.get_second();
					tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn];
					tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn];
					tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn];

					for(int han=0;han<5;han++){
						tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][mod2tn][han] = tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][mod2tn][han];
					}

					double prob_tmpbest[38], han_prob_tmpbest[5][38], tenpai_tmpbest[38], exp_tmpbest[38];
					int hai_effective_flag[38], pon_ten_flag[38], pon_ron_flag[38];
					for(int hai=0;hai<38;hai++){
						hai_effective_flag[hai] = 0;
						prob_tmpbest[hai] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn];
						tenpai_tmpbest[hai] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn];
						exp_tmpbest[hai] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn];

						for(int han=0;han<5;han++){
							han_prob_tmpbest[han][hai] = tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][mod2tn][han];
						}
						pon_ten_flag[hai] = 0;
						pon_ron_flag[hai] = 0;
					}



					//if(mode>0){
						const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
						for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
							const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
							if (agari.get_ten_ron(my_pid, game_state) > 0) {
								if (using_haikind_array[agari.agari_info.get_hai()] - get_const_ta_cgn(cn, gn).using_haikind_num(agari.agari_info.get_hai()) < 1) {
									if (agari.ron_exp > exp_tmpbest[agari.agari_info.get_hai()]){
										hai_effective_flag[agari.agari_info.get_hai()] = 1;
										prob_tmpbest[agari.agari_info.get_hai()] = 1.0;
										exp_tmpbest[agari.agari_info.get_hai()] = agari.ron_exp;

										for(int han=0;han<5;han++){
											han_prob_tmpbest[han][agari.agari_info.get_hai()] = 0.0;
										}
										int agari_han = agari.agari_info.get_han_ron();
										han_prob_tmpbest[std::min(4,agari_han)][agari.agari_info.get_hai()] = 1.0;
										// 裏ドラについて考えるべきかもしれない。（そもそもhan_prob_tmpbestは使われていないが。）


										//if(candidates[cn].tav[gn].riich_flag==0){
											pon_ron_flag[agari.agari_info.get_hai()] = 1;
										//}
									}
								}
							}
						}
					//}


					//if(mode==2){
						const std::array<int, 3>& fuuro_edge_loc = tehai_calculator_work.get_const_fuuro_edge_loc(cn, gn);
						for (int acn = fuuro_edge_loc[1]; acn < fuuro_edge_loc[2]; acn++) {
							const Tehai_Action& fuuro_edge = tehai_calculator_work.cand_graph_sub_fuuro_work[fuuro_edge_loc[0]][acn];
							const int dst_group = fuuro_edge.dst_group;
							const int dst_group_sub = fuuro_edge.dst_group_sub;
							const int hai = fuuro_edge.hai;
							const Tehai_Location& dst_loc = tehai_calculator_work.candidates_work[dst_group].ta_loc[dst_group_sub];
							const int dst_loc_first = dst_loc.get_first();
							const int dst_loc_second = dst_loc.get_second();
							if(tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev] > tenpai_tmpbest[hai]){
								hai_effective_flag[hai] = 1;
								tenpai_tmpbest[hai] = tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								if(fuuro_edge.action_type==AT_PON){
									pon_ten_flag[hai] = 1;
								}
							}
							//if(agari_prob[cand_graph_sub[1][cn][gn][acn].dst_group][cand_graph_sub[1][cn][gn][acn].dst_group_sub][tn-1] > prob_tmpbest[cand_graph_sub[1][cn][gn][acn].hai]){
							if(tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] > exp_tmpbest[hai]){
								hai_effective_flag[hai] = 1;
								prob_tmpbest[hai] = tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								exp_tmpbest[hai] = tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];

								for(int han=0;han<5;han++){
									han_prob_tmpbest[han][hai] = tehai_calculator_work.agari_han_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev][han];
								}

								//if(cand_graph_sub[1][cn][gn][acn].action==2){
								if(fuuro_edge.action_type==AT_PON || fuuro_edge.action_type==AT_DAIMINKAN){
									pon_ron_flag[hai] = 1;
								}else{
									//pon_ron_flag[hai] = 0;
								}
							}
						}
					//}

					for(int hai=0;hai<38;hai++){
						if(hai%10 != 0 && hai_effective_flag[hai]==1){
							double nokori_tmp = 4.0 - get_const_ta_cgn(cn, gn).using_haikind_num(hai) - hai_visible[hai];
							if (using_haikind_array[hai] - get_const_ta_cgn(cn, gn).using_haikind_num(hai) > 0){
								nokori_tmp -= using_haikind_array[hai] - get_const_ta_cgn(cn, gn).using_haikind_num(hai);
							}
							if(tn!=tsumo_num){
								nokori_tmp *= rn_para;
							}
							if(tenpai_tmpbest[hai] > tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn]){
								tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] += (tenpai_tmpbest[hai] - tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn))*(1.0+2.0*pon_ten_flag[hai]);
							}
							//if(prob_tmpbest[hai] > agari_prob[cn][gn][tn]){
							if(exp_tmpbest[hai] > tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]){
								tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] += (prob_tmpbest[hai] - tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn))*(1.0+2.0*pon_ron_flag[hai]);
								tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] += (exp_tmpbest[hai] - tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn))*(1.0+2.0*pon_ron_flag[hai]);

								for(int han=0;han<5;han++){
									tehai_calculator_work.agari_han_prob_work[loc_first][loc_second][mod2tn][han] += (han_prob_tmpbest[han][hai] - tehai_calculator_work.agari_han_prob_to_work[loc_first][loc_second][mod2tn][han])*nokori_tmp/(nokori_hai_num-(tsumo_num-tn))*(1.0+2.0*pon_ron_flag[hai]);
								}
							}
						}
					}
				}
			}
		}
	//}

	clock_t check2 = clock();
	if (out_console) { std::cout << "cal_prob_time:" << (double)(check1 - check0)/CLOCKS_PER_SEC << " " << (double)(check2 - check1)/CLOCKS_PER_SEC << std::endl; }

}

void Tehai_Calculator::calc_DP(
	const int act_num,
	const int tsumo_num,
	const double exp_other, const double exp_other_ar, const double exp_other_kan,
	const std::array<float, 4>& tenpai_prob_now,
	const std::array<std::array<float, 38>, 4>& houjuu_hai_prob,
	const std::array<std::array<float, 38>, 4>& houjuu_hai_value,
	const int ori_choice_mode,
	const double reach_regression_coeff,
	double my_tenpai_prob,
	double ryuukyoku_prob_now,
	const Hai_Array& hai_visible_kind, const Game_State& game_state,
    const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp_ar,
	const Tactics& tactics
){
	clock_t check0 = clock();
	set_agari_exp(game_state, kyoku_end_pt_exp);

	if (out_console) { std::cout << "DP_in:" << act_num << " " << tsumo_num << " " << exp_other << std::endl; }

	double **tenpai_prob_other, **reach_tenpai_prob_other;
	double **houjuu_p_hai, **reach_houjuu_p_hai;
	double **houjuu_e_hai, **reach_houjuu_e_hai;
	double *other_end_prob, *reach_other_end_prob;
	other_end_prob = new double[tsumo_num+1];
	reach_other_end_prob = new double[tsumo_num+1];
	tenpai_prob_other = new double*[4];
	reach_tenpai_prob_other = new double*[4];
	for(int pid=0;pid<4;pid++){
		tenpai_prob_other[pid] = new double[tsumo_num+1];
		reach_tenpai_prob_other[pid] = new double[tsumo_num+1];
	}
	houjuu_p_hai = new double*[tsumo_num+1];
	houjuu_e_hai = new double*[tsumo_num+1];
	reach_houjuu_p_hai = new double*[tsumo_num+1];
	reach_houjuu_e_hai = new double*[tsumo_num+1];
	for(int tn=0;tn<=tsumo_num;tn++){
		houjuu_p_hai[tn] = new double[38];
		houjuu_e_hai[tn] = new double[38];
		reach_houjuu_p_hai[tn] = new double[38];
		reach_houjuu_e_hai[tn] = new double[38];
	}

	set_tenpai_prob_other(my_pid, game_state, tsumo_num, tenpai_prob_now, tenpai_prob_other, reach_tenpai_prob_other);

	double exp_ryuukyoku[2] = {};
	double exp_ryuukyoku_ar = 0.0;

	set_exp_ryuukyoku_DP(my_pid, tactics.reach_regression_mode_default, tenpai_prob_other, reach_tenpai_prob_other, ryuukyoku_pt_exp, ryuukyoku_pt_exp_ar, exp_ryuukyoku, exp_ryuukyoku_ar);

	if (out_console) {
		std::cout << "exp_ryuukyoku:" << exp_ryuukyoku[0] << " " << exp_ryuukyoku[1] << " " << exp_ryuukyoku_ar << std::endl;
	}

	for(int tn=0;tn<tsumo_num+1;tn++){
		for(int hai=0;hai<38;hai++){
			houjuu_p_hai[tn][hai] = 0.0;
			houjuu_e_hai[tn][hai] = 0.0;
			reach_houjuu_p_hai[tn][hai] = 0.0;
			reach_houjuu_e_hai[tn][hai] = 0.0;
		}
		for(int hai=1;hai<38;hai++){
			for(int pid=0;pid<4;pid++){
				if(pid!=my_pid){
					houjuu_p_hai[tn][hai] += tenpai_prob_other[pid][tn]*houjuu_hai_prob[pid][hai];
					houjuu_e_hai[tn][hai] += tenpai_prob_other[pid][tn]*houjuu_hai_prob[pid][hai]*houjuu_hai_value[pid][hai];
					reach_houjuu_p_hai[tn][hai] += reach_tenpai_prob_other[pid][tn]*houjuu_hai_prob[pid][hai];
					reach_houjuu_e_hai[tn][hai] += reach_tenpai_prob_other[pid][tn]*houjuu_hai_prob[pid][hai]*houjuu_hai_value[pid][hai];
				}
			}
		}
	}

	set_other_end_prob(my_pid, tsumo_num, act_num, tenpai_prob_now, other_end_prob, reach_other_end_prob, game_state);

	if (out_console) {
		std::cout << "tenpai_prob_DP:" << act_num << std::endl;
		for(int tn=0;tn<=tsumo_num;tn++){
			std::cout << other_end_prob[tn] << " ";
			for(int pid=0;pid<4;pid++){
				if(pid!=my_pid){
					std::cout << tenpai_prob_other[pid][tn] << " ";
				}
			}
			std::cout << "- " << reach_other_end_prob[tn] << " ";
			for(int pid=0;pid<4;pid++){
				if(pid!=my_pid){
					std::cout << reach_tenpai_prob_other[pid][tn] << " ";
				}
			}
			std::cout << std::endl;
		}
	}

	exec_calc_DP(
		reach_regression_coeff, ori_choice_mode,
		my_pid, tsumo_num,
		tehai_calculator_work,
		ryuukyoku_prob_now, exp_ryuukyoku, exp_ryuukyoku_ar,
		exp_ryuukyoku, false,
		houjuu_p_hai, reach_houjuu_p_hai, houjuu_e_hai, reach_houjuu_e_hai,
		other_end_prob, reach_other_end_prob, exp_other, exp_other_ar, exp_other_kan,
		my_tenpai_prob, houjuu_hai_prob, tenpai_prob_other, game_state, tactics
	);

	for(int pid=0;pid<4;pid++){
		delete[] tenpai_prob_other[pid];
		delete[] reach_tenpai_prob_other[pid];
	}
	delete[] tenpai_prob_other;
	delete[] reach_tenpai_prob_other;

	for(int tn=0;tn<=tsumo_num;tn++){
		delete[] houjuu_p_hai[tn];
		delete[] houjuu_e_hai[tn];
		delete[] reach_houjuu_p_hai[tn];
    	delete[] reach_houjuu_e_hai[tn];  
	}
	delete[] houjuu_p_hai;
	delete[] houjuu_e_hai;
	delete[] reach_houjuu_p_hai;
  	delete[] reach_houjuu_e_hai;

	delete[] other_end_prob;
	delete[] reach_other_end_prob;

	clock_t check1 = clock();
	if (out_console) { std::cout << "cal_prob_time:" << (double)(check1 - check0)/CLOCKS_PER_SEC << std::endl; }
}
