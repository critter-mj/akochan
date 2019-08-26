#pragma once

#include "../share/include.hpp"
#include <boost/algorithm/combination.hpp>

#include "../share/calc_shanten.hpp"
#include "tehai_state2.hpp"
#include "yaku_pattern.hpp"
#include "tenpai_est.hpp"

void push_back_all(std::vector<int>& v1, const std::vector<int>& v2);

class Tehai_Pattern_Titoi{
	public:
	Tehai_Pattern_Titoi();
	Tehai_Pattern_Titoi(std::vector<int> h, std::vector<int> k1, std::vector<int> k2);
	void reset_all();

	int head[7][2];
	std::vector<int> remain;
	int shanten_num;
	int head_num;
	int k1_num;
	std::vector< std::vector<int> > hai_in_pattern;

	void cal_shanten();
	void cal_hai_in_pattern();

	void out_info();
};

class Tehai_Pattern{
	public:
	Tehai_Pattern(int pid);
	//Tehai_Pattern(int pid, std::vector<int> h, std::vector<int> m, std::vector<int> t, std::vector<int> k1, std::vector<int> k2, int fuuro[4][6], int fuuro_num);
	Tehai_Pattern(int pid, const std::vector<int>& h, const std::vector<int>& m, const std::vector<int>& t, const std::vector<int>& k1, const std::vector<int>& k2, const Tehai_State2& tehai_state, int fuuro_num);
	void reset_all();

	int my_pid;
	int mentu[4][3];
	int head[2];
	std::vector<int> remain;
	int shanten_num;
	std::vector< std::vector<int> > hai_in_block[5];
	std::vector< std::vector<int> > hai_in_pattern;

	void cal_shanten();
	void cal_hai_in_block();
	int is_in_remain(std::vector<int> test);
	int check_hai_in_error(const Hai_Array& tehai_kind, const Fuuro_Vector& fuuro_kind, const int i0, const int i1, const int i2, const int i3, const int i4);
	void cal_hai_in_pattern(const Hai_Array& tehai_kind, const Fuuro_Vector& fuuro_kind);
	int yaku_check(int nip);
	double cal_priority(const Game_State& game_state, const Hai_Array& hai_visible_kind, int nip);

	void out_info();
};
