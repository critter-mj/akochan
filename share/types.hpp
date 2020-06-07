#pragma once

#include "include.hpp"

int next_player(const int pid, const int arg);

enum Color_Type {
    CT_MANZU = 0,
    CT_PINZU = 1,
    CT_SOZU = 2,
    CT_JIHAI = 3,
    CT_NUM = 4,
};
ENABLE_ENUM_OPERATORS(Color_Type)

enum Num_Type {
    NT_CHUJAN = 0,
    NT_SUYAO = 1,
    NT_JIHAI = 2,
};
ENABLE_ENUM_OPERATORS(Num_Type)

enum Fuuro_Type {
	FT_CHI = 1,
	FT_PON = 2, // ポン
	FT_DAIMINKAN = 3, // 大明カン
	FT_ANKAN = 4, // 暗カン
	FT_KAKAN = 5, // 加カン
};

enum Machi_Type {
    MT_RYANMEN = 0,
    MT_SHABO = 1,
    MT_TANKI = 2,
    MT_KANCHAN = 3,
    MT_PENCHAN = 4,
};

Color_Type hai_color(const int hai);
Num_Type hai_yaochuu(const int hai);

class Fuuro_Elem {
	public:
	Fuuro_Type type;
	int hai;
	std::vector<int> consumed;
    int target_relative; // 1. 下家、2:対面、3:上家、相対位置で記憶させておくと画像表示する際に都合がよい。

    Fuuro_Elem();
};

class Agari_Info {
    public:
    int hai;
    int han_tsumo;
    int han_ron;

    Agari_Info();
};

class Tenpai_Info {
    public:
    int mentu_shanten_num;
    int titoi_shanten_num;
    std::vector<Agari_Info> agari_vec;

    Tenpai_Info();
    int shanten_num() const;
};

class Sutehai {
	public:
	int hai;
    bool tsumogiri;
	bool is_reach;

	Sutehai();
};

using Hai_Array = std::array<int, 38>;
using Fuuro_Vector = std::vector<Fuuro_Elem>;
using Kawa = std::vector<Sutehai>;

json11::Json hai_array_to_json(const Hai_Array& hai_array);

void replace_pon_to_kakan(Fuuro_Vector& fuuro_vector, const Fuuro_Elem fuuro_elem);

class Player_State {
    public:
    int score;
    int jikaze; // 0,1,2,3 : 東南西北
    Hai_Array tehai;
    Fuuro_Vector fuuro;
    Kawa kawa;
    bool reach_declared;
    bool reach_accepted;

    Player_State();
    void reset_tehai_state();
    bool is_menzen() const;
};

class Game_State {
    public:
    int bakaze; // 0,1,2,3 : 東南西北
    int kyoku; // 1,2,3,4
    int honba;
    int kyotaku;
    std::vector<int> dora_marker;
    Player_State player_state[4];

    Game_State();
    void reset_all_tehai_state();
    void set_all_jikaze(const int oya);
};

class Player_Result {
	public:
	Player_Result(int pid_in, int score_in, int jikaze_first_in);
	bool operator<(const Player_Result& rhs) const;
	int pid;
	int score;
	int jikaze_first;
};

using Moves = std::vector<json11::Json>;

std::string moves_to_string(const Moves& moves);
void moves_to_file(const Moves& moves, std::string file_name);

bool is_last_kyoku(const int bakaze, const int kyoku, const std::string& rule);
bool is_definite_last_kyoku(const int bakaze, const int kyoku, const std::string& rule);

bool is_valid_player(const int pid);

int haikind(const int hai); // 牌種を返す関数。10,20,30がそれぞれマンズ、ピンズ、ソーズの赤5牌という想定。
Hai_Array haikind(const Hai_Array& tehai);
Fuuro_Elem haikind(Fuuro_Elem elem);
Fuuro_Vector haikind(Fuuro_Vector fuuro);

int dora_to_dora_marker(const int hai);
int dora_marker_to_dora(const int hai);
std::vector<int> dora_marker_to_dora(const std::vector<int>& dora_marker);

int count_dora(const Hai_Array& Hai_Array, const Fuuro_Vector& fuuro, const std::vector<int>& dora_marker);
int count_dora(const Hai_Array& Hai_Array, const Fuuro_Vector& fuuro, const std::vector<int>& dora_marker, const std::vector<int>& uradora_marker);

int get_hai38(const int hai136);

int kaze_str_to_int(const std::string kaze_str);
std::string kaze_int_to_str(const int kaze);

bool is_valid_hai(const int hai);
int hai_str_to_int(const std::string hai_str);
std::string hai_int_to_str(const int hai);

int ceil_fu(const int fu);

Game_State get_game_state_start_kyoku(const json11::Json& action_json);
void go_next_state(Game_State& game_state, const json11::Json& action_json);
Game_State get_game_state(const Moves& game_record);

bool is_reach_accepted(const Moves& game_record, const int pid);
bool is_ippatsu_valid(const Moves& game_record, const int pid);
std::pair<int, int> count_tsumo_num(const Moves& game_record);
int count_tsumo_num_all(const Moves& game_record);

int get_kyoku_first(const Moves& game_record);
bool get_aka_flag(const Moves& game_record);

int get_bakaze(const Moves& game_record);
int get_kyoku(const Moves& game_record);
int get_oya(const Moves& game_record);
int get_honba(const Moves& game_record);
int get_kyotaku(const Moves& game_record);

bool is_tobi_any(const std::array<int, 4>& scores);
bool is_tobi_any(const json11::Json& kyoku_result_move);
bool is_30000_any(const std::array<int, 4>& scores);
bool is_30000_any(const json11::Json& kyoku_result_move);
bool has_highest_score(const std::array<int, 4>& scores, const int pid);
bool has_highest_score(const json11::Json& kyoku_result_move, const int pid);

std::pair<int, int> cal_next_bakaze_kyoku(const Moves& game_record);
int cal_next_honba(const Moves& game_record);
int cal_next_oya(const Moves& game_record);

bool is_menzen(const Moves& game_record, const int pid);
std::array<bool, 38> get_furiten_flags(const Moves& game_record, const Game_State& game_state, const int pid, const bool skip_latest);

std::vector<json11::Json> load_game_record_from_file(const std::string& file_name, int length);