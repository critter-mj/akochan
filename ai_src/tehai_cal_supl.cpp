#include "tehai_cal_supl.hpp"

extern const std::array<json11::Json, 4> tactics_json;

int mod2(const int x) {
	return x % 2;
}

bool same_element_exist(const std::vector<int>& vec1, const std::vector<int>& vec2) {
	for (int i1 = 0; i1 < vec1.size(); i1++) {
		for (int i2 = 0; i2 < vec2.size(); i2++) {
			if (vec1[i1] == vec2[i2]) {
				return true;
			}
		}
	}
	return false;
}

void set_tsumo_node_exec(
	Tehai_Change& tehai_change_new,
	bool is_in0,
	boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
	int& in0num
){
	//mtx.lock();
	if (candidates_work.size() >= MAX_CANDIDATES_NUM) {
		// 何かアラートを出すべき？
		return;
	}

	tehai_change_new.id = candidates_work.size();
	// tehai_changeにidをふることはmapでやったほうがわかりやすいかもしれない。要検討
	const std::pair< boost::unordered_set<Tehai_Change>::iterator, bool> ret = tcs.insert(tehai_change_new);
	//std::pair<std::unordered_set<Tehai_Change, Tehai_Change_Hash, Tehai_Change_EqualTo>::iterator, bool> ret;
	if(ret.second){
		if(is_in0){
			in0num++;
		}
		Tehai_Group cand_group;
		cand_group.tehai_change = tehai_change_new;
		candidates_work.push_back(cand_group);
	}
	//mtx.unlock();
}

void set_tsumo_node_child(
	const Bit_Hai_Num& bit_hai_num,
	const int tehai_all_num,
	std::vector<int>& hai_in_pattern_nip,
	std::vector<int>& remain,
	int nin,
	boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
	int& in0num
){
	Tehai_Change tehai_change_tmp_new;
	tehai_change_tmp_new.tehai_base = bit_hai_num;
	const int nout = tehai_all_num - 13 + nin;
	do{
		for(int i=0;i<nin;i++){
			tehai_change_tmp_new.tehai_base.add_hai(hai_in_pattern_nip[i]);
		}
		do{
			for(int i=0;i<nout;i++){
				tehai_change_tmp_new.tehai_base.delete_hai(remain[i]);
			}
			set_tsumo_node_exec(tehai_change_tmp_new, false, candidates_work, tcs, in0num);
			for(int i=0;i<nout;i++){
				tehai_change_tmp_new.tehai_base.add_hai(remain[i]);
			}
		}while(boost::next_combination(remain.begin(), remain.begin()+nout, remain.end()));
		for(int i=0;i<nin;i++){
			tehai_change_tmp_new.tehai_base.delete_hai(hai_in_pattern_nip[i]);
		}
	}while(boost::next_combination(hai_in_pattern_nip.begin(), hai_in_pattern_nip.begin()+nin, hai_in_pattern_nip.end()));
}

void set_tsumo_node_mentu(
	const Bit_Hai_Num& bit_hai_num,
	const int tehai_all_num,
	Tehai_Analyzer ta_in,
	const int fuuro_cand_hai,
	const int in_num_end,
	boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates_work,
	boost::unordered_set<Tehai_Change>& tcs,
	int& in0num
){
	for(int nin=1;nin<=in_num_end;nin++){
		// ここのfor文を並列化する際には、必ずset_tsumo_node_execでmtxをロックする必要がある。
		//#pragma omp parallel
		//#pragma omp for
		for(int nip=0;nip<ta_in.inout_pattern_vec[in_num_end].size();nip++){
			if(fuuro_cand_hai!=0 && ta_in.get_tehai_num() + ta_in.get_fuuro_num()*3==14){
				ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern.push_back(haikind(fuuro_cand_hai));
				std::sort(ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern.begin(), ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern.end());
			}
			if(same_element_exist(ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern, ta_in.inout_pattern_vec[in_num_end][nip].hai_out_pattern)==0){
				//if(in_num_end<5){
					set_tsumo_node_child(bit_hai_num, tehai_all_num,
						ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern, ta_in.inout_pattern_vec[in_num_end][nip].hai_out_pattern,
						nin, candidates_work, tcs, in0num
					);
				//}
			}
			if(fuuro_cand_hai!=0 && ta_in.get_tehai_num() + ta_in.get_fuuro_num()*3==14){
				ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern.erase(ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern.begin() + loc_intvec(ta_in.inout_pattern_vec[in_num_end][nip].hai_in_pattern, haikind(fuuro_cand_hai)));
			}
		}
	}
}

void set_tenpai_prob_other(const int my_pid, const Game_State& game_state, const int tsumo_num, const std::array<float, 4>& tenpai_prob_now, double **tenpai_prob_other, double **reach_tenpai_prob_other) {
	if (tactics_json[my_pid]["tenpai_after_est"] == "instant") {
		for(int pid=0;pid<4;pid++){
			for(int tn=0;tn<=tsumo_num;tn++){
				tenpai_prob_other[pid][tsumo_num-tn] = tenpai_prob_now[pid];
				reach_tenpai_prob_other[pid][tsumo_num-tn] = tenpai_prob_now[pid];
			}
		}
	} else if (tactics_json[my_pid]["tenpai_after_est"] == "ako") {
		assert_with_out(!tactics_json[my_pid]["tenpai_after_est_begin"].is_null(), "tenpai_after_est_begin_is_null");
		const int act_num = game_state.player_state[my_pid].kawa.size();
		const int after_begin = tactics_json[my_pid]["tenpai_after_est_begin"].int_value(); // ako のデフォルトは0
		const int other_reach_declared_num = get_other_reach_declared_num(my_pid, game_state);
		for (int pid = 0; pid < 4; pid++) {
			for (int tn = 0; tn <= tsumo_num; tn++) {
				if (game_state.player_state[pid].reach_declared) {
					tenpai_prob_other[pid][tsumo_num-tn] = 1.0;
					reach_tenpai_prob_other[pid][tsumo_num-tn] = 1.0;
				} else {
					tenpai_prob_other[pid][tsumo_num-tn] = cal_tenpai_after_prob_ako(false, act_num, act_num + after_begin + tn, tenpai_prob_now[pid]);
					reach_tenpai_prob_other[pid][tsumo_num-tn] = cal_tenpai_after_prob_ako(true, act_num, act_num + after_begin + tn, tenpai_prob_now[pid]);
				}
				if (0 < other_reach_declared_num) { // to do ako この処理を kanon へ反映したほうがよいかもしれない。
					tenpai_prob_other[pid][tsumo_num-tn] = reach_tenpai_prob_other[pid][tsumo_num-tn];
				}
			}
		}
	} else {
		std::cout << "tenpai_after_est_error:" << std::endl;
		assert(false);
	}
}

float cal_other_end_prob_ako(const bool my_reach, const int act_num1, const int act_num2, const int other_reach_num, const float other_end_prob_input) {
	const int lb = my_reach ? 7 : 6;
	const int ub = my_reach ? 15 : 16;
	const int ac1 = std::min(std::max(lb, act_num1), ub);
	const int ac2 = std::min(std::max(lb, act_num2), ub);
	const std::string file_name = my_reach ?
		"params/other_end_prob/ako/reach_other_end_para" + std::to_string(ac1) + "_" + std::to_string(ac2) + "_1000.txt" :
		"params/other_end_prob/ako/other_end_para" + std::to_string(ac1) + "_" + std::to_string(ac2) + "_4000.txt";
	float w[3];
	read_parameters(w, 3, file_name);

	float x[3];
	x[0] = 1.0;
	x[1] = other_reach_num;
	x[2] = other_end_prob_input;
	return logistic(w, x, 3);
}

float cal_tenpai_after_prob_ako(const bool my_reach, const int act_num1, const int act_num2, const float tenpai_prob_now) {
	if (act_num2 <= act_num1) { return tenpai_prob_now; }

	const std::string file_name = my_reach ?
		"params/tenpai_after/ako/reach_tenpai_after_para" + std::to_string(std::min(std::max(1,act_num1),15)) + "_" + std::to_string(std::min(std::max(3, act_num2),18)) + "_500.txt" :
		"params/tenpai_after/ako/tenpai_after_para" + std::to_string(std::min(std::max(1,act_num1),17)) + "_" + std::to_string(std::min(std::max(2, act_num2),18)) + "_4000.txt";

	float w[2];
	read_parameters(w, 2, file_name);

	float x[2];
	x[0] = 1.0;
	x[1] = my_logit(tenpai_prob_now);
	return logistic(w, x, 2);
}

void set_other_end_prob(const int my_pid, const int tsumo_num, const int act_num, const std::array<float, 4>& tenpai_prob_now, double *other_end_prob, double *reach_other_end_prob, const Game_State& game_state) {
	if (tactics_json[my_pid]["other_end_prob_est"] == "instant") {
		for(int tn=0;tn<=tsumo_num;tn++){
			other_end_prob[tsumo_num-tn] = 0.1;
			reach_other_end_prob[tsumo_num-tn] = 0.1;
		}
	} else if (tactics_json[my_pid]["other_end_prob_est"] == "ako") {
		float other_end_prob_input = 1.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid && !game_state.player_state[pid].reach_declared){
				other_end_prob_input *= (1.0 - tenpai_prob_now[pid]);
			}
		}
		other_end_prob_input = 1.0 - other_end_prob_input;
		for (int tn=0;tn<=tsumo_num;tn++) {
			other_end_prob[tsumo_num-tn] = cal_other_end_prob_ako(false, act_num, act_num + tn, get_other_reach_declared_num(my_pid, game_state), other_end_prob_input);
			reach_other_end_prob[tsumo_num-tn] = cal_other_end_prob_ako(true, act_num, act_num + tn, get_other_reach_declared_num(my_pid, game_state), other_end_prob_input);
		}
	} else { assert_with_out(false, "cal_other_end_prob_error:"); }
}

void set_exp_ryuukyoku_DP(
    const int my_pid, const int reach_regression_mode,
    const double * const * tenpai_prob_other, const double * const * reach_tenpai_prob_other,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp,
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp_ar,
    double exp_ryuukyoku[2], double& exp_ryuukyoku_ar
) {
    int flag_tmp[4];
	for(int t0=0;t0<2;t0++){
		for(int t1=0;t1<2;t1++){
			for(int t2=0;t2<2;t2++){
				for(int t3=0;t3<2;t3++){
					flag_tmp[0] = t0;
					flag_tmp[1] = t1;
					flag_tmp[2] = t2;
					flag_tmp[3] = t3;
					double prob_tmp = 1.0;
					for(int pid=0;pid<4;pid++){
						if(pid!=my_pid){
							if(flag_tmp[pid]==1){
								prob_tmp *= tenpai_prob_other[pid][0];
							}else{
								prob_tmp *= 1.0 - tenpai_prob_other[pid][0];
							}
						}
					}
					exp_ryuukyoku[flag_tmp[my_pid]] += ryuukyoku_pt_exp[t0][t1][t2][t3]*prob_tmp;
					if(flag_tmp[my_pid]==1){
						prob_tmp = 1.0;
						for(int pid=0;pid<4;pid++){
							if(pid!=my_pid){
								if(flag_tmp[pid]==1){
									prob_tmp *= reach_tenpai_prob_other[pid][0];
								}else{
									prob_tmp *= 1.0 - reach_tenpai_prob_other[pid][0];
								}
							}
						}
						exp_ryuukyoku_ar += ryuukyoku_pt_exp_ar[t0][t1][t2][t3]*prob_tmp;
					}
				}
			}
		}
	}
}

double cal_ori_exp_ako(
	const int my_pid, const int cn, const int gn, const int tn, const float wa[3], Tehai_Calculator_Work& tehai_calculator_work,
	double **houjuu_p_hai, double **houjuu_e_hai, double exp_other, const int tsumo_num, const int ori_choice_mode, double ryuukyoku_prob, double noten_ryuukyoku_value,
	const Hai_Array tehai_kind, const Fuuro_Vector fuuro_kind
) {
	Hai_Array using_haikind_array = using_hai_array(tehai_kind, fuuro_kind);
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	const int loc_first = loc.get_first();
	const int loc_second = loc.get_second();
	std::array<float, 38> houjuu_prob = {};
	std::array<float, 38> houjuu_value = {};
	for(int hai=1;hai<38;hai++){
		houjuu_prob[hai] = houjuu_p_hai[tn][hai];
		if (hai%10 != 0 && using_haikind_array[hai] - tehai_calculator_work.cal_tav_work[loc_first][loc_second].using_haikind_num(hai) > 0){
			houjuu_prob[hai] = 0.0;
			if(hai%10 == 5) {
				houjuu_prob[hai+5] = 0.0;
			}
		}
		if(houjuu_prob[hai]>0.0){
			houjuu_value[hai] = houjuu_e_hai[tn][hai]/houjuu_prob[hai];
		}
	}
	Betaori betaori;
	betaori.set_condition(houjuu_prob, houjuu_value, exp_other, noten_ryuukyoku_value);
	betaori.set_tehai_ta(tehai_calculator_work.cal_tav_work[loc_first][loc_second]);
	betaori.set_betaori_value(tn);
	if(ori_choice_mode==2){
		if(tehai_calculator_work.cal_tav_work[loc_first][loc_second].get_fuuro_num() < 2){
			betaori.modify_betaori_value_with_ryuukyoku_ako(
				tehai_calculator_work.cal_tav_work[loc_first][loc_second].get_fuuro_num(),
				wa, ryuukyoku_prob
			);
		}
	}
	return betaori.betaori_exp;
}

double cal_ori_exp(
	const int my_pid, const int cn, const int gn, const int tn, Tehai_Calculator_Work& tehai_calculator_work,
	double **houjuu_p_hai, double **houjuu_e_hai, double exp_other, const int tsumo_num, const int ori_choice_mode, double ryuukyoku_prob, double noten_ryuukyoku_value,
	const Hai_Array tehai_kind, const Fuuro_Vector fuuro_kind, const Game_State& game_state
) {
	Hai_Array using_haikind_array = using_hai_array(tehai_kind, fuuro_kind);
	const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
	const int loc_first = loc.get_first();
	const int loc_second = loc.get_second();
	std::array<float, 38> houjuu_prob = {};
	std::array<float, 38> houjuu_value = {};
	for (int hai = 1; hai < 38; hai++) {
		houjuu_prob[hai] = houjuu_p_hai[tn][hai];
		if (hai%10 != 0 && using_haikind_array[hai] - tehai_calculator_work.cal_tav_work[loc_first][loc_second].using_haikind_num(hai) > 0){
			houjuu_prob[hai] = 0.0;
			if (hai % 10 == 5) {
				houjuu_prob[hai+5] = 0.0;
			}
		}
		if (houjuu_prob[hai] > 0.0) {
			houjuu_value[hai] = houjuu_e_hai[tn][hai] / houjuu_prob[hai];
		}
	}
	Betaori betaori;

	betaori.set_condition(houjuu_prob, houjuu_value, exp_other, noten_ryuukyoku_value);

	betaori.set_tehai_ta(tehai_calculator_work.cal_tav_work[loc_first][loc_second]);
	betaori.set_betaori_value(tn);
	return betaori.betaori_exp;
}

float get_coeff_for_ron_DP_ako(
    const int hai, const int my_pid, const Tehai_Analyzer_Basic& ta,
    const int pon_ron_flag[38], const int ron_flag[38], 
    const double my_tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, double** tenpai_prob_other, const int tn,
    const float reach_ron_ratio_para[5], const float not_reach_ron_ratio_para[5], const float dama_ron_ratio[6]
) {
	float get_coeff_reg = 1.0 + 2.0*pon_ron_flag[hai];
    if (ron_flag[hai] == 1) {
        if (ta.get_fuuro_num() == 0) {
            if (hai>30) {
                get_coeff_reg = dama_ron_ratio[0]/(1.0-dama_ron_ratio[0]);
            } else {
                int tmpi = 5 - abs(hai%10-5);
                get_coeff_reg = dama_ron_ratio[tmpi]/(1.0-dama_ron_ratio[tmpi]);
            }
        } else {
			float x[3] = {};
			float tmp;
			x[0] = 1.0;
			for (int pid = 0; pid < 4;pid++) {
				if (pid != my_pid) {
					x[2] += tenpai_prob_other[pid][tn];
				}
			}
			x[1] = my_tenpai_prob*houjuu_hai_prob[my_pid][hai];
			tmp = logistic(not_reach_ron_ratio_para, x, 3);

			if(tmp!=1.0){
				get_coeff_reg = tmp/(1.0-tmp);
			}
        }
    }
    return get_coeff_reg;
}

float get_coeff_for_ron_DP(
    const int hai, const int reach_regression_mode, const double reach_regression_coeff,
    const int my_pid, const Tehai_Analyzer_Basic& ta,
    const int pon_ron_flag[38], const int ron_flag[38], 
    const double my_tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, double** tenpai_prob_other, const int tn,
    const float dama_ron_ratio[6], const Game_State& game_state
) {
	float get_coeff = 1.0 + 2.0*pon_ron_flag[hai];
	if (tactics_json[my_pid]["ron_ratio_est"] == "instant") {
    	return get_coeff;
	} else {
		std::cout << "cal_ron_ratio_error" << std::endl;
		assert(false);
	}
    return get_coeff;
}

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
) {
	const std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav = tehai_calculator_work.cal_tav_work;
	const boost::container::static_vector<Tehai_Group, MAX_CANDIDATES_NUM>& candidates = tehai_calculator_work.candidates_work;
	const Hai_Array tehai_kind = haikind(game_state.player_state[my_pid].tehai);
	const Fuuro_Vector fuuro_kind = haikind(game_state.player_state[my_pid].fuuro);

	const int nokori_hai_num = get_nokori_hai_num(my_pid, game_state);
	const Hai_Array hai_visible = haikind(get_hai_visible_wo_tehai(my_pid, game_state));
	const Hai_Array using_haikind_array = haikind(using_hai_array(game_state.player_state[my_pid].tehai, game_state.player_state[my_pid].fuuro));

    //for(int mode=0;mode<3;mode++){
	for(int cn=0;cn<candidates.size();cn++){
		for(int gn=0;gn<candidates[cn].ta_loc.size();gn++){
			const Tehai_Location& loc = candidates[cn].ta_loc[gn];
			const int loc_first = loc.get_first();
			const int loc_second = loc.get_second();
			tehai_calculator_work.agari_prob_work[loc_first][loc_second][0] = 0.0;
			tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][0] = 0.0;
			tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][0] = 1.0 * cal_tav[loc_first][loc_second].get_tenpai_flag();
			tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][0] = 1.0 * cal_tav[loc_first][loc_second].get_tenpai_flag();
			tehai_calculator_work.agari_exp_work[loc_first][loc_second][0] = 0.0;
			tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][0] = 0.0;
			if (cal_tav[loc_first][loc_second].get_tenpai_flag() == 1) {
				tehai_calculator_work.ten_exp_work[loc_first][loc_second][0] = exp_ryuukyoku[1];
				tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][0] = exp_ryuukyoku[1];
			} else {
				tehai_calculator_work.ten_exp_work[loc_first][loc_second][0] = exp_ryuukyoku[0];
				tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][0] = exp_ryuukyoku[0];
			}
		}
	}

	// akoでない場合特に使わないパラメータ
	float reach_ron_ratio_para[5] = {};
	float not_reach_ron_ratio_para[5] = {};
	float dama_ron_ratio[6] = {};
	//double yama_ratio_para[2] = {};
	float wa[MAX_TSUMO_NUM][3];

	if (tactics_json[my_pid]["ron_ratio_est"] == "ako") {
		read_parameters(reach_ron_ratio_para, 3, "params/ron_ratio/ako/reach_ron_ratio_para4000.txt");
		read_parameters(not_reach_ron_ratio_para, 3, "params/ron_ratio/ako/not_reach_ron_ratio_para4000.txt");
		read_parameters(dama_ron_ratio, 6, "params/ron_ratio/ako/dama_ron_ratio.txt");
	} 
	if (tactics_json[my_pid]["inclusive_fold_est"] == "ako") {
		for (int t = 2; t <= 18; t++) {
			read_parameters(wa[t], 3, "params/inclusive_fold/ako/betaori_houjuu_para" + std::to_string(t) + "_3000.txt");
		}
	}

	for(int tn=1;tn<=tsumo_num;tn++){
		const int mod2tn = mod2(tn);
		const int mod2tn_prev = mod2(tn-1);
		const double ryuukyoku_prob = 1.0 + tn*(ryuukyoku_prob_now-1.0)/tsumo_num;
		if (ori_choice_mode > 0) {
			if (tactics_json[my_pid]["inclusive_fold_est"] == "ako") {
				const int model_idx = std::min(std::max(2, (int)game_state.player_state[my_pid].kawa.size() + tsumo_num - tn), 18);
				#pragma omp parallel
				#pragma omp for
				for(int cn=0;cn<candidates.size();cn++){
					for(int gn=0;gn<candidates[cn].ta_loc.size();gn++){
						const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
						tehai_calculator_work.ori_exp_work[loc.get_first()][loc.get_second()] = cal_ori_exp_ako(
							my_pid, cn, gn, tn, wa[model_idx], tehai_calculator_work,
							houjuu_p_hai, houjuu_e_hai, exp_other, tsumo_num, ori_choice_mode, ryuukyoku_prob, exp_ryuukyoku[0],
							tehai_kind, fuuro_kind
						);
					}
				}
			} else {
				#pragma omp parallel
				#pragma omp for
				for(int cn=0;cn<candidates.size();cn++){
					for(int gn=0;gn<candidates[cn].ta_loc.size();gn++){
						const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
						tehai_calculator_work.ori_exp_work[loc.get_first()][loc.get_second()] = cal_ori_exp(
							my_pid, cn, gn, tn, tehai_calculator_work,
							houjuu_p_hai, houjuu_e_hai, exp_other, tsumo_num, ori_choice_mode, ryuukyoku_prob, exp_ryuukyoku[0],
							tehai_kind, fuuro_kind, game_state
						);
					}
				}
			}
		}

		const double norisk_ratio = 0.0; // 0.1;
		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates.size();cn++){
			for(int gn=0;gn<candidates[cn].ta_loc.size();gn++){
				const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
				const int loc_first = loc.get_first();
				const int loc_second = loc.get_second();

				tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] = 0.0;
				tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] = 0.0;
				tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn] = 0.0;
				tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] = 0.0;

				double prob_tmpbest[38], tenpai_tmpbest[38], agari_exp_tmpbest[38], exp_tmpbest[38];
				double prob_tmpbest_norisk[38], tenpai_tmpbest_norisk[38], agari_exp_tmpbest_norisk[38], exp_tmpbest_norisk[38];
				for(int hai=0;hai<38;hai++){
					if(hai%10!=0){
						prob_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai])*tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn_prev];
						tenpai_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai])*tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev];
						agari_exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai])*tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn_prev];
						exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai])*tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev] + houjuu_e_hai[tn-1][hai];
						if (norisk_ratio > 0.0) {
							prob_tmpbest_norisk[hai] = tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn_prev];
							tenpai_tmpbest_norisk[hai] = tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn_prev];
							agari_exp_tmpbest_norisk[hai] = tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn_prev];
							exp_tmpbest_norisk[hai] = tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn_prev];
						}
					}
				}
				const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
				for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
					const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
					if (agari.get_ten_tsumo(my_pid, game_state) > 0) {
						if(agari.tsumo_exp > exp_tmpbest[agari.agari_info.get_hai()]){
							tenpai_tmpbest[agari.agari_info.get_hai()] = 0.0;
							prob_tmpbest[agari.agari_info.get_hai()] = 1.0;
							agari_exp_tmpbest[agari.agari_info.get_hai()] = agari.tsumo_exp;
							exp_tmpbest[agari.agari_info.get_hai()] = agari.tsumo_exp;
						}
						if (norisk_ratio > 0.0) {
							if (agari.tsumo_exp > exp_tmpbest_norisk[agari.agari_info.get_hai()]) {
								tenpai_tmpbest_norisk[agari.agari_info.get_hai()] = 0.0;
								prob_tmpbest_norisk[agari.agari_info.get_hai()] = 1.0;
								agari_exp_tmpbest_norisk[agari.agari_info.get_hai()] = agari.tsumo_exp;
								exp_tmpbest_norisk[agari.agari_info.get_hai()] = agari.tsumo_exp;
							}
						}
					}
				}

				const std::array<int, 3>& tsumo_edge_loc = tehai_calculator_work.get_const_tsumo_edge_loc(cn, gn);
				for (int acn = tsumo_edge_loc[1]; acn < tsumo_edge_loc[2]; acn++) {
					const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_tsumo_work[tsumo_edge_loc[0]][acn];
					const int& hai = ac_tmp.hai;
					const int& hai_out = ac_tmp.hai_out;
					const int& dst_group = ac_tmp.dst_group;
					const int& dst_group_sub = ac_tmp.dst_group_sub;
					const Tehai_Location& dst_loc = tehai_calculator_work.candidates_work[dst_group].ta_loc[dst_group_sub];
					const int dst_loc_first = dst_loc.get_first();
					const int dst_loc_second = dst_loc.get_second();
					//if(agari_prob[cand_graph_sub[cn][gn][acn].dst_group][cand_graph_sub[cn][gn][acn].dst_group_sub][tn-1] > prob_tmpbest[cand_graph_sub[cn][gn][acn].hai]){
					//if(ten_exp[cand_graph_sub[cn][gn][acn].dst_group][cand_graph_sub[cn][gn][acn].dst_group_sub][tn-1] > exp_tmpbest[cand_graph_sub[cn][gn][acn].hai]){
					if((1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] + houjuu_e_hai[tn-1][hai_out] > exp_tmpbest[hai]){
						if (cal_tav[dst_loc_first][dst_loc_second].get_neg_flag()==0) {
							tenpai_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							prob_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							agari_exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.agari_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] + houjuu_e_hai[tn-1][hai_out];
						}
					}
					if (norisk_ratio > 0.0) {
						if (tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] > exp_tmpbest_norisk[hai]) {
							if (cal_tav[dst_loc_first][dst_loc_second].get_neg_flag()==0) {
								tenpai_tmpbest_norisk[hai] = tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								prob_tmpbest_norisk[hai] = tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								agari_exp_tmpbest_norisk[hai] = tehai_calculator_work.agari_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];
								exp_tmpbest_norisk[hai] = tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							}
						}
					}
				}

				double nokori_all2 = 0;
				for(int hai=0;hai<38;hai++){
					if(hai%10!=0){
						double nokori_tmp = 4 - cal_tav[loc_first][loc_second].using_haikind_num(hai) - hai_visible[hai];
						if (using_haikind_array[hai] - cal_tav[loc_first][loc_second].using_haikind_num(hai) > 0){
							nokori_tmp -= using_haikind_array[hai] - cal_tav[loc_first][loc_second].using_haikind_num(hai);
						}
						nokori_all2 += nokori_tmp;

						tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * tenpai_tmpbest[hai] * nokori_tmp;
						tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * prob_tmpbest[hai] * nokori_tmp;
						tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * agari_exp_tmpbest[hai] * nokori_tmp;
						tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * exp_tmpbest[hai] * nokori_tmp;

						if (norisk_ratio > 0.0) {
							tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] += norisk_ratio * tenpai_tmpbest_norisk[hai] * nokori_tmp;
							tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] += norisk_ratio * prob_tmpbest_norisk[hai] * nokori_tmp;
							tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn] += norisk_ratio * agari_exp_tmpbest_norisk[hai] * nokori_tmp;
							tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] += norisk_ratio * exp_tmpbest_norisk[hai] * nokori_tmp;
						}
					}
				}
				tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn]/nokori_all2;
				tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn]/nokori_all2;
				tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn]/nokori_all2;
				tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]/nokori_all2;

				//if(ori_choice_flag==1 && ori_exp[cn][gn][tn] > ten_exp_to[cn][gn][tn]){
				//if(ori_choice_mode>0 && cal_tav[loc_first][loc_second].get_reach_flag()==0 && tehai_calculator_work.ori_exp_work[loc_first][loc_second][tn] > tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]){
				if(ori_choice_mode>0 && tehai_calculator_work.ori_exp_work[loc_first][loc_second] > tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]){
					if (tactics.use_ori_exp_at_dp_fuuro || cal_tav[loc_first][loc_second].get_fuuro_num() == game_state.player_state[my_pid].fuuro.size()) {
						tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn] = 0.0;
						tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn] = 0.0;
						tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn] = 0.0;
						tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ori_exp_work[loc_first][loc_second];
					}
				}
			}
		}

		#pragma omp parallel
		#pragma omp for
		for(int cn=0;cn<candidates.size();cn++){
			for(int gn=0;gn<candidates[cn].ta_loc.size();gn++){
				const Tehai_Location& loc = tehai_calculator_work.candidates_work[cn].ta_loc[gn];
				const int loc_first = loc.get_first();
				const int loc_second = loc.get_second();
				tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn];

				double prob_tmpbest[38], tenpai_tmpbest[38], agari_exp_tmpbest[38], exp_tmpbest[38];
				int pon_ron_flag[38], ron_flag[38];
				double prob_tmpbest_norisk[38], tenpai_tmpbest_norisk[38], agari_exp_tmpbest_norisk[38], exp_tmpbest_norisk[38];
				int pon_ron_flag_norisk[38], ron_flag_norisk[38];
				for(int hai=0;hai<38;hai++){
					if(hai%10!=0){
						prob_tmpbest[hai] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn];
						tenpai_tmpbest[hai] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn];
						agari_exp_tmpbest[hai] = tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn];
						exp_tmpbest[hai] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn];

						pon_ron_flag[hai] = 0;
						ron_flag[hai] = 0;

						if (norisk_ratio > 0.0) {
							prob_tmpbest_norisk[hai] = tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn];
							tenpai_tmpbest_norisk[hai] = tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn];
							agari_exp_tmpbest_norisk[hai] = tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn];
							exp_tmpbest_norisk[hai] = tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn];

							pon_ron_flag_norisk[hai] = 0.0;
							ron_flag_norisk[hai] = 0.0;
						}
					}
				}

				//if(mode>0){
				const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
				for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
					const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
					if (agari.get_ten_ron(my_pid, game_state) > 0) {
						if (using_haikind_array[agari.agari_info.get_hai()] - cal_tav[loc_first][loc_second].using_haikind_num(agari.agari_info.get_hai()) < 1){
							if(agari.ron_exp > exp_tmpbest[agari.agari_info.get_hai()]){
								tenpai_tmpbest[agari.agari_info.get_hai()] = 0.0;
								prob_tmpbest[agari.agari_info.get_hai()] = 1.0;
								agari_exp_tmpbest[agari.agari_info.get_hai()] = agari.ron_exp;
								exp_tmpbest[agari.agari_info.get_hai()] = agari.ron_exp;
								//if(candidates[cn].tav[gn].riich_flag==0){
									pon_ron_flag[agari.agari_info.get_hai()] = 1;
									ron_flag[agari.agari_info.get_hai()] = 1;
								//}
							}
							if (norisk_ratio > 0.0) {
								if (agari.ron_exp > exp_tmpbest[agari.agari_info.get_hai()]) {
									tenpai_tmpbest_norisk[agari.agari_info.get_hai()] = 0.0;
									prob_tmpbest_norisk[agari.agari_info.get_hai()] = 1.0;
									agari_exp_tmpbest_norisk[agari.agari_info.get_hai()] = agari.ron_exp;
									exp_tmpbest_norisk[agari.agari_info.get_hai()] = agari.ron_exp;

									pon_ron_flag_norisk[agari.agari_info.get_hai()] = 1;
									ron_flag_norisk[agari.agari_info.get_hai()] = 1;
								}
							}
						}
					}
				}
				//}

				//if(mode==2){
				const std::array<int, 3>& fuuro_edge_loc = tehai_calculator_work.get_const_fuuro_edge_loc(cn, gn);
				for (int acn = fuuro_edge_loc[1]; acn < fuuro_edge_loc[2]; acn++) {
					const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_fuuro_work[fuuro_edge_loc[0]][acn];
					const int& hai = ac_tmp.hai;
					const int& hai_out = ac_tmp.hai_out;
					const int& dst_group = ac_tmp.dst_group;
					const int& dst_group_sub = ac_tmp.dst_group_sub;
					const Tehai_Location& dst_loc = tehai_calculator_work.candidates_work[dst_group].ta_loc[dst_group_sub];
					const int dst_loc_first = dst_loc.get_first();
					const int dst_loc_second = dst_loc.get_second();
					//if(agari_prob[cand_graph_sub[cn][gn][acn].dst_group][cand_graph_sub[cn][gn][acn].dst_group_sub][tn-1] > prob_tmpbest[cand_graph_sub[cn][gn][acn].hai]){
					//if(ten_exp[cand_graph_sub[1][cn][gn][acn].dst_group][cand_graph_sub[1][cn][gn][acn].dst_group_sub][tn-1] > exp_tmpbest[cand_graph_sub[1][cn][gn][acn].hai]){
					if((1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] + houjuu_e_hai[tn-1][hai_out] > exp_tmpbest[hai]){
						tenpai_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
						prob_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
						agari_exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.agari_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];
						exp_tmpbest[hai] = (1.0-houjuu_p_hai[tn-1][hai_out])*tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] + houjuu_e_hai[tn-1][hai_out];

						if(ac_tmp.action_type==AT_PON){
							pon_ron_flag[hai] = 1;
						}else{
							pon_ron_flag[hai] = 0;
						}
					}

					if (norisk_ratio > 0.0) {
						if(tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev] > exp_tmpbest_norisk[hai]){
							tenpai_tmpbest_norisk[hai] = tehai_calculator_work.tenpai_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							prob_tmpbest_norisk[hai] = tehai_calculator_work.agari_prob_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							agari_exp_tmpbest_norisk[hai] = tehai_calculator_work.agari_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];
							exp_tmpbest_norisk[hai] = tehai_calculator_work.ten_exp_work[dst_loc_first][dst_loc_second][mod2tn_prev];

							if(ac_tmp.action_type==AT_PON){
								pon_ron_flag_norisk[hai] = 1;
							}else{
								pon_ron_flag_norisk[hai] = 0;
							}
						}
					}
				}
				//}
				for(int hai=0;hai<38;hai++){
					if(hai%10!=0){
						int nokori_tmp = 4 - cal_tav[loc_first][loc_second].using_haikind_num(hai) - hai_visible[hai];
						if (using_haikind_array[hai] - cal_tav[loc_first][loc_second].using_haikind_num(hai) > 0){
							nokori_tmp -= using_haikind_array[hai] - cal_tav[loc_first][loc_second].using_haikind_num(hai);
						}
						//if(prob_tmpbest[hai] > agari_prob[cn][gn][tn]){
						if(exp_tmpbest[hai] > tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]){
							float get_coeff = tactics_json[my_pid]["ron_ratio_est"] == "ako" ?
							get_coeff_for_ron_DP_ako(
								hai, my_pid, cal_tav[loc_first][loc_second], pon_ron_flag, ron_flag, 
								my_tenpai_prob, houjuu_hai_prob, tenpai_prob_other, tn,
								reach_ron_ratio_para, not_reach_ron_ratio_para, dama_ron_ratio
							):
							get_coeff_for_ron_DP(
								hai, tactics.reach_regression_mode_default, reach_regression_coeff, my_pid, cal_tav[loc_first][loc_second], pon_ron_flag, ron_flag, 
								my_tenpai_prob, houjuu_hai_prob, tenpai_prob_other, tn,
								dama_ron_ratio, game_state
							);
							tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * (tenpai_tmpbest[hai] - tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
							tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * (prob_tmpbest[hai] - tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
							tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * (agari_exp_tmpbest[hai] - tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
							tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] += (1.0 - norisk_ratio) * (exp_tmpbest[hai] - tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
						}
						if (norisk_ratio > 0.0) {
							if (exp_tmpbest_norisk[hai] > tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn]) {
								float get_coeff = tactics_json[my_pid]["ron_ratio_est"] == "ako" ?
								get_coeff_for_ron_DP_ako(
									hai, my_pid, cal_tav[loc_first][loc_second], pon_ron_flag_norisk, ron_flag_norisk, 
									my_tenpai_prob, houjuu_hai_prob, tenpai_prob_other, tn,
									reach_ron_ratio_para, not_reach_ron_ratio_para, dama_ron_ratio
								):
								get_coeff_for_ron_DP(
									hai, tactics.reach_regression_mode_default, reach_regression_coeff, my_pid, cal_tav[loc_first][loc_second], pon_ron_flag_norisk, ron_flag_norisk, 
									my_tenpai_prob, houjuu_hai_prob, tenpai_prob_other, tn,
									dama_ron_ratio, game_state
								);
								tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] += norisk_ratio * (tenpai_tmpbest_norisk[hai] - tehai_calculator_work.tenpai_prob_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
								tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] += norisk_ratio * (prob_tmpbest_norisk[hai] - tehai_calculator_work.agari_prob_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
								tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn] += norisk_ratio * (agari_exp_tmpbest_norisk[hai] - tehai_calculator_work.agari_exp_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
								tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] += norisk_ratio * (exp_tmpbest_norisk[hai] - tehai_calculator_work.ten_exp_to_work[loc_first][loc_second][mod2tn])*double(nokori_tmp)/(nokori_hai_num-(tsumo_num-tn))*get_coeff;
							}
						}
					}
				}

				double other_end_prob_tmp, other_end_value_tmp;
				if (cal_tav[loc_first][loc_second].get_kan_changed_flag() == 1) {
					other_end_prob_tmp = other_end_prob[tn-1];
					other_end_value_tmp = exp_other_kan;
				} else {
					other_end_prob_tmp = other_end_prob[tn-1];
					other_end_value_tmp = exp_other;
				}

				tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] = (1.0 - other_end_prob_tmp)*tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] = (1.0 - other_end_prob_tmp)*tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn] = (1.0 - other_end_prob_tmp)*tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn];
				tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] = other_end_prob_tmp*other_end_value_tmp + (1.0 - other_end_prob_tmp)*tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn];

				//if(ori_choice_mode>0 && cal_tav[loc_first][loc_second].get_reach_flag()==0 && tehai_calculator_work.ori_exp_work[loc_first][loc_second][tn] > tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn]){
				if(ori_choice_mode>0 && tehai_calculator_work.ori_exp_work[loc_first][loc_second] > tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn]){
					if (tactics.use_ori_exp_at_dp_fuuro || cal_tav[loc_first][loc_second].get_fuuro_num() == game_state.player_state[my_pid].fuuro.size()) {
						tehai_calculator_work.tenpai_prob_work[loc_first][loc_second][mod2tn] = 0.0;
						tehai_calculator_work.agari_prob_work[loc_first][loc_second][mod2tn] = 0.0;
						tehai_calculator_work.agari_exp_work[loc_first][loc_second][mod2tn] = 0.0;
						//tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ori_exp_work[loc_first][loc_second][tn];
						tehai_calculator_work.ten_exp_work[loc_first][loc_second][mod2tn] = tehai_calculator_work.ori_exp_work[loc_first][loc_second];
					}
				}
			}
		}
	}
}