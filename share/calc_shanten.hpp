#pragma once

#include "include.hpp"
#include "types.hpp"
#include "calc_agari.hpp"

Hai_Array using_hai_array(const Hai_Array& tehai, const Fuuro_Vector& fuuro);

void tenpai_check(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info    
);

void analyze_tatu(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
);

void cut_syuntu(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp,
    const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
);

void cut_kotu(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& using_array, const Hai_Array& tehai, Hai_Array& tehai_tmp,
    const int start, const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info
);

void titoi_shanten(const int bakaze_hai, const int jikaze_hai, const Hai_Array& tehai, const Fuuro_Vector& fuuro, Tenpai_Info& tenpai_info);

void analyze_tehai(
	const int bakaze_hai, const int jikaze_hai,
	const Hai_Array using_array, const Hai_Array tehai, const Fuuro_Vector fuuro, Tenpai_Info& tenpai_info
);

Tenpai_Info cal_tenpai_info(const int bakaze, const int jikaze, const Hai_Array& tehai, const Fuuro_Vector& fuuro);

int count_yaochu_kind(const Hai_Array& tehai);