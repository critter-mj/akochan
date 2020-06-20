#pragma once

#include "include.hpp"
#include "types.hpp"
#include "calc_yaku.hpp"

const std::array<std::array<int, 3>, 6> ALL_PERM = {{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};

template<class Fn> bool all_tile_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai, Fn check);
template<class Fn> bool all_include_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, Fn check);

Hai_Array get_syuntu_array(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type);
Hai_Array get_kotu_array(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type);

// 24 points
bool all_even_pungs_check(const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type);
bool full_flush_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
// Pure Triple Chow -> at agari
// Pure Shifted Pungs -> at agari
bool upper_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool middle_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool lower_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);

// 16 points

// Pure Straight -> at agari ?
bool three_suited_terminal_chows_check(const Hai_Array& tehai_tate, const Hai_Array& syuntu, const int machi_hai, const Machi_Type machi_type);
// Pure Shifted Chows -> at agari ?
bool all_fives_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai);
bool triple_pung_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type);
// Three Concealed Pungs -> at agari

// 12 points
bool lower_four_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool upper_four_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
// Big Three Winds -> at agari ?

// 8 points
// Mixed Straight -> at agari ?
bool reversible_tiles_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
// Mixed Triple Chow -> at agari ?
// Mixed Shifted Pungs -> at agari ?
// Chicken Hand -> at agari ?
// Last Tile Draw, Last Tile Claim, Out With Replacement Tile, Robbing The Kongs -> not here
// Two Concealed Kongs -> at agari

// 6 points
bool all_pungs_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type);
bool half_flush_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
// Mixed Shifted Chows -> at agari
bool all_types_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool melded_hand_check(const Fuuro_Vector& fuuro);
// Two Dragon Pungs -> at agari

// 4 points
bool outside_hand_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai);

// Fully Concealed -> not here
// Two Melded Kongs -> at agari
// Last Tile -> not here

// 2 points
bool all_chows_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type);
int tile_hog_count(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool all_simples_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);

// 1 point
bool one_voided_suit_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool no_honors_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);

int ankan_num_count(const Fuuro_Vector& fuuro);

int knitted_tiles_count(const Hai_Array& tehai, const int machi_hai, const std::array<int, 3>& perm);
int honors_and_knitted_tiles_check(const Hai_Array& tehai, const int machi_hai);
