#include "tenpai_prob_calc.hpp"

float infer_tenpai_prob_ako_old(const Kawa& kawa, const int fuuro_num) {
    // ako がcal_nokori_expの時に利用している。
	const int tedashi_num = count_tedashi_num(kawa);

    
    const std::array<bool, 38> sute_kind_flag = get_sute_kind_flag(kawa);
    int sute_flag[38];
    for (int hai = 0; hai < 38; hai++) {
        sute_flag[hai] = sute_kind_flag[hai] ? 1 : 0;
    }

	int sute_kind[6] = {};
	sute_kind[0] = sute_flag[31] + sute_flag[32] + sute_flag[33] + sute_flag[34] + sute_flag[35] + sute_flag[36] + sute_flag[37];
	sute_kind[1] = sute_flag[1] + sute_flag[9] + sute_flag[11] + sute_flag[19] + sute_flag[21] + sute_flag[29];
	sute_kind[2] = sute_flag[2] + sute_flag[8] + sute_flag[12] + sute_flag[18] + sute_flag[22] + sute_flag[28];
	sute_kind[3] = sute_flag[3] + sute_flag[7] + sute_flag[13] + sute_flag[17] + sute_flag[23] + sute_flag[27];
	sute_kind[4] = sute_flag[4] + sute_flag[6] + sute_flag[14] + sute_flag[16] + sute_flag[24] + sute_flag[26];
	for(int i=0;i<3;i++){
		if(sute_flag[i*10+5]||sute_flag[i*10+10]){
			sute_kind[5]++;
		}
	}

	const int DVec = 9;
	int x[DVec];
	x[0] = 1.0;
	x[1] = fuuro_num;
	x[2] = tedashi_num;
	for(int i=0;i<6;i++){
		x[3+i] = sute_kind[i];
	}

	double w[DVec];
	w[0] = -4.9433334901103434e+000;
	w[1] = 1.2079080379420084e+000;
	w[2] = 4.9703758302681988e-003;
	w[3] = 2.2804536169498757e-001;
	w[4] = 1.3372123812601580e-001;
	w[5] = 3.4313212938013748e-001;
	w[6] = 4.0650499365173909e-001;
	w[7] = 4.8931336938797670e-001;
	w[8] = 3.6756628774530170e-001;

	double a = 0.0;
	for(int i=0;i<DVec;i++){
		a = a + w[i]*x[i];
	}

	return 1.0/(1.0+exp(-a));
}

float infer_tenpai_prob_ako(const Moves& game_record, const Game_State& game_state, const int target) {
    const int fn = game_state.player_state[target].fuuro.size();
    const int kn = game_state.player_state[target].kawa.size();
    const int lb = [&] {
        if      (fn == 0) { return 2; }
        else if (fn == 1) { return 2; }
        else if (fn == 2) { return 3; }
        else if (fn == 3) { return 7; }
        else { assert_with_out(false, "infer_tenpai_prob_ako lb_error"); return 0; }
	}();
    const int ub = 18;
    const std::string file_name = "params/tenpai_prob/ako/fuuro" + std::to_string(fn) + "_para" + std::to_string(std::min(std::max(lb, kn), ub)) + "_4000.txt";
    float w[5];
    read_parameters(w, 5, file_name);

	float x[5];

    bool other_reach_flag = false;
    int discard_after_other_reach = 0;
    int kyoku_begin = -1;
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        if (game_record[i]["type"] == "start_kyoku") {
            kyoku_begin = i;
            break;
        }
    }
    assert(kyoku_begin != -1);

    for (int i = kyoku_begin; i < game_record.size(); i++) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"] == "start_game" ||
            action_json["type"] == "end_game" ||
            action_json["type"] == "end_kyoku" ||
            action_json["type"] == "hora"
        ) {
            assert(false);
        } else if (action_json["type"] == "start_kyoku") {
            continue;
        } else if (action_json["type"] == "reach") {
            if (action_json["actor"].int_value() != target) {
                other_reach_flag = true;
            }
        } else if (action_json["type"] == "dahai") {
            if (action_json["actor"].int_value() == target && other_reach_flag) {
                discard_after_other_reach++;
            }
        } else if (action_json["type"] == "chi" || action_json["type"] == "pon") {
			if (action_json["actor"].int_value() == target && other_reach_flag) {
				discard_after_other_reach = -1;
				// 現状discard_after_other_reachに対するこの処理はakoにのみ行われている。
			}
		}
    }

    int sute_kind[6] = {};
    std::array<bool, 38> sute_kind_flag = get_sute_kind_flag(game_state.player_state[target].kawa);
    for (int hai = 0; hai < 38; hai++) {
        if (sute_kind_flag[hai]) {
            if (30 < hai) { sute_kind[0]++; }
            else if (hai % 10 == 1 || hai % 10 == 9) { sute_kind[1]++; }
            else if (hai % 10 == 2 || hai % 10 == 8) { sute_kind[2]++; }
            else if (hai % 10 == 3 || hai % 10 == 7) { sute_kind[3]++; }
            else if (hai % 10 == 4 || hai % 10 == 6) { sute_kind[4]++; }
            else if (hai % 10 == 5) { sute_kind[5]++; }
            else {
                assert(false);
            }
        }
    }

	x[0] = 1.0;
	x[1] = (float)count_tedashi_num(game_state.player_state[target].kawa);
	x[2] = (float)discard_after_other_reach;
	x[3] = (float)(sute_kind[0] + sute_kind[1]);
	x[4] = (float)(sute_kind[2] + sute_kind[3] + sute_kind[4] + sute_kind[5]);

	if (fn == 0) {
		return std::min((float)0.12, logistic(w, x, 5));
	} else {
		return logistic(w, x, 5);
	}
}

bool somete_possible(const Fuuro_Vector& fuuro, const Color_Type color) {
    for (const Fuuro_Elem& f : fuuro) {
	    if (hai_color(f.consumed[0]) != color && hai_color(f.consumed[0]) != CT_JIHAI) {
			return false;
		}
	}
	return true;
}

std::array<int, 4> get_somete_feature(const Kawa& kawa, const Color_Type color) {
	int nclength = 0;
	int ncstart = 0;
	int nclength_tmp = 0;
	int ncstart_tmp = 0;
	for (int kn = 0; kn < kawa.size(); kn++) {
		if (hai_color(kawa[kn].hai) == color || hai_color(kawa[kn].hai) == CT_JIHAI) {
			if(nclength_tmp > nclength) {
				nclength = nclength_tmp;
				ncstart = ncstart_tmp;
			}
			nclength_tmp = 0;
			ncstart_tmp = kn+1;
		} else {
			nclength_tmp++;
		}
	}

	if (nclength_tmp > nclength) {
		nclength = nclength_tmp;
		ncstart = ncstart_tmp;
	}

	int before_jihai_flag = 0;
	int before_suhai_flag = 0;

	for (int kn = 0; kn < ncstart; kn++) {
		if (hai_color(kawa[kn].hai) == CT_JIHAI) {
			before_jihai_flag = 1;
		}else if(hai_color(kawa[kn].hai) == color){
			before_suhai_flag = 1;
		}
	}
	int last_tedashi_color_flag = 0;
	for (int kn = 0; kn < kawa.size(); kn++) {
		if (!kawa[kn].tsumogiri) {
			if (hai_color(kawa[kn].hai) == color || hai_color(kawa[kn].hai) == CT_JIHAI) {
				last_tedashi_color_flag = 1;
			} else {
				last_tedashi_color_flag = 0;
			}
		}
	}
    std::array<int, 4> feature;
	feature[0] = nclength;
	feature[1] = nclength*last_tedashi_color_flag;
	feature[2] = before_jihai_flag;
	feature[3] = before_suhai_flag;
    return feature;
}


std::array<int, 2> get_somete_tenpai_post_feature(const Kawa& kawa, const Color_Type color){
	int sute_flag[38] = {};
	for (int i = 0; i < kawa.size(); i++) {
		sute_flag[kawa[i].hai] = 1;
	}
	int sute_jihai = 0;
	int sute_suhai = 0;
	for (int i = 0; i < 7; i++) {
		sute_jihai += sute_flag[31+i];
	}
	for (int i = 1; i <= 10; i++) {
		sute_suhai += sute_flag[10*color+i];
	}
    std::array<int, 2> feature;
	feature[0] = sute_jihai;
	feature[1] = sute_suhai;
    return feature;
}

float cal_somete_ako(const Game_State& game_state, const int target, const Color_Type color) {
	if (game_state.player_state[target].reach_declared || game_state.player_state[target].fuuro.size() == 0) {
		return 0.0;
	} else if (!somete_possible(game_state.player_state[target].fuuro, color)) {
		return 0.0;
	} else {
		char file_name[100];
		if (game_state.player_state[target].fuuro.size() >= 3) {
			sprintf(file_name, "params/tenpai_prob/ako/somete/somete_fuuro3_para%d_100.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),18)));
		} else if (game_state.player_state[target].fuuro.size() == 2) {
			sprintf(file_name, "params/tenpai_prob/ako/somete/somete_fuuro2_para%d_1000.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),16)));
		} else if (game_state.player_state[target].fuuro.size() == 1) {
			sprintf(file_name, "params/tenpai_prob/ako/somete/somete_fuuro1_para%d_3000.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),17)));
		} else {
			assert_with_out(false, "cal_somete_ako error");
		}
		float w[5], x[5];
		read_parameters(w, 5, file_name);
		std::array<int, 4> feature = get_somete_feature(game_state.player_state[target].kawa, color);
		x[0] = 1.0;
		for (int i = 0; i < 4; i++) {
			x[i+1] = feature[i];
		}
		return logistic(w, x, 5);
	}
}

float cal_somete_tenpai_post(const Game_State& game_state, const int target, const Color_Type color) {
	char file_name[100];
	if (game_state.player_state[target].fuuro.size() >= 3) {
		sprintf(file_name, "params/tenpai_prob/ako/somete/somete_tenpai_fuuro3_para%d_150.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),17)));
	} else if (game_state.player_state[target].fuuro.size() == 2) {
		sprintf(file_name, "params/tenpai_prob/ako/somete/somete_tenpai_fuuro2_para%d_300.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),18)));
	} else if (game_state.player_state[target].fuuro.size() == 1) {
		sprintf(file_name, "params/tenpai_prob/ako/somete/somete_tenpai_fuuro1_para%d_200.txt", std::max(7,std::min((int)game_state.player_state[target].kawa.size(),17)));
	} else {
		return 0.0;
	}

	float w[3], x[3];
	read_parameters(w, 3, file_name);
	std::array<int, 2> feature = get_somete_tenpai_post_feature(game_state.player_state[target].kawa, color);
	x[0] = 1.0;
	for (int i = 0; i < 2; i++) {
		x[i+1] = feature[i];
	}
	return logistic(w, x, 3);
}