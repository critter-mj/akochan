#pragma once

#include "include.hpp"
#include "types.hpp"
#include "calc_yaku.hpp"

Agari_Info calc_agari(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool titoi_flag
);

