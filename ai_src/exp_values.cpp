#include "exp_values.hpp"

extern const bool out_console;
extern const std::array<json11::Json, 4> tactics_json;

float get_ryuukyoku_pattern_prob(const std::array<int, 4>& flag, const std::array<float, 4>& keiten_prob) {
	float res = 1.0;
	for (int pid = 0; pid < 4; pid++) {
		if (flag[pid] == 1) {
			res *= keiten_prob[pid];
		} else {
			res *= 1.0 - keiten_prob[pid];
		}
	}
	return res;
}

int cal_tsumo_num_exp(const int my_pid, const Game_State& game_state, const int dahai_inc, const std::array<float, 4> tenpai_prob) {
	// dahai_inc は本来取り入れる要素と思われるが、現状利用されていない。
	if (tactics_json[my_pid]["tsumo_num_est"] == "instant") {
		const int acn = game_state.player_state[my_pid].kawa.size();
		return ceil(std::max(18 - acn, 0) * 0.7);
	} else if (tactics_json[my_pid]["tsumo_num_est"] == "ako") {
		const int acn = std::min(std::max(1, (int)game_state.player_state[my_pid].kawa.size()), 16);
		float w[4]; float x[4] = {}; int rp = 0;
		const std::string file_name = "params/tsumo_num/ako/para" + std::to_string(acn) + "_4000.txt";
		read_parameters(w, 4, file_name);

		float tp = 1.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) {
				tp = tp * (1.0 - infer_tenpai_prob_ako_old(game_state.player_state[pid].kawa, game_state.player_state[pid].fuuro.size()));
			}
		}
		tp = 1.0 - tp;

		x[0] = 1.0;
		if (rp >= 2) {
			x[2] = 1.0;
		} else if (rp == 1) {
			x[1] = 1.0;
		}
		x[3] = tp;
		return ceil(logistic(w, x, 4) * (18 - acn));
	} else {
		assert_with_out(false, "tsumo_num_est error!");
		return 0;
	}
}

float cal_my_agari_prob(const int my_pid, const Moves& game_record, const float agari_prob_sol, const Game_State& game_state, const int dahai_inc, const std::array<float, 4>& tenpai_prob) {
	assert_with_out(!tactics_json[my_pid]["use_agari_coeff_tp_fnm"].is_null(), "use_agari_coeff_tp_fnm is null");
	assert_with_out(!tactics_json[my_pid]["use_agari_coeff_tp_an"].is_null(), "use_agari_coeff_tp_an is null");
	if (game_record[game_record.size()-1]["type"] == "tsumo" &&
		game_state.player_state[my_pid].fuuro.size() == 0 &&
		get_other_fuuro_num_max(my_pid, game_state) <= tactics_json[my_pid]["use_agari_coeff_tp_fnm"].int_value() &&
		game_state.player_state[my_pid].kawa.size() <= tactics_json[my_pid]["use_agari_coeff_tp_an"].int_value()
	) {
		return agari_prob_sol;
	}
	
	if (tactics_json[my_pid]["agari_prob_est"].string_value() == "instant") {
		return 0.5 * agari_prob_sol;
	} else if (tactics_json[my_pid]["agari_prob_est"].string_value() == "ako") {
		// ako to do ako で使っているlightの時のモデルは異なるもの。
		const int act_num = (int)game_state.player_state[my_pid].kawa.size() + dahai_inc;
		const std::string file_name = "params/agari_prob/ako/agari_para" + std::to_string(std::max(1, std::min(act_num,15))) + ".txt";
		float w[4];
		read_parameters(w, 4, file_name);

		float x[4];
		x[0] = 1.0;
		x[1] = 0;
		float tmp = 1.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) {
				tmp *= 1.0 - tenpai_prob[pid];
			}
		}
		x[2] = 1.0 - tmp;
		x[3] = my_logit(agari_prob_sol);
		return logistic(w, x, 4);
	} else {
		std::cout << "cal_my_agari_prob_error" << std::endl;
		assert(false);
		return 0.0;
	}
}

float cal_my_agari_value(const float agari_prob_sol, const float value_sol, const float value_not_agari) {
	if (agari_prob_sol == 0.0) {
		return 0.0;
	} else {
		return (value_sol - (1.0 - agari_prob_sol) * value_not_agari) / agari_prob_sol;
	}
}

float cal_ryuukyoku_prob(const int my_pid, const Game_State& game_state, const float my_agari_prob, const std::array<float, 4>& tenpai_prob, const int dahai_inc) {
	if (tactics_json[my_pid]["ryukyoku_prob_est"] == "instant") {
		return 1.0 - 0.75 * std::max(18 - (int)game_state.player_state[my_pid].kawa.size() - dahai_inc, 0) / 18.0;
	} else if (tactics_json[my_pid]["ryukyoku_prob_est"] == "ako") {
		const int act_num = game_state.player_state[my_pid].kawa.size() + dahai_inc;
		std::string file_name;
		if (act_num <= 6) {
			file_name = "params/ryukyoku_prob/ako/para1_6_4000.txt";
		} else {
			file_name = "params/ryukyoku_prob/ako/para" + std::to_string(std::min(act_num, 18)) + "_4000.txt";
		}
		float w[4];
		read_parameters(w, 4, file_name);
		float x[4];
		x[0] = 1.0;
		if (act_num <= 6) {
			x[1] = act_num;
			x[2] = 0.0;
		} else {
			x[1] = 0.0; x[2] = 0.0;
		}
		float tmp = 1.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) {
				tmp *= 1.0 - tenpai_prob[pid];
			}
		}
		x[3] = 1.0 - tmp;
		return logistic(w, x, 4);
	} else {
		assert_with_out(false, "ryukyoku_prob_est_error");
		return 0.0;
	}
}

float cal_my_keiten_prob(const int my_pid, const Game_State& game_state,  const int dahai_inc, const float keiten_prob_sol) {
	if (tactics_json[my_pid]["my_keiten_prob_est"] == "instant") {
		if (keiten_prob_sol == 0.0) {
			return 0.0;
		} else {
			return 0.6 * keiten_prob_sol;
		}
	} else if (tactics_json[my_pid]["my_keiten_prob_est"] == "ako") {
		const int act_num = (int)game_state.player_state[my_pid].kawa.size() + dahai_inc;
		const std::string file_name = "params/my_keiten_prob/ako/para" + std::to_string(std::min(act_num, 17)) + "_4000.txt";
		float w[3];
		read_parameters(w, 3, file_name);

		float x[3];
		x[0] = 1.0;
		x[1] = keiten_prob_sol;
		x[2] = 0;
		return logistic(w, x, 3);
	} else {
		assert_with_out(false, "my_keiten_prob_est error!");
		return 0.0;
	}
}

float cal_other_keiten_prob(const int my_pid, const int target_pid, const Game_State& game_state,  const int dahai_inc, const float current_tenpai_prob) {
	if (tactics_json[my_pid]["other_keiten_prob_est"] == "instant") {
		return 0.5;
	} else if (tactics_json[my_pid]["other_keiten_prob_est"] == "ako") {
		const int act_num = (int)game_state.player_state[my_pid].kawa.size() + dahai_inc;
		const std::string file_name = "params/other_keiten_prob/ako/para" + std::to_string(std::min(act_num, 17)) + "_4000.txt";
		float w[2], x[2];
		read_parameters(w, 2, file_name);

		x[0] = 1.0; x[1] = current_tenpai_prob;
		return logistic(w, x, 2);
	} else {
		assert_with_out(false, "other_keiten_prob_est error!");
		return 0.0;
	}
}

std::array<float, 4> cal_keiten_prob(const int my_pid, const Game_State& game_state, const int dahai_inc, const float keiten_prob_sol, const std::array<float, 4>& tenpai_prob) {
	std::array<float, 4> keiten_prob;
	for (int pid = 0; pid < 4; pid++) {
		if (pid == my_pid) {
			keiten_prob[pid] = cal_my_keiten_prob(my_pid, game_state, dahai_inc, keiten_prob_sol);
		} else {
			keiten_prob[pid] = cal_other_keiten_prob(my_pid, pid, game_state, dahai_inc, tenpai_prob[pid]);
		}
	}
	return keiten_prob;
}

float cal_ryuukyoku_value(const std::array<float, 4>& keiten_prob, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp) {
	float ryuukyoku_value = 0.0;
	std::array<int, 4> flag_tmp;
	for (int t0 = 0; t0 < 2; t0++) {
		for (int t1 = 0; t1 < 2; t1++) {
			for (int t2 = 0; t2 < 2; t2++) {
				for (int t3 = 0; t3 < 2; t3++) {
					flag_tmp[0] = t0;
					flag_tmp[1] = t1;
					flag_tmp[2] = t2;
					flag_tmp[3] = t3;
					ryuukyoku_value += ryuukyoku_pt_exp[t0][t1][t2][t3] * get_ryuukyoku_pattern_prob(flag_tmp, keiten_prob);
				}
			}
		}
	}
	return ryuukyoku_value;
}

float cal_noten_ryuukyoku_value(const int my_pid, const std::array<float, 4>& keiten_prob, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp) {
	std::array<float, 4> keiten_prob_new = keiten_prob;
	keiten_prob_new[my_pid] = 0.0;
	return cal_ryuukyoku_value(keiten_prob_new, ryuukyoku_pt_exp);
}

std::array<std::array<float, 4>, 4> cal_target_prob(const int my_pid, const std::array<float, 4>& risk_array, const Game_State& game_state, const int dahai_inc, const int fuuro_num_inc) {
	std::array<std::array<float, 4>, 4> target_prob;
	if (tactics_json[my_pid]["houjuu_est"] == "instant") {
		for (int pid1 = 0; pid1 < 4; pid1++) {
			for (int pid2 = 0; pid2 < 4; pid2++) {
				if (pid1 == pid2) {
					target_prob[pid1][pid2] = 5.0 / 11.0;
				} else {
					target_prob[pid1][pid2] = 2.0 / 11.0;
				}
			}
		}
	} else if (tactics_json[my_pid]["houjuu_est"] == "ako") {
		float w[3], x[3]; x[0] = 1.0;
		const int act_num = (int)game_state.player_state[my_pid].kawa.size() + dahai_inc;
		std::string file_name = "params/houjuu_prob/ako/para" + std::to_string(std::max(6, std::min(act_num, 14))) + "_4000.txt";
		read_parameters(w, 3, file_name);

		for (int pid1 = 0; pid1 < 4; pid1++) {
			if (pid1 != my_pid) {
				x[1] = risk_array[pid1];
				x[2] = (float)(game_state.player_state[my_pid].fuuro.size() + fuuro_num_inc);
				if (act_num <= 4) {
					target_prob[pid1][my_pid] = 2.0/11.0;
				} else {
					target_prob[pid1][my_pid] = logistic(w, x, 2);
				}
				for (int pid2 = 0; pid2 < 4; pid2++) {
					if (pid2 != my_pid) {
						if (pid1 == pid2) {
							target_prob[pid1][pid2] = (1.0 - target_prob[pid1][my_pid])*5.0/9.0;
						} else {
							target_prob[pid1][pid2] = (1.0 - target_prob[pid1][my_pid])*2.0/9.0;
						}
					}
				}
			}
		}
		for (int pid = 0; pid < 4;pid++) {
			if (pid == my_pid) {
				target_prob[my_pid][pid] = 5.0/11.0;
			} else {
				target_prob[my_pid][pid] = 2.0/11.0;
			}
		}
	} else {
		std::cout << "cal_target_prob_error" << std::endl;
		assert(false);
	}
	return target_prob;
}

std::array<std::array<float, 4>, 4> cal_target_prob_other(const int my_pid) {
	std::array<std::array<float, 4>, 4> target_prob;
	for (int pid1 = 0; pid1 < 4; pid1++) {
		for (int pid2 = 0; pid2 < 4; pid2++) {
			if (pid1 == my_pid || pid2 == my_pid) {
				target_prob[pid1][pid2] = 0.0;
			} else if (pid1 == pid2) {
				target_prob[pid1][pid2] = 5.0 / 9.0;
			} else {
				target_prob[pid1][pid2] = 2.0 / 9.0;
			}
		}
	}
	return target_prob;
}

float cal_target_value_child (
	const std::array<float, 100>& kyoku_end_pt_exp, const std::array<float, 100>& han_prob
) {
	float value = 0.0;
	for (int han = 0; han < 100; han++) {
		value += kyoku_end_pt_exp[han] * han_prob[han];
	}
	return value;
}

std::array<std::array<float, 4>, 4> cal_target_value(
	const int my_pid, const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp, const float my_agari_value,
	const std::array<std::array<float, 100>, 4>& tsumo_prob, const std::array<std::array<float, 100>, 4>& ron_prob
) {
	std::array<std::array<float, 4>, 4> target_value;
	for (int pid1 = 0; pid1 < 4; pid1++) {
		for (int pid2 = 0; pid2 < 4; pid2++) {
			if (pid1 == my_pid) {
				target_value[pid1][pid2] = my_agari_value;
			} else if (pid1 == pid2) {
				target_value[pid1][pid2] = cal_target_value_child(kyoku_end_pt_exp[pid1][pid2], tsumo_prob[pid1]);
			} else {
				target_value[pid1][pid2] = cal_target_value_child(kyoku_end_pt_exp[pid1][pid2], ron_prob[pid1]);
			}
		}
	}
	return target_value;
}


std::array<float, 5> cal_kyoku_result_prob(const int my_pid, const Game_State& game_state, const int dahai_inc, const float my_agari_prob, const std::array<float, 4>& tenpai_prob, const float ryuukyoku_prob) {
	std::array<float, 5> kyoku_result_prob;
	kyoku_result_prob[my_pid] = my_agari_prob;
	kyoku_result_prob[4] = (1.0 - my_agari_prob) * ryuukyoku_prob;

	if (tactics_json[my_pid]["result_other_est"] == "instant") {
		float other_tenpai_prob_sum = 0.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) { other_tenpai_prob_sum += tenpai_prob[pid]; }
			kyoku_result_prob[pid] = 0.0;
		}
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) {
				if (other_tenpai_prob_sum > 0.00001) {
					kyoku_result_prob[pid] = (1.0 - my_agari_prob) * (1.0 - ryuukyoku_prob) * tenpai_prob[pid] / other_tenpai_prob_sum;
				} else {
					kyoku_result_prob[pid] = (1.0 - my_agari_prob) * (1.0 - ryuukyoku_prob) * 1.0 / 3.0;
					// モードによっては、全員のテンパイ率が0の時もある。
				}
			}
		}
	} else if (tactics_json[my_pid]["result_other_est"] == "ako") {
		const int act_num = game_state.player_state[my_pid].kawa.size() + dahai_inc;
		float w[21];
		const std::string file_name = (act_num <= 7) ?
			"params/result_other/ako/para1_6_4000.txt":
			"params/result_other/ako/para" + std::to_string(std::min(act_num, 17)) + "_4000.txt";
		read_parameters(w, 21, file_name);
		float x[7];
		x[0] = 1.0;
		for (int pid_add = 1; pid_add <= 3; pid_add++) {
			const int pid = (my_pid + pid_add) % 4;
			x[pid_add] = 0.0;
			x[pid_add+3] = tenpai_prob[pid];
		}
		float tmp_array[3];
		MC_logistic(w, x, tmp_array, 7, 3);
		for (int pid_add = 1; pid_add <= 3; pid_add++) {
			const int pid = (my_pid + pid_add) % 4;
			kyoku_result_prob[pid] = (1.0 - my_agari_prob) * (1.0 - ryuukyoku_prob) * tmp_array[pid_add-1];
		}
	} else {
		std::cout << "cal_kyoku_result_error" << std::endl;
		assert(false);
	}
	return kyoku_result_prob;
}

std::array<float, 5> cal_kyoku_result_value(
	const std::array<std::array<float, 4>, 4>& target_prob, const std::array<std::array<float, 4>, 4>& target_value, const float ryuukyoku_value
) {
	std::array<float, 5> kyoku_result_value;
	for (int pid1 = 0; pid1 < 4; pid1++) {
		kyoku_result_value[pid1] = 0.0;
		for (int pid2 = 0; pid2 < 4; pid2++) {
			kyoku_result_value[pid1] += target_prob[pid1][pid2] * target_value[pid1][pid2];
		}
	}
	kyoku_result_value[4] = ryuukyoku_value;
	return kyoku_result_value;
}

std::array<float, 4> cal_risk_array(const int my_pid, const Game_State& game_state, const int dahai_inc, const Hai_Array& tehai, const std::array<float, 4> tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob) {
	std::array<float, 4> risk_array;
	const int tsumo_num_exp = cal_tsumo_num_exp(my_pid, game_state, dahai_inc, tenpai_prob);
	std::array<float, 38> houjuu_hai_value_tmp;
	for (int hai = 0; hai < 38; hai++) {
		houjuu_hai_value_tmp[hai] = -1.0;
	}
	const Hai_Array& tehai_kind = haikind(tehai);
	for (int pid = 0; pid < 4; pid++) {
		Betaori betaori = cal_betaori(tehai_kind, houjuu_hai_prob[pid], houjuu_hai_value_tmp, 0.0, 0.0, tsumo_num_exp);
		risk_array[pid] = betaori.betaori_houjuu_prob;
	}
	return risk_array;
}

float cal_exp(
	const int my_pid, const Moves& game_record, const Game_State& game_state, const Hai_Array& result_tehai, const float agari_prob_sol, const float value_sol, const float value_not_agari, const float keiten_prob_sol,
	const std::array<float, 4>& tenpai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, const std::array<std::array<float, 100>, 4>& tsumo_prob, const std::array<std::array<float, 100>, 4>& ron_prob,
	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp, const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp,
	const int dahai_inc, const int fuuro_inc
) {
	const float my_agari_prob = cal_my_agari_prob(my_pid, game_record, agari_prob_sol, game_state, dahai_inc, tenpai_prob);
	const float my_agari_value = cal_my_agari_value(agari_prob_sol, value_sol, value_not_agari);

	const float ryuukyoku_prob = cal_ryuukyoku_prob(my_pid, game_state, my_agari_prob, tenpai_prob, dahai_inc);
	const std::array<float, 4> keiten_prob = cal_keiten_prob(my_pid, game_state, dahai_inc, keiten_prob_sol, tenpai_prob); // to do tenpai_probの反映
	const float ryuukyoku_value = cal_ryuukyoku_value(keiten_prob, ryuukyoku_pt_exp);

	const std::array<float, 5> kyoku_result_prob = cal_kyoku_result_prob(my_pid, game_state, dahai_inc, my_agari_prob, tenpai_prob, ryuukyoku_prob);
	const std::array<float, 4> risk_array = cal_risk_array(my_pid, game_state, dahai_inc, result_tehai, tenpai_prob, houjuu_hai_prob);
	const std::array<std::array<float, 4>, 4> target_prob = cal_target_prob(my_pid, risk_array, game_state, dahai_inc, fuuro_inc);
	const std::array<std::array<float, 4>, 4> target_value = cal_target_value(my_pid, kyoku_end_pt_exp, my_agari_value, tsumo_prob, ron_prob);
	const std::array<float, 5> kyoku_result_value = cal_kyoku_result_value(target_prob, target_value, ryuukyoku_value);

	float exp_value = 0.0;
	for (int i = 0; i < 5; i++) {
		exp_value += kyoku_result_prob[i] * kyoku_result_value[i];
		if (out_console) {
			std::cout << "krcheck:" << kyoku_result_prob[i] << " " << kyoku_result_value[i] << std::endl;
		}
	}
	if (out_console) {
		std::cout << "cal_exp_check:" << agari_prob_sol << " " << keiten_prob_sol << " " << value_sol << " " << value_not_agari << " " << my_agari_prob << " " << my_agari_value << " " << kyoku_result_value[4] << " " << exp_value << std::endl;
	}
	return exp_value;
}

float cal_passive_ryuukyoku_value(
	const int my_pid, const Game_State& game_state, const std::array<float, 4>& tenpai_prob, 
	const std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2>& ryuukyoku_pt_exp
) {
	std::array<float, 4> keiten_prob = cal_keiten_prob(my_pid, game_state, 0, 0.0, tenpai_prob); // to do tenpai_probの反映
	keiten_prob[my_pid] = 0.0; // 本来cal_keiten_probの内部で、keiten_prob_sol = 0.0の時は、keiten_prob[my_pid]が0になるようにすべきと思われるが、akoの方がそのようになっていないため、passive_ryuukyoku_valueの途中で0にする。
	return cal_ryuukyoku_value(keiten_prob, ryuukyoku_pt_exp);
}

float cal_other_end_value(
	const int my_pid, const Game_State& game_state, const std::array<float, 4>& tenpai_prob,
	const std::array<std::array<float, 100>, 4>& tsumo_prob, const std::array<std::array<float, 100>, 4>& ron_prob,
	const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
) {
	const std::array<float, 5> kyoku_result_prob = cal_kyoku_result_prob(my_pid, game_state, 0, 0.0, tenpai_prob, 0.0);
	const std::array<std::array<float, 4>, 4> target_prob = cal_target_prob_other(my_pid);
	const std::array<std::array<float, 4>, 4> target_value = cal_target_value(my_pid, kyoku_end_pt_exp, 0.0, tsumo_prob, ron_prob);
	const std::array<float, 5> kyoku_result_value = cal_kyoku_result_value(target_prob, target_value, 0.0);

	float exp_value = 0.0;
	for (int i = 0; i < 5; i++) {
		exp_value += kyoku_result_prob[i] * kyoku_result_value[i];
	}
	return exp_value;
}

float cal_betaori_exp(
	const int my_pid, const Game_State& game_state,
	const Hai_Array& tehai_tmp, const std::array<float, 38>& betaori_houjuu_hai_prob, const std::array<float, 38>& total_houjuu_hai_value,
	const float not_agari_value, const float other_end_value, const float passive_ryuukyoku_value, const float passive_ryuukyoku_prob,
	const int tsumo_num_exp, const Tactics& tactics
) {
	// ここのbetaoriのデフォルト値を、not_agari_valueにするのは、other_end_valueだと、other_end_value が not_agari_valueよりかなり大きい場合に、
	// 望ましくない状況で、calc_agari_probの結果を無視することが起こるため。このようにしたほうが実際に強いかは未検証。
	// 求めるべきは、降りを選択した際に、流局も含めて放銃とならない確率なので、モデルはinclusive_foldの方が正しいと思われる。
	if (tactics.betaori_est == "instant") {
		Betaori betaori = cal_betaori(tehai_tmp, betaori_houjuu_hai_prob, total_houjuu_hai_value, not_agari_value, not_agari_value, tsumo_num_exp);
		return betaori.betaori_exp;
	} else if (tactics.betaori_est == "ako") {
		// to do betaori に関して入力が一致しているか未確認
		Betaori betaori = cal_betaori(tehai_tmp, betaori_houjuu_hai_prob, total_houjuu_hai_value, other_end_value, passive_ryuukyoku_value, tsumo_num_exp);
		float w[3];
		const int act_num = std::min(std::max(2, (int)game_state.player_state[my_pid].kawa.size()), 18); // akoの方はkawa.size()+1になっていないのでそうする。
		const std::string file_name = "params/betaori/ako/betaori_houjuu_para" + std::to_string(act_num) + "_3000.txt";
		read_parameters(w, 3, file_name);
		betaori.modify_betaori_value_with_ryuukyoku_ako(game_state.player_state[my_pid].fuuro.size(), w, passive_ryuukyoku_prob);
		return betaori.betaori_exp;
	} else {
		assert_with_out(false, "cal_betaori_exp invalid tactics.betaori_est");
		return 0.0;
	}
}
