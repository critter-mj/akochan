#include "calc_yaku_chn.hpp"

template<class Fn> bool all_tile_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai, Fn check) {
    if (!check(machi_hai)) { return false; }
    for (int hai = 0; hai < 38; hai++) {
        if (0 < tehai[hai] && !check(hai)) {
            return false;
        }
    }
    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_CHI) {
            if (!check(elem.hai) || !check(elem.consumed[0]) || !check(elem.consumed[1])) {
                return false;
            }
        } else if (!check(elem.consumed[0])) {
            return false;
        }
    }
    return true;
}

template<class Fn> bool all_include_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, Fn check) {
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate[hai]>=2 && !check(hai)) {
            return false;
        }
    }
    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_CHI) {
            if (!check(elem.hai) && !check(elem.consumed[0]) && !check(elem.consumed[1])) {
                return false;
            }
        } else if (!check(elem.consumed[0])) {
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
        for (int i = 1; i <= 7; i++) {
            if (0 < syuntu[10*j+i]) {
                if (!check(10*j+i) && !check(10*j+i+1) && !check(10*j+i+2)) {
                    return false;
                }
            }
        }
    }

    if (check(machi_hai)) {
        return true;
    } else {
        for(int hai = 0; hai < 38; hai++) {
            if (tehai_tmp[hai] > 0 && check(hai)) {
                return true;
            }
        }
    }
    return false;
}

Hai_Array get_syuntu_array(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type) {
    Hai_Array tehai_syuntu = {};
    Hai_Array syuntu = {};
    for (int hai = 0; hai < 30;hai++) {
        tehai_syuntu[hai] = tehai_tate_cut[hai] - tehai_tmp[hai];
        syuntu[hai] = 0;
    }
    add_tehai_syuntu_to_syuntu(tehai_syuntu, syuntu);

    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_CHI) {
            syuntu[std::min({elem.hai, elem.consumed[0], elem.consumed[1]})]++;
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
    return syuntu;
}

Hai_Array get_kotu_array(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type) {
    Hai_Array kotu = {};
    for (int hai = 0; hai < 30; hai++) {
        if (tehai_tate[hai] == 3) {
            kotu[hai]++;
        }
    }

    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_PON || elem.type == FT_ANKAN || elem.type == FT_DAIMINKAN || elem.type == FT_KAKAN) {
            kotu[haikind(elem.consumed[0])]++;
        }
    }

    if (machi_type == MT_SHABO) {
        kotu[machi_hai]++;
    }
    return kotu;
}

// 24 points

bool all_even_pungs_check(const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type) {
    if (!all_pungs_check(tehai_tate_cut, fuuro, tehai_tmp, machi_type)) { return false; }
    for (const Fuuro_Elem& elem : fuuro) {
        if (31 <= elem.consumed[0] || elem.consumed[0] % 2 == 1) {
            return false;
        }
    }
    for (int hai = 0; hai < 38; hai++) {
        if (0 < tehai[hai] && ((31 <= hai) || (hai % 2 == 1))) {
            return false;
        }
    }
    return true;
}

bool full_flush_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    return chinitsu_check(tehai, fuuro);
}

// Pure Triple Chow -> at agari
// Pure Shifted Pungs -> at agari

bool upper_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_upper = [](const int hai) {
        if (31 <= hai) { return false; }
        return 7 <= hai % 10;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_upper);
}

bool middle_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_middle = [](const int hai) {
        if (31 <= hai) { return false; }
        return (4 <= hai % 10) && (6 <= hai % 10);
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_middle);
}

bool lower_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_lower = [](const int hai) {
        if (31 <= hai) { return false; }
        return hai % 10 <= 3;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_lower);
}


// 16 points

// Pure Straight -> at agari ?

bool three_suited_terminal_chows_check(const Hai_Array& tehai_tate, const Hai_Array& syuntu, const int machi_hai, const Machi_Type machi_type) {
           if (syuntu[1] == 1 && syuntu[7] == 1 && syuntu[11] == 1 && syuntu[17] == 1) {
        return tehai_tate[25] == 2 || (machi_hai == 25 && machi_type == MT_TANKI);
    } else if (syuntu[1] == 1 && syuntu[7] == 1 && syuntu[21] == 1 && syuntu[27] == 1) {
        return tehai_tate[15] == 2 || (machi_hai == 15 && machi_type == MT_TANKI);
    } else if (syuntu[11] == 1 && syuntu[17] == 1 && syuntu[21] == 1 && syuntu[27] == 1) {
        return tehai_tate[5] == 2 || (machi_hai == 5 && machi_type == MT_TANKI);
    } else {
        return false;
    }
}

// Pure Shifted Chows -> at agari ?

bool all_fives_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai) {
    auto is_five = [](const int hai) {
        if (31 <= hai) { return false; }
        return hai % 10 == 5;
    };
    return all_include_check(tehai, tehai_tate, fuuro, tehai_tmp, machi_hai, is_five);
}

bool triple_pung_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type) {
    return sanshikidoukou_check(tehai_tate, fuuro, machi_hai, machi_type);
}

// Three Concealed Pungs -> at agari

// 12 points
// Lesser Honors and Knitted Tiles -> pending
// Knitted Straight -> pending

bool upper_four_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_upper_four = [](const int hai) {
        if (31 <= hai) { return false; }
        return 6 <= hai % 10;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_upper_four);
}

bool lower_four_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_lower_four = [](const int hai) {
        if (31 <= hai) { return false; }
        return hai % 10 <= 4;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_lower_four);
}

// Big Three Winds -> at agari ?

// 8 points

// Mixed Straight -> at agari ?

bool reversible_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_reversible = [](const int hai) {
        if      (11 <= hai && hai <= 15) { return true; }
        else if (hai == 18 || hai == 19 || hai == 22) { return true; }
        else if (24 <= hai && hai <= 26) { return true; }
        else if (hai == 28 || hai == 29 || hai == 35) { return true; }
        return false;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_reversible);
}

// Mixed Triple Chow -> at agari ?
// Mixed Shifted Pungs -> at agari ?
// Chicken Hand -> at agari ?
// Last Tile Draw, Last Tile Claim, Out With Replacement Tile, Robbing The Kongs -> not here
// Two Concealed Kongs -> at agari

// 6 points

bool all_pungs_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type) {
    return toitoi_check(tehai_tate_cut, fuuro, tehai_tmp, machi_type);
}

bool half_flush_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    return honitsu_check(tehai, fuuro);
}

// Mixed Shifted Chows -> at agari

bool all_types_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    std::array<bool, 5> flags;
    auto get_type = [](const int hai) {
             if (hai <= 10) { return 0; }
        else if (hai <= 20) { return 1; }
        else if (hai <= 30) { return 2; }
        else if (hai <= 34) { return 3; }
        else                { return 4; }
    };
    for (int hai = 0; hai < 38; hai++) {
        if (0 < tehai[hai]) {
            flags[get_type(hai)] = true;
        }
    }
    for (const Fuuro_Elem& elem : fuuro) {
        flags[get_type(elem.consumed[0])] = true;
    }
    flags[get_type(machi_hai)] = true;
    for (int i = 0; i < 5; i++) {
        if (!flags[i]) { return false; }
    }
    return true;
}

bool melded_hand_check(const Fuuro_Vector& fuuro) {
    if (fuuro.size() != 4) { return false; }
    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_ANKAN) { return false; }
    }
    return true;
    // この段階では、ロンあがりのチェックはできない。
}

// Two Dragon Pungs -> at agari

// 4 points

bool outside_hand_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai) {
    auto is_yaochu = [](const int hai) {
        return hai_yaochuu(hai) != NT_CHUJAN;
    };
    return all_include_check(tehai, tehai_tate, fuuro, tehai_tmp, machi_hai, is_yaochu);
}

// Fully Concealed -> not here
// Two Melded Kongs -> at agari
// Last Tile -> not here

// 2 points

bool all_chows_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type) {
    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type != FT_CHI) {
            return false;
        }
    }
    if (machi_type == MT_SHABO) { return false; }
    if (31 <= machi_hai) { return false; }
    for (int hai = 0; hai < 38; hai++) {
        if (tehai_tate[hai] == 3) {
            return false;
        }
    }
    return true;
}

int tile_hog_count(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    Hai_Array hai_used = tehai;
    for (const Fuuro_Elem& elem : fuuro) {
        if (elem.type == FT_CHI || elem.type == FT_PON) {
            hai_used[elem.hai]++;
            hai_used[elem.consumed[0]]++;
            hai_used[elem.consumed[1]]++;
        }
    }
    hai_used[machi_hai]++;
    int ret = 0;
    for (int hai = 0; hai < 38; hai++) {
        if (hai_used[hai] == 4) {
            ret++;
        }
    }
    return ret;
}

bool all_simples_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_simple = [](const int hai) {
        return hai_yaochuu(hai) == NT_CHUJAN;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_simple);
}

// 1 point

bool one_voided_suit_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_not_manzu = [](const int hai) {
        return hai_color(hai) != CT_MANZU;
    };
    auto is_not_pinzu = [](const int hai) {
        return hai_color(hai) != CT_PINZU;
    };
    auto is_not_sozu = [](const int hai) {
        return hai_color(hai) != CT_SOZU;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_not_manzu) || all_tile_check(tehai, fuuro, machi_hai, is_not_pinzu) || all_tile_check(tehai, fuuro, machi_hai, is_not_sozu);
}

bool no_honors_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai) {
    auto is_not_honor = [](const int hai) {
        return hai_color(hai) != CT_JIHAI;
    };
    return all_tile_check(tehai, fuuro, machi_hai, is_not_honor);
}

int ankan_num_count(const Fuuro_Vector& fuuro) {
    int ankan_num = 0;
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_ANKAN) {
            ankan_num++;
        }
    }
    return ankan_num;
}
