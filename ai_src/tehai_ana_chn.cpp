#include "tehai_ana_chn.hpp"

extern const bool out_console;

Tehai_Analyzer_Basic_Chn::Tehai_Analyzer_Basic_Chn(){
	reset_tehai_analyzer_basic();
}

Tehai_Analyzer_Chn::Tehai_Analyzer_Chn() : Tehai_Analyzer_Basic_Chn() {
}

void Tehai_Analyzer_Basic_Chn::reset_tenpai(){
	set_tenpai_flag(0);
	set_mentu_shanten_num(8);
	set_titoi_shanten_num(6);
	set_agari_shanten_num(8);
	//agariv.erase(agariv.begin(), agariv.end());
	//riich_flag = 0;reset_withでリーチフラグを入れているのに、ここで0にするのは不味い。今まで0していたので弊害がないか注意。
}

void Tehai_Analyzer_Basic_Chn::reset_tehai_analyzer_basic(){
	set_neg_flag(0);
	set_kan_changed_flag(0);
	set_tehai_num(0);

	set_fuuro_num(0);
	set_ankan_num(0);

	reset_tenpai();
	tehai_state.reset();
}

void Tehai_Analyzer_Basic_Chn::reset_tehai_analyzer_basic_with(const Hai_Array& tehai_src, const Fuuro_Vector& fuuro){
	reset_tehai_analyzer_basic();
	set_tehai_num(0);
	for (int hai = 1; hai < 38; hai++) {
		for (int i = 0; i < tehai_src[hai]; i++) {
			tehai_bit.add_hai(haikind(hai));
			add_tehai_num(1);
		}
	}
	set_fuuro(fuuro);
	tehai_state.reset_with(tehai_src, fuuro);
}

void Tehai_Analyzer_Basic_Chn::reset_tehai_analyzer_basic_with2(const Bit_Hai_Num& tehai_bit_src, const Tehai_State2& ts) {
	reset_tehai_analyzer_basic();
	tehai_bit = tehai_bit_src;
	set_tehai_num(tehai_bit.get_size());
	
	Tehai_State2 tehai_state_empty;
	int fuuro[4][6];
	ts.get_fuuro(fuuro, tehai_state_empty);
	for (int i=0;i<4;i++) {
		if (fuuro[i][0] != 0) {
			add_fuuro_num(1);
		}
		if (fuuro[i][0] == 4) {
			add_ankan_num(1);
		}
	}
	tehai_state = ts;
}

void Tehai_Analyzer_Chn::reset_tehai_analyzer() {
	reset_tehai_analyzer_basic();
	pattern_flag = 0;
	mentu_change_num_max = 0;
	titoi_change_num_max = 0;
	for(int i=0;i<9;i++){
		//pattern_vec[i].erase(pattern_vec[i].begin(),pattern_vec[i].end());
		inout_pattern_vec[i].erase(inout_pattern_vec[i].begin(),inout_pattern_vec[i].end());
	}
	for(int i=0;i<7;i++){
		pattern_titoi_vec[i].erase(pattern_titoi_vec[i].begin(),pattern_titoi_vec[i].end());
	}
}

void Tehai_Analyzer_Chn::reset_tehai_analyzer_with(const Hai_Array& tehai_src, const Fuuro_Vector& fuuro){
	reset_tehai_analyzer();
	reset_tehai_analyzer_basic_with(tehai_src, fuuro);
}

bool Tehai_Analyzer_Basic_Chn::operator==(const Tehai_Analyzer_Basic_Chn& rhs) const {
	if(tehai_bit!=rhs.tehai_bit){
		return false;
	}
	// 2017/11/17以下のtehai_stateの同一条件を追加。今までこれが無くても問題ない打牌をしていたと思われるが、
	// tehai_stateの同一性判定の有無で挙動が変わらないか、検討、検証する必要がありそう。
	if (tehai_state != rhs.tehai_state) {
		return false;
	}
	return true;
}

bool Tehai_Analyzer_Basic_Chn::operator!=(const Tehai_Analyzer_Basic_Chn& rhs) const {
	return !(*this==rhs);
}

int Tehai_Analyzer_Basic_Chn::get_tenpai_flag() const {
	return num_and_flags & 1;
}

int Tehai_Analyzer_Basic_Chn::get_neg_flag() const {
	return ( num_and_flags >> 2 ) & 1;
}

int Tehai_Analyzer_Basic_Chn::get_kan_changed_flag() const {
	return ( num_and_flags >> 3 ) & 1;
}

void Tehai_Analyzer_Basic_Chn::set_tenpai_flag(const int flag) {
	uint32_t bit = 1;
	if (flag == 1) {
		num_and_flags = num_and_flags | bit;
	} else {
		num_and_flags = num_and_flags & (~bit);
	}
}

void Tehai_Analyzer_Basic_Chn::set_neg_flag(const int flag) {
	uint32_t bit = 1 << 2;
	if (flag == 1) {
		num_and_flags = num_and_flags | bit;
	} else {
		num_and_flags = num_and_flags & (~bit);
	}
}

void Tehai_Analyzer_Basic_Chn::set_kan_changed_flag(const int flag) {
	uint32_t bit = 1 << 3;
	if (flag == 1) {
		num_and_flags = num_and_flags | bit;
	} else {
		num_and_flags = num_and_flags & (~bit);
	}
}

int Tehai_Analyzer_Basic_Chn::get_tehai_num() const {
	return (num_and_flags & 0x000000F0) >> 4; 
}

void Tehai_Analyzer_Basic_Chn::set_tehai_num(const int num) {
	assert(num <= 15); // 有効牌を得るために、14枚の手牌にさらに1枚足すことがある。
	num_and_flags = (num_and_flags & 0xFFFFFF0F) + (uint32_t(num) << 4);
}

void Tehai_Analyzer_Basic_Chn::add_tehai_num(const int num) {
	num_and_flags += (uint32_t(num) << 4);
}

void Tehai_Analyzer_Basic_Chn::reduce_tehai_num(const int num) {
	num_and_flags -= (uint32_t(num) << 4);
}

int Tehai_Analyzer_Basic_Chn::get_fuuro_num() const {
	return (num_and_flags & 0x00000F00) >> 8; 
}

void Tehai_Analyzer_Basic_Chn::set_fuuro_num(const int num) {
	assert(num <= 4);
	num_and_flags = (num_and_flags & 0xFFFFF0FF) + (uint32_t(num) << 8);
}

void Tehai_Analyzer_Basic_Chn::add_fuuro_num(const int num) {
	num_and_flags += (uint32_t(num) << 8);
}

void Tehai_Analyzer_Basic_Chn::reduce_fuuro_num(const int num) {
	num_and_flags -= (uint32_t(num) << 8);
}

int Tehai_Analyzer_Basic_Chn::get_ankan_num() const {
	return (num_and_flags & 0x0000F000) >> 12; 
}

void Tehai_Analyzer_Basic_Chn::set_ankan_num(const int num) {
	assert(num <= 4);
	num_and_flags = (num_and_flags & 0xFFFF0FFF) + (uint32_t(num) << 12);
}

void Tehai_Analyzer_Basic_Chn::add_ankan_num(const int num) {
	num_and_flags += (uint32_t(num) << 12);
}

int Tehai_Analyzer_Basic_Chn::get_mentu_shanten_num() const {
	return (num_and_flags & 0x000F0000) >> 16;
}

int Tehai_Analyzer_Basic_Chn::get_titoi_shanten_num() const {
	return (num_and_flags & 0x00F00000) >> 20;
}

int Tehai_Analyzer_Basic_Chn::get_agari_shanten_num() const {
	return (num_and_flags & 0x0F000000) >> 24;
}

void Tehai_Analyzer_Basic_Chn::set_mentu_shanten_num(const int num) {
	num_and_flags = (num_and_flags & 0xFFF0FFFF) + (uint32_t(num) << 16);
}

void Tehai_Analyzer_Basic_Chn::set_titoi_shanten_num(const int num) {
	num_and_flags = (num_and_flags & 0xFF0FFFFF) + (uint32_t(num) << 20);
}

void Tehai_Analyzer_Basic_Chn::set_agari_shanten_num(const int num) {
	num_and_flags = (num_and_flags & 0xF0FFFFFF) + (uint32_t(num) << 24);
}

int Tehai_Analyzer_Basic_Chn::count_hai(const int hai) const {
	//assert_with_out(hai%10 != 0, "count_hai_error");
	return tehai_bit.count_hai(hai);
}
	  
void Tehai_Analyzer_Basic_Chn::add_hai(const int hai) {
	add_tehai_num(1);
	if(hai%10 == 0) {
		assert(hai != 0);
		tehai_bit.add_hai(hai-5);
		tehai_state.set_aka_inside(hai/10-1, 1);
	} else {
		tehai_bit.add_hai(hai);
	}
}

void Tehai_Analyzer_Basic_Chn::delete_hai(const int hai) {
	reduce_tehai_num(1);
	if(hai%10 == 0) {
		assert(hai != 0);
		tehai_bit.delete_hai(hai-5);
		tehai_state.set_aka_inside(hai/10-1, 0);
	} else {
		tehai_bit.delete_hai(hai);
	}
}

void Tehai_Analyzer_Basic_Chn::add_daiminkan(const int hai, const int hai2) {
	assert(hai%10 != 0);
	assert(hai == haikind(hai2));
	tehai_state.add_minkan(hai);
	if (hai != hai2) {
		tehai_state.set_aka_inside(hai/10, 0);
		tehai_state.set_aka_outside(hai/10, 1);
	}
	add_fuuro_num(1);
}

void Tehai_Analyzer_Basic_Chn::change_pon_to_kakan(const int hai, const int hai2) {
    assert(hai%10 != 0);
	assert(hai == haikind(hai2));

	tehai_state.delete_pon(hai);
	tehai_state.add_minkan(hai);
	if (hai != hai2) {
		tehai_state.set_aka_inside(hai/10, 0);
		tehai_state.set_aka_outside(hai/10, 1);
	}
}

void Tehai_Analyzer_Basic_Chn::add_ankan(const int hai, const int hai2) {
	assert(hai%10 != 0);
	assert(hai == haikind(hai2));
	tehai_state.add_ankan(hai);
	if (hai != hai2) {
		tehai_state.set_aka_inside(hai/10, 0);
		tehai_state.set_aka_outside(hai/10, 1);
	}
	add_fuuro_num(1);
	add_ankan_num(1);
}

void Tehai_Analyzer_Basic_Chn::set_fuuro(const Fuuro_Vector& fuuro){
	set_fuuro_num(0);
	set_ankan_num(0);
	for (const auto& elem : fuuro) {
		add_fuuro_num(1);
		if (elem.type == FT_ANKAN) {
			add_ankan_num(1);
		}
	}
	tehai_state.set_fuuro(fuuro);
}

bool Tehai_Analyzer_Basic_Chn::rule_base_decision(const int my_pid) {
	if (get_shanten_num() >= 4) {
		return true;
	}
	//if (field.player[my_pid].tactics.use_rule_base_at_mentu5_titoi3_shanten) {
		if (get_mentu_shanten_num() >= 5 && get_titoi_shanten_num() >= 3) {
			return true;
		}
	//}
	return false;
}

void Tehai_Analyzer_Basic_Chn::pattern_push_func(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kind, Tehai_Pattern_Source& tps) {
}

void Tehai_Analyzer_Basic_Chn::pattern_titoi_push_func(Tehai_Pattern_Source& tps){
}

void Tehai_Analyzer_Chn::pattern_push_func(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kind, Tehai_Pattern_Source& tps) {
	const Fuuro_Vector fuuro_kind = haikind(game_state.player_state[my_pid_new].fuuro);
	const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
	const Hai_Array hai_visible_kind = sum_hai_array(hai_visible_all, tehai_kind);
	// to do 上をいちいち計算するのが面倒なので、はじめに計算しておく。


	Tehai_Pattern tehai_pattern(my_pid_new, tps.head_hai, tps.mentu_hai, tps.tatu_hai, tps.koritu1_hai, tps.koritu2_hai, tehai_state, get_fuuro_num());
	tehai_pattern.cal_shanten();
	//tehai_pattern.out_info();
	if(tehai_pattern.shanten_num<=get_mentu_change_num_max()) {
		tehai_pattern.cal_hai_in_block();
		tehai_pattern.cal_hai_in_pattern(tehai_kind, fuuro_kind);
		if(tehai_pattern.hai_in_pattern.size()>0){
			for(int nip=0;nip<tehai_pattern.hai_in_pattern.size();nip++){
				Tehai_Inout_Pattern tehai_io_pattern;
				tehai_io_pattern.priority = tehai_pattern.cal_priority(game_state, hai_visible_kind, nip);
				push_back_all(tehai_io_pattern.hai_in_pattern, tehai_pattern.hai_in_pattern[nip]);
				push_back_all(tehai_io_pattern.hai_out_pattern, tehai_pattern.remain);
				inout_pattern_vec[tehai_pattern.shanten_num].push_back(tehai_io_pattern);
			}
		}
	} else {
		tehai_pattern.cal_hai_in_block();
		tehai_pattern.cal_hai_in_pattern(tehai_kind, fuuro_kind);
		if (tehai_pattern.hai_in_pattern.size() > 0) {
			for(int nip=0;nip<tehai_pattern.hai_in_pattern.size();nip++) {
				//const int honitsu_check = honitsu_check_pattern(tehai_pattern.mentu, tehai_pattern.head, tehai_pattern.hai_in_pattern[nip]);
				//if (0 < honitsu_check && field.player[my_pid_new].tactics.use_cn_max_addition_if_honitsu) {
					Tehai_Inout_Pattern tehai_io_pattern;
					tehai_io_pattern.priority = tehai_pattern.cal_priority(game_state, hai_visible_kind, nip);
					push_back_all(tehai_io_pattern.hai_in_pattern, tehai_pattern.hai_in_pattern[nip]);
					push_back_all(tehai_io_pattern.hai_out_pattern, tehai_pattern.remain);
					inout_pattern_vec[tehai_pattern.shanten_num].push_back(tehai_io_pattern);
				//}
			}
		}
	}
}

void Tehai_Analyzer_Chn::pattern_titoi_push_func(Tehai_Pattern_Source& tps) {
	Tehai_Pattern_Titoi tehai_pattern_titoi(tps.head_hai, tps.koritu1_hai, tps.koritu2_hai);
	tehai_pattern_titoi.cal_shanten();
	if(tehai_pattern_titoi.shanten_num<=titoi_change_num_max){
		tehai_pattern_titoi.cal_hai_in_pattern();
		//tehai_pattern_titoi.out_info();
		if(tehai_pattern_titoi.hai_in_pattern.size()>0){
			pattern_titoi_vec[tehai_pattern_titoi.shanten_num].push_back(tehai_pattern_titoi);
		}
	}
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::tenpai_check(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, Agari_Vector& agariv){
	int rest = 0;
	for(int hai=0;hai<38;hai++){
		rest = rest + tehai_tmp[hai];
	}
	//Agari agari_tmp;
	if(rest==0){
		//入力がすでにメンツ手アガリ形の場合。とりあえず特に何もしない。
	}else if(rest==1){
		for(int hai=0;hai<38;hai++){
			if(tehai_tmp[hai] && using_haikind_num(hai)!=4){
				//printf("tanki:%d\n",hai);
				set_tenpai_flag(1);
				agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai, MT_TANKI, 0, agariv);
			}
		}
	}else if(rest==2){
		for(int hai=0;hai<38;hai++){
			if(tehai_tmp[hai]==2 && using_haikind_num(hai)!=4){
				//printf("syabo:%d\n",hai);
				set_tenpai_flag(1);
				agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai, MT_SHABO, 0, agariv);
			}
		}
		for(int hai=0;hai<30;hai++){
			if(tehai_tmp[hai]&&tehai_tmp[hai+1]){
				if(hai%10==1){
					//printf("penchan:%d\n",hai+2);
					if(using_haikind_num(hai+2)!=4){
						set_tenpai_flag(1);
						agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai+2, MT_PENCHAN, 0, agariv);
					}
				}else if(hai%10==8){
					//printf("penchan:%d\n",hai-1);
					if(using_haikind_num(hai-1)!=4){
						set_tenpai_flag(1);
						agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai-1, MT_PENCHAN, 0, agariv);
					}
				}else{
					//printf("ryanmen:%d:%d\n",hai-1,hai+2);
					if(using_haikind_num(hai-1)!=4){
						set_tenpai_flag(1);
						agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai-1, MT_RYANMEN, 0, agariv);
					}
					if(using_haikind_num(hai+2)!=4){
						set_tenpai_flag(1);
						agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai+2, MT_RYANMEN, 0, agariv);
					}
				}
			}
			if(tehai_tmp[hai]&&tehai_tmp[hai+2]){
				if(hai%10!=9){
					//printf("kanchan:%d\n",hai+1);
					if(using_haikind_num(hai+1)!=4){
						set_tenpai_flag(1);
						agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai+1, MT_KANCHAN, 0, agariv);
					}
				}
			}
		}
	}
	if(get_tenpai_flag()==1){
		set_mentu_shanten_num(0);
	}
}

void Tehai_Analyzer_Basic_Chn::cut_koritu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, int start, Tehai_Pattern_Source& tps){
	if(mentu_num + get_fuuro_num() + kouho_num < 4){
		for(int hai=start;hai<38;hai++){
			if(tehai_tmp[hai]>=1){
				tehai_tmp[hai]--;
				tps.koritu1_hai.push_back(hai);
				cut_koritu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num+1, hai, tps);
				tehai_tmp[hai]++;
				tps.koritu1_hai.pop_back();
			}
		}
	}

	for(int hai=0;hai<38;hai++){
		for(int i=0;i<tehai_tmp[hai];i++){
			tps.koritu2_hai.push_back(hai);
		}
	}

	pattern_push_func(my_pid_new, game_state, tehai_kcp, tps);

	tps.koritu2_hai.erase(tps.koritu2_hai.begin(),tps.koritu2_hai.end());
}

void Tehai_Analyzer_Basic_Chn::analyze_koritu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, Tehai_Pattern_Source& tps){
	if(head_num==0){
		for(int hai=0;hai<38;hai++){
			if(tehai_tmp[hai]>=1){
				tehai_tmp[hai] = tehai_tmp[hai]-1;
				tps.head_hai.push_back(hai);
				cut_koritu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num+1, kouho_num, 0, tps);
				tehai_tmp[hai] = tehai_tmp[hai]+1;
				tps.head_hai.pop_back();
			}
		}
	}
	cut_koritu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num, 0, tps);
}

void Tehai_Analyzer_Basic_Chn::cut_tatu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, int start, Tehai_Pattern_Source& tps){
	if(mentu_num + get_fuuro_num() + kouho_num<4){
		for(int hai=start;hai<38;hai++){
			if(tehai_tmp[hai]==2 && tehai_kcp[hai]!=4){
				tehai_tmp[hai] -= 2;
				tps.tatu_hai.push_back(hai);
				tps.tatu_hai.push_back(hai);
				cut_tatu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num+1, hai, tps);
				tehai_tmp[hai] += 2;
				tps.tatu_hai.pop_back();
				tps.tatu_hai.pop_back();
			}
			if(tehai_tmp[hai] && tehai_tmp[hai+1] && hai<30){
				tehai_tmp[hai]--;
				tehai_tmp[hai+1]--;
				tps.tatu_hai.push_back(hai);
				tps.tatu_hai.push_back(hai+1);
				cut_tatu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num+1, hai, tps);
				tehai_tmp[hai]++;
				tehai_tmp[hai+1]++;
				tps.tatu_hai.pop_back();
				tps.tatu_hai.pop_back();
			}
			if(tehai_tmp[hai] && tehai_tmp[hai+2] && hai<30 && hai%10<=8){
				tehai_tmp[hai]--;
				tehai_tmp[hai+2]--;
				tps.tatu_hai.push_back(hai);
				tps.tatu_hai.push_back(hai+2);
				cut_tatu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num+1, hai, tps);
				tehai_tmp[hai]++;
				tehai_tmp[hai+2]++;
				tps.tatu_hai.pop_back();
				tps.tatu_hai.pop_back();
			}
		}
	}

	//printf("num:%d %d %d\n", mentu_num, kouho_num, head_num);
	int tmp = 8-(mentu_num + get_fuuro_num() )*2-kouho_num-head_num;
	if(mentu_num + get_fuuro_num() ==4){
		int flag = 1;
		for(int hai=0;hai<38;hai++){
			if(tehai_tmp[hai]==1 && tehai_kcp[hai]!=4){
				flag = 0;
			}
		}
		if(flag==1){
			tmp++;
		}
	}
	if(tmp<get_mentu_shanten_num()){
		set_mentu_shanten_num(tmp);
	}

	// 本来、tmp<=get_mentu_change_num_max()を満たさないものはここで捨てた方が最善手を得る上では高速だが、
	// 検討用の出力をするために、一度その判定を緩める。
	//if(get_pattern_flag()==1 && tmp<=get_mentu_change_num_max()){
	if (get_pattern_flag() == 1 && tmp <= get_mentu_change_num_max()) {
		analyze_koritu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num, head_num, kouho_num, tps);
	}
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::analyze_tatu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, Tehai_Pattern_Source& tps, Agari_Vector& agariv){
	int head_num_tmp = 0;

	//何をヘッドとしたのか確認
	for(int hai=0;hai<38;hai++){
		if(tehai_kcp[hai] == tehai_tate_cut[hai] + 2){
			head_num_tmp = 1;
		}
	}

	//ここから明らかに無駄なパターンの削除
	/*
	if(head_num_tmp==0){
		for(int hai=0;hai<38;hai++){
			if(tehai_tmp[hai]>=2){
				return;
			}
		}
	}
	*/

	for(int hai=0;hai<38;hai++){
		if(tehai_tmp[hai]>=3){
			return;
		}
	}

	if(get_pattern_flag()==0){
		for(int j=0;j<3;j++){
			for(int i=1;i<=7;i++){
				if(tehai_tmp[10*j+i]&&tehai_tmp[10*j+i+1]&&tehai_tmp[10*j+i+2]){
					return;
				}
			}
		}
	}
	//ここまで明らかに無駄なパターンの削除

	//out_tehai(tehai_tmp);
	tenpai_check(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, agariv);

	if(get_tenpai_flag()==0 || get_pattern_flag()==1){
		int rest = 0;
		for(int hai=0;hai<38;hai++){
			rest = rest + tehai_tmp[hai];
		}
		int mentu_num_tmp = (std::accumulate(tehai_kcp.begin(), tehai_kcp.end(), 0) - rest - head_num_tmp*2)/3;
		cut_tatu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, mentu_num_tmp, head_num_tmp, 0, 0, tps);
	}
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::cut_syuntu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps, Agari_Vector& agariv){
	for(int hai=start;hai<30;hai++){
		if(tehai_tmp[hai]&&tehai_tmp[hai+1]&&tehai_tmp[hai+2]){
			tehai_tmp[hai] = tehai_tmp[hai]-1;
			tehai_tmp[hai+1] = tehai_tmp[hai+1]-1;
			tehai_tmp[hai+2] = tehai_tmp[hai+2]-1;
			tps.mentu_hai.push_back(hai);
			tps.mentu_hai.push_back(hai+1);
			tps.mentu_hai.push_back(hai+2);
			cut_syuntu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, hai, tps, agariv);
			tehai_tmp[hai] = tehai_tmp[hai]+1;
			tehai_tmp[hai+1] = tehai_tmp[hai+1]+1;
			tehai_tmp[hai+2] = tehai_tmp[hai+2]+1;
			tps.mentu_hai.pop_back();
			tps.mentu_hai.pop_back();
			tps.mentu_hai.pop_back();
		}
	}

	analyze_tatu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, tps, agariv);
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::cut_kotu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps, Agari_Vector& agariv){
	for(int hai=start;hai<38;hai++){
		if(tehai_tmp[hai]>=3){
			tehai_tmp[hai] = tehai_tmp[hai] - 3;
			tps.mentu_hai.push_back(hai);
			tps.mentu_hai.push_back(hai);
			tps.mentu_hai.push_back(hai);
			cut_kotu(my_pid_new, game_state, tehai_kcp, tehai_tmp, hai, tps, agariv);
			tehai_tmp[hai] = tehai_tmp[hai] + 3;
			tps.mentu_hai.pop_back();
			tps.mentu_hai.pop_back();
			tps.mentu_hai.pop_back();
		}
	}

	Hai_Array tehai_tate_cut = tehai_tmp;
	cut_syuntu(my_pid_new, game_state, tehai_kcp, tehai_tate_cut, tehai_tmp, 0, tps, agariv);
}

void Tehai_Analyzer_Basic_Chn::titoi_cut_koritu(const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps){
	for(int hai=start;hai<38;hai++){
		if(tehai_tmp[hai]==1 && tehai_kcp[hai]==1 && tps.head_hai.size()/2+tps.koritu1_hai.size()<7){
			tehai_tmp[hai] = tehai_tmp[hai] - 1;
			tps.koritu1_hai.push_back(hai);
			titoi_cut_koritu(tehai_kcp, tehai_tmp, hai+1, tps);
			tehai_tmp[hai] = tehai_tmp[hai] + 1;
			tps.koritu1_hai.pop_back();
		}
	}
	for(int hai=0;hai<38;hai++){
		for(int i=0;i<tehai_tmp[hai];i++){
			tps.koritu2_hai.push_back(hai);
		}
	}

	pattern_titoi_push_func(tps);

	tps.koritu2_hai.erase(tps.koritu2_hai.begin(),tps.koritu2_hai.end());
}

void Tehai_Analyzer_Basic_Chn::titoi_cut_head(const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps){
	for(int hai=start;hai<38;hai++){
		if(tehai_tmp[hai]>=2){
			tehai_tmp[hai] = tehai_tmp[hai] - 2;
			tps.head_hai.push_back(hai);
			tps.head_hai.push_back(hai);
			titoi_cut_head(tehai_kcp, tehai_tmp, hai+1, tps);
			tehai_tmp[hai] = tehai_tmp[hai] + 2;
			tps.head_hai.pop_back();
			tps.head_hai.pop_back();
		}
	}
	titoi_cut_koritu(tehai_kcp, tehai_tmp, 0, tps);
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::titoi_shanten(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, Tehai_Pattern_Source& tps, Agari_Vector& agariv){
	int head_num = 0;
	int koritu_num = 0;
	for(int hai=0;hai<38;hai++){
		if(tehai_kcp[hai]>=2){
			head_num++;
		}else if(tehai_kcp[hai]>0){
			koritu_num++;
		}
	}
	if(head_num==7){
		//入力がチートイアガリの場合。とりあえず何もしない。
	}else if(head_num==6){
		for(int hai=0;hai<38;hai++){
			if(tehai_kcp[hai]==1){
				set_tenpai_flag(1);
				agari_push_func(my_pid_new, game_state, tehai_kcp, tehai_kcp, tehai_kcp, hai, MT_TANKI, 1, agariv);
			}
		}
	}

	int tmp = 13 - 2*head_num - std::min(7-head_num,koritu_num);
	if(tmp < get_titoi_shanten_num()){
		set_titoi_shanten_num(tmp);
	}

	if(get_pattern_flag()==1 && get_titoi_shanten_num() <= get_titoi_change_num_max() && get_titoi_shanten_num() <= 3){
		titoi_cut_head(tehai_kcp, tehai_tmp, 0, tps);
	}

}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::analyze_tehai(const int my_pid_new, const Game_State& game_state, Agari_Vector& agariv){
	Hai_Array tehai_kcp;
	Tehai_Pattern_Source tps;

	tehai_bit.insert_to_array38(tehai_kcp);
	set_tehai_num(std::accumulate(tehai_kcp.begin(), tehai_kcp.end(), 0));
	assert(get_tehai_num() == tehai_bit.get_size());

	reset_tenpai();
	//agariv.clear();
	Hai_Array tehai_tmp = tehai_kcp;
	for(int hai=0;hai<38;hai++){
		if(tehai_kcp[hai]>=2){
			tehai_tmp[hai] = tehai_tmp[hai]-2;
			tps.head_hai.push_back(hai);
			tps.head_hai.push_back(hai);
			cut_kotu(my_pid_new, game_state, tehai_kcp, tehai_tmp, 0, tps, agariv);
			tehai_tmp[hai] = tehai_tmp[hai]+2;
			tps.head_hai.pop_back();
			tps.head_hai.pop_back();
		}
	}
	cut_kotu(my_pid_new, game_state, tehai_kcp, tehai_tmp, 0, tps, agariv);

	if(get_fuuro_num() == 0) {
		titoi_shanten(my_pid_new, game_state, tehai_kcp, tehai_tmp, tps, agariv);
	}
}

template void Tehai_Analyzer_Basic_Chn::analyze_tehai<boost::container::static_vector<Agari_Info, 10>>(const int my_pid_new, const Game_State& game_state, boost::container::static_vector<Agari_Info, 10>& agariv);
template void Tehai_Analyzer_Basic_Chn::analyze_tehai<boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>>(const int my_pid_new, const Game_State& game_state, boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>& agariv);
template void Tehai_Analyzer_Basic_Chn::analyze_tehai<bool>(const int my_pid_new, const Game_State& game_state, bool& agariv);


void Tehai_Analyzer_Basic_Chn::analyze_tenpai(const int my_pid_new, const Game_State& game_state) {
	bool dummy = false;
	analyze_tehai(my_pid_new, game_state, dummy);
	// 本当は、analyze_tehai(my_pid_new, false)としたいが、第二引数が参照のため、このようにできない。もう少しスマートな方法もあるはず。
}

void Tehai_Analyzer_Basic_Chn::print_tehai() const {
	//if (!out_console) return;

	for(int hai=1;hai<38;hai++){
		for(int i=0;i<count_hai(hai);i++){
			std::cout << hai << " ";
		}
	}
	std::cout << std::endl;
}

void Tehai_Analyzer_Basic_Chn::out_tenpai(){
	if (!out_console) return;
	printf("%d shanten\n", get_shanten_num());
}

int Tehai_Analyzer_Basic_Chn::using_haikind_num(int hai) const {
	assert(hai%10 != 0);
	const int res = count_hai(hai) + tehai_state.get_haikind_num(hai);
	return res;
}

int Tehai_Analyzer_Basic_Chn::get_shanten_num() const {
	return std::min(get_mentu_shanten_num(), get_titoi_shanten_num());
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::agari_push_func_child(const Agari_Info agari, const int pid, const Game_State& game_state, Agari_Vector& agariv) {
	assert(agariv.size() < 10);
	agariv.push_back(agari);

	// アガリに関する整理を行う。通常呼び出されることが無い（形込みでも10種待ちがあることは稀）ため、確実でないかもしれない。
	if (agariv.size() == 10) {
		for (int i = 9; 0 <= i; i--) {
			for (int j = 0; j < i; j++) {
				if (agariv[i].hai == agariv[j].hai) {
					if (agariv[i].han_tsumo > agariv[j].han_tsumo) {
						agariv[j].han_tsumo = agariv[i].han_tsumo;
					}
					if (agariv[i].han_ron > agariv[j].han_ron) {
						agariv[j].han_ron = agariv[i].han_ron;
					}
					agariv.erase(agariv.begin() + i);
					break;
				}
			}
		}
	}
	assert(agariv.size() < 10); // 通常のテンパイ形の待ちの種類は9が最大のはず。
}

template <class Agari_Vector> void Tehai_Analyzer_Basic_Chn::agari_push_func(
	const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
	const int mh, const Machi_Type mt, const bool ttf, Agari_Vector& agariv
) {
	const Tehai_State2 empty_tehai_state;
	Fuuro_Vector fuuro = tehai_state.get_fuuro(empty_tehai_state);
	Fuuro_Vector fuuro_kind = haikind(fuuro);
	Agari_Info agari_tmp = calc_agari(31 + game_state.bakaze, 31 + game_state.player_state[pid].jikaze, tkcp, ttc, tt, fuuro_kind, mh, mt, ttf);
	
	agari_push_func_child(agari_tmp, pid, game_state, agariv);
}

void Tehai_Analyzer_Basic_Chn::agari_push_func(
	const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
	const int mh, const Machi_Type mt, const bool ttf, bool flag
) {
	assert(flag == false);
	// agariのvectorをセットしない場合こちらの関数を呼ぶ。何もしない。
}

void Tehai_Analyzer_Basic_Chn::agari_push_func(
	const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
	const int mh, const Machi_Type mt, const bool ttf, boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>& agariv
) {
	if (agariv.size() < MAX_AGARI_NUM_PER_THREAD) {
		const Tehai_State2 empty_tehai_state;
		Fuuro_Vector fuuro = tehai_state.get_fuuro(empty_tehai_state);
		Fuuro_Vector fuuro_kind = haikind(fuuro);
		Agari_Info agari_tmp = calc_agari(31 + game_state.bakaze, 31 + game_state.player_state[pid].jikaze, tkcp, ttc, tt, fuuro_kind, mh, mt, ttf);
		
		agariv.push_back(agari_info_to_agari_calc(agari_tmp));
	} else {
		// 何かアラートを出すべき…？
	}
}

bool is_same_tehai_ta(const Hai_Array& tehai, const Tehai_Analyzer_Basic_Chn& tehai_analyzer) {
	for(int hai = 1; hai < 38; hai++) {
		if(tehai[hai] != tehai_analyzer.count_hai(hai)){
			return false;
		}
	}
	return true;
}

int find_hai_out_ta(const Hai_Array& tehai, const Tehai_Analyzer_Basic_Chn& tehai_analyzer) {
	for (int hai = 1; hai < 38; hai++) {
		if (tehai[hai] - tehai_analyzer.count_hai(hai) == 1) {
			return hai;
		}
	}
	return 0;
}

void get_tehai_ta(const Tehai_Analyzer_Basic_Chn& tehai_analyzer, int tehai[38]) {
	tehai[0] = 0;
	for(int hai=1;hai<38;hai++) {
		tehai[hai] = tehai_analyzer.count_hai(hai);
	}
}

void get_tehai_kcp_ta(const Tehai_Analyzer_Basic_Chn& tehai_analyzer, int tehai_kcp[38]) {
	get_tehai_ta(tehai_analyzer, tehai_kcp);
	for(int i=1;i<=3;i++){
		if(tehai_kcp[i*10]==1){
			tehai_kcp[i*10-5]++;
			tehai_kcp[i*10] = 0;
		}
	}
}