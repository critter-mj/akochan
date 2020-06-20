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
    MT_OTHER = 5, // Knitted Straight, Greater Honors and Knitted Tiles のような特殊役
};

enum Hora_Type {
    HT_DEFAULT = 0,
    HT_TITOI = 1,
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
    Machi_Type machi_type;
    int han_tsumo;
    int han_ron;

    Agari_Info();
};

class Agari_Result {
    public:
    // 88 points
    bool big_four_winds, big_three_dragons, four_kongs;
    // 64 points
    bool four_concealed_pungs;
    // 32 points
    bool three_kongs;
    // 24 points
    bool seven_pairs, all_even_pungs, full_flush, pure_triple_chow, pure_shifted_pungs, upper_tiles, middle_tiles, lower_tiles; // Greater Honors and Knitted Tiles
    // 16 points
    bool pure_straight, three_suited_terminal_chows, pure_shifted_chows, all_fives, triple_pung, three_concealed_pungs;
    // 12 points
    bool upper_four, lower_four, big_three_winds; // lesser_honors_and_knitted_tiles, knitted_straight;
    // 8 points
    bool mixed_straight, reversible_tiles, mixed_triple_chow, mixed_shifted_pungs, two_concealed_kongs; //, chicken_hand, last_tile_draw, last_tile_claim, out_with_replacement_tile, robbing_the_kongs;
    // 6 points
    bool all_pungs, half_flush, mixed_shifted_chows, all_types, melded_hand, two_dragon_pungs;
    // 4 points
    bool outside_hand, fully_concealed, two_melded_kongs; // , last_tile
    // 2 points bool
    bool dragon_pung, prevalent_wind, seat_wind, all_chows, two_concealed_pungs, concealed_kong, all_simples, concealed_hand;
    // 1 point bool
    bool melded_kong, one_voided_suit, no_honors, edge_wait, closed_wait, single_wait, self_drawn;
    // 2 points int
    int tile_hog_num, double_pung_num;
    // 1 point int
    int pure_double_chow_num, mixed_double_chow_num, short_straight_num, two_terminal_chows_num, pung_of_terminals_or_honors_num;

    Agari_Result();
    int calc_point() const;
    json11::Json to_json() const;
};

class Agari_Info_Detail {
    public:
    int hai;
    Machi_Type machi_type;
    Agari_Result result_tsumo;
    Agari_Result result_ron;

    Agari_Info_Detail();
    Agari_Info_Detail(const int hai_in, const Machi_Type machi_type_in, const Agari_Result& result_tsumo_in, const Agari_Result& result_ron_in);
};

class Tenpai_Info {
    public:
    int mentu_shanten_num;
    int titoi_shanten_num;
    std::vector<Agari_Info_Detail> agari_vec;

    Tenpai_Info();
    int shanten_num() const;
    void check_wait(); // calculation of Edge Wait, Closed Wait, Single Wait
};

class Sutehai {
	public:
	int hai;
    bool tsumogiri;

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