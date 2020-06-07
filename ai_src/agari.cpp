#include "agari.hpp"

Agari_Info_Bit::Agari_Info_Bit() {
    reset();
}

void Agari_Info_Bit::reset() {
    data = 0;
}

int Agari_Info_Bit::get_han_tsumo() const {
    return data & 0x000000FF;
}

int Agari_Info_Bit::get_han_ron() const {
    return (data & 0x0000FF00) >> 8; 
}

int Agari_Info_Bit::get_hai() const {
    return (data & 0x00FF0000) >> 16;
}

void Agari_Info_Bit::set_han_tsumo(const int han) {
    data = (data & 0xFFFFFF00) + uint32_t(std::min(han, 256));
}

void Agari_Info_Bit::set_han_ron(const int han) {
    data = (data & 0xFFFF00FF) + (uint32_t(std::min(han, 256)) << 8);
}

void Agari_Info_Bit::set_hai(const int hai) {
    assert(0 <= hai && hai < 38);
    data = (data & 0xFF00FFFF) + (uint32_t(hai) << 16);
}

Agari_Basic::Agari_Basic() {
}

Agari_Calc::Agari_Calc() : Agari_Basic() {
}

float Agari_Basic::get_ten_exp_direct(
    const int my_pid, const int target, const int chankan_han, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
    const Hai_Array& hai_visible_kind, const Game_State& game_state,
    const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
) const {
    const int agari_han = (my_pid == target) ? agari_info.get_han_tsumo() : (agari_info.get_han_ron() + chankan_han);
    if (agari_han > 0) {
        return kyoku_end_pt_exp[my_pid][target][std::min(agari_han, 99)];
    } else {
        return -200.0;
    }
}

std::array<float, 2> Agari_Basic::get_ten_exp(
    const int my_pid, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
    const Hai_Array& hai_visible_kind, const Game_State& game_state,
    const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp
) const {
    std::array<float, 2> result;
    for (int i = 0; i < 2; i++) { result[i] = -200.0; }

    const int han_tsumo = agari_info.get_han_tsumo();
    if (0 < han_tsumo) {
        result[0] = kyoku_end_pt_exp[my_pid][my_pid][std::min(han_tsumo, 99)];
    }
    const int han_ron = agari_info.get_han_ron();
    if (0 < han_ron) {
        float tmp = 0.0;
        for (int pid = 0;pid < 4; pid++) {
            if (pid != my_pid) {
                tmp += kyoku_end_pt_exp[my_pid][pid][std::min(han_ron, 99)];
            }
        }
        tmp = tmp / 3.0;
        result[1] = tmp;
    }
    return result;
}

Agari_Basic agari_info_to_agari_basic(const Agari_Info& agari_info) {
    Agari_Basic agari_basic;
    agari_basic.agari_info.set_han_tsumo(agari_info.han_tsumo);
    agari_basic.agari_info.set_han_ron(agari_info.han_ron);
    agari_basic.agari_info.set_hai(agari_info.hai);
    //agari_basic.agari_info.set_dora_num(agari_info.dora_num);
    return agari_basic;
}

Agari_Calc agari_info_to_agari_calc(const Agari_Info& agari_info) {
    Agari_Calc agari_calc;
    agari_calc.agari_info.data = agari_info_to_agari_basic(agari_info).agari_info.data;
    agari_calc.tsumo_exp = -200.0;
    agari_calc.ron_exp = -200.0;
    return agari_calc;
}

// to do : needless for chn

std::array<double, 5> calc_ura_elem(const Hai_Array& visible_me_kind, const Hai_Array& tehai){
    std::array<double, 5> ura_elem = {};
    Hai_Array tehai_kind = haikind(tehai);

	int invisible_total = 0;
	for (int hai = 1; hai < 38; hai++) {
		if (hai % 10 != 0) {
			ura_elem[tehai_kind[dora_marker_to_dora(hai)]] += 4 - visible_me_kind[hai];
			invisible_total += 4 - visible_me_kind[hai];
		}
	}
	for (int i = 0; i < 5; i++) {
		ura_elem[i] = ura_elem[i]/invisible_total;
	}
    return ura_elem;
}

std::array<double, 13> multiply_ura_prob(const std::array<double, 5>& ura_elem, const int dora_num) {
	std::array<double, 13> ura_prob_tmp = {};
	std::array<double, 13> ura_prob = {};

	ura_prob[0] = 1.0;
	for (int dn = 0; dn < dora_num; dn++) {
		for (int i = 0; i < 13;i++) {
			ura_prob_tmp[i] = ura_prob[i];
			ura_prob[i] = 0.0;
		}
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 5;j++) {
				if (i + j >= 12) {
					ura_prob[12] += ura_prob_tmp[i]*ura_elem[j];
				} else {
					ura_prob[i+j] += ura_prob_tmp[i]*ura_elem[j];
				}
			}
		}
	}
	return ura_prob;
}
