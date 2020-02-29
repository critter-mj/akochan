#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "../share/agari_ten.hpp"
#include "../share/make_move.hpp"

#include "mjutil.hpp"
#include "jun_calc.hpp"
#include "tehai_action.hpp"
#include "tehai_cal.hpp"
#include "exp_values.hpp"

class Hai_Choice {
	public:
	Action_Type action_type;
	// ツモした時の行動を入れる想定、AT_DAHAI, AT_REACH_DECLARE, AT_ANKAN, AT_KAKAN, AT_TSUMO_AGARI, AT_KYUSHUKYUHAIのいずれか。
	// AT_REACH_DECLAREの場合、打牌もセットにする。
	int hai;
	float pt_exp_after;
	float pt_exp_total;
	float pt_exp_after_ori;

	Hai_Choice();
	bool operator<(const Hai_Choice& rhs) const;
	int is_choice_reach_declare();
	Moves out_moves(const Game_State& game_state, const int my_pid, const int tsumo_hai) const;
};

class Fuuro_Choice {
	public:

	int fuuro_hai;
	Action_Type fuuro_action_type;
	int hai_out;
	int sarashi_hai[3];

	float pt_exp_after;
	float pt_exp_after_prev;
	float pt_exp_total;
	float pt_exp_total_prev;

	Fuuro_Choice();
	void reset();
	bool operator<(const Fuuro_Choice& rhs) const;
	Moves out_moves(const int my_pid, const int target) const;
};

std::array<std::array<std::array<std::array<float, 12>, 14>, 4>, 4> cal_kyoku_end_pt_exp(const Moves& game_record, const Game_State& game_state, const int my_pid, const bool reach_mode);
std::array<std::array<std::array<std::array<float, 2>, 2>, 2>, 2> cal_ryuukyoku_pt_exp(const Moves& game_record, const Game_State& game_state, const int my_pid, const bool reach_mode);
int cal_tsumo_num_DP(const Moves& game_record, const int my_pid);

void set_sarashi_hai(const Tehai_State2& dst_tehai_state, const Fuuro_Vector& present_fuuro, const int current_hai, int sarashi_hai[3]);
int koritu_most_needless(const Hai_Array& tehai, const Hai_Array& visible, const int bakaze, const int jikaze, const std::vector<int>& dora_marker);

class Selector{
	public:
	std::vector<Hai_Choice> hai_choice;
	std::vector<Fuuro_Choice> fuuro_choice;

	Selector();
	void set_selector(const Moves& game_record, const int my_pid, const Tactics& tactics);
};

#ifdef WINSTD
__declspec(dllexport)
#endif 
Moves ai(const Moves& game_record, const int pid, const bool out_console_input);

#ifdef WINSTD
__declspec(dllexport)
#endif
std::vector<std::pair<Moves, float>> calc_moves_score(const Moves& game_record, const int pid);

#ifdef WINSTD
__declspec(dllexport)
#endif
void set_tactics(const json11::Json& match_setup_json);

#ifdef WINSTD
__declspec(dllexport)
#endif
void set_tactics_one(const json11::Json& match_setup_json);