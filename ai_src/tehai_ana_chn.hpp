#pragma once

#include "../share/include.hpp"
#include "../share/calc_agari_chn.hpp"
#include "bit_hai_num.hpp"
#include "agari_chn.hpp"
#include "tehai_pat.hpp"
#include "tehai_inout_pattern.hpp"
#include "tehai_state2.hpp"
#include "tehai_ana.hpp"
#include "tehai_ana_chn.hpp"


class Tehai_Analyzer_Basic_Chn {
public:
	Tehai_Analyzer_Basic_Chn();
	void reset_tenpai();
	void reset_tehai_analyzer_basic();
	void reset_tehai_analyzer_basic_with(const Hai_Array& tehai_src, const Fuuro_Vector& fuuro);
	//void reset_tehai_analyzer_basic_with2(const int tehai_src[38], const Tehai_State2& ts);
	void reset_tehai_analyzer_basic_with2(const Bit_Hai_Num& tehai_bit_src, const Tehai_State2& ts);
	bool operator==(const Tehai_Analyzer_Basic_Chn& rhs) const;
	bool operator!=(const Tehai_Analyzer_Basic_Chn& rhs) const;

	void set_fuuro(const Fuuro_Vector& fuuro);

	Bit_Hai_Num tehai_bit;

	Tehai_State2 tehai_state;

	uint32_t num_and_flags; // tehai_stateは外部から見える情報が同一かどうかの判定に使うため、それ以外の情報格納用
	int get_tenpai_flag() const;
	int get_neg_flag() const;
	int get_kan_changed_flag() const;

	void set_tenpai_flag(const int flag);
	void set_neg_flag(const int flag);
	void set_kan_changed_flag(const int flag);

	int get_tehai_num() const;
	void set_tehai_num(const int num);
	void add_tehai_num(const int num);
	void reduce_tehai_num(const int num);

	int get_fuuro_num() const;
	void set_fuuro_num(const int num);
	void add_fuuro_num(const int num);
	void reduce_fuuro_num(const int num);

	int get_ankan_num() const;
	void set_ankan_num(const int num);
	void add_ankan_num(const int num);

	int get_mentu_shanten_num() const;
	int get_titoi_shanten_num() const;
	int get_agari_shanten_num() const;

	void set_mentu_shanten_num(const int num);
	void set_titoi_shanten_num(const int num);
	void set_agari_shanten_num(const int num);

	virtual int get_pattern_flag() {return 0;}
	virtual int get_titoi_change_num_max() {return 0;}
	virtual int get_mentu_change_num_max() {return 0;}

	int count_hai(const int hai) const;
	void add_hai(const int hai);
	void delete_hai(const int hai);

	void add_daiminkan(const int haikind, const int hai2);
	void change_pon_to_kakan(const int haikind, const int hai2);
	void add_ankan(const int haikind, const int hai2);
	  
	int using_haikind_num(int hai) const;
	int get_shanten_num() const;

	template <class Agari_Vector> void agari_push_func_child(const Agari_Info_Chn agari, const int pid, const Game_State& game_state, Agari_Vector& agariv);
	template <class Agari_Vector> void agari_push_func(
		const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
		const int mh, const Machi_Type mt, const bool ttf, Agari_Vector& agariv	
	);
	void agari_push_func(
		const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
		const int mh, const Machi_Type mt, const bool ttf, bool flag
	);
	void agari_push_func(
		const int pid, const Game_State& game_state, const Hai_Array& tkcp, const Hai_Array& ttc, const Hai_Array& tt,
		const int mh, const Machi_Type mt, const bool ttf, boost::container::static_vector<Agari_Calc_Chn, MAX_AGARI_NUM_PER_THREAD>& agariv
	);

	bool rule_base_decision(const int my_pid);

	virtual void pattern_push_func(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kind, Tehai_Pattern_Source& tps);
	virtual void pattern_titoi_push_func(Tehai_Pattern_Source& tps);

	template <class Agari_Vector> void tenpai_check(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, Agari_Vector& agariv);
	void cut_tatu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, int start, Tehai_Pattern_Source& tps);
	template <class Agari_Vector> void analyze_tatu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, Tehai_Pattern_Source& tps, Agari_Vector& agariv);
	template <class Agari_Vector> void cut_syuntu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps, Agari_Vector& agariv);
	template <class Agari_Vector> void cut_kotu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps, Agari_Vector& agariv);
	template <class Agari_Vector> void analyze_tehai(const int my_pid_new, const Game_State& game_state, Agari_Vector& agariv);

	void cut_koritu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, int start, Tehai_Pattern_Source& tps);
	void analyze_koritu(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tate_cut, Hai_Array& tehai_tmp, int mentu_num, int head_num, int kouho_num, Tehai_Pattern_Source& tps);

	void titoi_cut_koritu(const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps);
	void titoi_cut_head(const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, int start, Tehai_Pattern_Source& tps);
	template <class Agari_Vector> void titoi_shanten(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kcp, Hai_Array& tehai_tmp, Tehai_Pattern_Source& tps, Agari_Vector& agariv);

	void analyze_tenpai(const int my_pid_new, const Game_State& game_state);

	void print_tehai() const;
	void out_tenpai();
};

class Tehai_Analyzer_Chn : public Tehai_Analyzer_Basic_Chn {
public:
	Tehai_Analyzer_Chn();
	
	void reset_tehai_analyzer();
	void reset_tehai_analyzer_with(const Hai_Array& tehai_src, const Fuuro_Vector& fuuro);

	int pattern_flag;
	int mentu_change_num_max, titoi_change_num_max;
	//std::vector<Tehai_Pattern> pattern_vec[9];
	std::vector<Tehai_Inout_Pattern> inout_pattern_vec[9];
	std::vector<Tehai_Pattern_Titoi> pattern_titoi_vec[7];

	int get_pattern_flag() {return pattern_flag;}
	int get_titoi_change_num_max() {return titoi_change_num_max;}
	int get_mentu_change_num_max() {return mentu_change_num_max;}

	void pattern_push_func(const int my_pid_new, const Game_State& game_state, const Hai_Array& tehai_kind, Tehai_Pattern_Source& tps);
	void pattern_titoi_push_func(Tehai_Pattern_Source& tps);
};

bool is_same_tehai_ta(const Hai_Array& tehai, const Tehai_Analyzer_Basic_Chn& tehai_analyzer);

int find_hai_out_ta(const Hai_Array& tehai, const Tehai_Analyzer_Basic_Chn& tehai_analyzer);

void get_tehai_ta(const Tehai_Analyzer_Basic_Chn& tehai_analyzer, int tehai[38]);
void get_tehai_kcp_ta(const Tehai_Analyzer_Basic_Chn& tehai_analyzer, int tehai_kcp[38]);
