#include "mjutil.hpp"

int factorial (int x) {
	int res = 1;
	for(int i=1;i<=x;i++){
		res = res*i;
	}
	return res;
}

int combination(int n, int k) {
	if(k>n){
		return 0;
	}else{
		return factorial(n)/factorial(k)/factorial(n-k);
	}
}

float my_logit(const float x) {
	if (x >= 1.0) {
		return 10.0;
	} else if (x <= 0.0) {
		return -10.0;
	} else {
		return log(x/(1.0-x));
	}
}

void read_parameters(float* w, const int NPara, const std::string& file_name) {
	FILE *fp;
	fp = fopen(file_name.c_str(), "r");
	assert_with_out(fp != NULL, "read_parameters file_not_found:" + file_name);

	int idx = 0;
	double tmpd;
	while(fscanf(fp, "%lf", &tmpd) != EOF){
		w[idx] = tmpd;
		idx++;
		if(idx==NPara){
			break;
		}
	}
	assert_with_out(idx == NPara, "read_parameters_error:" + file_name);
	fclose(fp);
}

float logistic(const float* const w, const float* const x, const int DVec){
	float a = 0.0;
	for(int i=0;i<DVec;i++){
		a += w[i]*x[i];
	}
	return 1.0/(1.0+exp(-a));
}

void MC_logistic(const float* const w, const float* const x, float* p, const int DVec, const int NClass) {
	float* a = new float[NClass];
	float den = 0.0;
	for(int nc=0;nc<NClass;nc++){
		a[nc] = 0.0;
		for(int dv=0;dv<DVec;dv++){
			a[nc] = a[nc] + w[DVec*nc+dv]*x[dv];
		}
		p[nc] = exp(a[nc]);
		den = den + p[nc];
	}

	for(int nc=0;nc<NClass;nc++){
		p[nc] = p[nc]/den;
	}
	delete[] a;
}

void MC_logistic_mod(const float* const w, const float* const x, float* p, const int DVec, const int NClass) {
	float* a = new float[NClass];
	float den = 0.0;
	float max_elem;
	for(int nc=0;nc<NClass;nc++){
		a[nc] = 0.0;
		for(int dv=0;dv<DVec;dv++){
			a[nc] = a[nc] + w[DVec*nc+dv]*x[dv];
		}
		max_elem = nc == 0 ? a[nc] : std::max(max_elem, a[nc]);
	}

	for(int nc=0;nc<NClass;nc++){
		p[nc] = exp(a[nc] - max_elem);
		den = den + p[nc];
	}

	for(int nc=0;nc<NClass;nc++){
		p[nc] = p[nc]/den;
	}
	delete[] a;
}

bool is_aka_hai(const int hai) {
	return hai > 0 && hai % 10 == 0;
}

int mod_pid(const int kyoku, const int oya, const int pid) {
	int oya_first = (12 + oya - kyoku)%4;
	return (4 + pid - oya_first)%4;
}

int get_other_reach_declared_num(const int my_pid, const Game_State& game_state) {
	int ret = 0;
	for (int pid = 0; pid < 4; pid++) {
		if (pid != my_pid && game_state.player_state[pid].reach_declared) {
			ret++;
		}
	}
	return ret;
}

int get_other_fuuro_num_max(const int my_pid, const Game_State& game_state) {
	int ret = 0;
	for (int pid = 0; pid < 4; pid++) {
		if (pid != my_pid){
			ret = std::max(ret, (int)game_state.player_state[pid].fuuro.size());
		}
	}
	return ret;
}

Hai_Array get_hai_visible_all(const Game_State& game_state) {
	Hai_Array hai_visible_all;
	for (int hai = 0; hai < 38; hai++) {
		hai_visible_all[hai] = 0;
	}
	for (int pid = 0; pid < 4; pid++) {
		const Player_State& player_state = game_state.player_state[pid];
		for (int kn = 0; kn < player_state.kawa.size(); kn++) {
			hai_visible_all[player_state.kawa[kn].hai]++;
		}
		for (int fn = 0; fn < player_state.fuuro.size(); fn++) {
			for (int i = 0; i < player_state.fuuro[fn].consumed.size(); i++) {
				hai_visible_all[player_state.fuuro[fn].consumed[i]]++;
			}
		}
	}
	for (int c = 0; c < 3; c++) {
		if (hai_visible_all[c*10 + 10] > 1) {
			hai_visible_all[c*10 + 5] += hai_visible_all[c*10 + 10] - 1;
			hai_visible_all[c*10 + 10] = 1;
		}
		// 赤牌を副露して、その後カカンすると、赤牌が2枚カウントされてしまうことを予防。
	}
	return hai_visible_all;
}

int get_nokori_hai_num(const int my_pid, const Game_State& game_state) {
	Hai_Array hai_visible_all = get_hai_visible_all(game_state);
	int res = 136 - std::accumulate(hai_visible_all.begin(), hai_visible_all.end(), 0);
	return res - std::accumulate(game_state.player_state[my_pid].tehai.begin(), game_state.player_state[my_pid].tehai.end(), 0);
}

Hai_Array get_hai_visible_wo_tehai(const int my_pid, const Game_State& game_state) {
	Hai_Array hai_visible = get_hai_visible_all(game_state);
	for (int fn = 0; fn < game_state.player_state[my_pid].fuuro.size(); fn++) {
		for (int i = 0; i < game_state.player_state[my_pid].fuuro[fn].consumed.size(); i++) {
			hai_visible[game_state.player_state[my_pid].fuuro[fn].consumed[i]]--;
		}
		if (game_state.player_state[my_pid].fuuro[fn].type != FT_ANKAN) {
			hai_visible[game_state.player_state[my_pid].fuuro[fn].hai]--;
		}
	}
	return hai_visible;
}

Hai_Array get_hai_visible_me(const int my_pid, const Game_State& game_state) {
	Hai_Array hai_visible = get_hai_visible_all(game_state);
	for (int hai = 1; hai < 38; hai++) {
		hai_visible[hai] += game_state.player_state[my_pid].tehai[hai];
	}
	return hai_visible;
}

Hai_Array sum_hai_array(const Hai_Array& hai_array1, const Hai_Array& hai_array2) {
	Hai_Array hai_array_sum;
	for (int hai = 0; hai < 38; hai++) {
		hai_array_sum[hai] = hai_array1[hai] + hai_array2[hai];
	}
	return hai_array_sum;
}

Hai_Array cal_nokori_kind_array(const Hai_Array& visible_kind) {
	Hai_Array nokori_kind_array;
	for (int hai = 0; hai < 38; hai++) {
		if (hai % 10 == 0) {
			nokori_kind_array[hai] = 0;
		} else {
			nokori_kind_array[hai] = 4 - visible_kind[hai];
		}
	}
	return nokori_kind_array;
}

std::array<bool, 38> get_sute_kind_flag(const Kawa& kawa) {
	std::array<bool, 38> sute_kind_flag;
	std::fill(sute_kind_flag.begin(), sute_kind_flag.end(), false);
	for (int i = 0; i < kawa.size(); i++) {
		sute_kind_flag[haikind(kawa[i].hai)] = true;
	}
	return sute_kind_flag;
}

std::array<bool, 38> get_minogashi_flag(const Moves& game_record, const int target) {
	// 手出ししない限り、見逃したものと見做す。ルール上のフリテンとは異なる。
	std::array<bool, 38> minogashi_flag;
	std::fill(minogashi_flag.begin(), minogashi_flag.end(), false);
	for (int i = game_record.size() - 1; 0 <= i; i--) {
		if (game_record[i]["type"] == "start_kyoku") {
			break;
		} else if (game_record[i]["type"] == "dahai") {
			if (game_record[i]["actor"].int_value() == target && !game_record[i]["tsumogiri"].bool_value()) {
				break;
			} else {
				minogashi_flag[haikind(hai_str_to_int(game_record[i]["pai"].string_value()))] = true;
			}
		} 
	}
	return minogashi_flag;
}

std::array<bool, 38> get_minogashi_ar_flag(const Moves& game_record, const Game_State& game_state, const int target) {
	std::array<bool, 38> minogashi_ar_flag;
	std::fill(minogashi_ar_flag.begin(), minogashi_ar_flag.end(), false);
	if (game_state.player_state[target].reach_declared) {
		for (int i = game_record.size() - 1; 0 <= i; i--) {
			if (game_record[i]["type"] == "reach" && game_record[i]["actor"].int_value() == target) {
				break;
			} else if (game_record[i]["type"] == "dahai") {
				minogashi_ar_flag[haikind(hai_str_to_int(game_record[i]["pai"].string_value()))] = true;
			}
		}
	}
	return minogashi_ar_flag;
}

std::array<bool, 38> get_sute_before_reach_flag(const Kawa& kawa) {
	std::array<bool, 38> sute_before_reach_flag;
	std::fill(sute_before_reach_flag.begin(), sute_before_reach_flag.end(), false);

	for (int i = 0; i < kawa.size(); i++) {
		sute_before_reach_flag[kawa[i].hai] = true;
		// to do 赤牌を切った時に、赤でないものをtrueにする必要があるか検証。
		if (kawa[i].is_reach) {
			break;
		}
	}
	return sute_before_reach_flag;
}

int count_tedashi_num(const Kawa& kawa) {
	int tedashi_num = 0;
	for (int i = 0; i < kawa.size(); i++) {
		if (!kawa[i].tsumogiri) {
			tedashi_num++;
		}
	}
	return tedashi_num;
}

std::array<std::array<bool, 38>, 38> get_tas_flag(const Game_State& game_state, const int target) {
	std::array<std::array<bool, 38>, 38> tas_flag;
	std::array<bool, 38> sute_flag;
	// 最終手出し、またはそれより前にhai1を打牌していた場合、tas_flag[hai1][0] = 1;
	// hai1 を打牌した後に、hai2を手出しした場合、tas_flag[hai1][hai2] = 1;
	for(int hai1=0;hai1<38;hai1++){
		sute_flag[hai1] = false;
		for(int hai2=0;hai2<38;hai2++){
			tas_flag[hai1][hai2] = false;
		}
	}

	for (const auto& sutehai : game_state.player_state[target].kawa) {
		sute_flag[haikind(sutehai.hai)] = true;
		if (!sutehai.tsumogiri) {
			for (int hai = 0; hai < 38; hai++) {
				if (tas_flag[hai][0]) {
					tas_flag[hai][haikind(sutehai.hai)] = true;
				} else if (sute_flag[hai]) {
					tas_flag[hai][0] = true;
					if (hai != haikind(sutehai.hai)) {
						tas_flag[hai][haikind(sutehai.hai)] = true;
					}
				}
			}
		}
	}
	return tas_flag;
}

std::array<std::array<std::array<bool, 38>, 7>, 3> get_tas_ryoukei_flag(const Game_State& game_state, const int target) {
	const std::array<std::array<bool, 38>, 38> tas_flag = get_tas_flag(game_state, target);
	std::array<std::array<std::array<bool, 38>, 7>, 3> tas_ryoukei_flag;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 7; i++) {
			for (int hai = 0; hai < 38; hai++) {
				tas_ryoukei_flag[j][i][hai] = false;
			}
		}
	}
	// j,iで指定される両面ターツの牌を打牌した後にhaiを手出しで捨てた場合 tas_ryoukei_flag[j][i][hai] = true;
	for (int j = 0; j < 3; j++) {
		for (int i = 1; i <= 6; i++) {
			if (tas_flag[10*j+i+1][0]) {
				tas_ryoukei_flag[j][i][0] = true;
				for (int hai = 1; hai < 38; hai++) {
					if (tas_flag[10*j+i+1][hai]) {
						tas_ryoukei_flag[j][i][hai] = true;
					}
				}
			}
			if (tas_flag[10*j+i+2][0]) {
				tas_ryoukei_flag[j][i][0] = true;
				for (int hai = 1; hai < 38; hai++) {
					if (tas_flag[10*j+i+2][hai]) {
						tas_ryoukei_flag[j][i][hai] = true;
					}
				}
			}
		}
	}
	return tas_ryoukei_flag;
}

std::array<std::array<std::array<bool, 38>, 9>, 3> get_tas_kanchan_flag(const Game_State& game_state, const int target) {
	const std::array<std::array<bool, 38>, 38> tas_flag = get_tas_flag(game_state, target);
	std::array<std::array<std::array<bool, 38>, 9>, 3> tas_kanchan_flag;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 9; i++) {
			for (int hai = 0; hai < 38; hai++) {
				tas_kanchan_flag[j][i][hai] = false;
			}
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int i = 2;i <= 8; i++) {
			if (tas_flag[10*j+i-1][0]) {
				tas_kanchan_flag[j][i][0] = true;
				for (int hai = 1; hai < 38; hai++) {
					if (tas_flag[10*j+i-1][hai]) {
						tas_kanchan_flag[j][i][hai] = true;
					}
				}
			}
			if (tas_flag[10*j+i+1][0]) {
				tas_kanchan_flag[j][i][0] = true;
				for (int hai = 1; hai < 38; hai++) {
					if (tas_flag[10*j+i+1][hai]) {
						tas_kanchan_flag[j][i][hai] = true;
					}
				}
			}
		}
	}
	return tas_kanchan_flag;
}

std::array<std::array<std::array<bool, 38>, 2>, 3> get_tas_penchan_flag(const Game_State& game_state, const int target) {
	const std::array<std::array<bool, 38>, 38> tas_flag = get_tas_flag(game_state, target);
	std::array<std::array<std::array<bool, 38>, 2>, 3> tas_penchan_flag;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 2; i++) {
			for (int hai = 0; hai < 38; hai++) {
				tas_penchan_flag[j][i][hai] = false;
			}
		}
	}
	for (int j = 0; j < 3; j++) {
		if (tas_flag[10*j+1][0]) {
			tas_penchan_flag[j][0][0] = true;
			for (int hai = 1; hai < 38; hai++) {
				if (tas_flag[10*j+1][hai]) {
					tas_penchan_flag[j][0][hai] = true;
				}
			}
		}
		if (tas_flag[10*j+2][0]) {
			tas_penchan_flag[j][0][0] = true;
			for (int hai = 1; hai < 38; hai++) {
				if (tas_flag[10*j+2][hai]) {
					tas_penchan_flag[j][0][hai] = true;
				}
			}
		}

		if (tas_flag[10*j+8][0]) {
			tas_penchan_flag[j][0][1] = true;
			for (int hai = 1; hai < 38; hai++) {
				if (tas_flag[10*j+8][hai]) {
					tas_penchan_flag[j][1][hai] = true;
				}
			}
		}
		if (tas_flag[10*j+9][0]) {
			tas_penchan_flag[j][0][1] = true;
			for (int hai = 1; hai < 38; hai++) {
				if (tas_flag[10*j+9][hai]) {
					tas_penchan_flag[j][1][hai] = true;
				}
			}
		}
	}
	return tas_penchan_flag;
}

void han_prob_shift(std::array<float, 14>& han_prob, const int shift_num) {
	std::array<float, 14> han_prob_tmp = {};
	for (int han = 0; han <= 13; han++) {
		han_prob_tmp[std::min(han + shift_num, 13)] += han_prob[han];
	}
	for (int han = 0; han < 14; han++) {
		han_prob[han] = han_prob_tmp[han];
	}
}

void hanfu_prob_han_shift(std::array<std::array<float, 12>, 14>& hanfu_prob, const int shift_num){
	std::array<std::array<float, 12>, 14> hanfu_prob_tmp;
	for (int han = 0; han < 14; han++) {
		for (int fu = 0; fu < 12; fu++) {
			hanfu_prob_tmp[han][fu] = 0.0;
		}
	}

	for (int han = 0; han < 14; han++) {
		for (int fu = 0; fu < 12; fu++) {
			hanfu_prob_tmp[std::min(han+shift_num,13)][fu] += hanfu_prob[han][fu];
		}
	}
	for (int han = 0; han < 14; han++) {
		for (int fu = 0; fu < 12; fu++) {
			hanfu_prob[han][fu] = hanfu_prob_tmp[han][fu];
		}
	}
}

void hanfu_prob_han_shift_with_prob(std::array<std::array<float, 12>, 14>& hanfu_prob, const std::array<float, 14>& shift_prob) {
	std::array<std::array<std::array<float, 12>, 14>, 14> prob_tmp;
	for (int sn = 0; sn < 14; sn++) {
		for (int han = 0; han < 14; han++) {
			for (int fu = 0; fu < 12; fu++) {
				prob_tmp[sn][han][fu] = hanfu_prob[han][fu];
			}
		}
		hanfu_prob_han_shift(prob_tmp[sn], sn);
	}
	for (int han = 0; han < 14; han++) {
		for (int fu = 0; fu < 12; fu++) {
			hanfu_prob[han][fu] = 0.0;
		}
	}
	for (int sn = 0; sn < 14; sn++) {
		for (int han = 0; han < 14; han++) {
			for (int fu = 0; fu < 12; fu++) {
				hanfu_prob[han][fu] += shift_prob[sn]*prob_tmp[sn][han][fu];
			}
		}
	}
}
