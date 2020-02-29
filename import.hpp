#pragma once

#include "share/include.hpp"
#include "share/types.hpp"

#ifdef WINSTD
__declspec(dllimport) std::vector<json11::Json> ai(const Moves& game_record, const int pid, const bool out_console_input);
__declspec(dllimport) void set_tactics(const json11::Json& setup_match_json);
__declspec(dllimport) void set_tactics_one(const json11::Json& setup_match_json);
#else
std::vector<json11::Json> ai(const Moves& game_record, const int pid, const bool out_console_input);
void set_tactics(const json11::Json& setup_match_json);
void set_tactics_one(const json11::Json& setup_match_json);
#endif
