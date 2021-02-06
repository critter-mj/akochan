#include "agari.hpp"

int fu_to_fuidx(const int fu) {
	if (fu == 20) {
		return 1;
	} else if (fu == 25) {
		return 2;
	} else {
		return (fu + 9) / 10;
	}
}

int fuidx_to_fu(const int fuidx) {
	if (fuidx == 1) {
		return 20;
	} else if (fuidx == 2) {
		return 25;
	} else {
		return fuidx * 10;
	}
}

Agari_Info_Bit::Agari_Info_Bit() {
    reset();
}

void Agari_Info_Bit::reset() {
    data = 0;
}

int Agari_Info_Bit::get_han_tsumo() const {
    return data & 0x0000000F;
}

int Agari_Info_Bit::get_han_ron() const {
    return (data & 0x000000F0) >> 4; 
}

int Agari_Info_Bit::get_fuidx_tsumo() const {
    return (data & 0x00000F00) >> 8;
}

int Agari_Info_Bit::get_fuidx_ron() const {
    return (data & 0x0000F000) >> 12;
}

int Agari_Info_Bit::get_hai() const {
    return (data & 0x00FF0000) >> 16;
}

int Agari_Info_Bit::get_dora_num() const {
    return (data & 0x0F000000) >> 24;
}

void Agari_Info_Bit::set_han_tsumo(const int han) {
    data = (data & 0xFFFFFFF0) + uint32_t(std::min(han, 13));
}

void Agari_Info_Bit::set_han_ron(const int han) {
    data = (data & 0xFFFFFF0F) + (uint32_t(std::min(han, 13)) << 4);
}

void Agari_Info_Bit::set_fuidx_tsumo(const int fuidx) {
    assert(0 <= fuidx && fuidx < 16);
    data = (data & 0xFFFFF0FF) + (uint32_t(fuidx) << 8);
}

void Agari_Info_Bit::set_fuidx_ron(const int fuidx) {
    assert(0 <= fuidx && fuidx < 16);
    data = (data & 0xFFFF0FFF) + (uint32_t(fuidx) << 12);
}

void Agari_Info_Bit::set_hai(const int hai) {
    assert(0 <= hai && hai < 38);
    data = (data & 0xFF00FFFF) + (uint32_t(hai) << 16);
}

void Agari_Info_Bit::set_dora_num(const int dora_num) {
    data = (data & 0xF0FFFFFF) + (uint32_t(std::min(dora_num, 13)) << 24);
}

Agari_Basic::Agari_Basic() {
}

Agari_Calc::Agari_Calc() : Agari_Basic() {
}

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

std::array<double, 13> Agari_Basic::calc_ura_prob(
    const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
    const Hai_Array& hai_visible_kind, const int dora_num
) const {
    if (tehai_state.get_reach_flag() == 0) {
        std::array<double, 13> ura_prob{};
        ura_prob[0] = 1.0;
        return ura_prob;
    } else {
        Hai_Array tehai_kcp_wm = {};
        for (int hai = 1; hai < 38; hai++) {
            if (hai%10 != 0) {
                tehai_kcp_wm[hai] += tehai_bit.count_hai(hai);
                tehai_kcp_wm[hai] += tehai_state.get_haikind_num(hai);
            }
        }
        assert(tehai_kcp_wm[agari_info.get_hai()] < 4);
        tehai_kcp_wm[agari_info.get_hai()]++;
        std::array<double, 5> ura_elem = calc_ura_elem(hai_visible_kind, tehai_kcp_wm);
        return multiply_ura_prob(ura_elem, dora_num);
    }
}

std::array<double, 2> Agari_Basic::get_ten_exp_direct(
    const int my_pid, const int target, const int incident_han, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
    const Hai_Array& hai_visible_kind, const Game_State& game_state,
    const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4>& kyoku_end_pt_exp
) const {
    std::array<double, 2> result;
    for (int i = 0; i < 2; i++) { result[i] = -200.0; }
    if (get_ten_tsumo(my_pid, game_state) > 0 || incident_han > 0) {
        const int agari_han = (my_pid == target) ? agari_info.get_han_tsumo() : (agari_info.get_han_ron() + incident_han);
        std::array<double, 13> ura_prob = calc_ura_prob(tehai_bit, tehai_state, hai_visible_kind, game_state.dora_marker.size());
        if (agari_han > 0) {
            const int fuidx = my_pid == target ? agari_info.get_fuidx_tsumo() : agari_info.get_fuidx_ron();

            double tmp = 0.0;
            for (int un = 0; un < 13; un++) {
                int han_tmp = std::min(13, un + agari_han);
                tmp += kyoku_end_pt_exp[my_pid][target][han_tmp][fuidx] * ura_prob[un];
            }
            result[0] = tmp;
            if (agari_info.get_hai() % 10 == 5 && agari_info.get_hai() < 30) {
                tmp = 0.0;
                for (int un = 0; un < 13; un++) {
                    int han_tmp = std::min(13, un + agari_han + 1);
                    tmp += kyoku_end_pt_exp[my_pid][target][han_tmp][fuidx] * ura_prob[un];
                }
                result[1] = tmp;
            }
        }
    }
    return result;
}

std::array<double, 4> Agari_Basic::get_ten_exp(
    const int my_pid, const Bit_Hai_Num& tehai_bit, const Tehai_State2& tehai_state,
    const Hai_Array& hai_visible_kind, const Game_State& game_state,
    const std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4>& kyoku_end_pt_exp,
    const int haitei_han
) const {
    std::array<double, 4> result;
    for (int i = 0; i < 4; i++) { result[i] = -200.0; }

    if (get_ten_tsumo(my_pid, game_state) > 0 || haitei_han > 0) {
        int agari_han = agari_info.get_han_tsumo() + haitei_han;
        std::array<double, 13> ura_prob = calc_ura_prob(tehai_bit, tehai_state, hai_visible_kind, game_state.dora_marker.size());
        if(agari_han>0){
            int fuidx = agari_info.get_fuidx_tsumo();
            double tmp = 0.0;
            for(int un = 0; un < 13;un++) {
                int han_tmp = std::min(13,un+agari_han);
                tmp += kyoku_end_pt_exp[my_pid][my_pid][han_tmp][fuidx] * ura_prob[un];
            }
            result[0] = tmp;

            if(agari_info.get_hai()%10==5 && agari_info.get_hai()<30){
                tmp = 0.0;
                for(int un=0;un<13;un++){
                    int han_tmp = std::min(13,un+agari_han+1);
                    tmp += kyoku_end_pt_exp[my_pid][my_pid][han_tmp][fuidx] * ura_prob[un];
                }
                result[2] = tmp;
            }
        }
        agari_han = agari_info.get_han_ron();
        if(agari_han>0){
            int fuidx = agari_info.get_fuidx_ron();
            double tmp = 0.0;
            for(int pid=0;pid<4;pid++){
                if(pid!=my_pid){
                    for(int un=0;un<13;un++){
                        int han_tmp = std::min(13,un+agari_han);
                        tmp += kyoku_end_pt_exp[my_pid][pid][han_tmp][fuidx] * ura_prob[un];
                    }
                }
            }
            tmp = tmp/3.0;
            result[1] = tmp;
            if(agari_info.get_hai()%10==5 && agari_info.get_hai()<30){
                tmp = 0.0;
                for(int pid=0;pid<4;pid++){
                    if(pid!=my_pid){
                        for(int un=0;un<13;un++){
                            int han_tmp = std::min(13,un+agari_han+1);
                            tmp += kyoku_end_pt_exp[my_pid][pid][han_tmp][fuidx] * ura_prob[un];
                        }
                    }
                }
                tmp = tmp/3.0;
                result[3] = tmp;
            }
        }
    }
    return result;
}

int Agari_Basic::get_ten_tsumo(const int pid, const Game_State& game_state) const {
    if (agari_info.get_han_tsumo() == 0) {
        return 0;
    } else {
        return tsumo_agari(agari_info.get_han_tsumo(), fuidx_to_fu(agari_info.get_fuidx_tsumo()), game_state.player_state[pid].jikaze == 0);
    }
}

int Agari_Basic::get_ten_ron(const int pid, const Game_State& game_state) const {
    if (agari_info.get_han_ron() == 0) {
        return 0;
    } else {
        return ron_agari(agari_info.get_han_ron(), fuidx_to_fu(agari_info.get_fuidx_ron()), game_state.player_state[pid].jikaze == 0);
    }
}

Agari_Basic agari_info_to_agari_basic(const Agari_Info& agari_info) {
    Agari_Basic agari_basic;
    agari_basic.agari_info.set_han_tsumo(agari_info.han_tsumo);
    agari_basic.agari_info.set_han_ron(agari_info.han_ron);
    agari_basic.agari_info.set_fuidx_tsumo(fu_to_fuidx(agari_info.fu_tsumo));
    agari_basic.agari_info.set_fuidx_ron(fu_to_fuidx(agari_info.fu_ron));
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

