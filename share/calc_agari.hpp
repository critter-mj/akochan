#pragma once

#include "include.hpp"
#include "types.hpp"
//#include "calc_yaku.hpp"
#include "calc_yaku_chn.hpp"

class Agari_Result_Chn {
    public:
    // 88 points
    bool big_four_winds, big_three_dragons, four_kongs;
    // 64 points
    bool four_concealed_pungs;
    // 32 points
    bool three_kongs;
    // 24 points
    bool seven_pairs, all_even_pungs, full_flush, pure_triple_chow, pure_shifted_pungs, upper_tiles, middle_tiles, lower_tiles; // Greater Honors and Knitted Tiles
    // 16 points
    bool pure_straight, three_suited_terminal_chows, pure_shifted_chows, all_fives, triple_pung, three_concealed_pungs;
    // 12 points
    bool upper_four, lower_four, big_three_winds; // lesser_honors_and_knitted_tiles, knitted_straight;
    // 8 points
    bool mixed_straight, reversible_tiles, mixed_triple_chow, mixed_shifted_pungs, two_concealed_kongs; //, chicken_hand, last_tile_draw, last_tile_claim, out_with_replacement_tile, robbing_the_kongs;
    // 6 points
    bool all_pungs, half_flush, mixed_shifted_chows, all_types, melded_hand, two_dragon_pungs;
    // 4 points
    bool outside_hand, fully_concealed, two_melded_kongs; // , last_tile
    // 2 points bool
    bool dragon_pung, prevalent_wind, seat_wind, all_chows, two_concealed_pungs, concealed_kong, all_simples, concealed_hand;
    // 1 point bool
    bool melded_kong, one_voided_suit, no_honors, edge_wait, closed_wait, single_wait, self_drawn;
    // 2 points int
    int tile_hog_num, double_pung_num;
    // 1 point int
    int pure_double_chow_num, mixed_double_chow_num, short_straight_num, two_terminal_chows_num, pung_of_terminals_or_honors_num;

    Agari_Result_Chn();
    int calc_point();
};

Agari_Info calc_agari(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool titoi_flag
);

