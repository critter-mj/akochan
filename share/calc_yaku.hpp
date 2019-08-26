#pragma once

#include "include.hpp"
#include "types.hpp"

bool menzen_check(const Fuuro_Vector& fuuro);
bool yakuhai_check(const int yakuhai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type);
bool pinfu_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Machi_Type machi_type, const int bakazehai, const int jikazehai);
bool chinitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
bool honitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
bool tanyao_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool honrou_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
bool suitsu_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro);

void add_tehai_syuntu_to_syuntu(Hai_Array& tehai_syuntu, Hai_Array& syuntu);

bool junchan_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai);
bool chanta_check(const Hai_Array& tehai, const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai);
bool toitoi_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const Machi_Type machi_type);
int peko_num_count(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type);
bool sanshikidoujun_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type);
bool sanshikidoukou_check(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro, const int machi_hai, const Machi_Type machi_type);
bool ittsuu_check(const Hai_Array& tehai_tate_cut, const Fuuro_Vector& fuuro, const Hai_Array& tehai_tmp, const int machi_hai, const Machi_Type machi_type);
bool chuuren_check(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int machi_hai);
int anko_num_count(const Hai_Array& tehai_tate, const Fuuro_Vector& fuuro);
int kan_num_count(const Fuuro_Vector& fuuro);

std::pair<int, int> calc_fu(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tate, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool menzen_flag, const bool pinfu_flag, const bool titoi_flag
);