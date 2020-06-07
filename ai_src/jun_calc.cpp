#include "jun_calc.hpp"

std::array<std::array<float, 4>, 4> calc_jun_prob_end(const std::array<int, 4>& ten, const int oya_first) {
    std::array<std::array<float, 4>, 4> jun_prob;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			jun_prob[i][j] = 0.0;
		}
	}
	std::vector<Player_Result> results;
	for (int j = 0; j < 4; j++) {
		results.push_back(Player_Result(j, ten[j], (4 + j - oya_first)%4 ));
	}
	std::sort(results.begin(), results.end());
	for (int j = 0; j < 4; j++) {
		jun_prob[results[j].pid][j] = 1.0;
	}
    return jun_prob;
}

std::array<int, 16> score_list_to_4onehot(const std::array<int, 4>& score) {
    std::vector<Player_Result> results;
	for (int i = 0; i < 4; i++) {
		results.push_back(Player_Result(i, score[i], i));
	}
	std::sort(results.begin(), results.end());
    
    std::array<int, 16> onehot;
    std::fill(onehot.begin(), onehot.end(), 0);
    for (int j = 0; j < 4; j++) {
        onehot[4*j + results[j].pid] = 1;
    }
    return onehot;
}

std::array<float, 24> infer_game_result_prob_ako(const std::array<int, 4>& score, const int kyoku_idx) {
	float w[96];
	const std::string file_name = kyoku_idx < 8 ?
		"params/rank_prob/ako/para" + std::to_string(kyoku_idx) + "_9000.txt" :
		"params/rank_prob/ako/para" + std::to_string(kyoku_idx) + "_nn.txt";

	read_parameters(w, 96, file_name);
	float x[4];
	x[0] = 1.0;
	for(int i=1;i<=3;i++){
		x[i] = ((float)(score[0] - score[i]))/10000.0;
	}
	float pk[24];
	MC_logistic(w, x, pk, 4, 24);

	std::array<float, 24> output;
	for (int i = 0; i < 24; i++) {
		output[i] = pk[i];
	}
	return output;
}

// オーラスで親がアガリまたはテンパイ流局した場合にゲームが終わるか判定
bool chan_end_oya(const std::array<int, 4>& ten, const int oyaid){
	if(ten[oyaid]>=30000){
		for(int pid=0;pid<4;pid++){
			if(pid==oyaid){
				continue;
			}else if(ten[pid]>=ten[oyaid]){
				return false;
			}
		}
		return true;
	}else{
		return false;
	}
}

// 西４で親がアガリまたはテンパイ流局した場合にゲームが終わるか判定
bool chan_end_oya_w4(const std::array<int, 4>& ten, const int oyaid) {
	for (int pid = 0; pid < 4; pid ++) {
		if (pid == oyaid) {
			continue;
		} else if (ten[pid] >= ten[oyaid]) {
			return false;
		}
	}
	return true;
}

std::array<std::array<float, 4>, 4> calc_jun_prob(const int kyoku, const std::array<int,4>& ten, const int oya) {
	const int oya_first = (12 + oya - kyoku) % 4;
	return calc_jun_prob_end(ten, oya_first);
}
