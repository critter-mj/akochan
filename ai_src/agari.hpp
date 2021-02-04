#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "../share/agari_ten.hpp"
#include "bit_hai_num.hpp"
#include "tehai_state2.hpp"
#include "jun_calc.hpp"

#ifdef WINSTD
const int MAX_AGARI_NUM_PER_THREAD = 100000;
#else
const int MAX_AGARI_NUM_PER_THREAD = 200000;
#endif

int fu_to_fuidx(const int fu);
int fuidx_to_fu(const int fuidx);

class Agari_Info_Bit {
public:
    Agari_Info_Bit();
    uint32_t data;
    void reset();
    int get_han_tsumo() const;
    int get_han_ron() const;
    int get_fuidx_tsumo() const;
    int get_fuidx_ron() const;
    int get_hai() const;
    int get_dora_num() const;
    void set_han_tsumo(const int han);
    void set_han_ron(const int han);
    void set_fuidx_tsumo(const int fuidx);
    void set_fuidx_ron(const int fuidx);
    void set_hai(const int hai);
    void set_dora_num(const int dora_num);
};

std::array<double, 5> calc_ura_elem(const Hai_Array& visible_me_kind, const Hai_Array& tehai);
std::array<double, 13> multiply_ura_prob(const std::array<double, 5>& ura_elem, const int dora_num);

class Agari_Basic {
public:
    Agari_Basic();
    Agari_Info_Bit agari_info;
    int get_ten_tsumo(const int pid, const Game_State& game_state) const;
    int get_ten_ron(const int pid, const Game_State& game_state) const;
    std::array<double, 13> calc_ura_prob(
        const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
        const Hai_Array& hai_visible_kind, const int dora_num
    ) const;
    std::array<double, 2> get_ten_exp_direct(
        const int my_pid, const int target, const int chankan_han, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
        const Hai_Array& hai_visible_kind, const Game_State& game_state,
        const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4>& kyoku_end_pt_exp
    ) const;
    std::array<double, 4> get_ten_exp(
        const int my_pid, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
        const Hai_Array& hai_visible_kind, const Game_State& game_state,
        const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4>& kyoku_end_pt_exp,
        const int haitei_han = 0
    ) const;
};

class Agari_Calc : public Agari_Basic {
    public:
    Agari_Calc();
    double tsumo_exp;
    double ron_exp;
};

Agari_Basic agari_info_to_agari_basic(const Agari_Info& agari_info);
Agari_Calc agari_info_to_agari_calc(const Agari_Info& agari_info);
