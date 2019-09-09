#include "game_rule.hpp"

Game_Rule::Game_Rule(){
	set_tenhou_hanchan();
}


void Game_Rule::set_tenhou_hanchan(){
	jun_pt[0] = 90;
	jun_pt[1] = 45;
	jun_pt[2] = 0;
	jun_pt[3] = -135;
	aka_flag = 1;
	kyoku_first = 0;
	hai_num_all = 136;
	for (int pid = 0; pid < 4; pid++) {
		ten_init[pid] = 25000;
	}
}

void Game_Rule::set_mjfg(){
	jun_pt[0] = 90;
	jun_pt[1] = 30;
	jun_pt[2] = -30;
	jun_pt[3] = -90;
	aka_flag = 1;
	kyoku_first = 4;
	hai_num_all = 136;
	for (int pid = 0; pid < 4; pid++) {
		ten_init[pid] = 25000;
	}
}

void Game_Rule::set_self_match(){
	jun_pt[0] = 90;
	jun_pt[1] = 30;
	jun_pt[2] = -30;
	jun_pt[3] = -90;
	aka_flag = 1;
	kyoku_first = 4;
	hai_num_all = 136;
	for (int pid = 0; pid < 4; pid++) {
		ten_init[pid] = 25000;
	}
}

int Game_Rule::next_player(const int pid, const int arg) const {
	return (pid + arg)%4;
}