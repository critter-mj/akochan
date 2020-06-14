#include "agari_ten.hpp"

int ron_agari(const int han, const int fu, const bool oya_flag){
	if (han == 0) {
		return 0;
	} else {
		if(oya_flag){
			if(han >= 13){
				return 48000;
			}else if(han >= 11){
				return 36000;
			}else if(han >= 8){
				return 24000;
			}else if(han >= 6){
				return 18000;
			}else if(han >= 5){
				return 12000;
			}else if(han == 4){
				if(fu==25){
					return 9600;
				}else if(fu<=30){
					return 11600;
				}else{
					return 12000;
				}
			}else if(han == 3){
				if(fu==25){
					return 4800;
				}else if(fu<=30){
					return 5800;
				}else if(fu<=40){
					return 7700;
				}else if(fu<=50){
					return 9600;
				}else if(fu<=60){
					return 11600;
				}else{
					return 12000;
				}
			}else if(han == 2){
				if(fu==25){
					return 2400;
				}else if(fu<=30){
					return 2900;
				}else if(fu<=40){
					return 3900;
				}else if(fu<=50){
					return 4800;
				}else if(fu<=60){
					return 5800;
				}else if(fu<=70){
					return 6800;
				}else if(fu<=80){
					return 7700;
				}else if(fu<=90){
					return 8700;
				}else if(fu<=100){
					return 9600;
				}else if(fu<=110){
					return 10600;
				}
			}else if(han == 1){
				if(fu<=30){
					return 1500;
				}else if(fu<=40){
					return 2000;
				}else if(fu<=50){
					return 2400;
				}else if(fu<=60){
					return 2900;
				}else if(fu<=70){
					return 3400;
				}else if(fu<=80){
					return 3900;
				}else if(fu<=90){
					return 4400;
				}else if(fu<=100){
					return 4800;
				}else if(fu<=110){
					return 5300;
				}
			}
		}else{
			if(han >= 13){
				return 32000;
			}else if(han >= 11){
				return 24000;
			}else if(han >= 8){
				return 16000;
			}else if(han >= 6){
				return 12000;
			}else if(han >= 5){
				return 8000;
			}else if(han == 4){
				if(fu==25){
					return 6400;
				}else if(fu<=30){
					return 7700;
				}else{
					return 8000;
				}
			}else if(han == 3){
				if(fu==25){
					return 3200;
				}else if(fu<=30){
					return 3900;
				}else if(fu<=40){
					return 5200;
				}else if(fu<=50){
					return 6400;
				}else if(fu<=60){
					return 7700;
				}else{
					return 8000;
				}
			}else if(han == 2){
				if(fu==25){
					return 1600;
				}else if(fu<=30){
					return 2000;
				}else if(fu<=40){
					return 2600;
				}else if(fu<=50){
					return 3200;
				}else if(fu<=60){
					return 3900;
				}else if(fu<=70){
					return 4500;
				}else if(fu<=80){
					return 5200;
				}else if(fu<=90){
					return 5800;
				}else if(fu<=100){
					return 6400;
				}else if(fu<=110){
					return 7100;
				}
			}else if(han == 1){
				if(fu<=30){
					return 1000;
				}else if(fu<=40){
					return 1300;
				}else if(fu<=50){
					return 1600;
				}else if(fu<=60){
					return 2000;
				}else if(fu<=70){
					return 2300;
				}else if(fu<=80){
					return 2600;
				}else if(fu<=90){
					return 2900;
				}else if(fu<=100){
					return 3200;
				}else if(fu<=110){
					return 3600;
				}
			}
		}
	}
	assert_with_out(false, "ron_agari error");
	return 0;
}

int tsumo_agari_loss(const int han, const int fu, const bool oya_flag){
	if (han == 0) {
		return 0;
	} else {
		if (oya_flag) {
			if(han >= 13){
				return 16000;
			}else if(han >= 11){
				return 12000;
			}else if(han >= 8){
				return 8000;
			}else if(han >= 6){
				return 6000;
			}else if(han >= 5){
				return 4000;
			}else if(han == 4){
				if(fu==20){
					return 2600;
				}else if(fu==25){
					return 3200;
				}else if(fu<=30){
					return 3900;
				}else{
					return 4000;
				}
			}else if(han == 3){
				if(fu==20){
					return 1300;
				}else if(fu==25){
					return 1600;
				}else if(fu<=30){
					return 2000;
				}else if(fu<=40){
					return 2600;
				}else if(fu<=50){
					return 3200;
				}else if(fu<=60){
					return 3900;
				}else{
					return 4000;
				}
			}else if(han == 2){
				if(fu==20){
					return 700;
				}else if(fu==25){
					return 800;
				}else if(fu<=30){
					return 1000;
				}else if(fu<=40){
					return 1300;
				}else if(fu<=50){
					return 1600;
				}else if(fu<=60){
					return 2000;
				}else if(fu<=70){
					return 2300;
				}else if(fu<=80){
					return 2600;
				}else if(fu<=90){
					return 2900;
				}else if(fu<=100){
					return 3200;
				}else if(fu<=110){
					return 3600;
				}
			}else if(han == 1){
				if(fu<=30){
					return 500;
				}else if(fu<=40){
					return 700;
				}else if(fu<=50){
					return 800;
				}else if(fu<=60){
					return 1000;
				}else if(fu<=70){
					return 1200;
				}else if(fu<=80){
					return 1300;
				}else if(fu<=90){
					return 1500;
				}else if(fu<=100){
					return 1600;
				}else if(fu<=110){
					return 1800;
				}
			}
		}else{
			if(han >= 13){
				return 8000;
			}else if(han >= 11){
				return 6000;
			}else if(han >= 8){
				return 4000;
			}else if(han >= 6){
				return 3000;
			}else if(han >= 5){
				return 2000;
			}else if(han == 4){
				if(fu==20){
					return 1300;
				}else if(fu==25){
					return 1600;
				}else if(fu<=30){
					return 2000;
				}else{
					return 2000;
				}
			}else if(han == 3){
				if(fu==20){
					return 700;
				}else if(fu==25){
					return 800;
				}else if(fu<=30){
					return 1000;
				}else if(fu<=40){
					return 1300;
				}else if(fu<=50){
					return 1600;
				}else if(fu<=60){
					return 2000;
				}else{
					return 2000;
				}
			}else if(han == 2){
				if(fu==20){
					return 400;
				}else if(fu<=30){
					return 500;
				}else if(fu<=40){
					return 700;
				}else if(fu<=50){
					return 800;
				}else if(fu<=60){
					return 1000;
				}else if(fu<=70){
					return 1200;
				}else if(fu<=80){
					return 1300;
				}else if(fu<=90){
					return 1500;
				}else if(fu<=100){
					return 1600;
				}else if(fu<=110){
					return 1800;
				}
			}else if(han == 1){
				if(fu<=30){
					return 300;
				}else if(fu<=40){
					return 400;
				}else if(fu<=50){
					return 400;
				}else if(fu<=60){
					return 500;
				}else if(fu<=70){
					return 600;
				}else if(fu<=80){
					return 700;
				}else if(fu<=90){
					return 800;
				}else if(fu<=100){
					return 800;
				}else{
					return 0; // あり得ないアガリ。
				}
			}
		}
	}
	assert_with_out(false, "tsumo_agari_loss error");
	return 0;
}

int tsumo_agari(const int han, const int fu, const bool oya_flag){
	if (oya_flag) {
		return tsumo_agari_loss(han, fu, oya_flag)*3;
	} else {
		return tsumo_agari_loss(han, fu, true) + tsumo_agari_loss(han, fu, false)*2;
	}
}

std::array<int, 4> ten_move_hora(const int who, const int from_who, const int han) {
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

std::array<int, 4> ten_move_ryukyoku(const std::array<bool, 4>& tenpai_flag) {
	int tenpai_num = 0;
    std::array<int, 4> ten_move;
	for(int pid = 0; pid < 4; pid++){
		ten_move[pid] = 0;
        tenpai_num += tenpai_flag[pid] ? 1 : 0;
	}

	if (tenpai_num == 1) {
		for (int pid = 0; pid < 4; pid++) {
			ten_move[pid] = tenpai_flag[pid] ? 3 : -1;
		}
	} else if (tenpai_num == 2) {
		for (int pid = 0; pid < 4; pid++) {
			ten_move[pid] = tenpai_flag[pid] ? 2 : -2;
		}
	} else if (tenpai_num == 3) {
		for (int pid = 0; pid < 4; pid++) {
			ten_move[pid] = tenpai_flag[pid] ? 1 : -3;
		}
	}
    return ten_move;
}
