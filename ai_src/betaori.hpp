#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "tehai_ana.hpp"

double pow_double(const double x, const int y);

class Hai_Risk{
	public:
	Hai_Risk();
	bool operator<(const Hai_Risk& rhs) const;

	int hai;
	int hai_num;
	double houjuu_prob;
	double houjuu_value;
	double risk_coeff;

	void set_value_prob(int hai_in, int hai_num_in, double houjuu_prob_in, double houjuu_decay);
	void set_value_exp(int hai_in, int hai_num_in, double houjuu_prob_in, double houjuu_value_in, double other_value_in, double houjuu_decay);
};

class Betaori {
	public:
	Betaori();
	Hai_Array tehai;
	double houjuu_decay;
	double other_value, noten_ryuukyoku_value;
	std::array<double, 38> houjuu_hai_prob, houjuu_hai_value;
	
	std::vector<Hai_Risk> hai_risk;
	double betaori_exp, betaori_houjuu_prob;

	void set_tehai(const Hai_Array& tehai_in);
	void set_tehai_ta(const Tehai_Analyzer_Basic& tehai_analyzer);
	void set_condition(const std::array<float, 38>& houjuu_hai_prob_in, const std::array<float, 38>& houjuu_hai_value_in, const double other_value_in, const double noten_ryuukyoku_value_in);

	void set_betaori_value(const int ori_num);
	void modify_betaori_value_with_ryuukyoku_ako(const int fuuro_num, const float w[3], const float ryuukyoku_prob);
};

Betaori cal_betaori(
	const Hai_Array& tehai_in, const std::array<float, 38>& houjuu_hai_prob_in, const std::array<float, 38>& houjuu_hai_value_in,
	const double other_value_in, const double noten_ryuukyoku_value_in, const int ori_num
);
