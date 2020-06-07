#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "../share/agari_ten_chn.hpp"
#include "../share/calc_agari_chn.hpp"
#include "bit_hai_num.hpp"
#include "tehai_state2.hpp"
#include "jun_calc_chn.hpp"

class Agari_Info_Bit_Chn {
public:
    Agari_Info_Bit_Chn();
    uint32_t data;
    void reset();
    int get_han_tsumo() const;
    int get_han_ron() const;
    int get_hai() const;
    void set_han_tsumo(const int han);
    void set_han_ron(const int han);
    void set_hai(const int hai);
};

class Agari_Basic_Chn {
public:
    Agari_Basic_Chn();
    Agari_Info_Bit_Chn agari_info;
    int get_ten_tsumo(const int pid, const Game_State& game_state) const;
    int get_ten_ron(const int pid, const Game_State& game_state) const;
    
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

class Agari_Calc_Chn : public Agari_Basic_Chn {
    public:
    Agari_Calc_Chn();
    float tsumo_exp;
    float ron_exp;
};

Agari_Basic_Chn agari_info_to_agari_basic(const Agari_Info_Chn& agari_info);
Agari_Calc_Chn agari_info_to_agari_calc(const Agari_Info_Chn& agari_info);
