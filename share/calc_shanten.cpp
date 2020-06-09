#include "calc_shanten.hpp"

Hai_Array using_hai_array(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    Hai_Array using_array = tehai;
    for (int i = 0; i < fuuro.size(); i++) {
        if (fuuro[i].type != FT_ANKAN) {
            using_array[fuuro[i].hai]++;
        }
        for (int j = 0; j < fuuro[i].consumed.size(); j++) {
            using_array[fuuro[i].consumed[j]]++;
        }
    }
    return using_array;
}

void tenpai_check(
	const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
) {
	int rest = 0;
	for(int hai = 0; hai < 38; hai++) {
		rest = rest + tehai_tmp[hai];
	}

	if (rest == 0) {
		//入力がすでにメンツ手アガリ形の場合。とりあえず特に何もしない。
	} else if (rest == 1) {
		for (int hai = 0; hai < 38;hai++) {
			if (tehai_tmp[hai] == 1 && using_array[hai] != 4) {
                tenpai_info.mentu_shanten_num = 0;
				tenpai_info.agari_vec.push_back(calc_agari_detail(
					bakaze_hai, jikaze_hai,
					tehai, tehai_tate_cut, tehai_tmp, fuuro,
    				hai, MT_TANKI, false
				));
			}
		}
	} else if (rest == 2) {
		for (int hai = 0; hai < 38; hai++) {
			if (tehai_tmp[hai] == 2 && using_array[hai] != 4) {
				tenpai_info.mentu_shanten_num = 0;
				tenpai_info.agari_vec.push_back(calc_agari_detail(
					bakaze_hai, jikaze_hai,
					tehai, tehai_tate_cut, tehai_tmp, fuuro,
    				hai, MT_SHABO, false
				));
			}
		}
		for (int hai = 0; hai < 30; hai++) {
			if (tehai_tmp[hai] == 1 && tehai_tmp[hai+1] == 1) {
				if (hai % 10 == 1) {
					if (using_array[hai+2] != 4) {
						tenpai_info.mentu_shanten_num = 0;
						tenpai_info.agari_vec.push_back(calc_agari_detail(
							bakaze_hai, jikaze_hai,
							tehai, tehai_tate_cut, tehai_tmp, fuuro,
							hai + 2, MT_PENCHAN, false
						));
					}
				} else if (hai % 10 == 8) {
					if (using_array[hai-1] != 4) {
						tenpai_info.mentu_shanten_num = 0;
						tenpai_info.agari_vec.push_back(calc_agari_detail(
							bakaze_hai, jikaze_hai,
							tehai, tehai_tate_cut, tehai_tmp, fuuro,
							hai - 1, MT_PENCHAN, false
						));
					}
				} else {
					if (using_array[hai-1] != 4) {
						tenpai_info.mentu_shanten_num = 0;
						tenpai_info.agari_vec.push_back(calc_agari_detail(
							bakaze_hai, jikaze_hai,
							tehai, tehai_tate_cut, tehai_tmp, fuuro,
							hai - 1, MT_RYANMEN, false
						));
					}
					if (using_array[hai+2] != 4) {
						tenpai_info.mentu_shanten_num = 0;
						tenpai_info.agari_vec.push_back(calc_agari_detail(
							bakaze_hai, jikaze_hai,
							tehai, tehai_tate_cut, tehai_tmp, fuuro,
							hai + 2, MT_RYANMEN, false
						));
					}
				}
			}
			if (tehai_tmp[hai] == 1 && tehai_tmp[hai+2] == 1) {
				if (hai % 10 != 9) {
					if (using_array[hai+1] != 4) {
						tenpai_info.mentu_shanten_num = 0;
						tenpai_info.agari_vec.push_back(calc_agari_detail(
							bakaze_hai, jikaze_hai,
							tehai, tehai_tate_cut, tehai_tmp, fuuro,
							hai + 1, MT_KANCHAN, false
						));
					}
				}
			}
		}
	}
}

void analyze_tatu(
	const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
) {
	// ここから明らかに無駄なパターンの削除。（tehai_tmpにメンツが含まれるものは以後の処理を行わないようにする。）
	for(int hai=0;hai<38;hai++){
		if(tehai_tmp[hai]>=3){
			return;
		}
	}

    for (int j = 0; j < 3;j++) {
        for (int i = 1; i <= 7; i++) {
            if (tehai_tmp[10*j+i] > 0 && tehai_tmp[10*j+i+1] > 0 && tehai_tmp[10*j+i+2] > 0) {
                return;
            }
        }
    }
	//ここまで明らかに無駄なパターンの削除

	tenpai_check(
		bakaze_hai, jikaze_hai,
        using_array, tehai, tehai_tate_cut, tehai_tmp,
        fuuro, tenpai_info
    );
}

void cut_syuntu(
	const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const int start, const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
) {
	for (int hai = start; hai < 30; hai++) {
		if (tehai_tmp[hai] && tehai_tmp[hai+1] && tehai_tmp[hai+2]) {
			tehai_tmp[hai] = tehai_tmp[hai]-1;
			tehai_tmp[hai+1] = tehai_tmp[hai+1]-1;
			tehai_tmp[hai+2] = tehai_tmp[hai+2]-1;
			cut_syuntu(
				bakaze_hai, jikaze_hai,
                using_array, tehai, tehai_tate_cut, tehai_tmp,
                hai, fuuro, tenpai_info
            );
			tehai_tmp[hai] = tehai_tmp[hai]+1;
			tehai_tmp[hai+1] = tehai_tmp[hai+1]+1;
			tehai_tmp[hai+2] = tehai_tmp[hai+2]+1;
		}
	}
	analyze_tatu(
		bakaze_hai, jikaze_hai,
        using_array, tehai, tehai_tate_cut, tehai_tmp,
        fuuro, tenpai_info
    );
}

void cut_kotu(
	const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, Hai_Array& tehai_tmp,
    const int start, const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
) {
	for (int hai = start; hai < 38; hai++) {
		if (tehai_tmp[hai] >= 3) {
			tehai_tmp[hai] = tehai_tmp[hai] - 3;
			cut_kotu(
				bakaze_hai, jikaze_hai,
                using_array, tehai, tehai_tmp,
                hai, fuuro, tenpai_info
            );
			tehai_tmp[hai] = tehai_tmp[hai] + 3;
		}
	}

	Hai_Array tehai_tate_cut = tehai_tmp;
	cut_syuntu(
		bakaze_hai, jikaze_hai,
        using_array, tehai, tehai_tate_cut, tehai_tmp,
        0, fuuro, tenpai_info
    );
}

void titoi_shanten(const int bakaze_hai, const int jikaze_hai, const Hai_Array& tehai, const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info) {
	int head_num = 0;
	int koritu_num = 0;
	for (int hai = 0; hai < 38;hai++) {
		if (tehai[hai] >= 2) {
			head_num++;
		} else if (tehai[hai] > 0) {
			koritu_num++;
		}
	}
	if (head_num == 7) {
		//入力がチートイアガリの場合。とりあえず何もしない。
	} else if (head_num == 6) {
		for (int hai = 0; hai < 38;hai++) {
			if (tehai[hai] == 1) {
                tenpai_info.titoi_shanten_num = 0;
				tenpai_info.agari_vec.push_back(calc_agari_detail(
    				bakaze_hai, jikaze_hai,
    				tehai, tehai, tehai, fuuro,
    				hai, MT_TANKI, true
				));
			}
		}
	}

	int tmp = 13 - 2 * head_num - std::min(7 - head_num, koritu_num);
    tenpai_info.titoi_shanten_num = std::min(tmp, tenpai_info.titoi_shanten_num);
}

void analyze_tehai(
	const int bakaze_hai, const int jikaze_hai,
	const Hai_Array using_array, const Hai_Array tehai, const Fuuro_Vector fuuro, Tenpai_Info& tenpai_info
) {
    // アガリ牌が手牌で既に4枚使っているような手はテンパイではないため、手牌で各牌種を何枚使っているか配列 using_array に保存しておく。
	// この関数の引数は、Hai_Array Fuuro_Vectorの牌は赤が無いものである必要がある。
	Hai_Array tehai_tmp = tehai;
	for (int hai = 0; hai < 38; hai++) {
		if (tehai[hai] >= 2) {
			tehai_tmp[hai] = tehai_tmp[hai] - 2;
			cut_kotu(
				bakaze_hai, jikaze_hai,
                using_array, tehai, tehai_tmp,
                0, fuuro, tenpai_info
            );
			tehai_tmp[hai] = tehai_tmp[hai] + 2;
		}
	}
	cut_kotu(
		bakaze_hai, jikaze_hai,
        using_array, tehai, tehai_tmp,
        0, fuuro, tenpai_info
    );

	if(fuuro.size() == 0) {
		titoi_shanten(bakaze_hai, jikaze_hai, tehai, fuuro, tenpai_info);
	}
}

Tenpai_Info cal_tenpai_info(const int bakaze, const int jikaze, const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
	const int bakaze_hai = 31 + bakaze;
	const int jikaze_hai = 31 + jikaze;
	const Hai_Array tehai_kind = haikind(tehai);
	const Fuuro_Vector fuuro_kind = haikind(fuuro);
	Tenpai_Info tenpai_info;

	analyze_tehai(bakaze_hai, jikaze_hai, using_hai_array(tehai_kind, fuuro_kind), tehai_kind, fuuro_kind, tenpai_info);
    return tenpai_info;
}

int count_yaochu_kind(const Hai_Array& tehai) {
	int yaochu_kind_num = 0;
	for (int c = 0; c < 3; c++) {
		if (tehai[c*10 + 1] > 0) {
			yaochu_kind_num++;
		}
		if (tehai[c*10 + 9] > 0) {
			yaochu_kind_num++;
		}
	}
	for (int hai = 31; hai < 38; hai++) {
		if (tehai[hai] > 0) {
			yaochu_kind_num++;
		}
	}
	return yaochu_kind_num;
}