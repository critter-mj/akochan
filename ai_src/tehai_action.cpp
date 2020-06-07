#include "tehai_action.hpp"

extern const bool out_console;

bool is_pon(Action_Type action_type) {
    return (action_type == AT_PON || action_type == AT_PON_WITH_AKA);
}

bool is_daiminkan(Action_Type action_type) {
    return action_type == AT_DAIMINKAN;
}

bool is_ankan(Action_Type action_type) {
    return action_type == AT_ANKAN;
}

bool is_kakan(Action_Type action_type) {
    return action_type == AT_KAKAN;
}

bool is_chi_low(Action_Type action_type) {
    return (action_type == AT_CHI_LOW || action_type == AT_CHI_LOW_WITH_AKA);
}

bool is_chi_middle(Action_Type action_type) {
    return (action_type == AT_CHI_MIDDLE || action_type == AT_CHI_MIDDLE_WITH_AKA);
}

bool is_chi_high(Action_Type action_type) {
    return (action_type == AT_CHI_HIGH || action_type == AT_CHI_HIGH_WITH_AKA);
}

bool is_chi(Action_Type action_type) {
    return ( is_chi_low(action_type) || is_chi_middle(action_type) || is_chi_high(action_type) );
}

Tsumo_Edge::Tsumo_Edge() {
}

Tsumo_Edge::Tsumo_Edge(const int sg, const uint8_t hi, const uint8_t ho, const int dg) {
	src_group = sg;
	hai_in = hi;
	hai_out = ho;
	dst_group = dg;
}


Tehai_Action::Tehai_Action(){
}

void Tehai_Action::out_info() const {
	if (!out_console) return;
	std::cout << "hai:" << int(hai) << "  action_type:" << action_type << "  hai_out:" << int(hai_out) << std::endl;
	std::cout << "dst_group:" << int(dst_group) << " dst_group_sub:" << int(dst_group_sub) << std::endl;
}
