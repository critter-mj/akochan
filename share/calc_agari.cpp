#include "calc_agari.hpp"

Agari_Result::Agari_Result() {
    // 88 points
    big_four_winds = false; big_three_dragons = false, four_kongs = false;
    // 64 points
    four_concealed_pungs = false;
    // 32 points
    three_kongs = false;
    // 24 points
    seven_pairs = false; all_even_pungs = false; full_flush = false; pure_triple_chow = false; pure_shifted_pungs = false; upper_tiles = false; middle_tiles = false; lower_tiles = false; // Greater Honors and Knitted Tiles
    // 16 points
    pure_straight = false; three_suited_terminal_chows = false; pure_shifted_chows = false; all_fives = false; triple_pung = false; three_concealed_pungs = false;
    // 12 points
    upper_four = false; lower_four = false; big_three_winds = false; // lesser_honors_and_knitted_tiles, knitted_straight;
    // 8 points
    mixed_straight = false; reversible_tiles = false; mixed_triple_chow = false; mixed_shifted_pungs = false; two_concealed_kongs = false; //, chicken_hand, last_tile_draw, last_tile_claim, out_with_replacement_tile, robbing_the_kongs;
    // 6 points
    all_pungs = false; half_flush = false; mixed_shifted_chows = false; all_types = false; melded_hand = false; two_dragon_pungs = false;
    // 4 points
    outside_hand = false; fully_concealed = false; two_melded_kongs = false; // , last_tile
    // 2 points bool
    dragon_pung = false; prevalent_wind = false; seat_wind = false; all_chows = false; two_concealed_pungs = false; concealed_kong = false; all_simples = false; //concealed_hand,
    // 1 point bool
    melded_kong = false; one_voided_suit = false; no_honors = false;
    // 2 points int
    tile_hog_num = 0; double_pung_num = 0;
    // 1 point int
    pure_double_chow_num = 0; mixed_double_chow_num = 0; short_straight_num = 0; two_terminal_chows_num = 0; pung_of_terminals_or_honors_num = 0;
}

int Agari_Result::calc_point() {
    int ret = 0;
    if (big_four_winds) { ret += 88; }
    if (big_three_dragons) { ret += 88; }
    if (four_kongs) { ret += 88; }
    if (four_concealed_pungs) { ret += 64; }
    if (three_kongs) { ret += 32; }
    if (seven_pairs) { ret += 24; }
    if (all_even_pungs) { ret += 24; }
    if (full_flush) { ret += 24; }
    if (pure_triple_chow) { ret += 24; }
    if (pure_shifted_pungs) { ret += 24; }
    if (upper_tiles) { ret += 24; }
    if (middle_tiles) { ret += 24; }
    if (lower_tiles) { ret += 24; }
    if (pure_straight) { ret += 16; }
    if (three_suited_terminal_chows) { ret += 16; }
    if (pure_shifted_chows) { ret += 16; }
    if (all_fives) { ret += 16; }
    if (triple_pung) { ret += 16; }
    if (three_concealed_pungs) { ret += 16; }
    if (upper_four) { ret += 12; }
    if (lower_four) { ret += 12; }
    if (big_three_winds) { ret += 12; }
    if (mixed_straight) { ret += 8; }
    if (reversible_tiles) { ret += 8; }
    if (mixed_triple_chow) { ret += 8; }
    if (mixed_shifted_pungs) { ret += 8; }
    if (two_concealed_pungs) { ret += 8; }
    if (all_pungs) { ret += 6; }
    if (half_flush) { ret += 6; }
    if (mixed_shifted_chows) { ret += 6; }
    if (all_types) { ret += 6; }
    if (melded_hand) { ret += 6; }
    if (two_dragon_pungs) { ret += 6; }
    if (outside_hand) { ret += 4; }
    if (fully_concealed) { ret += 4; }
    if (two_melded_kongs) { ret += 4; }
    if (dragon_pung) { ret += 2; }
    if (prevalent_wind) { ret += 2; }
    if (seat_wind) { ret += 2; }
    if (all_chows) { ret += 2; }
    if (two_concealed_pungs) { ret += 2; }
    if (concealed_kong) { ret += 2; }
    if (all_simples) { ret += 2; }
    if (concealed_hand) { ret += 2; }
    if (melded_kong) { ret += 1; }
    if (one_voided_suit) { ret += 1; }
    if (no_honors) { ret += 1; }
    ret += (tile_hog_num + double_pung_num) * 2;
    ret += pure_double_chow_num + mixed_double_chow_num + short_straight_num + two_terminal_chows_num + pung_of_terminals_or_honors_num;
    return ret;
}

Agari_Info_Detail::Agari_Info_Detail(){}
Agari_Info_Detail::Agari_Info_Detail(const int hai_in, const Agari_Result& result_tsumo_in, const Agari_Result& result_ron_in){
    hai = hai_in;
    result_tsumo = result_tsumo_in;
    result_ron = result_ron_in;
}

Agari_Info_Detail calc_agari_detail(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool titoi_flag
) {
    Hai_Array tehai_tate;
    for(int hai = 0; hai < 38; hai++) {
        tehai_tate[hai] = tehai[hai] - tehai_tate_cut[hai];
    }

    Agari_Result tsumo_res, ron_res;
    tsumo_res.full_flush = full_flush_check(tehai, fuuro);

    tsumo_res.upper_tiles = upper_tiles_check(tehai, fuuro, machi_hai);
    tsumo_res.middle_tiles = middle_tiles_check(tehai, fuuro, machi_hai);
    tsumo_res.lower_tiles = lower_tiles_check(tehai, fuuro, machi_hai);

    tsumo_res.upper_four = tsumo_res.upper_tiles ? false : upper_four_check(tehai, fuuro, machi_hai);
    tsumo_res.lower_four = tsumo_res.lower_tiles ? false : lower_four_check(tehai, fuuro, machi_hai);

    tsumo_res.reversible_tiles = reversible_tiles_check(tehai, fuuro, machi_hai);

    tsumo_res.half_flush = tsumo_res.full_flush ? false : half_flush_check(tehai, fuuro);
    tsumo_res.all_types = all_types_check(tehai, fuuro, machi_hai);

    tsumo_res.all_simples = tsumo_res.middle_tiles ? false : all_simples_check(tehai, fuuro, machi_hai);

    tsumo_res.one_voided_suit = (tsumo_res.full_flush || tsumo_res.half_flush || tsumo_res.reversible_tiles) ? false : one_voided_suit_check(tehai, fuuro, machi_hai);
    tsumo_res.no_honors = (tsumo_res.full_flush || tsumo_res.upper_tiles || tsumo_res.middle_tiles || tsumo_res.lower_tiles || tsumo_res.upper_four || tsumo_res.lower_four || tsumo_res.all_simples) ? false : no_honors_check(tehai, fuuro, machi_hai);

    tsumo_res.tile_hog_num = tile_hog_count(tehai, fuuro, machi_hai);

    if (titoi_flag) {
        tsumo_res.seven_pairs = true;
        ron_res = tsumo_res;
    } else {
        tsumo_res.all_even_pungs = all_even_pungs_check(tehai, tehai_tate_cut, fuuro, tehai_tmp, machi_type);
        tsumo_res.all_pungs = tsumo_res.all_even_pungs ? false : all_pungs_check(tehai_tate_cut, fuuro, tehai_tmp, machi_type);

        Hai_Array syuntu = get_syuntu_array(tehai_tate_cut, fuuro, tehai_tmp, machi_hai, machi_type);
        Hai_Array kotu = get_kotu_array(tehai_tate, fuuro, machi_hai, machi_type);
        std::array<std::array<int, 3>, 6> all_perm = {{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};

        tsumo_res.three_suited_terminal_chows = three_suited_terminal_chows_check(tehai_tate, syuntu, machi_hai, machi_type);

        tsumo_res.outside_hand = outside_hand_check(tehai, tehai_tate, fuuro, tehai_tmp, machi_hai);
        tsumo_res.all_chows = all_chows_check(tehai_tate, fuuro, tehai_tmp, machi_hai, machi_type);

        // Nothing to do if four chows combination is found.
        if (!tsumo_res.three_suited_terminal_chows) {
            Hai_Array syuntu_used = {};
            // Three chows combination. They do not occur in same time and do not duplicate.
            for (int hai = 0; hai < 30; hai++) {
                if (syuntu[hai] == 3) {
                    tsumo_res.pure_triple_chow = true;
                    syuntu_used[hai] = 3;
                }
                if (0 < syuntu[hai] && 0 < syuntu[hai+1] && 0 < syuntu[hai+2]) {
                    tsumo_res.pure_shifted_chows = true;
                    syuntu_used[hai] = 1;
                    syuntu_used[hai+1] = 1;
                    syuntu_used[hai+2] = 1;
                }
            }
            for (int i = 1; i <= 7; i++) {
                if (0 < syuntu[i] && 0 < syuntu[10+i] && 0 < syuntu[20+i]) {
                    tsumo_res.mixed_triple_chow = true;
                    syuntu_used[i] = 1;
                    syuntu_used[10+i] = 1;
                    syuntu_used[20+i] = 1;
                }
            }
            for (const auto& perm : all_perm) {
                if (0 < syuntu[10*perm[0] + 1] && 0 < syuntu[10*perm[1] + 4] && 0 < syuntu[10*perm[2] + 7]) {
                    tsumo_res.mixed_straight = true;
                    syuntu_used[10*perm[0] + 1] = 1;
                    syuntu_used[10*perm[1] + 4] = 1;
                    syuntu_used[10*perm[2] + 7] = 1;
                }
                for (int i = 1; i <= 5; i++) {
                    if (0 < syuntu[10*perm[0] + i] && 0 < syuntu[10*perm[1] + i + 1] && 0 < syuntu[10*perm[2] + i + 2]) {
                        tsumo_res.mixed_shifted_chows = true;
                        syuntu_used[10*perm[0] + i] = 1;
                        syuntu_used[10*perm[1] + i + 1] = 1;
                        syuntu_used[10*perm[2] + i + 2] = 1;
                    }
                }
            }

            // Two chows combination. Combination with a used chow is prior.
            //pure_double_chow_num = 0; mixed_double_chow_num = 0; short_straight_num = 0; two_terminal_chows_num = 0;
            for (int loop = 0; loop < 5; loop++) {
                assert_with_out(loop < 4, "too much loop in counting two chows combination");
                bool found = false;
                for (int hai = 0; hai < 30; hai++) {
                    if (syuntu[hai] == 2 && syuntu_used[hai] == 1) {
                        tsumo_res.pure_double_chow_num++;
                        syuntu_used[hai] = 2;
                        found = true;
                    }
                }
                for (int hai = 0; hai < 30; hai++) {
                    if (syuntu_used[hai] < syuntu[hai]) {
                        for (int c = 0; c < 3; c++) {
                            if (c == hai % 10) { continue; }
                            const int cand = c * 10 + hai % 10;
                            if (0 < syuntu[cand] && syuntu_used[cand] == syuntu[cand]) {
                                tsumo_res.mixed_double_chow_num++;
                                syuntu_used[hai]++;
                                found = true;
                            }  
                        }
                    }
                }
                for (int hai = 0; hai < 30; hai++) {
                    if (0 < syuntu[hai] && 0 < syuntu[hai+3]) {
                        if (syuntu_used[hai] < syuntu[hai] && syuntu_used[hai+3] == syuntu[hai+3]) {
                            tsumo_res.short_straight_num++;
                            syuntu_used[hai]++;
                            found = true;
                        } else if (syuntu_used[hai] == syuntu[hai] && syuntu_used[hai+3] < syuntu[hai+3]) {
                            tsumo_res.short_straight_num++;
                            syuntu_used[hai+3]++;
                            found = true;
                        }
                    }
                }
                for (int c = 0; c < 3; c++) {
                    if (0 < syuntu[10*c + 1] && 0 < syuntu[10*c + 7]) {
                        if (syuntu_used[10*c + 1] < syuntu[10*c + 1] && syuntu_used[10*c + 7] == syuntu[10*c + 7]) {
                            tsumo_res.two_terminal_chows_num++;
                            syuntu_used[10*c + 1]++;
                            found = true;
                        } else if (syuntu_used[10*c + 1] == syuntu[10*c + 1] && syuntu_used[10*c + 7] < syuntu[10*c + 7]) {
                            tsumo_res.two_terminal_chows_num++;
                            syuntu_used[10*c + 7]++;
                            found = true;
                        }
                    }
                }
                if (found) { continue; }

                for (int hai = 0; hai < 30; hai++) {
                    if (syuntu[hai] == 2 && syuntu_used[hai] == 0) {
                        tsumo_res.pure_double_chow_num++;
                        syuntu_used[hai] = 2;
                        found = true;
                    }
                }
                for (int hai = 0; hai < 30; hai++) {
                    if (syuntu_used[hai] < syuntu[hai]) {
                        for (int c = 0; c < 3; c++) {
                            if (c == hai % 10) { continue; }
                            const int cand = c * 10 + hai % 10;
                            if (0 < syuntu[cand] && syuntu_used[cand] < syuntu[cand]) {
                                tsumo_res.mixed_double_chow_num++;
                                syuntu_used[hai]++;
                                syuntu_used[cand]++;
                                found = true;
                            }  
                        }
                    }
                }
                for (int hai = 0; hai < 30; hai++) {
                    if (0 < syuntu[hai] && 0 < syuntu[hai+3]) {
                        if (syuntu_used[hai] < syuntu[hai] && syuntu_used[hai+3] < syuntu[hai+3]) {
                            tsumo_res.short_straight_num++;
                            syuntu_used[hai]++;
                            syuntu_used[hai+3]++;
                            found = true;
                        }
                    }
                }
                for (int c = 0; c < 3; c++) {
                    if (0 < syuntu[10*c + 1] && 0 < syuntu[10*c + 7]) {
                        if (syuntu_used[10*c + 1] < syuntu[10*c + 1] && syuntu_used[10*c + 7] < syuntu[10*c + 7]) {
                            tsumo_res.two_terminal_chows_num++;
                            syuntu_used[10*c + 1]++;
                            syuntu_used[10*c + 7]++;
                            found = true;
                        } 
                    }
                }
                if (!found) { break; }
            }
        }

        switch(kotu[35] + kotu[36] + kotu[37]) {
            case 3:
                tsumo_res.big_three_dragons = true;
                break;
            case 2:
                tsumo_res.two_dragon_pungs = true; // to do 小三元
                break;
            case 1:
                tsumo_res.dragon_pung = true;
                break;
            default:
                break;
        }

        switch(kotu[31] + kotu[32] + kotu[33] + kotu[34]) {
            case 4:
                tsumo_res.big_four_winds = true;
                break;
            case 3:
                tsumo_res.big_three_winds = true; // to do 小四喜
                break;
            default:
                break;
        }
        if (!tsumo_res.big_four_winds) {
            if (0 < kotu[bakaze_hai]) { tsumo_res.prevalent_wind = true; }
            if (0 < kotu[jikaze_hai]) { tsumo_res.seat_wind = true; }
        }

        for (int hai = 0; hai < 30; hai++) {
            if (0 < kotu[hai] && 0 < kotu[hai+1] && 0 < kotu[hai+2]) {
                tsumo_res.pure_shifted_pungs = true;
            }
        }
        for (int i = 1; i <= 9; i++) {
            if (kotu[i] + kotu[10+i] + kotu[20+i] == 3) {
                tsumo_res.triple_pung = true;
            } else if (kotu[i] + kotu[10+i] + kotu[20+i] == 2) {
                tsumo_res.double_pung_num++;
            }
            if (i == 1 || i == 9) {
                tsumo_res.pung_of_terminals_or_honors_num += kotu[i] + kotu[10+i] + kotu[20+i]; // true for triple_pung or double_pung ?
            }
            for (const auto& perm : all_perm) {
                if (0 < kotu[10*perm[0] + i] && 0 < kotu[10*perm[1] + i + 1] && 0 < kotu[10*perm[2] + i + 2]) {
                    tsumo_res.mixed_shifted_pungs = true;
                }
            }
        }
        if (!tsumo_res.big_four_winds) {
            for (int hai = 31; hai <= 34; hai++) {
                if (hai != jikaze_hai && hai != bakaze_hai) {
                    tsumo_res.pung_of_terminals_or_honors_num += kotu[hai];
                }
            }
        }

        const int kan_num = kan_num_count(fuuro);
        const int ankan_num = ankan_num_count(fuuro);
        switch (kan_num) {
            case 4:
                tsumo_res.four_kongs = true;
                break;
            case 3:
                tsumo_res.three_kongs = true;
                break;
            case 2:
                if (ankan_num != 2) { tsumo_res.two_melded_kongs = true; }
                break;
            case 1:
                if (ankan_num != 1) { tsumo_res.melded_kong = true; }
                break;
            default:
                break;
        }
        switch (ankan_num) {
            case 2:
                tsumo_res.two_concealed_kongs = true;
                break;
            case 1:
                tsumo_res.concealed_kong = true;
                break;
            default:
                break;
        }

        ron_res = tsumo_res;
        const int anko_num = anko_num_count(tehai_tate, fuuro);
        switch (anko_num) {
            case 4:
                tsumo_res.four_concealed_pungs = true;
                ron_res.four_concealed_pungs = true;
                break;
            case 3:
                if (machi_type == MT_SHABO) { tsumo_res.four_concealed_pungs = true; } else { tsumo_res.three_concealed_pungs = true; }
                ron_res.three_concealed_pungs = true;
                break;
            case 2:
                if (machi_type == MT_SHABO) { tsumo_res.three_concealed_pungs = true; } else { tsumo_res.two_concealed_pungs = !tsumo_res.two_concealed_kongs; }
                ron_res.two_concealed_pungs = !ron_res.two_concealed_kongs;
                break;
            case 1:
                if (machi_type == MT_SHABO) { tsumo_res.two_concealed_pungs = true; }
                break;
            default:
                break;
        }

        if (menzen_check(fuuro)) {
            tsumo_res.fully_concealed = true;
            ron_res.concealed_kong = true;
        } else {
            tsumo_res.self_drawn = true;
            if (melded_hand_check(fuuro)) {
                ron_res.melded_hand = true;
            }
        }
    }

    return Agari_Info_Detail(machi_hai, tsumo_res, ron_res);
}

Agari_Info calc_agari(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool titoi_flag
) {
    Agari_Info_Detail detail = calc_agari_detail(bakaze_hai, jikaze_hai, tehai, tehai_tate_cut, tehai_tmp, fuuro, machi_hai, machi_type, titoi_flag);
    Agari_Info agari_info;
    agari_info.hai = machi_hai;
    agari_info.han_tsumo = detail.result_tsumo.calc_point();
    agari_info.han_ron = detail.result_ron.calc_point();
    return agari_info;
}