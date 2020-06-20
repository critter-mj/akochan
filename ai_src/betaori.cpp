#include "betaori.hpp"

double pow_double(const double x, const int y) {
	double res = 1.0;
	for (int i = 0; i < y; i++) {
		res = res * x;
	}
	return res;
}

Hai_Risk::Hai_Risk(){
}

bool Hai_Risk::operator<(const Hai_Risk& rhs) const{
	if(risk_coeff<rhs.risk_coeff){
		return true;
	}else if(risk_coeff>rhs.risk_coeff){
		return false;
	}

	if(hai_num<rhs.hai_num){
		return true;
	}else{
		return false;
	}
}

void Hai_Risk::set_value_prob(int hai_in, int hai_num_in, double houjuu_prob_in, double houjuu_decay){
	hai = hai_in;
	hai_num = hai_num_in;
	houjuu_prob = houjuu_prob_in;
	double beta = 1.0 - pow_double(houjuu_decay, hai_num);
	risk_coeff = houjuu_prob/(1.0-houjuu_prob)/beta;
}

void Hai_Risk::set_value_exp(int hai_in, int hai_num_in, double houjuu_prob_in, double houjuu_value_in, double other_value, double houjuu_decay){
	hai = hai_in;
	hai_num = hai_num_in;
	houjuu_prob = houjuu_prob_in;
	houjuu_value = houjuu_value_in;
	double beta = 1.0 - pow_double(houjuu_decay, hai_num);
	risk_coeff = houjuu_prob*(other_value-houjuu_value)/(houjuu_prob+beta-houjuu_prob*beta);
}

Betaori::Betaori(){
	houjuu_decay = 0.9;
}

void Betaori::set_tehai(const Hai_Array& tehai_in){
	for (int hai = 0; hai < 38; hai++) {
		tehai[hai] = tehai_in[hai];
	}
	for (int c = 0; c < 3; c++) {
		if (tehai[10*c+5] > 0 && tehai[10*c+10] > 0) {
			tehai[10*c+5] += tehai[10*c+10];
			tehai[10*c+10] = 0;
		}
	}
}

void Betaori::set_tehai_ta(const Tehai_Analyzer_Basic& tehai_analyzer) {
	tehai[0] = 0;
	for(int hai=1;hai<38;hai++){
		tehai[hai] = tehai_analyzer.count_hai(hai);
	}
	for(int c=0;c<3;c++){
		if(tehai[10*c+5]>0 && tehai[10*c+10]>0){
			tehai[10*c+5] += tehai[10*c+10];
			tehai[10*c+10] = 0;
		}
	}
}

void Betaori::set_condition(const std::array<float, 38>& houjuu_hai_prob_in, const std::array<float, 38>& houjuu_hai_value_in, const double other_value_in, const double noten_ryuukyoku_value_in) {
	other_value = other_value_in;
	noten_ryuukyoku_value = noten_ryuukyoku_value_in;
	for (int hai = 0; hai < 38; hai++) {
		houjuu_hai_prob[hai] = houjuu_hai_prob_in[hai];
		houjuu_hai_value[hai] = houjuu_hai_value_in[hai];
	}
}

void Betaori::set_betaori_value(const int ori_num){
	hai_risk.clear();
	Hai_Risk hai_risk_tmp;
	for (int hai = 1; hai < 38; hai++) {
		if (tehai[hai] > 0) {
			hai_risk_tmp.set_value_exp(hai, tehai[hai], houjuu_hai_prob[hai], houjuu_hai_value[hai], other_value, houjuu_decay);
			hai_risk.push_back(hai_risk_tmp);
		}
	}

	std::sort(hai_risk.begin(), hai_risk.end());
	int num = 0;
	betaori_houjuu_prob = 0.0;
	betaori_exp = 0.0;
	double coeff_tmp = 1.0;
	for (int n = 0; n < hai_risk.size(); n++) {
		if (num >= ori_num) {
			break;
		}
		betaori_houjuu_prob += coeff_tmp*houjuu_hai_prob[hai_risk[n].hai];
		betaori_exp += coeff_tmp*houjuu_hai_prob[hai_risk[n].hai]*houjuu_hai_value[hai_risk[n].hai];
		coeff_tmp = coeff_tmp*(1.0-houjuu_hai_prob[hai_risk[n].hai])*pow_double(houjuu_decay, tehai[hai_risk[n].hai]);
		num += hai_risk[n].hai_num;
	}
	betaori_exp += (1.0 - betaori_houjuu_prob) * other_value;
}

void Betaori::modify_betaori_value_with_ryuukyoku_ako(const int fuuro_num, const float w[3], const float ryuukyoku_prob){
	if(betaori_houjuu_prob>0.0){
		betaori_exp -= (1.0-betaori_houjuu_prob)*other_value;
		float x[3];
		x[0] = 1.0;
		x[1] = fuuro_num;
		x[2] = my_logit(betaori_houjuu_prob);
		float prob_mod = logistic(w, x, 3);
		betaori_exp *= prob_mod/betaori_houjuu_prob;
		betaori_houjuu_prob = prob_mod;
		//betaori_exp += (1.0-betaori_houjuu_prob)*other_value;
		betaori_exp += (1.0-betaori_houjuu_prob)*(ryuukyoku_prob*noten_ryuukyoku_value + (1.0-ryuukyoku_prob)*other_value);
	}else{
		betaori_exp = ryuukyoku_prob*noten_ryuukyoku_value + (1.0-ryuukyoku_prob)*other_value;
	}
}

Betaori cal_betaori(
	const Hai_Array& tehai_in, const std::array<float, 38>& houjuu_hai_prob_in, const std::array<float, 38>& houjuu_hai_value_in,
	const double other_value_in, const double noten_ryuukyoku_value_in, const int ori_num
) {
	Betaori betaori;
	betaori.set_tehai(tehai_in);
	betaori.set_condition(houjuu_hai_prob_in, houjuu_hai_value_in, other_value_in, noten_ryuukyoku_value_in);
	betaori.set_betaori_value(ori_num);
	return betaori;
}
