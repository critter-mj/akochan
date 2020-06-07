#include "agari_ten_chn.hpp"

std::array<int, 4> ten_move_hora_chn(const int who, const int from_who, const int han) {
	std::array<int, 4> ten_move;
	for (int pid = 0; pid < 4; pid++) {
		ten_move[pid] = 0;
	}
	if (who == from_who) {
		for (int pid = 0; pid < 4; pid++) {
			if (pid == who) { 
				ten_move[pid] += han * 3;
			} else {
				ten_move[pid] -= han;
			}
		}
	} else {
		ten_move[who] += han;
		ten_move[from_who] -= han; 
	}
	return ten_move;
}
