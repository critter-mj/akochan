#pragma once

#include "share/include.hpp"
#include "share/types.hpp"

#ifdef WINSTD
__declspec(dllimport) std::vector<json11::Json> ai(const Moves& game_record, const int pid, const bool out_console_input);
__declspec(dllimport) void set_tactics(const json11::Json& setup_match_json);
__declspec(dllimport) void set_tactics_one(const json11::Json& setup_match_json);
__declspec(dllimport) std::vector<std::pair<Moves, float>> calc_moves_score(const Moves& game_record, const int pid);
__declspec(dllimport) json11::Json ai_review(const Moves& game_record, const int pid);
__declspec(dllimport) void ai_test();
#else
std::vector<json11::Json> ai(const Moves& game_record, const int pid, const bool out_console_input);
void set_tactics(const json11::Json& setup_match_json);
void set_tactics_one(const json11::Json& setup_match_json);
std::vector<std::pair<Moves, float>> calc_moves_score(const Moves& game_record, const int pid);
json11::Json ai_review(const Moves& game_record, const int pid);
void ai_test();
#endif
