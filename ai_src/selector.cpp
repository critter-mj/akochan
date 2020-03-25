#include "selector.hpp"

Tehai_Calculator_Work tehai_calculator_work;
bool out_console;

std::array<json11::Json, 4> tactics_json;

Hai_Choice::Hai_Choice(){
}

bool Hai_Choice::operator<(const Hai_Choice& rhs) const {
	if (pt_exp_total < rhs.pt_exp_total) {
		return true;
	} else {
		return false;
	}
}

Moves Hai_Choice::out_moves(const Game_State& game_state, const int my_pid, const int tsumo_hai) const {
	Moves moves;
	if (action_type == AT_TSUMO_AGARI) {
		moves.push_back(make_hora(my_pid, my_pid, tsumo_hai));
		return moves;
	}
	if (action_type == AT_KYUSHUKYUHAI) {
		moves.push_back(make_kyushukyuhai(my_pid));
		return moves;
	}
	if (action_type == AT_ANKAN) {
		if (haikind(hai) < 30 && haikind(hai)%10 == 5) {
			moves.push_back(make_ankan_aka(my_pid, hai));
		} else {
			moves.push_back(make_ankan_default(my_pid, hai));
		}
		return moves;
	}
	if (action_type == AT_KAKAN) {
		if (hai < 30 && hai % 10 == 5) {
			moves.push_back(make_kakan_aka(my_pid, hai));
		} else {
			moves.push_back(make_kakan_default(my_pid, hai));
		}
		return moves;
	}

	if (action_type == AT_REACH_DECLARE) {
		if (!game_state.player_state[my_pid].reach_declared) {
			moves.push_back(make_reach(my_pid));
		}
	}
	if (action_type == AT_DAHAI || AT_REACH_DECLARE) {
		moves.push_back(make_dahai(my_pid, hai, hai == tsumo_hai));
	}
	return moves;
}

void Fuuro_Choice::reset(){
	fuuro_hai = 0;
	fuuro_action_type = AT_NULL;
	hai_out = 0;
	//execute_flag = 0;

	for(int i=0;i<3;i++){
		sarashi_hai[i] = 0;
	}
	pt_exp_total = 0.0;
	pt_exp_total_prev = 0.0;
	pt_exp_after = 0.0;
	pt_exp_after_prev = 0.0;
}

Fuuro_Choice::Fuuro_Choice(){
	reset();
}

bool Fuuro_Choice::operator<(const Fuuro_Choice& rhs) const {
	if (pt_exp_total < rhs.pt_exp_total) {
		return true;
	} else {
		return false;
	}
}

Moves Fuuro_Choice::out_moves(const int my_pid, const int target) const {
	Moves moves;
	if (fuuro_action_type == AT_RON_AGARI) {
		moves.push_back(make_hora(my_pid, target, fuuro_hai));
	} else if (fuuro_action_type == AT_FUURO_PASS) {
		moves.push_back(make_none(my_pid));
	} else if (fuuro_action_type == AT_DAIMINKAN) {
		moves.push_back(make_daiminkan(my_pid, target, fuuro_hai, {sarashi_hai[0], sarashi_hai[1], sarashi_hai[2]}));
	} else if (is_pon(fuuro_action_type)) {
		moves.push_back(make_pon(my_pid, target, fuuro_hai, {sarashi_hai[0], sarashi_hai[1]}));
		moves.push_back(make_dahai(my_pid, hai_out, false));
	} else if (is_chi(fuuro_action_type)) {
		moves.push_back(make_chi(my_pid, target, fuuro_hai, {sarashi_hai[0], sarashi_hai[1]}));
		moves.push_back(make_dahai(my_pid, hai_out, false));
	}
	return moves;
}

std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4> cal_kyoku_end_pt_exp(const Moves& game_record, const Game_State& game_state, const int my_pid, const bool reach_mode, const Tactics& tactics) {
	int riibou = game_state.kyotaku;
    for (int pid = 0; pid < 4; pid++) {
        if (game_state.player_state[pid].reach_declared && !game_state.player_state[pid].reach_accepted) {
            riibou++;
        }
        // リーチに関して、declared_flagを用いる。（宣言牌に対する副露判断を適切なものにするため）
	    // 宣言牌に対するロンに関しては、個別に期待利得を計算する。
    }
	const int kyoku_mod = get_kyoku_first(game_record) + game_state.bakaze*4 + game_state.kyoku - 1;
    const int oyaid = get_oya(game_record);

	std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4> kyoku_end_pt_exp = {};
    #pragma omp parallel
	#pragma omp for
	for (int pid1 = 0; pid1 < 4; pid1++) {
		for (int pid2 = 0; pid2 < 4; pid2++) {
			for (int han = 1; han <= 13; han++) {
				for (int fu = 1; fu <= 11; fu++) {
					if (fu > 2 && han >= 5) {
                        kyoku_end_pt_exp[pid1][pid2][han][fu] = kyoku_end_pt_exp[pid1][pid2][han][2];
						// マンガン以上は、符は点数に寄与しないためコピーする。
					} else {
                        kyoku_end_pt_exp[pid1][pid2][han][fu] = 0;
                        int fu_mod = 0;
                        if (fu == 1 && pid1 != pid2) { continue; } // 他家からピンフツモはない
                        if (fu == 1 && pid1 == pid2 && han == 1) { continue; } // ピンフツモで1ハンはない
                        if (fu == 2 && pid1 == pid2 && han < 3) { continue; } // 七対子ツモは3ハン以上
                        if (fu == 2 && pid1 != pid2 && han == 1) { continue; } // 七対子ロンは2ハン以上
                        
                        if (fu == 1) {
                            fu_mod = 20;
                        } else if (fu == 2) {
                            fu_mod = 25;
                        } else {
                            fu_mod = fu * 10;
                        }
						
						const std::array<int, 4> ten_move = ten_move_hora(pid1, pid2, han, fu_mod, oyaid, game_state.honba, riibou, false);
                        std::array<int, 4> ten_tmp;
                        for (int pid = 0; pid < 4; pid++) {
							ten_tmp[pid] = game_state.player_state[pid].score + ten_move[pid];
                            if ((game_state.player_state[pid].reach_declared || (pid == my_pid && reach_mode)) && !game_state.player_state[pid].reach_accepted) {
                                ten_tmp[pid] -= 1000;
								ten_tmp[pid1] += 1000;
                            }
                        }

						assert(oyaid - kyoku_mod + 12 >= 0);
						const int oya_first = (oyaid - kyoku_mod + 12) % 4;
						const int kyoku_mod_next = (pid1 == oyaid) ? kyoku_mod : kyoku_mod + 1;
						const int oyaid_next = (pid1 == oyaid) ? oyaid : (oyaid + 1) % 4;
						const std::array<std::array<float, 4>, 4> jun_prob = calc_jun_prob(kyoku_mod_next, ten_tmp, oyaid_next, pid1 == oyaid, tactics_json[my_pid]);
                        for (int j = 0; j < 4; j++) {
                            kyoku_end_pt_exp[pid1][pid2][han][fu] += jun_prob[my_pid][j] * tactics.jun_pt[j];
                        }
					}
				}
			}
        }
    }
    return kyoku_end_pt_exp;
}

std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2> cal_ryuukyoku_pt_exp(const Moves& game_record, const Game_State& game_state, const int my_pid, const bool reach_mode, const Tactics& tactics) {
	int riibou = game_state.kyotaku;
    for (int pid = 0; pid < 4; pid++) {
        if (game_state.player_state[pid].reach_declared && !game_state.player_state[pid].reach_accepted) {
            riibou++;
        }
        // リーチに関して、declared_flagを用いる。（宣言牌に対する副露判断を適切なものにするため）
	    // 宣言牌に対するロンに関しては、個別に期待利得を計算する。
    }
	const int kyoku_mod = get_kyoku_first(game_record) + game_state.bakaze*4 + game_state.kyoku - 1;
    const int oyaid = get_oya(game_record);

	std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2> ryuukyoku_pt_exp = {};

	std::array<bool, 4> tenpai_flag;
	for(int t0=0;t0<2;t0++){
		for(int t1=0;t1<2;t1++){
			for(int t2=0;t2<2;t2++){
				for(int t3=0;t3<2;t3++){
					tenpai_flag[0] = (t0 == 1);
					tenpai_flag[1] = (t1 == 1);
					tenpai_flag[2] = (t2 == 1);
					tenpai_flag[3] = (t3 == 1);
					const std::array<int, 4> ten_move = ten_move_ryukyoku(tenpai_flag);

					std::array<int, 4> ten_tmp;
					for (int pid = 0; pid < 4; pid++) {
						ten_tmp[pid] = game_state.player_state[pid].score + ten_move[pid];
						if ((game_state.player_state[pid].reach_declared || (pid == my_pid && reach_mode)) && !game_state.player_state[pid].reach_accepted) {
							ten_tmp[pid] -= 1000;
						}
					}

					assert(oyaid - kyoku_mod + 12 >= 0);
					const int oya_first = (oyaid - kyoku_mod + 12) % 4;
					const int kyoku_mod_next = tenpai_flag[oyaid] ? kyoku_mod : kyoku_mod + 1;
					const int oyaid_next = tenpai_flag[oyaid] ? oyaid : (oyaid + 1) % 4;
					const std::array<std::array<float, 4>, 4> jun_prob = calc_jun_prob(kyoku_mod_next, ten_tmp, oyaid_next, tenpai_flag[oyaid], tactics_json[my_pid]);
					for (int j = 0; j < 4; j++) {
						ryuukyoku_pt_exp[t0][t1][t2][t3] += jun_prob[my_pid][j] * tactics.jun_pt[j];
					}
				}
			}
		}
	}
	return ryuukyoku_pt_exp;
}

int cal_tsumo_num_DP(const Moves& game_record, const int my_pid) {
	const int tsumo_num_all = count_tsumo_num_all(game_record);
	const int result_tmp = (70 - tsumo_num_all) / 4;
	const json11::Json& last_action = game_record[game_record.size()-1];

	const int next_tsumo_player = last_action["type"] == "dahai" ? next_player(last_action["actor"].int_value(), 1) : next_player(my_pid, 1);
	if ((70 - tsumo_num_all)%4 > (4 + my_pid - next_tsumo_player)%4) {
		return result_tmp + 1;
	} else {
		return result_tmp;
	}
}

void set_sarashi_hai(const Tehai_State2& dst_tehai_state, const Fuuro_Vector& present_fuuro, const int current_hai, int sarashi_hai[3]) {
	Tehai_State2 present_tehai_state;
	present_tehai_state.set_fuuro(present_fuuro);
	int fuuro_tmp[4][6];
	dst_tehai_state.get_fuuro(fuuro_tmp, present_tehai_state);
	bool fuuro_cand_passed_flag = false;
	int sarashi_counter = 0;
	for (int i = 0; i < 4; i++) {
		if (fuuro_tmp[present_fuuro.size()][2+i] == 0) {
			break;
		}
		if (fuuro_tmp[present_fuuro.size()][2+i] == current_hai && fuuro_cand_passed_flag == false) {
			fuuro_cand_passed_flag = true;
		} else {
			sarashi_hai[sarashi_counter] = fuuro_tmp[present_fuuro.size()][2+i];
			sarashi_counter++;
		}
	}
}

int koritu_most_needless(const Hai_Array& tehai, const Hai_Array& visible, const int bakaze, const int jikaze, const std::vector<int>& dora_marker) {
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==3){
			return h;
		}
	}
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==2){
			return h;
		}
	}
	const std::vector<int> dorav = dora_marker_to_dora(dora_marker);
	for(int h=31;h<35;h++){
		if(tehai[h]==1 && visible[h]==1 && h!=31+bakaze && h!=31+jikaze && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int h=31;h<35;h++){
		if(tehai[h]==1 && visible[h]==0 && h!=31+bakaze && h!=31+jikaze && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==1 && h!=31+jikaze && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==1 && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int c=0;c<3;c++){
		if(tehai[c*10+1]==1 && tehai[c*10+2]==0 && tehai[c*10+3]==0 && tehai[c*10+4]==1 && std::count(dorav.begin(), dorav.end(), c*10+1) == 0){
			return c*10+1;
		}
		if(tehai[c*10+9]==1 && tehai[c*10+8]==0 && tehai[c*10+7]==0 && tehai[c*10+6]==1 && std::count(dorav.begin(), dorav.end(), c*10+9) == 0){
			return c*10+9;
		}
	}
	for(int c=0;c<3;c++){
		if(tehai[c*10+1]==1 && tehai[c*10+2]==0 && tehai[c*10+3]==0 && std::count(dorav.begin(), dorav.end(), c*10+1) == 0){
			return c*10+1;
		}
		if(tehai[c*10+9]==1 && tehai[c*10+8]==0 && tehai[c*10+7]==0 && std::count(dorav.begin(), dorav.end(), c*10+9) == 0){
			return c*10+9;
		}
	}
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==0 && h!=31+jikaze && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int h=31;h<38;h++){
		if(tehai[h]==1 && visible[h]==0 && std::count(dorav.begin(), dorav.end(), h) == 0){
			return h;
		}
	}
	for(int c=0;c<3;c++){
		if(tehai[c*10+1]==0 && tehai[c*10+2]==1 && tehai[c*10+3]==0 && tehai[c*10+4]==0 && std::count(dorav.begin(), dorav.end(), c*10+2) == 0){
			return c*10+2;
		}
		if(tehai[c*10+9]==0 && tehai[c*10+8]==1 && tehai[c*10+7]==0 && tehai[c*10+6]==0 && std::count(dorav.begin(), dorav.end(), c*10+8) == 0){
			return c*10+8;
		}
	}
	for (int hai = 37; 0 < hai; hai--) {
		if (tehai[hai] > 0) {
			return hai;
		}
	}
	return 0;
}

Selector::Selector() {
    hai_choice.clear();
	fuuro_choice.clear();
}

void Selector::set_selector(const Moves& game_record, const int my_pid, const Tactics& tactics) {
	//out_console = true;
	hai_choice.clear();
	fuuro_choice.clear();
	const Game_State game_state = get_game_state(game_record);
	const Hai_Array& current_tehai = game_state.player_state[my_pid].tehai;
	const Hai_Array& current_tehai_kind = haikind(current_tehai);
	const Fuuro_Vector& current_fuuro = game_state.player_state[my_pid].fuuro;
	const bool current_reach = game_state.player_state[my_pid].reach_declared;
	const json11::Json& current_action = game_record[game_record.size() - 1];
	assert(current_action["type"] == "tsumo" || (current_action["type"] == "dahai" && current_action["actor"].int_value() != my_pid));
	const int current_hai = hai_str_to_int(current_action["pai"].string_value());
	const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
	const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);
	const Hai_Array hai_visible_kind = sum_hai_array(hai_visible_all_kind, current_tehai_kind);

	const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4> kyoku_end_pt_exp = cal_kyoku_end_pt_exp(game_record, game_state, my_pid, false, tactics);
	const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4> kyoku_end_pt_exp_ar = game_state.player_state[my_pid].reach_accepted ? kyoku_end_pt_exp : cal_kyoku_end_pt_exp(game_record, game_state, my_pid, true, tactics);
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2> ryuukyoku_pt_exp = cal_ryuukyoku_pt_exp(game_record, game_state, my_pid, false, tactics);
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2> ryuukyoku_pt_exp_ar = game_state.player_state[my_pid].reach_accepted ? ryuukyoku_pt_exp : cal_ryuukyoku_pt_exp(game_record, game_state, my_pid, true, tactics);

	if (out_console) {
		std::cout << "ryuukyoku_pt_exp:" << std::endl; 
		for (int t0 = 0; t0 < 2; t0++) {
			for (int t1 = 0; t1 < 2; t1++) {
				for (int t2 = 0; t2 < 2; t2++) {
					for (int t3 = 0; t3 < 2; t3++) {
						std::cout << t0 << " " << t1 << " " << t2 << " " << t3 << " " << ryuukyoku_pt_exp[t0][t1][t2][t3] << " " << ryuukyoku_pt_exp_ar[t0][t1][t2][t3] << std::endl;
					}
				}
			}
		}
	}

	// set_tehai_calculator1_single_thread

	Tehai_Analyzer tehai_analyzer, tehai_analyzer_af;
	if (out_console) {
		std::cout << "current_hai:" << current_hai << std::endl;
		std::cout << "check2:" << current_fuuro.size() << std::endl;
		for (int hai = 0; hai < 38; hai++) {
			for (int i = 0; i < current_tehai[hai]; i++) {
				std::cout << hai << " ";
			}
		}
		std::cout << std::endl;
	}
	tehai_analyzer.reset_tehai_analyzer_with(current_tehai, current_reach, current_fuuro);
	if (out_console) {
		tehai_analyzer.print_tehai();
	}

	if (current_reach && current_action["type"] == "tsumo") {
		tehai_analyzer.delete_hai(current_hai);
	}
	tehai_analyzer.analyze_tenpai(my_pid, game_state);
	const int mentu_shanten_num = tehai_analyzer.get_mentu_shanten_num();
	const int titoi_shanten_num = tehai_analyzer.get_titoi_shanten_num();
	const bool rule_base_flag = tehai_analyzer.rule_base_decision(my_pid);
	if (out_console) {
		std::cout << "shanten_num:" << mentu_shanten_num << " " << titoi_shanten_num << " " << (int)rule_base_flag << std::endl;
	}

	// set_tenpai_estimator

	std::array<Tenpai_Estimator_Simple, 4> tenpai_estimator;
	for (int pid = 0; pid < 4; pid++) {
		tenpai_estimator[pid].set_tenpai_estimator(game_record, game_state, my_pid, pid, tactics);
	}
	if (out_console) {
		std::cout << "tenpai_prob:";
		for (int pid = 0; pid < 4; pid++) {
			std::cout << tenpai_estimator[pid].tenpai_prob << " ";
		}
		std::cout << std::endl;
	}

	const std::pair<std::array<std::array<float, 38>, 4>, std::array<std::array<float, 38>, 4>> houjuu_hai_prob_value = cal_houjuu_hai_prob_value(
		tenpai_estimator, kyoku_end_pt_exp, my_pid, false
	);
	const std::pair<std::array<std::array<float, 38>, 4>, std::array<std::array<float, 38>, 4>> houjuu_hai_prob_value_now = cal_houjuu_hai_prob_value(
		tenpai_estimator, kyoku_end_pt_exp, my_pid, true
	);
	const std::array<std::array<float, 38>, 4>& houjuu_hai_prob = houjuu_hai_prob_value.first;
	const std::array<std::array<float, 38>, 4>& houjuu_hai_value = houjuu_hai_prob_value.second;
	const std::array<std::array<float, 38>, 4>& houjuu_hai_prob_now = houjuu_hai_prob_value_now.first;
	const std::array<std::array<float, 38>, 4>& houjuu_hai_value_now = houjuu_hai_prob_value_now.second;
	const std::array<std::array<std::array<float, 12>, 14>, 4> tsumo_hanfu_prob = cal_agari_hanfu_prob_array(tenpai_estimator, true);
	const std::array<std::array<std::array<float, 12>, 14>, 4> ron_hanfu_prob = cal_agari_hanfu_prob_array(tenpai_estimator, false);
	const std::array<std::array<std::array<float, 12>, 14>, 4> tsumo_hanfu_prob_kan = cal_hanfu_prob_kan(tsumo_hanfu_prob, tactics.han_shift_prob_kan);
	//const std::array<std::array<std::array<float, 12>, 14>, 4> ron_hanfu_prob_kan = cal_hanfu_prob_kan(ron_hanfu_prob, tactics.han_shift_prob_kan);


	const std::pair<std::array<float, 38>, std::array<float, 38>> total_houjuu_hai_prob_value = cal_total_houjuu_hai_prob_value (
		my_pid, tenpai_estimator, houjuu_hai_prob, houjuu_hai_value
	);
	const std::array<float, 38>& total_houjuu_hai_prob = total_houjuu_hai_prob_value.first;
	const std::array<float, 38>& total_houjuu_hai_value = total_houjuu_hai_prob_value.second;

	const std::pair<std::array<float, 38>, std::array<float, 38>> total_houjuu_hai_prob_value_now = cal_total_houjuu_hai_prob_value (
		my_pid, tenpai_estimator, houjuu_hai_prob_now, houjuu_hai_value_now
	);
	const std::array<float, 38>& total_houjuu_hai_prob_now = total_houjuu_hai_prob_value_now.first;
	const std::array<float, 38>& total_houjuu_hai_value_now = total_houjuu_hai_prob_value_now.second;

	if (out_console) {
		for (int pid = 0; pid < 4; pid++) {
			std::cout << "houjuu_prob:" << pid << std::endl;
			for (int i = 1; i <= 7; i++) {
				printf("%5.4f %5.4f %5.4f %5.4f\n", houjuu_hai_prob[pid][i], houjuu_hai_prob[pid][10+i], houjuu_hai_prob[pid][20+i], houjuu_hai_prob[pid][30+i]);
			}
			printf("%5.4f %5.4f %5.4f\n", houjuu_hai_prob[pid][8], houjuu_hai_prob[pid][10+8], houjuu_hai_prob[pid][20+8]);
			printf("%5.4f %5.4f %5.4f\n", houjuu_hai_prob[pid][9], houjuu_hai_prob[pid][10+9], houjuu_hai_prob[pid][20+9]);
		}
		for (int pid = 0; pid < 4; pid++) {
			std::cout << "houjuu_hai_value:" << pid << std::endl;
			for (int i = 1; i <= 7; i++) {
				printf("%5.4f %5.4f %5.4f %5.4f\n", houjuu_hai_value[pid][i], houjuu_hai_value[pid][10+i], houjuu_hai_value[pid][20+i], houjuu_hai_value[pid][30+i]);
			}
			printf("%5.4f %5.4f %5.4f\n", houjuu_hai_value[pid][8], houjuu_hai_value[pid][10+8], houjuu_hai_value[pid][20+8]);
			printf("%5.4f %5.4f %5.4f\n", houjuu_hai_value[pid][9], houjuu_hai_value[pid][10+9], houjuu_hai_value[pid][20+9]);
		}
		std::cout << "total_houjuu:" << std::endl;
		for (int hai = 0; hai < 38; hai++) {
			std::cout << hai << " " << total_houjuu_hai_prob[hai] << " " << total_houjuu_hai_value[hai] << " " << total_houjuu_hai_prob_now[hai] << " " << total_houjuu_hai_value_now[hai] << std::endl;
		}
	}

	//selector.set_exp_other_tmp();
	const std::array<float, 4> tenpai_prob_array = get_tenpai_prob_array(tenpai_estimator);
	const float passive_ryuukyoku_prob = cal_ryuukyoku_prob(my_pid, game_state, 0.0, tenpai_prob_array, 0);
	const double passive_ryuukyoku_value = cal_passive_ryuukyoku_value(my_pid, game_state, tenpai_prob_array, ryuukyoku_pt_exp);
	const double not_agari_value = cal_exp(my_pid, game_record, game_state, current_tehai, 0.0, 0.0, 0.0, 0.0, tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob, ron_hanfu_prob, kyoku_end_pt_exp, ryuukyoku_pt_exp, 0, 0);
	const double other_end_value = cal_other_end_value(my_pid, game_state, tenpai_prob_array, tsumo_hanfu_prob, ron_hanfu_prob, kyoku_end_pt_exp);
	assert_with_out(!tactics_json[my_pid]["use_other_end_ar"].is_null(), "use_other_end_ar is null");
	const double other_end_value_ar = tactics_json[my_pid]["use_other_end_ar"].bool_value() ?
		cal_other_end_value(my_pid, game_state, tenpai_prob_array, tsumo_hanfu_prob, ron_hanfu_prob, kyoku_end_pt_exp_ar) : other_end_value;
	const double other_end_value_kan = cal_other_end_value(my_pid, game_state, tenpai_prob_array, tsumo_hanfu_prob_kan, ron_hanfu_prob, kyoku_end_pt_exp);

	const int tsumo_num_exp = std::min(cal_tsumo_num_exp(my_pid, game_state, 1, tenpai_prob_array), cal_tsumo_num_DP(game_record, my_pid)); // to do ツモの時と副露の時でdahai_incは異なると思われる。
	assert(tsumo_num_exp >= 0);

	if (out_console) {
		std::cout << "passive_ryuukyoku_prob:" << passive_ryuukyoku_prob << std::endl;
		std::cout << "other_end_value:" << other_end_value << " " << other_end_value_ar << " " << other_end_value_kan << std::endl;
	}

	if (rule_base_flag) {
		if (current_action["type"] == "tsumo") {
			if (tactics.do_kyushukyuhai) {
				const json11::Json kyushukyuhai = make_kyushukyuhai(my_pid);
				if (is_legal_kyushukyuhai(game_record, game_state, kyushukyuhai)) {
					Hai_Choice hai_choice_tmp;
					hai_choice_tmp.action_type = AT_KYUSHUKYUHAI;
					hai_choice.push_back(hai_choice_tmp);
					assert(hai_choice.size() == 1);
					return;
				}
			}
			bool ori_flag = false;
			for (int pid_add = 1; pid_add < 4; pid_add++) {
				const int pid = next_player(my_pid, pid_add);
				if (game_state.player_state[pid].reach_declared || tenpai_estimator[pid].tenpai_prob > 0.5) {
					ori_flag = true;
				}
			}
			if (ori_flag) {
				for (int hai = 0; hai < 38; hai++) {
					if (game_state.player_state[my_pid].tehai[hai] > 0) {
						Hai_Choice hai_choice_tmp;
						hai_choice_tmp.action_type = AT_DAHAI;
						hai_choice_tmp.hai = hai;

						Hai_Array tehai_tmp = current_tehai;
						tehai_tmp[hai]--;
						std::array<float, 38> betaori_houjuu_hai_prob = total_houjuu_hai_prob;
						betaori_houjuu_hai_prob[hai] = 0.0;

						hai_choice_tmp.pt_exp_after_ori = cal_betaori(tehai_tmp, betaori_houjuu_hai_prob, total_houjuu_hai_value, other_end_value, passive_ryuukyoku_value, tsumo_num_exp).betaori_exp;
						hai_choice_tmp.pt_exp_total = total_houjuu_hai_prob_now[hai] * total_houjuu_hai_value_now[hai] + (1.0 - total_houjuu_hai_prob_now[hai]) * hai_choice_tmp.pt_exp_after_ori;
						hai_choice_tmp.review["total_houjuu_hai_prob_now"] = total_houjuu_hai_prob_now[hai];
						if (total_houjuu_hai_prob_now[hai] != 0) {
							hai_choice_tmp.review["total_houjuu_hai_value_now"] = total_houjuu_hai_value_now[hai];
							hai_choice_tmp.review["pt_exp_after"] = hai_choice_tmp.pt_exp_after_ori;
						}
						hai_choice_tmp.review["pt_exp_total"] = hai_choice_tmp.pt_exp_total;
						hai_choice.push_back(hai_choice_tmp);
					}
				}
			} else {
				int hai_tmp = 0;
				hai_tmp = koritu_most_needless(game_state.player_state[my_pid].tehai, hai_visible_all, game_state.bakaze, game_state.player_state[my_pid].jikaze, game_state.dora_marker);
				Hai_Choice hai_choice_tmp;
				hai_choice_tmp.action_type = AT_DAHAI;
				hai_choice_tmp.hai = hai_tmp;
				hai_choice.push_back(hai_choice_tmp);
			}
			std::sort(hai_choice.rbegin(), hai_choice.rend());
		} else {
			Fuuro_Choice fuuro_choice_tmp;
			fuuro_choice_tmp.fuuro_action_type = AT_FUURO_PASS;
			fuuro_choice.push_back(fuuro_choice_tmp);
		}
		return;
	}

	if (current_reach) {
		tehai_analyzer.mentu_change_num_max = 0;
		tehai_analyzer.titoi_change_num_max = 0;
	} else {
		tehai_analyzer.pattern_flag = 1;
		tehai_analyzer.mentu_change_num_max = tehai_analyzer.get_mentu_shanten_num() + tactics.tegawari_num[tehai_analyzer.get_mentu_shanten_num()];
		tehai_analyzer.titoi_change_num_max = cal_titoi_change_num_max(titoi_shanten_num, mentu_shanten_num);
		tehai_analyzer.analyze_tenpai(my_pid, game_state);
	}

	tehai_analyzer_af.reset_tehai_analyzer_with(current_tehai, current_reach, current_fuuro);

	if (current_action["type"] == "dahai" && !current_reach) {
		tehai_analyzer_af.add_hai(current_hai);
		tehai_analyzer_af.analyze_tenpai(my_pid, game_state);
		tehai_analyzer_af.pattern_flag = 1;
		if(tehai_analyzer_af.get_mentu_shanten_num() <= 1){
			tehai_analyzer_af.mentu_change_num_max = tehai_analyzer_af.get_mentu_shanten_num() + 2;
		}else if(tehai_analyzer_af.get_mentu_shanten_num() <= 2){
			tehai_analyzer_af.mentu_change_num_max = tehai_analyzer_af.get_mentu_shanten_num() + 1;
		}else if(tehai_analyzer_af.get_mentu_shanten_num() <= 3){
			tehai_analyzer_af.mentu_change_num_max = tehai_analyzer_af.get_mentu_shanten_num();
		}
		tehai_analyzer_af.titoi_change_num_max = 0;
		tehai_analyzer_af.analyze_tenpai(my_pid, game_state);
		//tehai_analyzer_af.out_tenpai();
		//tehai_analyzer_af.print_tehai();
		//tehai_analyzer_af.tehai_state.out_info();
	}

	Tehai_Calculator tehai_calculator;
	tehai_calculator.reset(my_pid, get_aka_flag(game_record));
	tehai_calculator.fuuro_cand_hai = current_action["type"] == "dahai" ? current_hai : 0;
	tehai_calculator.get_effective(game_state, tehai_analyzer);
	tehai_calculator.tehai_all_num = tehai_analyzer.get_tehai_num() + tehai_analyzer.get_fuuro_num()*3;
	
	if (out_console) {
		std::cout << "shanten:" << tehai_analyzer.get_shanten_num() << std::endl;
		for (int i = 0; i < 9; i++) {
			std::cout << i << " " << tehai_analyzer.inout_pattern_vec[i].size() << std::endl;
		}
		std::cout << "candidates_num:" << tehai_calculator.candidates_size() << std::endl;
	}
	tehai_calculator.set_candidates3_single_thread(
		game_state, tehai_analyzer, tehai_analyzer_af,
		std::max({tehai_analyzer.mentu_change_num_max, tehai_analyzer.titoi_change_num_max, tehai_analyzer_af.mentu_change_num_max}), tactics
	);
	tehai_analyzer.pattern_flag = 0;

	// set_tehai_calculator1_multi_thread

	const std::array<bool, 38> sute_kind_flag = get_sute_kind_flag(game_state.player_state[my_pid].kawa);
	tehai_calculator.set_candidates3_multi_thread(game_record, game_state, sute_kind_flag, tehai_analyzer, tactics);
	// selector.set_tsumo_num();

	/* get_agari_shanten_num(chi_flag, cand_hai); あまり必要でないかもしれないので保留。 */
	const bool chi_flag = current_action["type"] == "dahai" ? ((current_action["actor"].int_value() + 1)%4 == my_pid) : false;
	const int fuuro_agari_shanten_num = tehai_calculator.get_fuuro_agari_shanten_num(game_state, current_tehai, chi_flag);
	// 副露時のみget_fuuro_agari_shanten_numの内部のset_agari_shanten_numが呼ばれる。おそらく問題は無い。
	
	const bool DP_flag = cal_dp_flag(tehai_analyzer.get_shanten_num(), fuuro_agari_shanten_num, get_other_reach_declared_num(my_pid, game_state) > 0 , current_action["type"] == "dahai", tactics);

	if (DP_flag) {
		// set_tehai_calculator2_DP();
		const int ori_choice_mode = 2;
		const int tsumo_num_DP = cal_tsumo_num_DP(game_record, my_pid);

		if (game_state.player_state[0].reach_declared || game_state.player_state[1].reach_declared || game_state.player_state[2].reach_declared || game_state.player_state[3].reach_declared || mentu_shanten_num > 0) {
			tehai_calculator.calc_DP(
				game_state.player_state[my_pid].kawa.size(), tsumo_num_DP, other_end_value, other_end_value_ar, other_end_value_kan, tenpai_prob_array, houjuu_hai_prob, houjuu_hai_value,
				ori_choice_mode, 1.0, 0.0, passive_ryuukyoku_prob,
				hai_visible_all_kind, game_state, kyoku_end_pt_exp, ryuukyoku_pt_exp, ryuukyoku_pt_exp_ar, tactics
			);
		} else {
			tehai_calculator.calc_DP(
				game_state.player_state[my_pid].kawa.size(), tsumo_num_DP, other_end_value, other_end_value_ar, other_end_value_kan, tenpai_prob_array, houjuu_hai_prob, houjuu_hai_value,
				ori_choice_mode, 1.0, tenpai_prob_array[my_pid], passive_ryuukyoku_prob,
				hai_visible_all_kind, game_state, kyoku_end_pt_exp, ryuukyoku_pt_exp, ryuukyoku_pt_exp_ar, tactics
			);
		}

		if (current_action["type"] == "tsumo") {
			bool tsumo_agari_possible_flag = false;
			for (int cn = 0; cn < tehai_calculator.in0num; cn++) {
				for (int gn = 0; gn < tehai_calculator.group_size(cn); gn++) {
					if (is_same_fuuro(tehai_calculator.get_const_ta_cgn(cn, gn).tehai_state, tehai_calculator.get_const_ta_cgn(cn, 0).tehai_state)) {
						const int hai = find_hai_out_ta(game_state.player_state[my_pid].tehai, tehai_calculator.get_const_ta_cgn(cn, gn));
						Hai_Choice hai_choice_tmp;
						hai_choice_tmp.action_type = AT_DAHAI;
						hai_choice_tmp.hai = hai;
						if (haikind(hai) == haikind(current_hai) &&
							(tehai_calculator.get_const_ta_cgn(cn, gn).get_reach_flag() == 1) == game_state.player_state[my_pid].reach_accepted
						) {
							Hai_Choice tsumo_agari_choice;
							tsumo_agari_choice.hai = current_hai;
							tsumo_agari_choice.pt_exp_total = -200;
							const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn, gn);
							for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
								const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
								if(haikind(current_hai) == agari.agari_info.get_hai() && agari.agari_info.get_han_tsumo() > 0){
									tsumo_agari_possible_flag = true;
									tsumo_agari_choice.action_type = AT_TSUMO_AGARI;
									const std::array<double, 4> ten_exp = agari.get_ten_exp(
										my_pid, tehai_calculator.get_const_ta_cgn(cn, gn).tehai_bit, tehai_calculator.get_const_ta_cgn(cn, gn).tehai_state,
										hai_visible_kind, game_state, kyoku_end_pt_exp
									);
									const double agari_exp = is_aka_hai(current_hai) ? ten_exp[2] : ten_exp[0];
									if (agari_exp > tsumo_agari_choice.pt_exp_total) {
										tsumo_agari_choice.pt_exp_total = agari_exp;
									}
								}
							}
							if(tsumo_agari_choice.action_type == AT_TSUMO_AGARI){
								hai_choice.push_back(tsumo_agari_choice);
							}
						}

						if(tehai_calculator.get_const_ta_cgn(cn, gn).get_reach_flag() == 1){
							hai_choice_tmp.action_type = AT_REACH_DECLARE;
						}
						hai_choice_tmp.pt_exp_after_ori = tehai_calculator.get_ori_exp(cn, gn, tsumo_num_DP);
						hai_choice_tmp.pt_exp_after = tehai_calculator.get_ten_exp(cn, gn, tsumo_num_DP);

						hai_choice.push_back(hai_choice_tmp);

						if (count_tsumo_num_all(game_record) < 70) {
							const std::array<int, 3>& tsumo_edge_loc = tehai_calculator_work.get_const_tsumo_edge_loc(cn, gn);
							for (int acn = tsumo_edge_loc[1]; acn < tsumo_edge_loc[2]; acn++) {
								const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_tsumo_work[tsumo_edge_loc[0]][acn];
								if (ac_tmp.hai_out == hai && (ac_tmp.action_type == AT_ANKAN || ac_tmp.action_type == AT_KAKAN)) {
									if ((!tactics.do_ankan_inclusive && ac_tmp.action_type == AT_ANKAN) ||
										(!tactics.do_kakan_inclusive && ac_tmp.action_type == AT_KAKAN)
									) {
										continue;
									}
									if (game_state.player_state[my_pid].reach_accepted && !tehai_calculator.get_const_ta_cgn(cn, gn).can_ankan_after_reach(hai)) {
										continue;
									}
									Hai_Choice kan_choice;
									kan_choice.hai = hai;
									kan_choice.action_type = ac_tmp.action_type;
									// AT_ANKAN_AND_REACH_DECLARE かもしれないが、move出力時の処理が面倒なのでac_tmp.action_typeのままにしておく
									// 結果的に、AT_ANKANの同じmovesが別スコアで出力される可能性がある点に注意。

									kan_choice.pt_exp_after_ori = tehai_calculator.get_ori_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_DP);
									kan_choice.pt_exp_after = tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_DP);
									hai_choice.push_back(kan_choice);
								}
							}
						}
					}
				}
			}
			for(int i=0;i<hai_choice.size();i++){
				if (is_ankan(hai_choice[i].action_type)){
					hai_choice[i].pt_exp_total = hai_choice[i].pt_exp_after;
				} else if (hai_choice[i].action_type != AT_TSUMO_AGARI) {
					const int hai_out = hai_choice[i].hai;
					hai_choice[i].pt_exp_total = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out]) * hai_choice[i].pt_exp_after;
					hai_choice[i].review["total_houjuu_hai_prob_now"] = total_houjuu_hai_prob_now[hai_out];
					if (total_houjuu_hai_prob_now[hai_out] != 0) {
						hai_choice[i].review["total_houjuu_hai_value_now"] = total_houjuu_hai_value_now[hai_out];
						hai_choice[i].review["pt_exp_after"] = hai_choice[i].pt_exp_after;
					}
				}
				hai_choice[i].review["pt_exp_total"] = hai_choice[i].pt_exp_total;
			}
			if (out_console) {
				for(int i=0;i<hai_choice.size();i++){
					std::cout << "hai_choice:" << hai_choice[i].hai << " " << hai_choice[i].pt_exp_total << " " << hai_choice[i].pt_exp_after_ori << " " << hai_choice[i].pt_exp_after << " ";
					std::cout << total_houjuu_hai_prob_now[hai_choice[i].hai] << " " << total_houjuu_hai_value_now[hai_choice[i].hai] << std::endl;
				}
			}
			std::sort(hai_choice.rbegin(), hai_choice.rend());
		} else {
			Fuuro_Choice fuuro_choice_tmp;

			int cn_fuuro_neg = 0;
			int gn_fuuro_neg = 0;
			for(int gn=0;gn<tehai_calculator.group_size(cn_fuuro_neg);gn++){
				if(is_same_tehai_ta(current_tehai, tehai_calculator.get_const_ta_cgn(cn_fuuro_neg, gn))){
					gn_fuuro_neg = gn;
					break;
				}
			}

			double ron_pt_exp = -200.0;
			bool doujun_furiten_flag = false;
			const std::array<bool, 38> furiten_flags = get_furiten_flags(game_record, game_state, my_pid, true);
			if (tehai_calculator.get_const_ta_cgn(cn_fuuro_neg, gn_fuuro_neg).get_furiten_flag() == 0) {
				const std::array<int, 3>& agari_loc = tehai_calculator_work.get_const_agari_loc(cn_fuuro_neg, gn_fuuro_neg);
				for (int an = agari_loc[1]; an < agari_loc[2]; an++) {
					const Agari_Calc& agari = tehai_calculator_work.agari_graph_work[agari_loc[0]][an];
					if (furiten_flags[agari.agari_info.get_hai()]) {
						doujun_furiten_flag = true;
					}
					if (haikind(current_hai) == agari.agari_info.get_hai() && agari.agari_info.get_han_ron()){
						fuuro_choice_tmp.fuuro_action_type = AT_RON_AGARI;
						fuuro_choice_tmp.fuuro_hai = current_hai;
						const std::array<double, 2> ten_exp = agari.get_ten_exp_direct(
							my_pid, current_action["actor"].int_value(), tehai_calculator.get_const_ta_cgn(cn_fuuro_neg, gn_fuuro_neg).tehai_bit, tehai_calculator.get_const_ta_cgn(cn_fuuro_neg, gn_fuuro_neg).tehai_state,
							hai_visible_kind, game_state, kyoku_end_pt_exp
						);
						const double agari_exp = is_aka_hai(current_hai) ? ten_exp[1] : ten_exp[0];
						if(agari_exp > ron_pt_exp){
							ron_pt_exp = agari_exp;
							fuuro_choice_tmp.pt_exp_total = ron_pt_exp;
						}
					}
				}
			}

			if (fuuro_choice_tmp.fuuro_action_type == AT_RON_AGARI && !doujun_furiten_flag) {
				fuuro_choice.push_back(fuuro_choice_tmp);
			}

			fuuro_choice_tmp.fuuro_action_type = AT_FUURO_PASS;
			fuuro_choice_tmp.pt_exp_total = std::max(tehai_calculator.get_ten_exp(cn_fuuro_neg, gn_fuuro_neg, tsumo_num_DP), not_agari_value); // to do 元はnot_agari_valueのかわりにpassive_valueだった。
			fuuro_choice.push_back(fuuro_choice_tmp);

			if (count_tsumo_num_all(game_record) < 70) { // ハイテイ牌をフーロしないための処理
				const std::array<int, 3>& fuuro_edge_loc = tehai_calculator_work.get_const_fuuro_edge_loc(cn_fuuro_neg, gn_fuuro_neg);
				for (int acn = fuuro_edge_loc[1]; acn < fuuro_edge_loc[2]; acn++) {
					const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_fuuro_work[fuuro_edge_loc[0]][acn];
					//ac_tmp.out_info();
					if (ac_tmp.hai != current_hai) {
						continue;
					} else if (my_pid != next_player(current_action["actor"].int_value(), 1) && is_chi(ac_tmp.action_type)) {
						continue;
					} else {
						//ac_tmp.out_info();
						fuuro_choice_tmp.reset();
						fuuro_choice_tmp.fuuro_hai = ac_tmp.hai;
						assert(ac_tmp.hai == current_hai);
						fuuro_choice_tmp.fuuro_action_type = ac_tmp.action_type;
						fuuro_choice_tmp.hai_out = ac_tmp.hai_out;
						set_sarashi_hai(tehai_calculator.get_const_ta_cgn(ac_tmp.dst_group, ac_tmp.dst_group_sub).tehai_state, game_state.player_state[my_pid].fuuro, current_hai, fuuro_choice_tmp.sarashi_hai);

						fuuro_choice_tmp.pt_exp_after = tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, std::max(tsumo_num_DP-1,0));
						fuuro_choice_tmp.pt_exp_after_prev = tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, std::max(tsumo_num_DP,0));

						fuuro_choice.push_back(fuuro_choice_tmp);
					}
				}
			}

			for (int i = 0; i < fuuro_choice.size(); i++) {
				if(fuuro_choice[i].fuuro_action_type != AT_FUURO_PASS && fuuro_choice[i].fuuro_action_type != AT_RON_AGARI) {
					const int hai_out = fuuro_choice[i].hai_out;
					fuuro_choice[i].pt_exp_total = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out])*fuuro_choice[i].pt_exp_after;
					fuuro_choice[i].pt_exp_total_prev = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out])*fuuro_choice[i].pt_exp_after_prev;
					fuuro_choice[i].pt_exp_total = std::min(fuuro_choice[i].pt_exp_total, fuuro_choice[i].pt_exp_total_prev);
					fuuro_choice[i].review["total_houjuu_hai_prob_now"] = total_houjuu_hai_prob_now[hai_out];
					if (total_houjuu_hai_prob_now[hai_out] != 0) {
						fuuro_choice[i].review["total_houjuu_hai_value_now"] = total_houjuu_hai_value_now[hai_out];
						fuuro_choice[i].review["pt_exp_after"] = fuuro_choice[i].pt_exp_after;
					}
				}
				fuuro_choice[i].review["pt_exp_total"] = fuuro_choice[i].pt_exp_total;
			}
			if (out_console) {
				for(int i=0;i<fuuro_choice.size();i++){
					std::cout << "fuuro_choice:" << (int)fuuro_choice[i].fuuro_action_type << " " << fuuro_choice[i].hai_out << " " << fuuro_choice[i].pt_exp_total << " " << fuuro_choice[i].pt_exp_total_prev << std::endl;
				}
			}

			std::sort(fuuro_choice.rbegin(), fuuro_choice.rend());
		}
	} else {
		// selector.set_tehai_calculator2();
		if (out_console) {
			std::cout << "not_agari_value:" << not_agari_value << std::endl;
			std::cout << "other_end_value:" << other_end_value << std::endl;
			std::cout << "passive_ryuukyoku_value:" << passive_ryuukyoku_value << std::endl;
		}
		tehai_calculator.calc_agari_prob(tsumo_num_exp, other_end_value, game_state, kyoku_end_pt_exp); // ako に合わせているが、 other_end_value でよいか。
		if (out_console) {
			std::cout << "calc_agari_prob_input:" << other_end_value << " " << tsumo_num_exp << std::endl;
		}
		if (current_action["type"] == "tsumo") {
			for (int cn = 0; cn < tehai_calculator.in0num; cn++) {
				for (int gn = 0; gn < tehai_calculator.group_size(cn); gn++) {
					if (is_ts_fuuro_consistent(tehai_calculator.get_const_ta_cgn(cn, gn).tehai_state, game_state.player_state[my_pid].fuuro)) {
						const int hai_out = find_hai_out_ta(game_state.player_state[my_pid].tehai, tehai_calculator.get_const_ta_cgn(cn, gn));
						Hai_Choice hai_choice_tmp;
						hai_choice_tmp.hai = hai_out;
						hai_choice_tmp.action_type = tehai_calculator.get_const_ta_cgn(cn, gn).get_reach_flag() ? AT_REACH_DECLARE : AT_DAHAI;

						const auto& kyoku_end_pt_exp_tmp = hai_choice_tmp.action_type == AT_REACH_DECLARE ? kyoku_end_pt_exp_ar : kyoku_end_pt_exp;
						const auto& ryuukyoku_pt_exp_tmp = hai_choice_tmp.action_type == AT_REACH_DECLARE ? ryuukyoku_pt_exp_ar : ryuukyoku_pt_exp;

						Hai_Array tehai_tmp = current_tehai;
						tehai_tmp[hai_out]--;
						hai_choice_tmp.pt_exp_after = cal_exp(
							my_pid, game_record, game_state, tehai_tmp, tehai_calculator.get_agari_prob(cn, gn, tsumo_num_exp), tehai_calculator.get_ten_exp(cn, gn, tsumo_num_exp),
							other_end_value, tehai_calculator.get_tenpai_prob(cn, gn, tsumo_num_exp),
							tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob, ron_hanfu_prob,
							kyoku_end_pt_exp_tmp, ryuukyoku_pt_exp_tmp, 1, 0
						);
						if (out_console) {
							std::cout << cn << " " << gn << " " << hai_out << " " << tehai_calculator.get_agari_prob(cn, gn, tsumo_num_exp) << " " << hai_choice_tmp.pt_exp_after << std::endl;
						}

						//Betaori betaori;
						std::array<float, 38> betaori_houjuu_hai_prob = total_houjuu_hai_prob;
						betaori_houjuu_hai_prob[hai_out] = 0.0;
						hai_choice_tmp.pt_exp_after_ori = cal_betaori_exp(
							my_pid, game_state, tehai_tmp, betaori_houjuu_hai_prob, total_houjuu_hai_value,
							not_agari_value, other_end_value, passive_ryuukyoku_value, passive_ryuukyoku_prob,
							tsumo_num_exp, tactics
						);
						hai_choice.push_back(hai_choice_tmp);

						if (tactics.do_kan_ordinary && count_tsumo_num_all(game_record) < 70) {
							const std::array<int, 3>& tsumo_edge_loc = tehai_calculator_work.get_const_tsumo_edge_loc(cn, gn);
							for (int acn = tsumo_edge_loc[1]; acn < tsumo_edge_loc[2]; acn++) {
								const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_tsumo_work[tsumo_edge_loc[0]][acn];
								if (ac_tmp.hai_out == hai_out && (ac_tmp.action_type == AT_ANKAN || ac_tmp.action_type == AT_KAKAN)) {
									if (game_state.player_state[my_pid].reach_accepted && !tehai_calculator.get_const_ta_cgn(cn, gn).can_ankan_after_reach(hai_out)) {
										continue;
									}
									Hai_Choice kan_choice;
									kan_choice.hai = hai_out;
									kan_choice.action_type = ac_tmp.action_type;
									Hai_Array kan_tehai = tehai_tmp;
									kan_tehai[hai_out] = 0;
									kan_tehai[haikind(hai_out)] = 0;
									// other_end_value は後退解析の基準値として用いるものなので、other_end_value_kanに変えないほうがよいはず。
									// dahai_inc を0にすべきかは要検討。
									kan_choice.pt_exp_after = cal_exp(
										my_pid, game_record, game_state, kan_tehai, tehai_calculator.get_agari_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp), tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp),
										other_end_value, tehai_calculator.get_tenpai_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp),
										tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob_kan, ron_hanfu_prob,
										kyoku_end_pt_exp_tmp, ryuukyoku_pt_exp_tmp, 0, 0
									);
									kan_choice.pt_exp_after_ori = cal_betaori_exp(
										my_pid, game_state, kan_tehai, betaori_houjuu_hai_prob, total_houjuu_hai_value,
										not_agari_value, other_end_value, passive_ryuukyoku_value, passive_ryuukyoku_prob,
										tsumo_num_exp, tactics
									);
									hai_choice.push_back(kan_choice);
								}
							}
						}
					}
				}
			}

			if (0 < get_other_reach_declared_num(my_pid, game_state) || 2 <= get_other_fuuro_num_max(my_pid, game_state)) {
				// 降りに関して、どのような条件を採用したときに強くなるかは要検証
				for (int i = 0; i < hai_choice.size(); i++) {
					if (hai_choice[i].pt_exp_after_ori > hai_choice[i].pt_exp_after) {
						hai_choice[i].pt_exp_after = hai_choice[i].pt_exp_after_ori;
					}
				}
			}

			for (int i = 0; i < hai_choice.size(); i++) {
				const int hai_out = hai_choice[i].hai;
				if (hai_choice[i].action_type == AT_DAHAI || hai_choice[i].action_type == AT_REACH_DECLARE || hai_choice[i].action_type == AT_KAKAN) {
					hai_choice[i].pt_exp_total = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out]) * hai_choice[i].pt_exp_after;
				} else if (hai_choice[i].action_type == AT_ANKAN) {
					hai_choice[i].pt_exp_total = hai_choice[i].pt_exp_after;
				} else {
					assert_with_out(false, "selector hai_choice action_type error");
				}
				
				hai_choice[i].review["total_houjuu_hai_prob_now"] = total_houjuu_hai_prob_now[hai_out];
				if (total_houjuu_hai_prob_now[hai_out] != 0) {
					hai_choice[i].review["total_houjuu_hai_value_now"] = total_houjuu_hai_value_now[hai_out];
					hai_choice[i].review["pt_exp_after"] = hai_choice[i].pt_exp_after;
				}
				hai_choice[i].review["pt_exp_total"] = hai_choice[i].pt_exp_total;
				if (out_console) {
					std::cout << "hai_choice:" << hai_choice[i].hai << " " << hai_choice[i].pt_exp_total << " " << hai_choice[i].pt_exp_after_ori << " " << hai_choice[i].pt_exp_after << std::endl;
					std::cout << total_houjuu_hai_prob_now[hai_choice[i].hai] << " " << total_houjuu_hai_value_now[hai_choice[i].hai] << std::endl;
				}
			}
			std::sort(hai_choice.rbegin(), hai_choice.rend());
		} else {
			fuuro_choice.erase(fuuro_choice.begin(), fuuro_choice.end());
			Fuuro_Choice fuuro_choice_tmp;

			int cn_fuuro_neg = 0;
			int gn_fuuro_neg = 0;
			for (int gn = 0; gn < tehai_calculator.group_size(cn_fuuro_neg); gn++) {
				if(is_same_tehai_ta(current_tehai, tehai_calculator.get_const_ta_cgn(cn_fuuro_neg, gn))){
					gn_fuuro_neg = gn;
					break;
				}
			}

			Fuuro_Choice pass_choice;
			pass_choice.fuuro_action_type = AT_FUURO_PASS;

			pass_choice.pt_exp_after = cal_exp(
				my_pid, game_record, game_state, current_tehai, tehai_calculator.get_agari_prob(cn_fuuro_neg, gn_fuuro_neg, tsumo_num_exp), tehai_calculator.get_ten_exp(cn_fuuro_neg, gn_fuuro_neg, tsumo_num_exp),
				other_end_value, tehai_calculator.get_tenpai_prob(cn_fuuro_neg, gn_fuuro_neg, tsumo_num_exp),
				tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob, ron_hanfu_prob,
				kyoku_end_pt_exp, ryuukyoku_pt_exp, 0, 0
			);

			fuuro_choice.push_back(pass_choice);

			clock_t check2 = clock();
			if (count_tsumo_num_all(game_record) < 70) { // ハイテイ牌をフーロしないための処理		
				const std::array<int, 3>& fuuro_edge_loc = tehai_calculator_work.get_const_fuuro_edge_loc(cn_fuuro_neg, gn_fuuro_neg);
				for (int acn = fuuro_edge_loc[1]; acn < fuuro_edge_loc[2]; acn++) {
					const Tehai_Action& ac_tmp = tehai_calculator_work.cand_graph_sub_fuuro_work[fuuro_edge_loc[0]][acn];
					//ac_tmp.out_info();
					if (ac_tmp.action_type != AT_TSUMO) {
						if (ac_tmp.hai != current_hai) {
							continue;
						} else if (my_pid != next_player(current_action["actor"].int_value(), 1) && is_chi(ac_tmp.action_type)){
							continue;
						} else {
							//ac_tmp.out_info();
							Fuuro_Choice fuuro_choice_tmp;
							fuuro_choice_tmp.fuuro_hai = ac_tmp.hai;
							fuuro_choice_tmp.fuuro_action_type = ac_tmp.action_type;
							fuuro_choice_tmp.hai_out = ac_tmp.hai_out;

							for (int i = 0; i < 3; i++) { // to do. sarashi_haiをvectorに変える。
								fuuro_choice_tmp.sarashi_hai[i] = 0;
							}
							set_sarashi_hai(tehai_calculator.get_const_ta_cgn(ac_tmp.dst_group, ac_tmp.dst_group_sub).tehai_state, game_state.player_state[my_pid].fuuro, current_hai, fuuro_choice_tmp.sarashi_hai);
							if (out_console) {
								std::cout << "choice_check:" << (int)ac_tmp.action_type << " " << (int)ac_tmp.hai_out << " " << tehai_calculator.get_agari_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp) << std::endl;
							}
							Hai_Array tehai_tmp = current_tehai;
							for (int i = 0; i < 3; i++) {
								tehai_tmp[fuuro_choice_tmp.sarashi_hai[i]]--;
							}
							tehai_tmp[fuuro_choice_tmp.hai_out]--;
							fuuro_choice_tmp.pt_exp_after = cal_exp(
								my_pid, game_record, game_state, tehai_tmp, tehai_calculator.get_agari_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp - 1), tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp - 1),
								other_end_value, tehai_calculator.get_tenpai_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp - 1),
								tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob, ron_hanfu_prob,
								kyoku_end_pt_exp, ryuukyoku_pt_exp, 1, 1
							);
							fuuro_choice_tmp.pt_exp_after_prev = cal_exp(
								my_pid, game_record, game_state, tehai_tmp, tehai_calculator.get_agari_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp), tehai_calculator.get_ten_exp(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp),
								other_end_value, tehai_calculator.get_tenpai_prob(ac_tmp.dst_group, ac_tmp.dst_group_sub, tsumo_num_exp),
								tenpai_prob_array, houjuu_hai_prob, tsumo_hanfu_prob, ron_hanfu_prob,
								kyoku_end_pt_exp, ryuukyoku_pt_exp, 0, 1
							);
							// to do tehai_cal以外のもののtsumo_numも1引いた値で計算。
							fuuro_choice.push_back(fuuro_choice_tmp);
						}
					}
				}
			}

			for (int i = 0; i < fuuro_choice.size(); i++) {
				if (fuuro_choice[i].fuuro_action_type == AT_FUURO_PASS) {
					fuuro_choice[i].pt_exp_total = fuuro_choice[i].pt_exp_after;
				} else if (fuuro_choice[i].fuuro_action_type != AT_RON_AGARI) {
					const int hai_out = fuuro_choice[i].hai_out;
					fuuro_choice[i].pt_exp_total = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out]) * fuuro_choice[i].pt_exp_after;
					fuuro_choice[i].pt_exp_total_prev = total_houjuu_hai_prob_now[hai_out] * total_houjuu_hai_value_now[hai_out] + (1.0 - total_houjuu_hai_prob_now[hai_out]) * fuuro_choice[i].pt_exp_after_prev;
					fuuro_choice[i].pt_exp_total = std::min(fuuro_choice[i].pt_exp_total, fuuro_choice[i].pt_exp_total_prev);
					fuuro_choice[i].review["total_houjuu_hai_prob_now"] = total_houjuu_hai_prob_now[hai_out];
					if (total_houjuu_hai_prob_now[hai_out] != 0) {
						fuuro_choice[i].review["total_houjuu_hai_value_now"] = total_houjuu_hai_value_now[hai_out];
						fuuro_choice[i].review["pt_exp_after"] = fuuro_choice[i].pt_exp_after;
					}
				}
				fuuro_choice[i].review["pt_exp_total"] = fuuro_choice[i].pt_exp_total;
				if (out_console) {
					std::cout << "fuuro_choice:" << (int)fuuro_choice[i].fuuro_action_type << " " << fuuro_choice[i].hai_out << " " << fuuro_choice[i].pt_exp_total << " " << fuuro_choice[i].pt_exp_total_prev << " ";
					if (fuuro_choice[i].fuuro_action_type != AT_FUURO_PASS && fuuro_choice[i].fuuro_action_type != AT_RON_AGARI) {
						std::cout << fuuro_choice[i].pt_exp_after << " " << total_houjuu_hai_prob_now[fuuro_choice[i].hai_out] << " " << total_houjuu_hai_value_now[fuuro_choice[i].hai_out] << std::endl;
					} else {
						std::cout << std::endl;
					}
				}
			}
			std::sort(fuuro_choice.rbegin(), fuuro_choice.rend());
		}
	}
}

#ifdef WINSTD
__declspec(dllexport)
#endif
Moves ai(const Moves& game_record, const int pid, const bool out_console_input) {
	assert(game_record.size() > 0);
    out_console = out_console_input;
	const json11::Json& last_action = game_record[game_record.size() - 1];
	Tactics tactics(tactics_json[pid]);

	if (tactics_json[pid].object_items().count("jun_pt") > 0) {
		json11::Json::array jun_pt = tactics_json[pid]["jun_pt"].array_items();
		assert_with_out(jun_pt.size() == 4, "jun_pt must have 4 elements.");
		for (auto i = 0; i < 4; i++) {
			tactics.jun_pt[i] = jun_pt[i].int_value();
		}
	}

    Selector selector;
    selector.set_selector(game_record, pid, tactics);
	if (out_console) {
		std::cout << "ai:" << pid << " " << selector.hai_choice.size() << " " << selector.fuuro_choice.size() << std::endl;
	}
	if (selector.hai_choice.size() > 0) {
		const Game_State game_state = get_game_state(game_record);
		if (out_console) {
			std::cout << moves_to_string(selector.hai_choice[0].out_moves(game_state, pid, hai_str_to_int(last_action["pai"].string_value()))) << std::endl;
		}
		return selector.hai_choice[0].out_moves(game_state, pid, hai_str_to_int(last_action["pai"].string_value()));
	} else if (selector.fuuro_choice.size() > 0) {
		if (out_console) {
			std::cout << "fuuro_choice:" << (int)selector.fuuro_choice[0].fuuro_action_type << " " << selector.fuuro_choice[0].fuuro_hai << std::endl;
			std::cout << moves_to_string(selector.fuuro_choice[0].out_moves(pid, last_action["actor"].int_value())) << std::endl;
		}
		return selector.fuuro_choice[0].out_moves(pid, last_action["actor"].int_value());
	} else {
		return {make_none(pid)};
	}
}

#ifdef WINSTD
__declspec(dllexport)
#endif
std::vector<std::pair<Moves, float>> calc_moves_score(const Moves& game_record, const int pid) {
	assert(game_record.size() > 0);
	out_console = false;
	const json11::Json& last_action = game_record[game_record.size() - 1];
	Tactics tactics(tactics_json[pid]);

	if (tactics_json[pid].object_items().count("jun_pt") > 0) {
		json11::Json::array jun_pt = tactics_json[pid]["jun_pt"].array_items();
		assert_with_out(jun_pt.size() == 4, "jun_pt must have 4 elements.");
		for (auto i = 0; i < 4; i++) {
			tactics.jun_pt[i] = jun_pt[i].int_value();
		}
	}

	std::vector<std::pair<Moves, float>> ret;
	Selector selector;
    selector.set_selector(game_record, pid, tactics);
	if (selector.hai_choice.size() > 0) {
		const Game_State game_state = get_game_state(game_record);
		for (const Hai_Choice& choice : selector.hai_choice) {
			ret.push_back(std::pair<Moves, float>({choice.out_moves(game_state, pid, hai_str_to_int(last_action["pai"].string_value())), choice.pt_exp_total}));
		}
	} else if (selector.fuuro_choice.size() > 0) {
		for (const Fuuro_Choice& choice : selector.fuuro_choice) {
			ret.push_back(std::pair<Moves, float>({choice.out_moves(pid, last_action["actor"].int_value()), choice.pt_exp_total}));
		}
	}
	return ret;
}

#ifdef WINSTD
__declspec(dllexport)
#endif
json11::Json ai_review(const Moves& game_record, const int pid) {
	assert(game_record.size() > 0);
	out_console = false;
	const json11::Json& last_action = game_record[game_record.size() - 1];
	Tactics tactics(tactics_json[pid]);

	if (tactics_json[pid].object_items().count("jun_pt") > 0) {
		auto jun_pt = tactics_json[pid]["jun_pt"].array_items();
		assert_with_out(jun_pt.size() == 4, "jun_pt must have 4 elements.");
		for (auto i = 0; i < 4; i++) {
			tactics.jun_pt[i] = jun_pt[i].int_value();
		}
	}

	json11::Json::array actions_array;
	Selector selector;
    selector.set_selector(game_record, pid, tactics);
	if (selector.hai_choice.size() > 0) {
		const Game_State game_state = get_game_state(game_record);
		for (const Hai_Choice& choice : selector.hai_choice) {
			actions_array.push_back(json11::Json::object{
				{"moves", choice.out_moves(game_state, pid, hai_str_to_int(last_action["pai"].string_value()))},
				{"review", choice.review},
			});
		}
	} else if (selector.fuuro_choice.size() > 0) {
		for (const Fuuro_Choice& choice : selector.fuuro_choice) {
			actions_array.push_back(json11::Json::object{
				{"moves", choice.out_moves(pid, last_action["actor"].int_value())},
				{"review", choice.review},
			});
		}
	}

	return json11::Json(actions_array);
}

#ifdef WINSTD
__declspec(dllexport)
#endif
void set_tactics(const json11::Json& match_setup_json) {
	assert(!match_setup_json["tactics"].is_null());
	assert(match_setup_json["tactics"].array_items().size() == 4);
	for (int pid = 0; pid < 4; pid++) {
		tactics_json[pid] = match_setup_json["tactics"][pid];
	}
}

#ifdef WINSTD
__declspec(dllexport)
#endif
void set_tactics_one(const json11::Json& match_setup_json) {
	assert(!match_setup_json["tactics"].is_null());
	for (int pid = 0; pid < 4; pid++) {
		tactics_json[pid] = match_setup_json["tactics"];
	}
}
