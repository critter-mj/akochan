#include "calc_yaku.hpp"

bool menzen_check(const Fuuro_Vector& fuuro) {
    for (int i = 0; i < fuuro.size(); i++) {
        if (fuuro[i].type != FT_ANKAN) {
            return false;
        }
    }
    return true;
}

bool yakuhai_check(const int yakuhai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type) {
    for (int i = 0; i < fuuro.size(); i++) {
        if (fuuro[i].consumed[0] == yakuhai) {
            return true;
        }
    }

    if (tehai_tate[yakuhai] == 3) {
        return true;
    } else if (machi_type == MT_SHABO && machi_hai == yakuhai) {
        return true;
    } else {
        return false;
    }
}

bool pinfu_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Machi_Type machi_type, const int bakazehai, const int jikazehai) {
    if (0 < fuuro.size()) {
        return false;
    }
    if (machi_type != MT_RYANMEN){
        return false;
    }
    for (int hai = 1; hai < 38; hai++) {
        if (tehai_tate[hai] == 3) {
            return false;
        }
    }
    if (tehai_tate[bakazehai] == 2 || tehai_tate[jikazehai] == 2) {
        return false;
    }
    if (tehai_tate[35] == 2 || tehai_tate[36] == 2 || tehai_tate[37] == 2) {
        return false;
    }
    return true;
}

bool chinitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    bool flags[3];
    for (int i = 0; i < 3;i++) {
        flags[i] = true;
    }
    for (Color_Type color = (Color_Type)0; color < CT_JIHAI; ++color) {
        for (int fn = 0; fn < fuuro.size(); fn++) {
            if (hai_color(fuuro[fn].consumed[0]) != color) {
                flags[color] = false;
                break;
            }
        }
        if (flags[color]) {
            for (int hai = 0; hai < 38; hai++) {
                if (tehai[hai] > 0 && hai_color(hai) != color) {
                    flags[color] = false;
                    break;
                }
            }
        }
    }
    return flags[0] || flags[1] || flags[2];
}

bool honitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    bool flags[3];
    for (int i = 0; i < 3; i++) {
        flags[i] = true;
    }
    for (Color_Type color = (Color_Type)0; color < CT_JIHAI; ++color) {
        for (int fn = 0; fn < fuuro.size(); fn++) {
            if (hai_color(fuuro[fn].consumed[0]) != color && hai_color(fuuro[fn].consumed[0]) != CT_JIHAI) {
                flags[color] = false;
                break;
            }
        }
        if (flags[color]) {
            for (int hai = 0; hai < 30; hai++) {
                if (tehai[hai] > 0 && hai_color(hai) != color) {
                    flags[color] = false;
                    break;
                }
            }
        }
    }
    return flags[0] || flags[1] || flags[2];
}

bool tanyao_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    for (int hai = 0; hai < 38; hai++) {
        if(tehai[hai] > 0 && hai_yaochuu(hai) != NT_CHUJAN) {
            return false;
        }
    }
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            if (hai_yaochuu(fuuro[fn].hai) != NT_CHUJAN || hai_yaochuu(fuuro[fn].consumed[0]) != NT_CHUJAN || hai_yaochuu(fuuro[fn].consumed[1]) != NT_CHUJAN) {
                return false;
            }
        } else if (hai_yaochuu(fuuro[fn].consumed[0]) != NT_CHUJAN) {
            return false;
        }
    }
    if (hai_yaochuu(machi_hai) != NT_CHUJAN) {
        return false;
    }
    return true;
}

bool honrou_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    for (int hai = 0; hai < 38; hai++) {
        if (tehai[hai] > 0 && hai_yaochuu(hai) == NT_CHUJAN) {
            return false;
        }
    }
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            return false;
        } else if (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) {
            return false;
        }
    }
    if (hai_yaochuu(machi_hai) == NT_CHUJAN) {
        return false;
    }
    return true;
}

bool suitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    for (int hai = 0; hai <= 30; hai++) {
        if(tehai[hai] != 0) {
            return false;
        }
    }
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (hai_color(fuuro[fn].consumed[0]) != CT_JIHAI) {
            return false;
        }
    }
    return true;
}

void add_tehai_syuntu_to_syuntu(Hai_Array& tehai_syuntu, Hai_Array& syuntu){
	for (int j = 0; j < 3; j++) {
		int ss = 1;
		while (ss <= 7) {
			if (tehai_syuntu[10*j+ss] && tehai_syuntu[10*j+ss+1] && tehai_syuntu[10*j+ss+2]) {
				syuntu[10*j+ss]++;
				tehai_syuntu[10*j+ss] = tehai_syuntu[10*j+ss]-1;
				tehai_syuntu[10*j+ss+1] = tehai_syuntu[10*j+ss+1]-1;
				tehai_syuntu[10*j+ss+2] = tehai_syuntu[10*j+ss+2]-1;
			} else {
				ss = ss + 1;
			}
		}
	}
}

bool junchan_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai) {
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate[hai] >= 2 && hai_yaochuu(hai) != NT_SUYAO) {
            return false;
        }
    }

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            if (hai_yaochuu(fuuro[fn].hai) != NT_SUYAO && hai_yaochuu(fuuro[fn].consumed[0]) != NT_SUYAO && hai_yaochuu(fuuro[fn].consumed[1]) != NT_SUYAO) {
                return false;
            }
        } else if (hai_yaochuu(fuuro[fn].consumed[0]) != NT_SUYAO) {
            return false;
        }
    }

    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30; hai++) {
        tehai_syuntu[hai] = tehai[hai] - tehai_tate[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);
    for (int j = 0; j < 3; j++) {
        for (int i = 2; i <= 6; i++) {
            if (syuntu[10 * j + i] > 0) {
                return false;
            }
        }
    }

    if (hai_yaochuu(machi_hai) == NT_SUYAO) {
        return true;
    } else {
        for (int hai = 0; hai < 38; hai++) {
            if (tehai_tmp[hai] > 0 && hai_yaochuu(hai) == NT_SUYAO) {
                return true;
            }
        }
    }
    return false;
}

bool chanta_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai) {
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate[hai]>=2 && hai_yaochuu(hai) == NT_CHUJAN) {
            return false;
        }
    }

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            if (hai_yaochuu(fuuro[fn].hai) == NT_CHUJAN && hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN && hai_yaochuu(fuuro[fn].consumed[1]) == NT_CHUJAN) {
                return false;
            }
        } else if (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) {
            return false;
        }
    }

    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30; hai++) {
        tehai_syuntu[hai] = tehai[hai] - tehai_tate[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);
    for (int j = 0;j < 3;j++) {
        for (int i = 2;i <= 6; i++) {
            if (syuntu[10*j+i]>0) {
                return false;
            }
        }
    }

    if (hai_yaochuu(machi_hai) != NT_CHUJAN) {
        return true;
    } else {
        for(int hai = 0; hai < 38; hai++) {
            if (tehai_tmp[hai] > 0 && hai_yaochuu(hai) != NT_CHUJAN) {
                return true;
            }
        }
    }
    return false;
}

bool toitoi_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type) {
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            return false;
        }
    }
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate_cut[hai] - tehai_tmp[hai] > 0) {
            return false;
        }
    }
    if (machi_type == MT_SHABO || machi_type == MT_TANKI) {
        return true;
    } else {
        return false;
    }
}

int peko_num_count(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type) {
    if (fuuro.size() > 0) {
        return false;
    }

    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30; hai++) {
        tehai_syuntu[hai] = tehai_tate_cut[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);
    if (machi_type == MT_RYANMEN || machi_type == MT_KANCHAN || machi_type == MT_PENCHAN) {
        for (int hai = 0; hai < 30; hai++) {
            if (tehai_tmp[hai] > 0) {
                if (machi_hai < hai) {
                    syuntu[machi_hai]++;
                } else {
                    syuntu[hai]++;
                }
                break;
            }
        }
    }

    int peko_num = 0;
    for (int hai = 0; hai < 30; hai++) {
        if (syuntu[hai] >= 2) {
            peko_num++;
        }
    }
    return peko_num;
}

bool sanshikidoukou_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type) {
    Hai_Array kotu = {};

    for (int hai = 0; hai < 30; hai++) {
        if (tehai_tate[hai] == 3) {
            kotu[hai]++;
        }
    }

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_PON || fuuro[fn].type == FT_ANKAN || fuuro[fn].type == FT_DAIMINKAN || fuuro[fn].type == FT_KAKAN) {
            kotu[haikind(fuuro[fn].consumed[0])]++;
        }
    }

    if (machi_type == MT_SHABO) {
        kotu[machi_hai]++;
    }

    for (int i = 1; i <= 9; i++) {
        if (kotu[i] == 1 && kotu[10+i] == 1 && kotu[20+i] == 1) {
            return true;
        }
    }
    return false;
}

bool sanshikidoujun_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type) {
    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30; hai++) {
        tehai_syuntu[hai] = tehai_tate_cut[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            syuntu[std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]})]++;
        }
    }

    if (machi_type == MT_RYANMEN || machi_type == MT_KANCHAN || machi_type == MT_PENCHAN) {
        for (int hai = 0; hai < 30; hai++) {
            if (tehai_tmp[hai] > 0) {
                if (machi_hai < hai) {
                    syuntu[machi_hai]++;
                } else {
                    syuntu[hai]++;
                }
                break;
            }
        }
    }

    for (int i = 1; i <= 7; i++) {
        if (syuntu[i] > 0 && syuntu[10+i] > 0 && syuntu[20+i] > 0) {
            return true;
        }
    }
    return false;
}

bool ittsuu_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type) {
    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30;hai++) {
        tehai_syuntu[hai] = tehai_tate_cut[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);

    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            syuntu[std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]})]++;
        }
    }

    if (machi_type == MT_RYANMEN || machi_type == MT_KANCHAN || machi_type == MT_PENCHAN) {
        for (int hai = 0; hai < 30; hai++) {
            if (tehai_tmp[hai] > 0) {
                if (machi_hai < hai) {
                    syuntu[machi_hai]++;
                } else {
                    syuntu[hai]++;
                }
                break;
            }
        }
    }

    for (int j = 0; j < 3;j++) {
        if(syuntu[j*10+1]>0 && syuntu[j*10+4]>0 && syuntu[j*10+7]>0){
            return true;
        }
    }
    return false;
}


bool chuuren_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    if (fuuro.size() > 0) {
        return false;
    }
    bool flags[3];
    Hai_Array machi_array = {};
    machi_array[machi_hai] = 1;
    for (int color = 0; color < 3; color++) {
        flags[color] = true;
        if(tehai[10*color+1] + machi_array[10*color+1]<3 || tehai[10*color+9] + machi_array[10*color+9]<3){
            flags[color] = false;
            continue;
        }
        for(int i=2;i<=8;i++){
            if(tehai[10*color+i] + machi_array[10*color+i]==0){
                flags[color] = false;
                break;
            }
        }
    }
    return flags[0] || flags[1] || flags[2];
}

int anko_num_count(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro) {
    int anko_num = 0;
    for (int fn = 0; fn < fuuro.size();fn++) {
        if(fuuro[fn].type == FT_ANKAN) {
            anko_num++;
        }
    }
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate[hai] == 3) {
            anko_num++;
        }
    }
    return anko_num;
}

int kan_num_count(const Fuuro_Vector& fuuro) {
    int kan_num = 0;
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_ANKAN || fuuro[fn].type == FT_DAIMINKAN || fuuro[fn].type == FT_KAKAN) {
            kan_num++;
        }
    }
    return kan_num;
}

/*
int haitei_check();
*/

std::pair<int, int> calc_fu(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tate, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool menzen_flag, const bool pinfu_flag, const bool titoi_flag
) {
    if (pinfu_flag) {
        return std::pair<int, int> (20, 30);
    }
    if (titoi_flag) {
        return std::pair<int, int> (25, 25);
    }

    int fu_tsumo = 20;
    for (int fn = 0; fn < fuuro.size(); fn++) {
        switch (fuuro[fn].type) {
            case FT_PON:
                fu_tsumo = (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) ? fu_tsumo + 2 : fu_tsumo + 4;
                break;
            case FT_DAIMINKAN:
            case FT_KAKAN:
                fu_tsumo = (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) ? fu_tsumo + 8 : fu_tsumo + 16;
                break;
            case FT_ANKAN:
                fu_tsumo = (hai_yaochuu(fuuro[fn].consumed[0]) == NT_CHUJAN) ? fu_tsumo + 16 : fu_tsumo + 32;
                break;
            default:
                break;
        }
    }

    for (int hai = 1; hai < 38; hai++) {
        if (tehai_tate[hai] == 3) {
            fu_tsumo = (hai_yaochuu(hai) == NT_CHUJAN) ? fu_tsumo + 4 : fu_tsumo + 8;
        } else if (tehai_tate[hai] == 2) {
            if (hai != machi_hai || machi_type != MT_SHABO) {
                if (hai >= 35 || hai == bakaze_hai || hai == jikaze_hai) {
                    fu_tsumo = fu_tsumo + 2;
                }
            }
        }
    }
    if (machi_type == MT_TANKI || machi_type == MT_KANCHAN || machi_type == MT_PENCHAN) {
        fu_tsumo = fu_tsumo + 2;
    }
    if (machi_type == MT_TANKI) {
        if (machi_hai >= 35 || machi_hai == bakaze_hai || machi_hai == jikaze_hai) {
            fu_tsumo = fu_tsumo + 2;
        }
    }

    int fu_ron = fu_tsumo;

    fu_tsumo = fu_tsumo + 2;
    if (machi_type == MT_SHABO) {
        if (hai_yaochuu(machi_hai) == NT_CHUJAN) {
            fu_tsumo = fu_tsumo + 4;
            fu_ron = fu_ron + 2;
        } else {
            fu_tsumo = fu_tsumo + 8;
            fu_ron = fu_ron + 4;
        }
    }
    if (menzen_flag) {
        fu_ron = fu_ron + 10;
    }

    if (fu_tsumo < 30) {
        fu_tsumo = 30;
    }
    if (fu_ron < 30) {
        fu_ron = 30;
    }

    return std::pair<int, int> (fu_tsumo, fu_ron);
}