#include "types.hpp"

int next_player(const int pid, const int arg) {
	return (pid + arg) % 4;
}

Color_Type hai_color(const int hai) {
	return (Color_Type) ((hai - 1) / 10);
}

Num_Type hai_yaochuu(const int hai) {
	if (hai <= 30) {
		if (hai % 10 == 1 || hai % 10 == 9) {
			return NT_SUYAO;
		} else {
			return NT_CHUJAN;
		}
	} else {
		return NT_JIHAI;
	}
}

Fuuro_Elem::Fuuro_Elem() {
	target_relative = -1;
}

Agari_Info::Agari_Info(){}

Agari_Result::Agari_Result() {
    // 88 points
    big_four_winds = false; big_three_dragons = false, four_kongs = false;
    // 64 points
    four_concealed_pungs = false;
    // 32 points
    three_kongs = false;
    // 24 points
    seven_pairs = false; all_even_pungs = false; full_flush = false; pure_triple_chow = false; pure_shifted_pungs = false; upper_tiles = false; middle_tiles = false; lower_tiles = false; // Greater Honors and Knitted Tiles
    // 16 points
    pure_straight = false; three_suited_terminal_chows = false; pure_shifted_chows = false; all_fives = false; triple_pung = false; three_concealed_pungs = false;
    // 12 points
    upper_four = false; lower_four = false; big_three_winds = false; // lesser_honors_and_knitted_tiles, knitted_straight;
    // 8 points
    mixed_straight = false; reversible_tiles = false; mixed_triple_chow = false; mixed_shifted_pungs = false; two_concealed_kongs = false; //, chicken_hand, last_tile_draw, last_tile_claim, out_with_replacement_tile, robbing_the_kongs;
    // 6 points
    all_pungs = false; half_flush = false; mixed_shifted_chows = false; all_types = false; melded_hand = false; two_dragon_pungs = false;
    // 4 points
    outside_hand = false; fully_concealed = false; two_melded_kongs = false; // , last_tile
    // 2 points bool
    dragon_pung = false; prevalent_wind = false; seat_wind = false; all_chows = false; two_concealed_pungs = false; concealed_kong = false; all_simples = false, concealed_hand = false;
    // 1 point bool
    melded_kong = false; one_voided_suit = false; no_honors = false;
    // 2 points int
    tile_hog_num = 0; double_pung_num = 0;
    // 1 point int
    pure_double_chow_num = 0; mixed_double_chow_num = 0; short_straight_num = 0; two_terminal_chows_num = 0; pung_of_terminals_or_honors_num = 0;
}

int Agari_Result::calc_point() const {
    int ret = 0;
    if (big_four_winds) { ret += 88; }
    if (big_three_dragons) { ret += 88; }
    if (four_kongs) { ret += 88; }
    if (four_concealed_pungs) { ret += 64; }
    if (three_kongs) { ret += 32; }
    if (seven_pairs) { ret += 24; }
    if (all_even_pungs) { ret += 24; }
    if (full_flush) { ret += 24; }
    if (pure_triple_chow) { ret += 24; }
    if (pure_shifted_pungs) { ret += 24; }
    if (upper_tiles) { ret += 24; }
    if (middle_tiles) { ret += 24; }
    if (lower_tiles) { ret += 24; }
    if (pure_straight) { ret += 16; }
    if (three_suited_terminal_chows) { ret += 16; }
    if (pure_shifted_chows) { ret += 16; }
    if (all_fives) { ret += 16; }
    if (triple_pung) { ret += 16; }
    if (three_concealed_pungs) { ret += 16; }
    if (upper_four) { ret += 12; }
    if (lower_four) { ret += 12; }
    if (big_three_winds) { ret += 12; }
    if (mixed_straight) { ret += 8; }
    if (reversible_tiles) { ret += 8; }
    if (mixed_triple_chow) { ret += 8; }
    if (mixed_shifted_pungs) { ret += 8; }
    if (two_concealed_pungs) { ret += 8; }
    if (all_pungs) { ret += 6; }
    if (half_flush) { ret += 6; }
    if (mixed_shifted_chows) { ret += 6; }
    if (all_types) { ret += 6; }
    if (melded_hand) { ret += 6; }
    if (two_dragon_pungs) { ret += 6; }
    if (outside_hand) { ret += 4; }
    if (fully_concealed) { ret += 4; }
    if (two_melded_kongs) { ret += 4; }
    if (dragon_pung) { ret += 2; }
    if (prevalent_wind) { ret += 2; }
    if (seat_wind) { ret += 2; }
    if (all_chows) { ret += 2; }
    if (two_concealed_pungs) { ret += 2; }
    if (concealed_kong) { ret += 2; }
    if (all_simples) { ret += 2; }
    if (concealed_hand) { ret += 2; }
    if (melded_kong) { ret += 1; }
    if (one_voided_suit) { ret += 1; }
    if (no_honors) { ret += 1; }
    ret += (tile_hog_num + double_pung_num) * 2;
    ret += pure_double_chow_num + mixed_double_chow_num + short_straight_num + two_terminal_chows_num + pung_of_terminals_or_honors_num;
    return ret;
}

json11::Json Agari_Result::to_json() const {
    json11::Json::array bools;
    if (big_four_winds) { bools.push_back("big_four_winds"); }
    if (big_three_dragons) { bools.push_back("big_three_dragons"); }
    if (four_kongs) { bools.push_back("four_kongs"); }
    if (four_concealed_pungs) { bools.push_back("four_concealed_pungs"); }
    if (three_kongs) { bools.push_back("three_kongs"); }
    if (seven_pairs) { bools.push_back("seven_pairs"); }
    if (all_even_pungs) { bools.push_back("all_even_pungs"); }
    if (full_flush) { bools.push_back("full_flush"); }
    if (pure_triple_chow) { bools.push_back("pure_triple_chow"); }
    if (pure_shifted_pungs) { bools.push_back("pure_shifted_pungs"); }
    if (upper_tiles) { bools.push_back("upper_tiles"); }
    if (middle_tiles) { bools.push_back("middle_tiles"); }
    if (lower_tiles) { bools.push_back("lower_tiles"); }
    if (pure_straight) { bools.push_back("pure_straight"); }
    if (three_suited_terminal_chows) { bools.push_back("three_suited_terminal_chows"); }
    if (pure_shifted_chows) { bools.push_back("pure_shifted_chows"); }
    if (all_fives) { bools.push_back("all_fives"); }
    if (triple_pung) { bools.push_back("triple_pung"); }
    if (three_concealed_pungs) { bools.push_back("three_concealed_pungs"); }
    if (upper_four) { bools.push_back("upper_four"); }
    if (lower_four) { bools.push_back("lower_four"); }
    if (big_three_winds) { bools.push_back("big_three_winds"); }
    if (mixed_straight) { bools.push_back("mixed_straight"); }
    if (reversible_tiles) { bools.push_back("reversible_tiles"); }
    if (mixed_triple_chow) { bools.push_back("mixed_triple_chow"); }
    if (mixed_shifted_pungs) { bools.push_back("mixed_shifted_pungs"); }
    if (two_concealed_pungs) { bools.push_back("two_concealed_pungs"); }
    if (all_pungs) { bools.push_back("all_pungs"); }
    if (half_flush) { bools.push_back("half_flush"); }
    if (mixed_shifted_chows) { bools.push_back("mixed_shifted_chows"); }
    if (all_types) { bools.push_back("all_types"); }
    if (melded_hand) { bools.push_back("melded_hand"); }
    if (two_dragon_pungs) { bools.push_back("two_dragon_pungs"); }
    if (outside_hand) { bools.push_back("outside_hand"); }
    if (fully_concealed) { bools.push_back("fully_concealed"); }
    if (two_melded_kongs) { bools.push_back("two_melded_kongs"); }
    if (dragon_pung) { bools.push_back("dragon_pung"); }
    if (prevalent_wind) { bools.push_back("prevalent_wind"); }
    if (seat_wind) { bools.push_back("seat_wind"); }
    if (all_chows) { bools.push_back("all_chows"); }
    if (two_concealed_pungs) { bools.push_back("two_concealed_pungs"); }
    if (concealed_kong) { bools.push_back("concealed_kong"); }
    if (all_simples) { bools.push_back("all_simples"); }
    if (concealed_hand) { bools.push_back("concealed_hand"); }
    if (melded_kong) { bools.push_back("melded_kong"); }
    if (one_voided_suit) { bools.push_back("one_voided_suit"); }
    if (no_honors) { bools.push_back("no_honors"); }

    json11::Json::object integers;
    if (0 < tile_hog_num) { integers["tile_hog"] = tile_hog_num; }
    if (0 < double_pung_num) { integers["double_pung"] = double_pung_num; }
    if (0 < pure_double_chow_num) { integers["pure_double_chow"] = pure_double_chow_num; }
    if (0 < mixed_double_chow_num) { integers["mixed_double_chow"] = mixed_double_chow_num; }
    if (0 < short_straight_num) { integers["short_straight"] = short_straight_num; }
    if (0 < two_terminal_chows_num) { integers["two_terminal_chows"] = two_terminal_chows_num; }
    if (0 < pung_of_terminals_or_honors_num) { integers["pung_of_terminals_or_honors"] = pung_of_terminals_or_honors_num; }

    json11::Json::object ret;
    ret["total_point"] = calc_point();
    ret["bools"] = bools;
    ret["integers"] = integers;
    return json11::Json(ret);
}

Agari_Info_Detail::Agari_Info_Detail(){}
Agari_Info_Detail::Agari_Info_Detail(const int hai_in, const Agari_Result& result_tsumo_in, const Agari_Result& result_ron_in){
    hai = hai_in;
    result_tsumo = result_tsumo_in;
    result_ron = result_ron_in;
}

Tenpai_Info::Tenpai_Info() {
	mentu_shanten_num = 8;
    titoi_shanten_num = 6; // 副露手では別の値にするべきかもしれないが、特に不都合はでないはず。
    agari_vec.clear();
}

int Tenpai_Info::shanten_num() const {
	return std::min(mentu_shanten_num, titoi_shanten_num);
}

Sutehai::Sutehai(){}

json11::Json hai_array_to_json(const Hai_Array& hai_array) {
    json11::Json::array arr;
    for (int hai = 1; hai < 38; hai++) {
        if (hai % 10 == 0) {
            continue;
        }
        for (int i = 0; i < hai_array[hai]; i++) {
            arr.push_back(hai_int_to_str(hai));
        }
        if (hai % 10 == 5 && hai < 30) {
            for (int i = 0; i < hai_array[hai+5]; i++) {
                arr.push_back(hai_int_to_str(hai+5));
            }
        }
    }
    return json11::Json(arr);
}

Player_State::Player_State(){}

void Player_State::reset_tehai_state() {
	for (int hai = 0; hai < 38; hai++) {
		tehai[hai] = 0;
	}
	fuuro.clear();
	kawa.clear();
}

void replace_pon_to_kakan(Fuuro_Vector& fuuro_vector, const Fuuro_Elem fuuro_elem) {
	assert(fuuro_elem.type == FT_KAKAN);
	for (int i = 0; i < fuuro_vector.size(); i++) {
		if (fuuro_vector[i].type == FT_PON && haikind(fuuro_vector[i].hai) == haikind(fuuro_elem.hai)) {
			fuuro_vector[i].type = FT_KAKAN;
			fuuro_vector[i].hai = fuuro_elem.hai;
			fuuro_vector[i].consumed = fuuro_elem.consumed;
		}
	}
}

bool Player_State::is_menzen() const {
	for (int i = 0; i < fuuro.size(); i++) {
		if (fuuro[i].type != FT_ANKAN) {
			return false;
		}
	}
	return true;
}

Game_State::Game_State(){}

void Game_State::reset_all_tehai_state() {
	for (int pid = 0; pid < 4; pid++) {
		player_state[pid].reset_tehai_state();
	}
}

void Game_State::set_all_jikaze(const int oya) {
	for (int pid = 0; pid < 4; pid++) {
		player_state[pid].jikaze = (4 + pid - oya) % 4;
	}
}

Player_Result::Player_Result(int pid_in, int score_in, int jikaze_first_in) {
	pid = pid_in;
	score = score_in;
	jikaze_first = jikaze_first_in;
}

bool Player_Result::operator<(const Player_Result& rhs) const {
	if (score > rhs.score) {
		return true;
	} else if (score == rhs.score && jikaze_first < rhs.jikaze_first) {
		return true;
	} else {
		return false;
	}
}

bool is_last_kyoku(const int bakaze, const int kyoku, const std::string& rule) {
    if (rule == "tonpu") {
        return bakaze == 1 || (bakaze == 0 && kyoku == 4);
    } else if (rule == "tonnan") {
        return bakaze == 2 || (bakaze == 1 && kyoku == 4);
    } else {
        assert_with_out(false, "is_last_kyoku error!");
        return false;
    }
	
}


bool is_definite_last_kyoku(const int bakaze, const int kyoku, const std::string& rule) {
    if (rule == "tonpu") {
        return bakaze == 1 && kyoku == 4;
    } else if (rule == "tonnan") {
        return bakaze == 2 && kyoku == 4;
    } else {
        assert_with_out(false, "is_definite_last_kyoku error!");
        return false;
    }
	
}

std::string moves_to_string(const Moves& moves) {
    std::string result;
    for (int i = 0; i < moves.size(); i++) {
		if (i == 0) {
			result += "[";
		} else {
			result += ",";
		}
        result += moves[i].dump();
		if (i == moves.size() - 1) {
			result += "]";
		}
    }
    return result;
}

void moves_to_file(const Moves& moves, std::string file_name) {
	std::ofstream outputfile(file_name);
	for (int i = 0; i < moves.size(); i++) {
    	outputfile << moves[i].dump() << std::endl;
	}
    outputfile.close();
}

bool is_valid_player(const int pid) {
	return 0 <= pid && pid <= 3;
}

int haikind(const int hai) {
	if (hai % 10 == 0) {
		return hai - 5;
	} else {
		return hai;
	}
}

Hai_Array haikind(const Hai_Array& tehai) {
	Hai_Array tehai_kind = {};
	for (int hai = 1; hai < 38; hai++) {
		tehai_kind[haikind(hai)] += tehai[hai];
	}
	return tehai_kind;
}

Fuuro_Elem haikind(Fuuro_Elem elem) {
	if (elem.type != FT_ANKAN) {
		elem.hai = haikind(elem.hai);
	}
	for (int i = 0; i < elem.consumed.size(); i++) {
		elem.consumed[i] = haikind(elem.consumed[i]);
	}
	return elem;
}

Fuuro_Vector haikind(Fuuro_Vector fuuro) {
	for (int i = 0; i < fuuro.size(); i++) {
		fuuro[i] = haikind(fuuro[i]);
	}
	return fuuro;
}

int get_hai38(const int hai136) {
	//if (hai136 == 16) {
	//	return 10;
	//} else if(hai136 == 52) {
	//	return 20;
	//} else if(hai136 == 88) {
	//	return 30;
	//} else {
		int haic = hai136/36;
		int hain = (hai136%36)/4 + 1;
		return 10*haic + hain;
	//}
}

int kaze_str_to_int(const std::string kaze_str) {
	if (kaze_str == "E") { return 0; }
	else if(kaze_str == "S") { return 1; }
	else if(kaze_str == "W") { return 2; }
	else if(kaze_str == "N") { return 3; }
	else {
		assert_with_out(false, "kaze_str_to_int invalid input");
        return -1;
	}
}

std::string kaze_int_to_str(const int kaze) {
	std::string kaze_str = "";
	if (kaze == 0) { kaze_str = "E"; }
	else if (kaze == 1) { kaze_str = "S"; }
	else if (kaze == 2) { kaze_str = "W"; }
	else if (kaze == 3) { kaze_str = "N"; }
	else {
		assert_with_out(false, "kaze_int_to_str invalid input");
	}
	return kaze_str;
}

bool is_valid_hai(const int hai) {
    return 0 < hai && hai < 38;
}

int hai_str_to_int(const std::string hai_str){
	if(hai_str == "1m"){ return 1;}
	else if(hai_str == "2m"){ return 2;}
	else if(hai_str == "3m"){ return 3;}
	else if(hai_str == "4m"){ return 4;}
	else if(hai_str == "5m"){ return 5;}
	else if(hai_str == "6m"){ return 6;}
	else if(hai_str == "7m"){ return 7;}
	else if(hai_str == "8m"){ return 8;}
	else if(hai_str == "9m"){ return 9;}
	else if(hai_str == "5mr"){ return 10;}
	else if(hai_str == "1p"){ return 11;}
	else if(hai_str == "2p"){ return 12;}
	else if(hai_str == "3p"){ return 13;}
	else if(hai_str == "4p"){ return 14;}
	else if(hai_str == "5p"){ return 15;}
	else if(hai_str == "6p"){ return 16;}
	else if(hai_str == "7p"){ return 17;}
	else if(hai_str == "8p"){ return 18;}
	else if(hai_str == "9p"){ return 19;}
	else if(hai_str == "5pr"){ return 20;}
	else if(hai_str == "1s"){ return 21;}
	else if(hai_str == "2s"){ return 22;}
	else if(hai_str == "3s"){ return 23;}
	else if(hai_str == "4s"){ return 24;}
	else if(hai_str == "5s"){ return 25;}
	else if(hai_str == "6s"){ return 26;}
	else if(hai_str == "7s"){ return 27;}
	else if(hai_str == "8s"){ return 28;}
	else if(hai_str == "9s"){ return 29;}
	else if(hai_str == "5sr"){ return 30;}
	else if(hai_str == "E"){ return 31;}
	else if(hai_str == "S"){ return 32;}
	else if(hai_str == "W"){ return 33;}
	else if(hai_str == "N"){ return 34;}
	else if(hai_str == "P"){ return 35;}
	else if(hai_str == "F"){ return 36;}
	else if(hai_str == "C"){ return 37;}
    else if(hai_str == "?"){ return -1;}
	else{
		assert_with_out(false, "hai_str_to_int invalid input");
        return 0;
	}
}

std::string hai_int_to_str(int hai_int){
	std::string hai_str = "";
	if(hai_int == 1){ hai_str = "1m";}
	else if(hai_int == 2){ hai_str = "2m";}
	else if(hai_int == 3){ hai_str = "3m";}
	else if(hai_int == 4){ hai_str = "4m";}
	else if(hai_int == 5){ hai_str = "5m";}
	else if(hai_int == 6){ hai_str = "6m";}
	else if(hai_int == 7){ hai_str = "7m";}
	else if(hai_int == 8){ hai_str = "8m";}
	else if(hai_int == 9){ hai_str = "9m";}
	else if(hai_int == 10){ hai_str = "5mr";}
	else if(hai_int == 11){ hai_str = "1p";}
	else if(hai_int == 12){ hai_str = "2p";}
	else if(hai_int == 13){ hai_str = "3p";}
	else if(hai_int == 14){ hai_str = "4p";}
	else if(hai_int == 15){ hai_str = "5p";}
	else if(hai_int == 16){ hai_str = "6p";}
	else if(hai_int == 17){ hai_str = "7p";}
	else if(hai_int == 18){ hai_str = "8p";}
	else if(hai_int == 19){ hai_str = "9p";}
	else if(hai_int == 20){ hai_str = "5pr";}
	else if(hai_int == 21){ hai_str = "1s";}
	else if(hai_int == 22){ hai_str = "2s";}
	else if(hai_int == 23){ hai_str = "3s";}
	else if(hai_int == 24){ hai_str = "4s";}
	else if(hai_int == 25){ hai_str = "5s";}
	else if(hai_int == 26){ hai_str = "6s";}
	else if(hai_int == 27){ hai_str = "7s";}
	else if(hai_int == 28){ hai_str = "8s";}
	else if(hai_int == 29){ hai_str = "9s";}
	else if(hai_int == 30){ hai_str = "5sr";}
	else if(hai_int == 31){ hai_str = "E";}
	else if(hai_int == 32){ hai_str = "S";}
	else if(hai_int == 33){ hai_str = "W";}
	else if(hai_int == 34){ hai_str = "N";}
	else if(hai_int == 35){ hai_str = "P";}
	else if(hai_int == 36){ hai_str = "F";}
	else if(hai_int == 37){ hai_str = "C";}
	else{
		assert_with_out(false, "hai_int_to_str invalid_input");
	}
	return hai_str;
}

int ceil_fu(const int fu) {
    if (fu % 10 == 0 || fu == 25) {
        return fu;
    } else {
        return fu + 10 - (fu % 10);
    }
}

Game_State get_game_state_start_kyoku(const json11::Json& action_json) {
    Game_State game_state;
    assert(action_json["type"].string_value() == "start_kyoku");
    game_state.reset_all_tehai_state();
    json11::Json::array haipai_array = action_json["tehais"].array_items();
    json11::Json::array score_array = action_json["scores"].array_items();
    const int oya_id = action_json["oya"].int_value();
    game_state.bakaze = kaze_str_to_int(action_json["bakaze"].string_value());
    game_state.kyoku = action_json["kyoku"].int_value();
    game_state.honba = action_json["honba"].int_value();
    game_state.kyotaku = action_json["kyotaku"].int_value();

    assert(haipai_array.size() == 4);
    for (int pid = 0; pid < 4; pid++) {
        game_state.player_state[pid].jikaze = (4 + pid - oya_id) % 4;
        game_state.player_state[pid].score = score_array[pid].int_value();
        json11::Json::array haipai_one_player = haipai_array[pid].array_items();
        for(int i = 0; i < 13; i++) {
            const int hai = hai_str_to_int(haipai_one_player[i].string_value());
            if (is_valid_hai(hai)) {
                game_state.player_state[pid].tehai[hai_str_to_int(haipai_one_player[i].string_value())]++;
            }
        }
    }
    return game_state;
}

void go_next_state(Game_State& game_state, const json11::Json& action_json) {
    if (action_json["type"].string_value() == "tsumo") {
        const int hai = hai_str_to_int(action_json["pai"].string_value());
        if (is_valid_hai(hai)) {
            const int actor = action_json["actor"].int_value();
            game_state.player_state[actor].tehai[hai]++;
        }
    } else if (action_json["type"].string_value() == "dahai") {
        const int hai = hai_str_to_int(action_json["pai"].string_value());
        const int actor = action_json["actor"].int_value();
        Sutehai sutehai;
        sutehai.hai = hai;
        sutehai.tsumogiri = action_json["tsumogiri"].bool_value();
        game_state.player_state[actor].kawa.push_back(sutehai);
        game_state.player_state[actor].tehai[hai]--;
    } else if (action_json["type"].string_value() == "chi") {
        const int actor = action_json["actor"].int_value();
        const int target = action_json["target"].int_value();
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][0].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][1].string_value())]--;
        Fuuro_Elem fuuro_elem;
        fuuro_elem.type = FT_CHI;
        fuuro_elem.hai = hai_str_to_int(action_json["pai"].string_value());
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
        fuuro_elem.target_relative = (4 + target - actor) % 4;
        game_state.player_state[actor].fuuro.push_back(fuuro_elem);
    } else if (action_json["type"].string_value() == "pon") {
        const int actor = action_json["actor"].int_value();
        const int target = action_json["target"].int_value();
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][0].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][1].string_value())]--;
        Fuuro_Elem fuuro_elem;
        fuuro_elem.type = FT_PON;
        fuuro_elem.hai = hai_str_to_int(action_json["pai"].string_value());
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
        fuuro_elem.target_relative = (4 + target - actor) % 4;
        game_state.player_state[actor].fuuro.push_back(fuuro_elem);
    } else if (action_json["type"].string_value() == "daiminkan") {
        const int actor = action_json["actor"].int_value();
        const int target = action_json["target"].int_value();
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][0].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][1].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][2].string_value())]--;
        Fuuro_Elem fuuro_elem;
        fuuro_elem.type = FT_DAIMINKAN;
        fuuro_elem.hai = hai_str_to_int(action_json["pai"].string_value());
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][2].string_value()));
        fuuro_elem.target_relative = (4 + target - actor) % 4;
        game_state.player_state[actor].fuuro.push_back(fuuro_elem);
    } else if (action_json["type"].string_value() == "ankan") {
        const int actor = action_json["actor"].int_value();
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][0].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][1].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][2].string_value())]--;
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["consumed"][3].string_value())]--;
        Fuuro_Elem fuuro_elem;
        fuuro_elem.type = FT_ANKAN;
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][2].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][3].string_value()));
        game_state.player_state[actor].fuuro.push_back(fuuro_elem);
    } else if (action_json["type"].string_value() == "kakan") {
        const int actor = action_json["actor"].int_value();
        game_state.player_state[actor].tehai[hai_str_to_int(action_json["pai"].string_value())]--;
        Fuuro_Elem fuuro_elem;
        fuuro_elem.type = FT_KAKAN;
        fuuro_elem.hai = hai_str_to_int(action_json["pai"].string_value());
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][0].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][1].string_value()));
        fuuro_elem.consumed.push_back(hai_str_to_int(action_json["consumed"][2].string_value()));
        replace_pon_to_kakan(game_state.player_state[actor].fuuro, fuuro_elem);
    }
}

Game_State get_game_state(const Moves& game_record) {
    Game_State game_state;
    int begin = 0;
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        if (game_record[i]["type"].string_value() == "start_kyoku") {
            begin = i;
            break;
        }
    }
    for (int i = begin; i < game_record.size(); i++) {
        const json11::Json& action_json = game_record[i];
		if (action_json["type"].string_value() == "start_game") {
			continue;
		} else if (action_json["type"].string_value() == "end_kyoku") {
			continue;
    	} else if (action_json["type"].string_value() == "hora") {
            game_state.kyotaku = 0;
        } else if (action_json["type"].string_value() == "start_kyoku") {
            game_state = get_game_state_start_kyoku(action_json);
    	} else {
            go_next_state(game_state, action_json);
        }
    }
    return game_state;
}

bool is_ippatsu_valid(const Moves& game_record, const int pid) {
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            return false;
        }
        if (action_json["type"].string_value() == "dahai" && action_json["actor"].int_value() == pid) {
            return false;
        }
        if (action_json["type"].string_value() == "pon" ||
            action_json["type"].string_value() == "chi" ||
            action_json["type"].string_value() == "daiminkan"
        ) {
            if (action_json["target"].int_value() == pid) {
                return false;
            }
        }
    }
    return false;
}

std::pair<int, int> count_tsumo_num(const Moves& game_record) {
    int tsumo_haiyama = 0;
    int tsumo_rinshan = 0;
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            break;
        }
        if (action_json["type"].string_value() == "tsumo") {
            assert(i > 0);
            assert(game_record[i-1]["type"].string_value() != "ankan");
            if (game_record[i-1]["type"].string_value() == "daiminkan" ||
                game_record[i-1]["type"].string_value() == "kakan"
            ) {
                tsumo_rinshan++;
            } else {
                tsumo_haiyama++;
            }
        }
    }
    assert(tsumo_haiyama + tsumo_rinshan <= 70);
    return std::pair<int, int> (tsumo_haiyama, tsumo_rinshan);
}

int count_tsumo_num_all(const Moves& game_record) {
   std::pair<int, int> tsumo_num_pair = count_tsumo_num(game_record);
   return tsumo_num_pair.first + tsumo_num_pair.second;
}

int get_kyoku_first(const Moves& game_record) {
    assert_with_out(0 < game_record.size(), "get_kyoku_first: game_record size error");
    assert_with_out(game_record[0]["type"].string_value() == "start_game", "get_kyoku_first: type error");
    assert_with_out(!game_record[0]["kyoku_first"].is_null(), "get_kyoku_first: kyoku_first is null");
    return game_record[0]["kyoku_first"].int_value();
}

bool get_aka_flag(const Moves& game_record) {
    assert_with_out(0 < game_record.size(), "get_aka_flag: game_record size error");
    assert_with_out(game_record[0]["type"].string_value() == "start_game", "get_aka_flag: type error");
    assert_with_out(!game_record[0]["aka_flag"].is_null(), "get_aka_flag: aka_flag is null");
    return game_record[0]["aka_flag"].bool_value();
}

int get_bakaze(const Moves& game_record) {
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            return kaze_str_to_int(action_json["bakaze"].string_value());
        }
    }
    return -1;
}

int get_kyoku(const Moves& game_record) {
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            return action_json["kyoku"].int_value();
        }
    }
    return -1;
}

int get_oya(const Moves& game_record) {
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            return action_json["oya"].int_value();
        }
    }
    return -1;
}

int get_honba(const Moves& game_record) {
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            return action_json["honba"].int_value();
        }
    }
    return 0;
}

int get_kyotaku(const Moves& game_record) {
    int tmp = 0;
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "hora") {
            return 0;
        }
        if (action_json["type"].string_value() == "start_kyoku") {
            return action_json["kyotaku"].int_value() + tmp;
        }
    }
    return tmp;
}

bool is_tobi_any(const std::array<int, 4>& scores) {
    for (int pid = 0; pid < 4; pid++) {
        if (scores[pid] < 0) {
            return true;
        }
    }
    return false;
}

bool is_tobi_any(const json11::Json& kyoku_result_move) {
    assert(kyoku_result_move["type"].string_value() == "hora" || kyoku_result_move["type"].string_value() == "ryukyoku");
    for (int pid = 0; pid < kyoku_result_move["scores"].array_items().size(); pid++) {
        if (kyoku_result_move["scores"].array_items()[pid].int_value() < 0) {
            return true;
        }
    }
    return false;
}

bool is_30000_any(const std::array<int, 4>& scores) {
    for (int pid = 0; pid < 4; pid++) {
        if (scores[pid] >= 30000) {
            return true;
        }
    }
    return false;
}

bool is_30000_any(const json11::Json& kyoku_result_move) {
    assert(kyoku_result_move["type"].string_value() == "hora" || kyoku_result_move["type"].string_value() == "ryukyoku");
    for (int pid = 0; pid < kyoku_result_move["scores"].array_items().size(); pid++) {
        if (kyoku_result_move["scores"].array_items()[pid].int_value() >= 30000) {
            return true;
        }
    }
    return false;
}

bool has_highest_score(const std::array<int, 4>& scores, const int pid) {
    for (int pid2 = 0; pid2 < 4; pid2++) {
        if (pid2 != pid && scores[pid2] >= scores[pid]) {
            return false;
        }
    }
    return true;
}

bool has_highest_score(const json11::Json& kyoku_result_move, const int pid) {
    assert(kyoku_result_move["type"].string_value() == "hora" || kyoku_result_move["type"].string_value() == "ryukyoku");
    for (int pid2 = 0; pid2 < kyoku_result_move["scores"].array_items().size(); pid2++) {
        if (pid2 != pid) {
            if (kyoku_result_move["scores"].array_items()[pid2].int_value() >= kyoku_result_move["scores"].array_items()[pid].int_value()) {
                return false;
            }
        }
    }
    return true;
}

std::pair<int, int> cal_next_bakaze_kyoku(const Moves& game_record) {
    const json11::Json& kyoku_result = game_record[game_record.size()-1];
    assert(kyoku_result["type"].string_value() == "hora" || kyoku_result["type"].string_value() == "ryukyoku");
    if (is_tobi_any(kyoku_result)) {
        return std::pair<int, int> (-1, -1);
    }
    const int bakaze = get_bakaze(game_record);
    const int kyoku = get_kyoku(game_record);
    if (is_last_kyoku(bakaze, kyoku, "tonpu")) {
        return std::pair<int, int> (-1, -1);
    } else {
        return std::pair<int, int> (kyoku == 4 ? bakaze + 1 : bakaze, kyoku == 4 ? 1 : kyoku + 1);
    }
}

int cal_next_honba(const Moves& game_record) {
    const json11::Json& kyoku_result = game_record[game_record.size()-1];
    assert(kyoku_result["type"].string_value() == "hora" || kyoku_result["type"].string_value() == "ryukyoku");
    if (kyoku_result["type"].string_value() == "ryukyoku") {
        return get_honba(game_record) + 1;
    }
    const int oya = get_oya(game_record);
    if (kyoku_result["type"].string_value() == "hora" && kyoku_result["actor"].int_value() == oya) {
        return get_honba(game_record) + 1;
    }
    return 0;
}

int cal_next_oya(const Moves& game_record) {
    const json11::Json& kyoku_result = game_record[game_record.size()-1];
    assert(kyoku_result["type"].string_value() == "hora" || kyoku_result["type"].string_value() == "ryukyoku");
    const int oya = get_oya(game_record);
    if ((kyoku_result["type"].string_value() == "hora" && kyoku_result["actor"].int_value() == oya) ||
        (kyoku_result["type"].string_value() == "ryukyoku" && kyoku_result["reason"].string_value() == "kyushukyuhai") ||
        (kyoku_result["type"].string_value() == "ryukyoku" && kyoku_result["tenpais"].array_items()[oya].bool_value())
    ) {
        return oya;
    } else {
        return (oya + 1) % 4;
    }
}

bool is_menzen(const Moves& game_record, const int pid) {
    // ポン、チー、ダイミンカンをしてないか
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            break;
        }
        if (action_json["type"].string_value() == "pon" ||
            action_json["type"].string_value() == "chi" ||
            action_json["type"].string_value() == "daiminkan"
        ) {
            if (action_json["actor"].int_value() == pid) {
                return false;
            }
        }
    }
    return true;
}

std::array<bool, 38> get_furiten_flags(const Moves& game_record, const Game_State& game_state, const int pid, const bool skip_latest) {
    // フリテンとなる牌のflagを取得。
    std::array<bool, 38> furiten_flags;
    std::fill(furiten_flags.begin(), furiten_flags.end(), false);
    bool once_dahai = false;
    for (int i = game_record.size() - 1; 0 <= i; i--) {
        const json11::Json& action_json = game_record[i];
        if (action_json["type"].string_value() == "start_kyoku") {
            break;
        }
        if (action_json["type"].string_value() == "dahai" || action_json["type"].string_value() == "kakan") {
            if (skip_latest && i == game_record.size() - 1) {
                continue;
            }
            if (action_json["actor"].int_value() == pid) {
                furiten_flags[haikind(hai_str_to_int(action_json["pai"].string_value()))] = true;
                once_dahai = true;
            } else if (!once_dahai) {
                furiten_flags[haikind(hai_str_to_int(action_json["pai"].string_value()))] = true;
            }
        }
    }
    return furiten_flags;
}

std::vector<json11::Json> load_game_record_from_file(const std::string& file_name, int length) {
    const Moves json_vec = load_json_vec_from_file(file_name);
    assert_with_out(length <= int(json_vec.size()), "load_game_record_from_file: length error");
    if (length < 0) {
        length = json_vec.size();
    }
    Moves game_record;
    std::array<int, 4> scores_ = {25000, 25000, 25000, 25000};
    for (int i = 0; i < length; i++) {
        if (json_vec[i]["type"] == "start_game" && (json_vec[i]["kyoku_first"].is_null() || json_vec[i]["aka_flag"].is_null())) {
            json11::Json::object obj = json_vec[i].object_items();
            if (json_vec[i]["kyoku_first"].is_null()) {
                obj["kyoku_first"] = 4;
            }
            if (json_vec[i]["aka_flag"].is_null()) {
                obj["aka_flag"] = true;
            }
            game_record.push_back(json11::Json(obj));
        } else if (json_vec[i]["type"] == "start_kyoku" && json_vec[i]["scores"].is_null()) {
            json11::Json::object obj = json_vec[i].object_items();
            obj["scores"] = scores_;
            game_record.push_back(json11::Json(obj));
        } else {
            game_record.push_back(json_vec[i]);
        }
        if (!json_vec[i]["scores"].is_null()) {
            for (int pid = 0; pid < 4; pid++) {
                scores_[pid] = json_vec[i]["scores"].array_items()[pid].int_value();
            }
        }
    }
    return game_record;
}