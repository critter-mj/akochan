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

class Agari_Info_Bit {
public:
    Agari_Info_Bit();
    uint32_t data;
    void reset();
    int get_han_tsumo() const;
    int get_han_ron() const;
    int get_hai() const;
    int get_wait_flag() const;
    void set_han_tsumo(const int han);
    void set_han_ron(const int han);
    void set_hai(const int hai);
    void set_wait_flag(const int flag);
};

class Agari_Basic {
public:
    Agari_Basic();
    Agari_Info_Bit agari_info;
    int get_ten_tsumo(const int pid, const Game_State& game_state) const { return agari_info.get_han_tsumo(); };
    int get_ten_ron(const int pid, const Game_State& game_state) const { return agari_info.get_han_ron(); };
    
    float get_ten_exp_direct(
        const int my_pid, const int target, const int chankan_han, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
        const Hai_Array& hai_visible_kind, const Game_State& game_state,
        const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
    ) const;
    std::array<float, 2> get_ten_exp(
        const int my_pid, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
        const Hai_Array& hai_visible_kind, const Game_State& game_state,
        const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
    ) const;
};

class Agari_Calc : public Agari_Basic {
    public:
    Agari_Calc();
    float tsumo_exp;
    float ron_exp;
};

Agari_Basic agari_info_detail_to_agari_basic(const Agari_Info_Detail& agari_info);
Agari_Calc agari_info_detail_to_agari_calc(const Agari_Info_Detail& agari_info);
