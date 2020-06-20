#include "tenpai_est.hpp"

extern const bool out_console;
extern const std::array<json11::Json, 4> tactics_json;

bool yakuhai_check_est (const int yakuhai, const Fuuro_Vector& fuuro, const int kotu[38], const int machi_hai, const Machi_Type machi_type) {
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if(fuuro[fn].consumed[0] == yakuhai) {
			return true;
		}
	}
	if (kotu[yakuhai] == 1) {
		return true;
	} else if(machi_type == MT_SHABO && machi_hai==yakuhai){
		return true;
	}else{
		return false;
	}
}

bool chinitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai) {
	bool flags[CT_JIHAI];
	for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
		flags[color] = true;
	}
	for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
		if (hai_color(machi_hai) != color){
			flags[color] = false;
			continue;
		}
		for (int fn = 0; fn < fuuro.size(); fn++) {
			if(hai_color(fuuro[fn].consumed[0]) != color) {
				flags[color] = false;
				break;
			}
		}
		if (flags[color]) {
			for (int hai = 0; hai < 38; hai++) {
				if ((head[hai]>0 || kotu[hai]>0 || (hai<30 && syuntu[hai]>0) ) && hai_color(hai) != color) {
					flags[color] = false;
					break;
				}
			}
		}
	}
	return flags[CT_MANZU] || flags[CT_PINZU] || flags[CT_SOZU];
}

bool honitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai) {
	bool flags[CT_JIHAI];
	for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
		flags[color] = true;
	}
	for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
		if (hai_color(machi_hai) != color && hai_color(machi_hai) != CT_JIHAI) {
			flags[color] = false;
			continue;
		}
		for (int fn = 0; fn < fuuro.size(); fn++) {
			if(hai_color(fuuro[fn].consumed[0]) != color && hai_color(fuuro[fn].consumed[0]) != CT_JIHAI) {
				flags[color] = false;
				break;
			}
		}
		if (flags[color]) {
			for (int hai = 0; hai < 30; hai++) {
				if ((head[hai]>0 || kotu[hai]>0 || (hai<30 && syuntu[hai]>0)) && hai_color(hai) != color) {
					flags[color] = false;
					break;
				}
			}
		}
	}
	return flags[CT_MANZU] || flags[CT_PINZU] || flags[CT_SOZU];
}

bool tanyao_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	for (int hc = 0; hc < 3; hc++) {
		if(head[hc*10+1]>0 || head[hc*10+9]>0 || kotu[hc*10+1]>0 || kotu[hc*10+9]>0 || syuntu[hc*10+1]>0 || syuntu[hc*10+7]>0){
			return false;
		}
	}
	for (int hai = 31; hai < 38; hai++) {
		if (head[hai] > 0 || kotu[hai] > 0) {
			return false;
		}
	}
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			if (hai_yaochuu(fuuro[fn].hai) != NT_CHUJAN || hai_yaochuu(fuuro[fn].consumed[0]) != NT_CHUJAN || hai_yaochuu(fuuro[fn].consumed[1]) != NT_CHUJAN) {
				return false;
			}
		} else {
			if (hai_yaochuu(fuuro[fn].consumed[0]) != NT_CHUJAN) {
				return false;
			}
		}
	}
	if (hai_yaochuu(machi_hai) != NT_CHUJAN) {
		return false;
	}
	if (machi_type == MT_PENCHAN) {
		return false;
	}
	if (machi_type == MT_KANCHAN && (machi_hai % 10 == 2 || machi_hai % 10 == 8)) {
		return false;
	}
	return true;
}

bool honrou_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	if ((machi_type != MT_SHABO && machi_type != MT_TANKI) || hai_yaochuu(machi_hai) == NT_CHUJAN) {
		return false;
	}
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if(hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN || hai_yaochuu(fuuro[fn].consumed[1]) == NT_CHUJAN) {
			return false;
		}
	}
	for (int hai = 0; hai < 30; hai++) {
		if (syuntu[hai] > 0) {
			return false;
		}
		if (hai_yaochuu(hai) == NT_CHUJAN && (head[hai] > 0 || kotu[hai] > 0)) {
			return false;
		}
	}
	return true;
}

bool suitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	if ((machi_type != MT_SHABO && machi_type != MT_TANKI) || hai_color(machi_hai) != CT_JIHAI) {
		return false;
	}
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].consumed[0] <= 30) {
			return false;
		}
	}
	for (int hai = 0; hai <= 30; hai++) {
		if (head[hai] != 0 || kotu[hai] != 0 || syuntu[hai] != 0) {
			return false;
		}
	}
	return true;
}

bool junchan_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			if (hai_yaochuu(fuuro[fn].hai) != NT_SUYAO && hai_yaochuu(fuuro[fn].consumed[0]) != NT_SUYAO && hai_yaochuu(fuuro[fn].consumed[1]) != NT_SUYAO) {
				return false;
			}
		} else if (hai_yaochuu(fuuro[fn].consumed[0]) != NT_SUYAO) {
			return false;
		}
	}

	for (int hai = 0; hai < 38; hai++) {
		if ((head[hai]>0 || kotu[hai]>0) && hai_yaochuu(hai) != NT_SUYAO) {
			return false;
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 2; i <= 6; i++) {
			if (syuntu[10*j+i] > 0) {
				return false;
			}
		}
	}

	if (hai_yaochuu(machi_hai) == NT_SUYAO || machi_type == MT_PENCHAN || (machi_type == MT_KANCHAN && (machi_hai % 10 == 2 || machi_hai % 10 == 8))) {
		return true;
	}
	return false;
}

bool chanta_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			if(hai_yaochuu(fuuro[fn].hai) == NT_CHUJAN && hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN && hai_yaochuu(fuuro[fn].consumed[1]) == NT_CHUJAN) {
				return false;
			}
		} else if (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) {
			return false;
		}
	}

	for (int hai = 0; hai < 38; hai++) {
		if ((head[hai]>0 || kotu[hai]>0) && hai_yaochuu(hai) == NT_CHUJAN) {
			return false;
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 2; i <= 6; i++) {
			if (syuntu[10*j+i] > 0) {
				return false;
			}
		}
	}

	if (hai_yaochuu(machi_hai) != NT_CHUJAN || machi_type == MT_PENCHAN || (machi_type == MT_KANCHAN && (machi_hai % 10 == 2 || machi_hai % 10 == 8))) {
		return true;
	}
	return false;
}

bool toitoi_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			return false;
		}
	}
	for (int hai = 0; hai < 30; hai++) {
		if (syuntu[hai] > 0) {
			return false;
		}
	}
	return machi_type == MT_SHABO || machi_type == MT_TANKI;
}

bool sanshikidoukou_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type) {
	int kotu_test[38];

    for (int hai = 0; hai < 38; hai++) {
        if (kotu[hai] > 0) {
            kotu_test[hai] = 1;
        } else {
            kotu_test[hai] = 0;
        }
    }

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type != FT_CHI) {
            kotu_test[fuuro[fn].consumed[0]]++;
        }
    }

    if (machi_type == MT_SHABO) {
        kotu_test[machi_hai]++;
    }

    for (int i = 1; i <= 9; i++) {
        if (kotu_test[i] == 1 && kotu_test[10+i] == 1 && kotu_test[20+i] == 1) {
            return true;
        }
    }
    return false;
}

bool sanshikidoujun_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id) {
	int syuntu_test[30];
	for (int hai = 0; hai < 30; hai++) {
		syuntu_test[hai] = syuntu[hai];
	}

	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			syuntu_test[std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]})]++;
		}
	}

	if (machi_type == MT_RYANMEN){
		if (machi_hai == ryanmen_id) {
			syuntu_test[machi_hai]++;
		} else {
			syuntu_test[machi_hai-2]++;
		}
	} else if (machi_type == MT_KANCHAN) {
		syuntu_test[machi_hai-1]++;
	} else if (machi_type == MT_PENCHAN){
		if ( machi_hai%10 == 3) {
			syuntu_test[machi_hai-2]++;
		} else {
			syuntu_test[machi_hai]++;
		}
	}

	for (int i = 1; i <= 7; i++) {
		if (syuntu[i] > 0 && syuntu[10+i] > 0 && syuntu[20+i] > 0) {
			return true;
		}
	}
	return false;
}

bool ittsuu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id) {
	int syuntu_test[30];
	for(int hai = 0; hai < 30; hai++) {
		syuntu_test[hai] = syuntu[hai];
	}

	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_CHI) {
			syuntu_test[std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]})]++;
		}
	}

	if (machi_type == MT_RYANMEN){
		if (machi_hai == ryanmen_id) {
			syuntu_test[machi_hai]++;
		} else {
			syuntu_test[machi_hai-2]++;
		}
	} else if (machi_type == MT_KANCHAN) {
		syuntu_test[machi_hai-1]++;
	} else if (machi_type == MT_PENCHAN){
		if ( machi_hai%10 == 3) {
			syuntu_test[machi_hai-2]++;
		} else {
			syuntu_test[machi_hai]++;
		}
	}

	for (int j = 0; j < 3; j++) {
		if (syuntu[j*10+1] > 0 && syuntu[j*10+4] > 0 && syuntu[j*10+7] > 0) {
			return true;
		}
	}
	return false;
}

int anko_num_count_est (const Fuuro_Vector& fuuro, const int kotu[38]) {
	int anko_num = 0;
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_ANKAN) {
			anko_num++;
		}
	}
	for (int hai = 0; hai < 38; hai++) {
		if (kotu[hai] == 1) {
			anko_num++;
		}
	}
	return anko_num;
}

int kan_num_count_est (const Fuuro_Vector& fuuro) {
	int kan_num = 0;
	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_DAIMINKAN ||
			fuuro[fn].type == FT_ANKAN ||
			fuuro[fn].type == FT_KAKAN	
		) {
			kan_num++;
		}
	}
	return kan_num;
}

Agari_Estimate_Info::Agari_Estimate_Info(){}

Agari_Estimate_Info yaku_check(
	const int bakaze, const int jikaze, const Fuuro_Vector fuuro,
	const Hai_Array tehai, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id
) {
	Agari_Estimate_Info agari;
	agari.hai = machi_hai;
	agari.han = 0;
	agari.fu = 0;
	if (chinitsu_check_est(fuuro, head, kotu, syuntu, machi_hai)) {
		agari.han += 5;
	} else if (honitsu_check_est(fuuro, head, kotu, syuntu, machi_hai)) {
		agari.han += 2;
	}
	
	if (tanyao_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 1;
	}

	if (honrou_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 2;
	}

	if (suitsu_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 13;
	}
	
	bool sangen_flag[3], kaze_flag[4];
	for(int i = 0; i < 3; i++) {
		sangen_flag[i] = yakuhai_check_est(35+i, fuuro, kotu, machi_hai, machi_type);
	}
	for(int i = 0; i < 4; i++) {
		kaze_flag[i] = yakuhai_check_est(31+i, fuuro, kotu, machi_hai, machi_type);
	}

	if (kaze_flag[bakaze]) {
		agari.han += 1;
	}

	if (kaze_flag[jikaze]) {
		agari.han += 1;
	}

	for (int i = 0; i < 3; i++) {
		if (sangen_flag[i]) {
			agari.han += 1;
		}
	}

	bool junchan_flag = false;
	if (junchan_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 2;
		junchan_flag = true;
	} else if (chanta_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 1;
	}

	bool toitoi_flag = false;
	if (toitoi_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 2;
		toitoi_flag = true;
	}

	if (sanshikidoukou_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type)) {
		agari.han += 2;
	}

	if (sanshikidoujun_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type, ryanmen_id)) {
		agari.han += 1;
	}
	
	if (ittsuu_check_est(fuuro, head, kotu, syuntu, machi_hai, machi_type, ryanmen_id)) {
		agari.han += 1;
	}

	int anko_num = anko_num_count_est(fuuro, kotu);
	if (anko_num == 3) {
		agari.han += 2;
	}

	int kan_num = kan_num_count_est(fuuro);
	if (kan_num == 3) {
		agari.han += 2;
	} else if(kan_num == 4) {
		agari.han += 13;
	}

	if (sangen_flag[0] && sangen_flag[1] && sangen_flag[2]) {
		agari.han += 13;
	} else {
		for (int i = 0; i < 3; i++) {
			if (sangen_flag[(i+1)%3] && sangen_flag[(i+2)%3] && (head[35+i]==1 || machi_hai==35+i)) {
				agari.han += 2;
			}
		}
	}

	if (kaze_flag[0] && kaze_flag[1] && kaze_flag[2] && kaze_flag[3]) {
		agari.han += 13;
	} else {
		for (int i = 0; i < 4;i++) {
			if (kaze_flag[(i+1)%4] && kaze_flag[(i+2)%4] && kaze_flag[(i+3)%4] && (head[31+i]==1 || machi_hai==31+i)){
				agari.han += 13;
			}
		}
	}

	if(junchan_flag==1 && toitoi_flag==1){
		agari.han = 13;
	}

	agari.fu = 20;

	for (int fn = 0; fn < fuuro.size(); fn++) {
		if (fuuro[fn].type == FT_PON) {
			if (hai_yaochuu(haikind(fuuro[fn].consumed[0])) == NT_CHUJAN) {
				agari.fu += 2;
			} else {
				agari.fu += 4;
			}
		} else if (fuuro[fn].type == FT_DAIMINKAN || fuuro[fn].type == FT_KAKAN) {
			if (hai_yaochuu(haikind(fuuro[fn].consumed[0])) == NT_CHUJAN) {
				agari.fu += 8;
			} else {
				agari.fu += 16;
			}
		} else if (fuuro[fn].type == FT_ANKAN) {
			if (hai_yaochuu(haikind(fuuro[fn].consumed[0])) == NT_CHUJAN) {
				agari.fu += 16;
			} else {
				agari.fu += 32;
			}
		}
	}
	for (int hai = 1; hai < 38; hai++) {
		if (kotu[hai] == 1) {
			if (hai_yaochuu(hai) == NT_CHUJAN) {
				agari.fu += 4;
			} else {
				agari.fu += 8;
			}
		} else if (head[hai] == 1 && machi_hai != hai) {
			if (hai >= 35 || hai == 31 + bakaze || hai == 31 + jikaze) {
				agari.fu += 2;
			}
		}
	}

	if (machi_type >= 2) {
		agari.fu = agari.fu + 2;
	}
	if (machi_type == 2) {
		if (machi_hai >= 35 || machi_hai == 31 + bakaze || machi_hai == 31 + jikaze) {
			agari.fu = agari.fu + 2;
		}
	}

	if (agari.han >= 13) {
		agari.han = 13;
	}
	return agari;
}

Tehai_Estimator_Element::Tehai_Estimator_Element (){}

Tehai_Estimator_Element::Tehai_Estimator_Element (Hai_Array tehai_in) {
	reset(tehai_in);
}

void Tehai_Estimator_Element::reset(Hai_Array tehai_in) {
	tehai = tehai_in;
	prob = 0.0;
	agariv.clear();
}

Color_Type Tehai_Estimator_Element::get_somete_color(const Fuuro_Vector& fuuro) {
	for (Color_Type color = (Color_Type)0; color < CT_NUM; ++color) {
		if (somete_possible(fuuro, color)) {
			bool flag = true;
			for (int hai = 0; hai <= 30; hai++) {
				if (0 < tehai[hai] && hai_color(hai) != color) {
					flag = false;
					break;
				}
			}
			if (flag) {
				return color;
			}
		}
	}
	return CT_NUM;
}

Tehai_Estimator::Tehai_Estimator(){}

void Tehai_Estimator::tenpai_check (
	const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
	int type, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
) {
	Tehai_Estimator_Element tee;
	//Tenpai_Estimator_Agari agari;
	if (type == 0) {
		for (int hc = 0; hc < 3; hc++) {
			for (int hn = 1; hn <= 6; hn++) {
				//ryanmen
				if (nokori[hc*10+hn+1] - tehai[hc*10+hn+1] > 0 && nokori[hc*10+hn+2] - tehai[hc*10+hn+2] > 0) {
					if (!minogashi[hc*10+hn] && !minogashi[hc*10+hn+3]) {
						tehai[hc*10+hn+1]++;
						tehai[hc*10+hn+2]++;
						tee.reset(tehai);
						tee.agariv.push_back(yaku_check(
							bakaze, jikaze, fuuro,
							tehai, head, kotu, syuntu, hc*10+hn, MT_RYANMEN, hc*10+hn
						));
						tee.agariv.push_back(yaku_check(
							bakaze, jikaze, fuuro,
							tehai, head, kotu, syuntu, hc*10+hn+3, MT_RYANMEN, hc*10+hn
						));
						if ((tee.agariv[0].han>0 || tee.agariv[1].han>0)) {
							teev.push_back(tee);
						}
						tehai[hc*10+hn+1]--;
						tehai[hc*10+hn+2]--;
					}
				}
			}
			for(int hn=2;hn<=8;hn++){
				//kanchan
				if (nokori[hc*10+hn-1]-tehai[hc*10+hn-1]>0 && nokori[hc*10+hn+1]-tehai[hc*10+hn+1]>0){
					if (!minogashi[hc*10+hn]) {
						tehai[hc*10+hn-1]++;
						tehai[hc*10+hn+1]++;
						tee.reset(tehai);
						tee.agariv.push_back(yaku_check(
							bakaze, jikaze, fuuro,
							tehai, head, kotu, syuntu, hc*10+hn, MT_KANCHAN, 0
						));
						if (tee.agariv[0].han > 0) {
							teev.push_back(tee);
						}
						tehai[hc*10+hn-1]--;
						tehai[hc*10+hn+1]--;
					}
				}
			}
			//penchan
			if(nokori[hc*10+1]-tehai[hc*10+1]>0 && nokori[hc*10+2]-tehai[hc*10+2]>0){
				if (!minogashi[hc*10+3]) {
					tehai[hc*10+1]++;
					tehai[hc*10+2]++;
					tee.reset(tehai);
					tee.agariv.push_back(yaku_check(
						bakaze, jikaze, fuuro,
						tehai, head, kotu, syuntu, hc*10+3, MT_PENCHAN, 0
					));
					if (tee.agariv[0].han > 0) {
						teev.push_back(tee);
					}
					tehai[hc*10+1]--;
					tehai[hc*10+2]--;
				}
			}

			if(nokori[hc*10+8]-tehai[hc*10+8]>0 && nokori[hc*10+9]-tehai[hc*10+9]>0){
				if (!minogashi[hc*10+7]) {
					tehai[hc*10+8]++;
					tehai[hc*10+9]++;
					tee.reset(tehai);
					tee.agariv.push_back(yaku_check(
						bakaze, jikaze, fuuro,
						tehai, head, kotu, syuntu, hc*10+7, MT_PENCHAN, 0
					));
					if (tee.agariv[0].han > 0) {
						teev.push_back(tee);
					}
					tehai[hc*10+8]--;
					tehai[hc*10+9]--;
				}
			}
		}
	} else if (type == 1) {
		tee.reset(tehai);
		for (int hai = 0; hai < 38; hai++) {
			if (head[hai] == 1) {
				if (minogashi[hai]) { return; }
				tee.agariv.push_back(yaku_check(
					bakaze, jikaze, fuuro,
					tehai, head, kotu, syuntu, hai, MT_SHABO, 0
				));
			}
		}
		if (tee.agariv.size() == 2) {
			if (tee.agariv[0].han>0 || tee.agariv[1].han>0) {
				teev.push_back(tee);
			}
		} else {
			std::cout << "tenpai_estimator_syabo_error:" << tee.agariv.size() << std::endl;
		}
	} else if (type == 2) {
		for (int hai = 0; hai < 38; hai++) {
			if (nokori[hai] - tehai[hai] > 0) {
				if (!minogashi[hai]) {
					tehai[hai]++;
					tee.reset(tehai);
					tee.agariv.push_back(yaku_check(
						bakaze, jikaze, fuuro,
						tehai, head, kotu, syuntu, hai, MT_TANKI, 0
					));
					if (tee.agariv[0].han > 0) {
						teev.push_back(tee);
					}
					tehai[hai]--;
				}
			}
		}
	}
}

void Tehai_Estimator::add_syuntu (
	const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
	int head_num, int kotu_num, int syuntu_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
) {
	int mentu_num = kotu_num + syuntu_num;
	if (mentu_num*3 + head_num*2 == tehai_max_num-1) {
		tenpai_check(bakaze, jikaze, fuuro, 2, tehai, head, kotu, syuntu, teev);
	} else if (mentu_num*3 + head_num*2 == tehai_max_num-2) {
		tenpai_check(bakaze, jikaze, fuuro, 0, tehai, head, kotu, syuntu, teev);
	} else if (mentu_num*3 + head_num*2 == tehai_max_num) {
		tenpai_check(bakaze, jikaze, fuuro, 1, tehai, head, kotu, syuntu, teev);
	} else if (mentu_num*3 + head_num*2 < tehai_max_num) {
		for (int haic = start/10; haic < 3; haic++) {
			for (int hain = start%10; hain <= 7; hain++) {
				if (nokori[haic*10+hain]-tehai[haic*10+hain]>0 && nokori[haic*10+hain+1]-tehai[haic*10+hain+1]>0 && nokori[haic*10+hain+2]-tehai[haic*10+hain+2]>0){
					tehai[haic*10+hain]++;
					tehai[haic*10+hain+1]++;
					tehai[haic*10+hain+2]++;
					syuntu[haic*10+hain]++;
					add_syuntu(bakaze, jikaze, fuuro, head_num, kotu_num, syuntu_num+1, haic*10+hain, tehai, head, kotu, syuntu, teev);
					tehai[haic*10+hain]--;
					tehai[haic*10+hain+1]--;
					tehai[haic*10+hain+2]--;
					syuntu[haic*10+hain]--;
				}
			}
		}
	}
}

void Tehai_Estimator::add_kotu (
	const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
	int head_num, int kotu_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
) {
	if (head_num*2 + kotu_num*3 < tehai_max_num) {
		for(int hai=start;hai<38;hai++){
			if(nokori[hai]-tehai[hai]>=3){
				tehai[hai] += 3;
				kotu[hai]++;
				add_kotu(bakaze, jikaze, fuuro, head_num, kotu_num+1, hai+1, tehai, head, kotu, syuntu, teev);
				add_syuntu(bakaze, jikaze, fuuro, head_num, kotu_num+1, 0, 1, tehai, head, kotu, syuntu, teev);
				tehai[hai] -= 3;
				kotu[hai]--;
			}
		}
	}
	add_syuntu(bakaze, jikaze, fuuro, head_num, kotu_num, 0, 1, tehai, head, kotu, syuntu, teev);
}

void Tehai_Estimator::add_head (
	const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
	int head_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
) {
	if (head_num<2 && head_num*2 < tehai_max_num) {
		for (int hai = start; hai < 38;hai++) {
			if (nokori[hai]-tehai[hai] >= 2) {
				tehai[hai] += 2;
				head[hai]++;
				add_head(bakaze, jikaze, fuuro, head_num+1, hai+1, tehai, head, kotu, syuntu, teev);
				add_kotu(bakaze, jikaze, fuuro, head_num+1, 0, 1, tehai, head, kotu, syuntu, teev);
				tehai[hai] -= 2;
				head[hai]--;
			}
		}
	}
	add_kotu(bakaze, jikaze, fuuro, head_num, 0, 1, tehai, head, kotu, syuntu, teev);
}

std::vector<Tehai_Estimator_Element> Tehai_Estimator::cal_teev (
	const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
	const int tehai_max_num_in, const Hai_Array& nokori_in, const std::array<bool, 38>& minogashi_in
) {
	tehai_max_num = tehai_max_num_in;
	nokori = nokori_in;
    minogashi = minogashi_in;

	Hai_Array tehai = {};
	int head[38] = {};
	int kotu[38] = {};
	int syuntu[30] = {};
	std::vector<Tehai_Estimator_Element> teev;
	add_head(bakaze, jikaze, fuuro, 0, 1, tehai, head, kotu, syuntu, teev);
	return teev;
}

std::vector<Tehai_Estimator_Element> Tehai_Estimator::cal_teev_with_prob (
	const Moves& game_record, const Game_State& game_state, const int my_pid, const int target,
	const int tehai_max_num_in, const Hai_Array& nokori_in, const std::array<bool, 38>& minogashi_in
) {
	std::vector<Tehai_Estimator_Element> teev = cal_teev(
		game_state.bakaze, game_state.player_state[target].jikaze, game_state.player_state[target].fuuro,
		tehai_max_num_in, nokori_in, minogashi_in
	);
	const std::array<bool, 38> minogashi_flag = get_minogashi_flag(game_record, target);
	for (int i = 0; i < teev.size(); i++) {
		teev[i].prob = 1.0;
		for (int hai = 0; hai < 38; hai++) {
			if (teev[i].tehai[hai] > 0) {
				teev[i].prob = teev[i].prob*combination(nokori_in[hai], teev[i].tehai[hai]);
			}
		}

		if (teev[i].agariv.size() < 2) {
			teev[i].prob = teev[i].prob * 0.2; // とりあえずakoが標準的に利用している値。
		}

		teev[i].prob_now = teev[i].prob;
		for (int an = 0; an < teev[i].agariv.size(); an++) {
			if (minogashi_flag[teev[i].agariv[an].hai]) {
				teev[i].prob_now = 0.0;
			}
		}
	}
	if (tactics_json[my_pid]["tenpai_prob_est"] == "ako") {
		// tenpai_prob_estでない項目を別途定義すべきかもしれないが、煩雑になるので保留
		std::array<float, 3> somete_prob, somete_tenpai_prob;
		for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
			somete_prob[color] = cal_somete_ako(game_state, target, color);
			somete_tenpai_prob[color] = somete_prob[color] * cal_somete_tenpai_post(game_state, target, color);
		}
		const float normal_tenpai_prob = (1.0 - somete_prob[CT_MANZU] - somete_prob[CT_PINZU] - somete_prob[CT_SOZU]) * infer_tenpai_prob_ako(game_record, game_state, target);
		const float tenpai_prob = normal_tenpai_prob + somete_tenpai_prob[CT_MANZU] + somete_tenpai_prob[CT_PINZU] + somete_tenpai_prob[CT_SOZU];
		normalize2(game_state.player_state[target].fuuro, tenpai_prob, normal_tenpai_prob, somete_tenpai_prob, teev);
	} else {
		float den = 0.0;
		float den_now = 0.0;
		for (int i = 0; i < teev.size(); i++) {
			den += teev[i].prob;
			den_now += teev[i].prob_now;
		}
		if (den > 0.0) {
			for (int i = 0; i < teev.size(); i++) {
				teev[i].prob = teev[i].prob/den;
			}
		}
		if (den_now > 0.0) {
			for (int i = 0; i < teev.size(); i++) {
				teev[i].prob_now = teev[i].prob_now/den;
			}
		}
	}
	return teev;
}

void Tehai_Estimator::normalize2 (
	const Fuuro_Vector& fuuro, const float tenpai_prob, const float normal_tenpai_prob, const std::array<float, 3> somete_tenpai_prob,
	std::vector<Tehai_Estimator_Element>& teev
) {
	float den_normal = 0.0;
	float den_normal_now = 0.0;
	float den_somete[3] = {};
	float den_somete_now[3] = {};
	for (int i = 0; i < teev.size();i++) {
		const Color_Type somete_color = teev[i].get_somete_color(fuuro);
		if (somete_color == CT_NUM) {
			den_normal += teev[i].prob;
			den_normal_now += teev[i].prob_now;
		} else {
			den_somete[somete_color] += teev[i].prob;
			den_somete_now[somete_color] += teev[i].prob_now;
		}
	}
	for (int i = 0; i < teev.size(); i++) {
		const Color_Type somete_color = teev[i].get_somete_color(fuuro);
		if (somete_color == CT_NUM) {
			if (den_normal > 0.0) {
				teev[i].prob = teev[i].prob/den_normal * normal_tenpai_prob/tenpai_prob;
			}
			if (den_normal_now > 0.0) {
				teev[i].prob_now = teev[i].prob_now/den_normal_now * normal_tenpai_prob/tenpai_prob;
			}
		} else {
			if (den_somete[somete_color] > 0.0) {
				teev[i].prob = teev[i].prob/den_somete[somete_color] * somete_tenpai_prob[somete_color]/tenpai_prob;
			}
			if (den_somete_now[somete_color] > 0.0) {
				teev[i].prob_now = teev[i].prob_now/den_somete_now[somete_color] * somete_tenpai_prob[somete_color]/tenpai_prob;
			}
		}
	}
}

std::array<std::array<float, 100>, 38> cal_hai_prob_from_teev(const std::vector<Tehai_Estimator_Element>& teev, const bool is_tsumo, const bool is_now) {
	std::array<std::array<float, 100>, 38> hai_prob;
	for (int hai = 0; hai < 38; hai++) {
		for (int han = 0; han < 100; han++) {
			hai_prob[hai][han] = 0.0;
		}
	}
	for (int i = 0; i < teev.size(); i++) {
		for (int an = 0; an < teev[i].agariv.size(); an++) {
			const int han = teev[i].agariv[an].han;
			if (han > 0) {
				if (is_now) {
					hai_prob[teev[i].agariv[an].hai][han] += teev[i].prob_now;
				} else {
					hai_prob[teev[i].agariv[an].hai][han] += teev[i].prob;
				}
			}
		}
	}
	return hai_prob;
}

Machi_Coeff::Machi_Coeff(){}

void Machi_Coeff::init_coeff(const int my_pid) {
	if (tactics_json[my_pid]["mc_init"] == "instant") {
		for (int i = 0; i < 4;i++) {
			katachi_prob[i] = 0.0;
		}
		for (int hai = 0; hai < 38;hai++) {
			if (hai % 10 != 0) {
				tanki_coeff[hai] = 1.0;
				syabo_coeff[hai] = 1.0;
			} else {
				tanki_coeff[hai] = 0.0;
				syabo_coeff[hai] = 0.0;
			}
		}

		for (int j = 0; j < 3; j++) {
			ryanmen_coeff[j][0] = 0.0;
			for (int i = 1; i <= 6; i++) {
				ryanmen_coeff[j][i] = 1.0;
			}

			kanchan_coeff[j][0] = 0.0;
			kanchan_coeff[j][1] = 0.0;
			for (int i = 2; i <= 8; i++) {
				kanchan_coeff[j][i] = 1.0;
			}

			penchan_coeff[j][0] = 1.0;
			penchan_coeff[j][1] = 1.0;
		}
	} else if (tactics_json[my_pid]["mc_init"] == "ako") {
		for (int i = 0; i < 4; i++) {
			katachi_prob[i] = 0.0;
		}
		for (int hai = 0; hai < 38; hai++) {
			if(hai%10!=0){
				tanki_coeff[hai] = 1.0;
			}else{
				tanki_coeff[hai] = 0.0;
				syabo_coeff[hai] = 0.0;
			}
		}
		syabo_coeff[31] = 0.242/4.0; syabo_coeff[32] = 0.242/4.0; syabo_coeff[33] = 0.242/4.0; syabo_coeff[34] = 0.242/4.0; syabo_coeff[35] = 0.206/3.0; syabo_coeff[36] = 0.206/3.0; syabo_coeff[37] = 0.206/3.0;

		for (int j = 0; j < 3; j++) {
			ryanmen_coeff[j][0] = 0.0; ryanmen_coeff[j][1] = 0.168/3.0; ryanmen_coeff[j][2] = 0.181/3.0; ryanmen_coeff[j][3] = 0.196/3.0; ryanmen_coeff[j][4] = 0.196/3.0; ryanmen_coeff[j][5] = 0.181/3.0; ryanmen_coeff[j][6] = 0.168/3.0;
			syabo_coeff[10*j+0] = 0.0; syabo_coeff[10*j+1] = 0.194/3.0; syabo_coeff[10*j+2] = 0.184/3.0; syabo_coeff[10*j+3] = 0.152/3.0; syabo_coeff[10*j+4] = 0.155/3.0; syabo_coeff[10*j+5] = 0.184/3.0; syabo_coeff[10*j+6] = 0.155/3.0; syabo_coeff[10*j+7] = 0.152/3.0; syabo_coeff[10*j+8] = 0.184/3.0; syabo_coeff[10*j+9] = 0.194/3.0;
			kanchan_coeff[j][0] = 0.0; kanchan_coeff[j][1] = 0.0; kanchan_coeff[j][2] = 0.12/3.0; kanchan_coeff[j][3] = 0.13/3.0; kanchan_coeff[j][4] = 0.10/3.0; kanchan_coeff[j][5] = 0.09/3.0; kanchan_coeff[j][6] = 0.10/3.0; kanchan_coeff[j][7] = 0.13/3.0; kanchan_coeff[j][8] = 0.12/3.0;
			penchan_coeff[j][0] = 0.105/3.0; penchan_coeff[j][1] = 0.105/3.0;
		}
	} else {
		assert_with_out(false, "mc_init_error!");
	}
}

void Machi_Coeff::set_katachi_prob(const int my_pid) {
	if (tactics_json[my_pid]["katachi_est"] == "instant") {
		katachi_prob[0] = 0.5;
		katachi_prob[1] = 0.25;
		katachi_prob[2] = 0.05;
		katachi_prob[3] = 0.2;
	} else if (tactics_json[my_pid]["katachi_est"] == "ako") {
		katachi_prob[0] = 0.6;
		katachi_prob[1] = 0.122;
		katachi_prob[2] = 0.08;
		katachi_prob[3] = 0.243;
		float total = katachi_prob[0] + katachi_prob[1] + katachi_prob[2] + katachi_prob[3];
		float hosei[19];
		hosei[18] = 1.0; hosei[17] = 0.99; hosei[16] = 0.98; hosei[15] = 0.97; hosei[14] = 0.96; hosei[13] = 0.95; hosei[12] = 0.94; hosei[11] = 0.93; hosei[10] = 0.92; hosei[9] = 0.91; hosei[8] = 0.90; hosei[7] = 0.89; hosei[6] = 0.88; hosei[5] = 0.85; hosei[4] = 0.78; hosei[3] = 0.68; hosei[2] = 0.55; hosei[1] = 0.35; hosei[0] = 0.0;

		int nokori = 18;
		for (int i = 0; i < 3; i++) {
			for (int j = 1;j <= 6; j++) {
				if (ryanmen_coeff[i][j] == 0.0) {
					nokori--;
				}
			}
		}
		float tmp;
		tmp = (total-katachi_prob[0]*hosei[nokori])/(total-katachi_prob[0]);
		katachi_prob[0] = katachi_prob[0]*hosei[nokori];
		katachi_prob[1] = katachi_prob[1]*tmp;
		katachi_prob[2] = katachi_prob[2]*tmp;
		katachi_prob[3] = katachi_prob[3]*tmp;
	} else {
		assert_with_out(false, "katachi_est_error!");
	}
}

void Machi_Coeff::safe_flag_to_coeff(const std::array<bool, 38>& safe_flag) {
	for (int hai = 1; hai < 38;hai++) {
		if (safe_flag[hai]) {
			tanki_coeff[hai] = 0;
			syabo_coeff[hai] = 0;
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int i = 1; i <= 6; i++) {
			if (safe_flag[10*j+i] || safe_flag[10*j+i+3]) {
				ryanmen_coeff[j][i] = 0;
			}
		}
		for (int i = 2; i <= 8; i++) {
			if (safe_flag[10*j+i]) {
				kanchan_coeff[j][i] = 0;
			}
		}
		if (safe_flag[10*j+3]) { penchan_coeff[j][0] = 0; }
		if (safe_flag[10*j+7]) { penchan_coeff[j][1] = 0; }
	}
}

void Machi_Coeff::visible_to_coeff(const Hai_Array& visible_all, const Hai_Array& visible) {
	for (int hai = 0; hai < 38; hai++) {
		if (visible_all[hai] >= 2) {
			syabo_coeff[hai] = 0;
		}
		if (visible_all[hai] >= 3) {
			tanki_coeff[hai] = 0;
		}
	}

	for (int hai = 0; hai < 38; hai++) {
		if (visible[hai] >= 3) {
			syabo_coeff[hai] = 0;
		}
		if (visible[hai] >= 4) {
			tanki_coeff[hai] = 0;
		}
	}

	for(int j=0;j<3;j++){
		for(int i=1;i<=6;i++){
			if(visible[j*10+i+1]==4 || visible[j*10+i+2]==4){
				ryanmen_coeff[j][i] = 0;
			}else if(visible[j*10+i+1]==3 || visible[j*10+i+2]==3){
				ryanmen_coeff[j][i] *= 0.5;
			}else if(visible[j*10+i+1]==2 || visible[j*10+i+2]==2){
				ryanmen_coeff[j][i] *= 0.8;
			}
		}
		for(int i=2;i<=8;i++){
			if(visible[j*10+i-1]==4 || visible[j*10+i+1]==4){
				kanchan_coeff[j][i] = 0;
			}else if(visible[j*10+i-1]==3 || visible[j*10+i+1]==3){
				kanchan_coeff[j][i] *= 0.5;
			}else if(visible[j*10+i-1]==2 || visible[j*10+i+1]==2){
				kanchan_coeff[j][i] *= 0.8;
			}
		}
		if(visible[j*10+1]==4 || visible[j*10+2]==4){
			penchan_coeff[j][0] = 0;
		}else if(visible[j*10+1]==3 || visible[j*10+2]==3){
			penchan_coeff[j][0] *= 0.5;
		}else if(visible[j*10+1]==2 || visible[j*10+2]==2){
			penchan_coeff[j][0] *= 0.8;
		}
		if(visible[j*10+8]==4 || visible[j*10+9]==4){
			penchan_coeff[j][1] = 0;
		}else if(visible[j*10+8]==3 || visible[j*10+9]==3){
			penchan_coeff[j][1] *= 0.5;
		}else if(visible[j*10+8]==2 || visible[j*10+9]==2){
			penchan_coeff[j][1] *= 0.8;
		}
	}
}

void Machi_Coeff::ratio_tas_to_coeff(const Game_State& game_state, const int my_pid, const int target) {
	if (tactics_json[my_pid]["tas_est"] == "instant") {
		return;
	} else if (tactics_json[my_pid]["tas_est"] == "ako") {
		int widx, sute_flag[38]; // sute_kind[6] 現状使っていないが何のためにあったか要確認(?)
		float init;

		float wr[3][7];
		wr[0][0] = -2.48736943975; wr[0][1] = -0.38358839454; wr[0][2] = -0.234425064762; wr[0][3] = -0.288325697454; wr[0][4] = -0.280881687069; wr[0][5] = -0.351783126768; wr[0][6] = -0.279311251629;
		wr[1][0] = -2.55624457004; wr[1][1] = -0.195951464576; wr[1][2] = -0.148550415776; wr[1][3] = -0.208170579705; wr[1][4] = -0.137090025131; wr[1][5] = -0.187566539347; wr[1][6] = -0.194074003517;
		wr[2][0] = -2.6184338081; wr[2][1] = -0.151380323926; wr[2][2] = -0.00916219707065; wr[2][3] = -0.0797609698755; wr[2][4] = -0.0990116476359; wr[2][5] = -0.00530004986828; wr[2][6] = 0.0775349456586;

		const std::array<std::array<std::array<bool, 38>, 7>, 3> tas_ryoukei_flag = get_tas_ryoukei_flag(game_state, target);
		for (int j = 0; j < 3; j++) {
			for (int i = 1; i <= 6; i++) {
				if (tas_ryoukei_flag[j][i][0]) {
					for (int hai = 1; hai < 38; hai++) {
						sute_flag[hai] = tas_ryoukei_flag[j][i][hai] ? 1 : 0;
					}
					float x[7]; x[0] = 1.0; 
					x[1] = sute_flag[31] + sute_flag[32] + sute_flag[33] + sute_flag[34] + sute_flag[35] + sute_flag[36] + sute_flag[37];
					x[2] = sute_flag[1] + sute_flag[9] + sute_flag[11] + sute_flag[19] + sute_flag[21] + sute_flag[29];
					x[3] = sute_flag[2] + sute_flag[8] + sute_flag[12] + sute_flag[18] + sute_flag[22] + sute_flag[28];
					x[4] = sute_flag[3] + sute_flag[7] + sute_flag[13] + sute_flag[17] + sute_flag[23] + sute_flag[27];
					x[5] = sute_flag[4] + sute_flag[6] + sute_flag[14] + sute_flag[16] + sute_flag[24] + sute_flag[26];
					x[6] = sute_flag[5] + sute_flag[15] + sute_flag[25];
					if      (i == 1 || i == 6) { widx = 0; init = 0.168/3.0; }
					else if (i == 2 || i == 5) { widx = 1; init = 0.181/3.0; }
					else                       { widx = 2; init = 0.196/3.0; }

					ryanmen_coeff[j][i] *= logistic(wr[widx], x, 7) / init;
				}
			}
		}

		float wk[4][7];
		wk[0][0] = -3.07019308916; wk[0][1] = -0.809086145736; wk[0][2] = -0.490092420912; wk[0][3] = -0.762531563447; wk[0][4] = -0.623096181354; wk[0][5] = -0.931984101813; wk[0][6] = -0.262703087174;
		wk[1][0] = -3.53708536445; wk[1][1] = -0.397471800839; wk[1][2] = -0.489336312143; wk[1][3] = -0.486722020237; wk[1][4] = -0.543432338931; wk[1][5] = -0.104184045047; wk[1][6] = -0.424668245222;
		wk[2][0] = -4.04134667107; wk[2][1] = -0.618952951894; wk[2][2] = -0.017984967262; wk[2][3] = -0.263343090477; wk[2][4] = 0.111704196848; wk[2][5] = -0.687961773374; wk[2][6] = -0.696477050202;
		wk[3][0] = -3.93164365221; wk[3][1] = -2.94236400573; wk[3][2] = -0.750416658748; wk[3][3] = 0.509000083432; wk[3][4] = -0.598111084665; wk[3][5] = -1.30118462769; wk[3][6] = -0.690204046642;

		const std::array<std::array<std::array<bool, 38>, 9>, 3> tas_kanchan_flag = get_tas_kanchan_flag(game_state, target);
		for (int j = 0; j < 3; j++) {
			for (int i = 2; i <= 8; i++) {
				if (tas_kanchan_flag[j][i][0]) {
					for (int hai = 1; hai < 38; hai++) {
						sute_flag[hai] = tas_kanchan_flag[j][i][hai] ? 1 : 0;
					}
					float x[7]; x[0] = 1.0;
					x[1] = sute_flag[31] + sute_flag[32] + sute_flag[33] + sute_flag[34] + sute_flag[35] + sute_flag[36] + sute_flag[37];
					x[2] = sute_flag[1] + sute_flag[9] + sute_flag[11] + sute_flag[19] + sute_flag[21] + sute_flag[29];
					x[3] = sute_flag[2] + sute_flag[8] + sute_flag[12] + sute_flag[18] + sute_flag[22] + sute_flag[28];
					x[4] = sute_flag[3] + sute_flag[7] + sute_flag[13] + sute_flag[17] + sute_flag[23] + sute_flag[27];
					x[5] = sute_flag[4] + sute_flag[6] + sute_flag[14] + sute_flag[16] + sute_flag[24] + sute_flag[26];
					x[6] = sute_flag[5] + sute_flag[15] + sute_flag[25];
					if      (i == 2 || i == 8) { widx = 0; init = 0.12/3.0; }
					else if (i == 3 || i == 7) { widx = 1; init = 0.13/3.0; }
					else if (i == 4 || i == 6) { widx = 2; init = 0.10/3.0; }
					else                       { widx = 3; init = 0.09/3.0; }

					kanchan_coeff[j][i] *= logistic(wk[widx], x, 7) / init;
				}
			}
		}

		float wp[7];
		wp[0] = -3.5286794012; wp[1] = -0.453487059423; wp[2] = -0.186048267121; wp[3] = -0.600585553325; wp[4] = -0.558353762301; wp[5] = -0.704824584067; wp[6] = -0.839010788576;

		const std::array<std::array<std::array<bool, 38>, 2>, 3> tas_penchan_flag = get_tas_penchan_flag(game_state, target);
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 2; i++) {
				if (tas_penchan_flag[j][i][0]) {
					for (int hai = 1; hai < 38; hai++) {
						sute_flag[hai] = tas_penchan_flag[j][i][hai] ? 1 : 0;
					}
					float x[7]; x[0] = 1.0;
					x[1] = sute_flag[31] + sute_flag[32] + sute_flag[33] + sute_flag[34] + sute_flag[35] + sute_flag[36] + sute_flag[37];
					x[2] = sute_flag[1] + sute_flag[9] + sute_flag[11] + sute_flag[19] + sute_flag[21] + sute_flag[29];
					x[3] = sute_flag[2] + sute_flag[8] + sute_flag[12] + sute_flag[18] + sute_flag[22] + sute_flag[28];
					x[4] = sute_flag[3] + sute_flag[7] + sute_flag[13] + sute_flag[17] + sute_flag[23] + sute_flag[27];
					x[5] = sute_flag[4] + sute_flag[6] + sute_flag[14] + sute_flag[16] + sute_flag[24] + sute_flag[26];
					x[6] = sute_flag[5] + sute_flag[15] + sute_flag[25];
					init = 0.105/3.0;

					penchan_coeff[j][i] *= logistic(wp, x, 7)/init;
				}
			}
		}
	} else {
		std::cout << "tas_est_error:" << std::endl;
		assert(false);
	}
}

void Machi_Coeff::std_coeff(){
	double sum[4];
	for (int i = 0; i < 4; i++) {
		sum[i] = 0.0;
	}
	for (int hai = 0; hai < 38; hai++) {
		sum[1] = sum[1] + syabo_coeff[hai];
		sum[2] = sum[2] + tanki_coeff[hai];
	}
	for (int hai = 0; hai < 38; hai++) {
		if (sum[1] > 0.0) {
			syabo_coeff[hai] = 2.0*(syabo_coeff[hai])/sum[1];
		}
		if (sum[2] > 0.0) {
			tanki_coeff[hai] = tanki_coeff[hai]/sum[2];
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 1; i <= 6; i++) {
			sum[0] = sum[0] + ryanmen_coeff[j][i];
		}
		for (int i = 2; i <= 8; i++) {
			sum[3] = sum[3] + kanchan_coeff[j][i];
		}
		sum[3] = sum[3] + penchan_coeff[j][0];
		sum[3] = sum[3] + penchan_coeff[j][1];
	}

	for (int j = 0; j < 3; j++) {
		if (sum[0] > 0.0) {
			for(int i = 1; i <= 6; i++) {
				ryanmen_coeff[j][i] = ryanmen_coeff[j][i]/sum[0];
			}
		}
		if (sum[3] > 0.0) {
			for(int i = 2; i <= 8; i++) {
				kanchan_coeff[j][i] = kanchan_coeff[j][i]/sum[3];
			}
			penchan_coeff[j][0] = penchan_coeff[j][0]/sum[3];
			penchan_coeff[j][1] = penchan_coeff[j][1]/sum[3];
		}
	}
}

std::array<std::array<float, 100>, 38> cal_hai_prob_from_machi_coeff(
	const Game_State& game_state, const Machi_Coeff& machi_coeff, const std::array<float, 100>& hanfu_weight, const bool is_tsumo
) {
	std::array<std::array<float, 100>, 38> hai_prob;
	for (int hai = 0; hai < 38; hai++) {
		for (int han = 0; han < 100; han++) {
			hai_prob[hai][han] = (machi_coeff.katachi_prob[1]*machi_coeff.syabo_coeff[hai] + machi_coeff.katachi_prob[2]*machi_coeff.tanki_coeff[hai]) * hanfu_weight[han];
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int i = 1; i <= 6; i++) {
			for (int han = 0; han < 100; han++) {
				hai_prob[j*10+i][han] += machi_coeff.katachi_prob[0] * machi_coeff.ryanmen_coeff[j][i] * hanfu_weight[han];
				hai_prob[j*10+i+3][han] += machi_coeff.katachi_prob[0] * machi_coeff.ryanmen_coeff[j][i] * hanfu_weight[han];
			}
		}
		for (int i = 2; i <= 8; i++) {
			for (int han = 0; han < 100; han++) {
				hai_prob[j*10+i][han] += machi_coeff.katachi_prob[3] * machi_coeff.kanchan_coeff[j][i] * hanfu_weight[han];
			}
		}

		for (int han = 0; han < 100; han++) {
			hai_prob[j*10+3][han] += machi_coeff.katachi_prob[3] * machi_coeff.penchan_coeff[j][0] * hanfu_weight[han];
			hai_prob[j*10+7][han] += machi_coeff.katachi_prob[3] * machi_coeff.penchan_coeff[j][1] * hanfu_weight[han];
		}
	}
	return hai_prob;
}

std::array<float, 100> cal_agari_hanfu_prob(const std::array<std::array<float, 100>, 38>& hai_prob) {
	std::array<float, 100> agari_prob;
	for (int han = 0; han < 100; han++) {
		agari_prob[han] = 0.0;
	}

	float den = 0.0;
	for (int han = 1; han < 100; han++) {
		for (int hai = 0; hai < 38; hai++) {
			agari_prob[han] += hai_prob[hai][han];
			den += hai_prob[hai][han];
		}
	}
	if(den > 0) {
		for (int han = 1; han < 100; han++) {
			agari_prob[han] = agari_prob[han] / den;
		}
	}
	return agari_prob;
}

Tenpai_Estimator_Simple::Tenpai_Estimator_Simple(){}

void Tenpai_Estimator_Simple::set_tenpai2(const Moves& game_record, const Game_State& game_state, const int my_pid, const int target) {
	if (tactics_json[my_pid]["tenpai_prob_est"] == "instant") {
		tenpai_prob = 0.25 * game_state.player_state[target].fuuro.size();
	} else if (tactics_json[my_pid]["tenpai_prob_est"] == "ako") {
		std::array<float, 3> somete_prob, somete_tenpai_prob;
		for (Color_Type color = CT_MANZU; color < CT_JIHAI; ++color) {
			somete_prob[color] = cal_somete_ako(game_state, target, color);
			somete_tenpai_prob[color] = somete_prob[color] * cal_somete_tenpai_post(game_state, target, color);
		}
		const float normal_tenpai_prob = (1.0 - somete_prob[CT_MANZU] - somete_prob[CT_PINZU] - somete_prob[CT_SOZU]) * infer_tenpai_prob_ako(game_record, game_state, target);
		tenpai_prob = normal_tenpai_prob + somete_tenpai_prob[CT_MANZU] + somete_tenpai_prob[CT_PINZU] + somete_tenpai_prob[CT_SOZU];
	} else {
		std::cout << "cal_tenpai_error:" << std::endl;
		assert(false);
	}
}

void Tenpai_Estimator_Simple::set_tenpai_estimator(const Moves& game_record, const Game_State& game_state, const int my_pid, const int target, const Tactics& tactics) {
	const Hai_Array visible_all = get_hai_visible_all(game_state);
	const Hai_Array visible_all_kind = haikind(visible_all);
	const Hai_Array tehai_kind = haikind(game_state.player_state[my_pid].tehai);
	const Hai_Array visible_kind = (target == my_pid) ? visible_all_kind : sum_hai_array(visible_all_kind, tehai_kind);
	const Hai_Array nokori_kind = cal_nokori_kind_array(visible_kind);
	const std::array<bool, 38> sute_kind_flag = get_sute_kind_flag(game_state.player_state[target].kawa);
	const std::array<bool, 38> minogashi_ar_flag = get_minogashi_ar_flag(game_record, game_state, target);
	const std::array<bool, 38> furiten_flag = get_furiten_flags(game_record, game_state, target, false);
	// 副露時は他家が見逃した前提で評価するためget_furiten_flagsのskip_latestはfalseにする。
		
	set_tenpai2(game_record, game_state, my_pid, target);
	if (game_state.player_state[target].fuuro.size() == 0) {
		Machi_Coeff machi_coeff;
		machi_coeff.init_coeff(my_pid);
		machi_coeff.safe_flag_to_coeff(sute_kind_flag);
		machi_coeff.safe_flag_to_coeff(minogashi_ar_flag);
		machi_coeff.visible_to_coeff(visible_all_kind, visible_kind);
		machi_coeff.set_katachi_prob(my_pid);
		machi_coeff.ratio_tas_to_coeff(game_state, my_pid, target);
		Machi_Coeff machi_coeff_now = machi_coeff;
		machi_coeff_now.safe_flag_to_coeff(furiten_flag);
		machi_coeff.std_coeff();
		machi_coeff_now.std_coeff();
		// to do machi_coeffあたりの処理をmachi_coeff側に移動、sute_flag_to_coeffに関して複数のフラグを行う。
		if (out_console) {
			std::cout << "katachi_prob" << std::endl;
			std::cout << machi_coeff.katachi_prob[0] << " " << machi_coeff.katachi_prob[1] << " " << machi_coeff.katachi_prob[2] << " " << machi_coeff.katachi_prob[3] << std::endl;
		}

		hai_ron_prob = cal_hai_prob_from_machi_coeff(game_state, machi_coeff, tactics.hanfu_weight_ron, false);
		hai_tsumo_prob = cal_hai_prob_from_machi_coeff(game_state, machi_coeff, tactics.hanfu_weight_tsumo, true);
		hai_ron_prob_now = cal_hai_prob_from_machi_coeff(game_state, machi_coeff_now, tactics.hanfu_weight_ron, false);
	} else {
		Tehai_Estimator tehai_estimator;
		const std::vector<Tehai_Estimator_Element> teev = tehai_estimator.cal_teev_with_prob (
			game_record, game_state, my_pid, target, std::min(7, 13 - (int)game_state.player_state[target].fuuro.size()*3), nokori_kind, sute_kind_flag
    	);
		hai_tsumo_prob = cal_hai_prob_from_teev(teev, true, false);
		hai_ron_prob = cal_hai_prob_from_teev(teev, false, false);
		hai_ron_prob_now = cal_hai_prob_from_teev(teev, false, true);
	}
}

std::array<float, 4> get_tenpai_prob_array (const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator) {
	std::array<float, 4> tenpai_prob_array;
	for (int pid = 0; pid < 4; pid++) {
		tenpai_prob_array[pid] = tenpai_estimator[pid].tenpai_prob;
	}
	return tenpai_prob_array;
}

std::array<std::array<float, 100>, 4> cal_agari_hanfu_prob_array(const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator, const bool is_tsumo) {
	std::array<std::array<float, 100>, 4> agari_hanfu_prob_array;
	for (int pid = 0; pid < 4; pid++) {
		const std::array<std::array<float, 100>, 38>& hai_prob = is_tsumo ? tenpai_estimator[pid].hai_tsumo_prob : tenpai_estimator[pid].hai_ron_prob;
		agari_hanfu_prob_array[pid] = cal_agari_hanfu_prob(hai_prob);
	}
	return agari_hanfu_prob_array;
}

std::pair<std::array<std::array<float, 38>, 4>, std::array<std::array<float, 38>, 4>> cal_houjuu_hai_prob_value(
	const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator, const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp,
	const int my_pid, const bool is_now
) {
	std::array<std::array<float, 38>, 4> houjuu_hai_prob, houjuu_hai_value;
	for (int pid = 0; pid < 4; pid++) {
		const std::array<std::array<float, 100>, 38>& houjuu_hai_hanfu_prob = is_now ?
			tenpai_estimator[pid].hai_ron_prob_now : tenpai_estimator[pid].hai_ron_prob;
		for (int hai = 0; hai < 38; hai++) {
			houjuu_hai_prob[pid][hai] = 0.0;
			houjuu_hai_value[pid][hai] = 0.0;
			for (int han = 0; han < 100; han++) {
				houjuu_hai_prob[pid][hai] += houjuu_hai_hanfu_prob[hai][han];
				houjuu_hai_value[pid][hai] += houjuu_hai_hanfu_prob[hai][han] * kyoku_end_pt_exp[pid][my_pid][han];
			}
			if (houjuu_hai_prob[pid][hai] > 0.0) {
				houjuu_hai_value[pid][hai] = houjuu_hai_value[pid][hai] / houjuu_hai_prob[pid][hai];
			}
		}
	}
	return std::pair<std::array<std::array<float, 38>, 4>, std::array<std::array<float, 38>, 4>>(houjuu_hai_prob, houjuu_hai_value);
}

std::pair<std::array<float, 38>, std::array<float, 38>> cal_total_houjuu_hai_prob_value (
	const int my_pid, const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator,
	const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_value
) {
	std::array<float, 38> total_houjuu_prob, total_houjuu_value;
	for (int hai = 0; hai < 38; hai++) {
		total_houjuu_prob[hai] = 0.0;
		total_houjuu_value[hai] = 0.0;
		for (int pid = 0; pid < 4; pid++) {
			if (pid != my_pid) {
				total_houjuu_prob[hai] += tenpai_estimator[pid].tenpai_prob * houjuu_hai_prob[pid][hai];
				total_houjuu_value[hai] += tenpai_estimator[pid].tenpai_prob * houjuu_hai_prob[pid][hai] * houjuu_hai_value[pid][hai];
			}
			if (out_console) { std::cout << houjuu_hai_prob[pid][hai] << " " << houjuu_hai_value[pid][hai] << " ";}
		}
		if (out_console) { std::cout << std::endl; }
		if (total_houjuu_prob[hai] > 0.0) {
			total_houjuu_value[hai] = total_houjuu_value[hai] / total_houjuu_prob[hai];
			total_houjuu_prob[hai] = std::min(total_houjuu_prob[hai], (float)1.0);
		} 
	}
	return std::pair<std::array<float, 38>, std::array<float, 38>> (total_houjuu_prob, total_houjuu_value);
}