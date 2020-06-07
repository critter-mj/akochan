#pragma once

#include "include.hpp"
#include "types.hpp"
#include "agari_ten.hpp"
#include "calc_shanten.hpp"

json11::Json make_start_game();
json11::Json make_start_game_with_initial_condition(const int kyoku, const std::array<int, 4>& scores);
json11::Json make_start_kyoku(const int bakaze, const int kyoku, const int honba, const int kyotaku, const std::array<int, 4>& scores); // pseudo game 用
json11::Json make_start_kyoku(const int bakaze, const int kyoku, const int honba, const int kyotaku, const int oya,
                              const std::array<std::array<int, 13>, 4>& tehais, const std::array<int, 4>& scores, const std::vector<int>& haiyama);
json11::Json make_none(const int actor);
json11::Json make_tsumo(const int actor, const int hai);

json11::Json make_dahai(const int actor, const int hai, const bool tsumogiri);

json11::Json make_pon(const int actor, const int target, const int hai, const std::vector<int> consumed);
json11::Json make_pon_default(const int actor, const int target, const int hai);
json11::Json make_pon_aka(const int actor, const int target, const int hai);

json11::Json make_daiminkan(const int actor, const int target, const int hai, const std::vector<int> consumed);
json11::Json make_daiminkan_default(const int actor, const int target, const int hai);
json11::Json make_daiminkan_aka(const int actor, const int target, const int hai);

json11::Json make_ankan(const int actor, const std::vector<int> consumed);
json11::Json make_ankan_default(const int actor, const int hai);
json11::Json make_ankan_aka(const int actor, const int hai);

json11::Json make_kakan(const int actor, const int hai, const std::vector<int> consumed);
json11::Json make_kakan_default(const int actor, const int hai);
json11::Json make_kakan_aka(const int actor, const int hai);

json11::Json make_chi(const int actor, const int target, const int hai, const std::vector<int> consumed);
json11::Json make_chi_low_default(const int actor, const int target, const int hai);
json11::Json make_chi_low_aka(const int actor, const int target, const int hai);
json11::Json make_chi_middle_default(const int actor, const int target, const int hai);
json11::Json make_chi_middle_aka(const int actor, const int target, const int hai);
json11::Json make_chi_high_default(const int actor, const int target, const int hai);
json11::Json make_chi_high_aka(const int actor, const int target, const int hai);

json11::Json make_hora(const int actor, const int target, const int hai_hora);
json11::Json make_hora(const int actor, const int target, const int hai_hora, const Hai_Array& tehai, const int han, const std::array<int, 4>& scores);
json11::Json make_ryukyoku_fanpai(const std::array<bool, 4>& tenpai, const std::array<json11::Json, 4>& tehais, const std::array<int, 4>& scores);
json11::Json make_kyushukyuhai(const int actor);
json11::Json make_kyushukyuhai(const int actor, const Hai_Array& tehai, const std::array<int, 4>& scores);

json11::Json make_end_kyoku();
json11::Json make_end_game(const std::array<int, 4>& scores);

bool is_valid_start_kyoku(const json11::Json& action_json);
bool is_valid_dahai(const json11::Json& action_json);
bool is_valid_chi(const json11::Json& action_json);
bool is_valid_pon(const json11::Json& action_json);
bool is_valid_daiminkan(const json11::Json& action_json);
bool is_valid_ankan(const json11::Json& action_json);
bool is_valid_kakan(const json11::Json& action_json);

bool is_valid_hora(const json11::Json& action_json);

bool is_valid_pon_and_dahai(const Moves& moves);
bool is_valid_chi_and_dahai(const Moves& moves);

bool is_valid_game_record(const Moves& game_record);

bool is_legal_dahai(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);
bool is_legal_chi(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);
bool is_legal_pon(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);
bool is_legal_daiminkan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);
bool is_legal_ankan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);
bool is_legal_kakan(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);

bool is_legal_hora(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);

bool is_legal_pon_and_dahai(const Moves& game_record, const Game_State& game_state, const Moves& moves);
bool is_legal_chi_and_dahai(const Moves& game_record, const Game_State& game_state, const Moves& moves);

bool is_legal_kyushukyuhai(const Moves& game_record, const Game_State& game_state, const json11::Json& action_json);

bool is_legal_none(const Moves& game_record, const json11::Json& action_json);
bool is_legal_single_move(const Moves& game_record, const json11::Json& action_json);
bool is_legal_dahai_after_fuuro(const Moves& game_record, const json11::Json& action_json);

std::vector<Moves> get_legal_tsumogiri_move(const Moves& game_record);
std::vector<Moves> get_legal_tedashi_move(const Moves& game_record);
std::vector<Moves> get_legal_pon_dahai_move(const Moves& game_record);
std::vector<Moves> get_legal_chi_dahai_move(const Moves& game_record);
std::vector<Moves> get_legal_daiminkan_move(const Moves& game_record);
std::vector<Moves> get_legal_ankan_move(const Moves& game_record);
std::vector<Moves> get_legal_kakan_move(const Moves& game_record);
std::vector<Moves> get_legal_ron_move(const Moves& game_record);
std::array<std::vector<Moves>, 4> get_all_legal_moves(const Moves& game_record);
std::vector<json11::Json> get_all_legal_single_action(const Moves& game_record);