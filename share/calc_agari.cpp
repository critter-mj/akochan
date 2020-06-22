#include "calc_agari.hpp"

Agari_Info_Detail calc_agari_detail(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const Hora_Type hora_type
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

    if (hora_type == HT_TITOI) {
        tsumo_res.seven_pairs = true;
        ron_res = tsumo_res;
    } else if (hora_type == HT_HONORS_AND_KNITTED) {
        tsumo_res.all_types = false;
        switch (honors_and_knitted_tiles_check(tehai, machi_hai)) {
            case 2:
                tsumo_res.greater_honors_and_knitted_tiles = true;
                break;
            case 1:
                tsumo_res.lesser_honors_and_knitted_tiles = true;
                break;
            case 0:
                assert_with_out(false, "agari_detail honors_and_knitted_tiles error");
                break;
        }
        ron_res = tsumo_res;
    } else {
        tsumo_res.all_even_pungs = all_even_pungs_check(tehai, tehai_tate_cut, fuuro, tehai_tmp, machi_type);
        tsumo_res.all_pungs = tsumo_res.all_even_pungs ? false : all_pungs_check(tehai_tate_cut, fuuro, tehai_tmp, machi_type);

        Hai_Array syuntu = get_syuntu_array(tehai_tate_cut, fuuro, tehai_tmp, machi_hai, machi_type);
        Hai_Array kotu = get_kotu_array(tehai_tate, fuuro, machi_hai, machi_type);

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
            for (const auto& perm : ALL_PERM) {
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
                            if (c == hai / 10) { continue; }
                            const int cand = c * 10 + hai % 10;
                            if (0 < syuntu[cand] && syuntu_used[cand] == syuntu[cand]) {
                                tsumo_res.mixed_double_chow_num++;
                                syuntu_used[hai]++;
                                found = true;
                                break;
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
                            if (c == hai / 10) { continue; }
                            const int cand = c * 10 + hai % 10;
                            if (0 < syuntu[cand] && syuntu_used[cand] < syuntu[cand]) {
                                tsumo_res.mixed_double_chow_num++;
                                syuntu_used[hai]++;
                                syuntu_used[cand]++;
                                found = true;
                                break;
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
            for (const auto& perm : ALL_PERM) {
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
            ron_res.concealed_hand = true;
        } else {
            tsumo_res.self_drawn = true;
            if (melded_hand_check(fuuro)) {
                ron_res.melded_hand = true;
            }
        }
    }

    return Agari_Info_Detail(machi_hai, machi_type, tsumo_res, ron_res);
}

Agari_Info calc_agari(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const Hora_Type hora_type
) {
    Agari_Info_Detail detail = calc_agari_detail(bakaze_hai, jikaze_hai, tehai, tehai_tate_cut, tehai_tmp, fuuro, machi_hai, machi_type, hora_type);
    Agari_Info agari_info;
    agari_info.hai = machi_hai;
    agari_info.machi_type = machi_type;
    agari_info.han_tsumo = detail.result_tsumo.calc_point();
    agari_info.han_ron = detail.result_ron.calc_point();
    return agari_info;
}