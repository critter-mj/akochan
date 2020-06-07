#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "mjutil.hpp"
#include "tactics.hpp"
#include "tenpai_prob_calc.hpp"

bool yakuhai_check_est (const int yakuhai, const Fuuro_Vector& fuuro, const int kotu[38], const int machi_hai, const Machi_Type machi_type);
bool chinitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai);
bool honitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai);
bool tanyao_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool honrou_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool suitsu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool junchan_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool chanta_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool toitoi_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool sanshikidoukou_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type);
bool sanshikidoujun_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id);
bool ittsuu_check_est (const Fuuro_Vector& fuuro, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id);
int anko_num_count_est (const Fuuro_Vector& fuuro, const int kotu[38]);
int kan_num_count_est (const Fuuro_Vector& fuuro);

class Agari_Estimate_Info {
    public:
    int hai;
    int han;
    int fu;
    Agari_Estimate_Info();
};

Agari_Estimate_Info yaku_check(
    const int bakaze, const int jikaze,
    const Hai_Array tehai, const int head[38], const int kotu[38], const int syuntu[30], const int machi_hai, const Machi_Type machi_type, const int ryanmen_id
);

class Tehai_Estimator_Element {
	public:
	float prob, prob_now;
	Hai_Array tehai;
	std::vector<Agari_Estimate_Info> agariv;

    Tehai_Estimator_Element();
	Tehai_Estimator_Element(Hai_Array tehai_in);
	void reset(Hai_Array tehai_in);
    Color_Type get_somete_color(const Fuuro_Vector& fuuro);
};

class Tehai_Estimator {
	private: // 作業用
    int tehai_max_num;
	Hai_Array nokori;
    std::array<bool, 38> minogashi;

    public:
    Tehai_Estimator();

    private:
    void tenpai_check (
        const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
        int type, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
    );
    void add_syuntu (
        const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
        int head_num, int kotu_num, int syuntu_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
    );
	void add_kotu (
        const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
        int head_num, int kotu_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
    );
	void add_head (
        const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
        int head_num, int start, Hai_Array& tehai, int head[38], int kotu[38], int syuntu[30], std::vector<Tehai_Estimator_Element>& teev
    );
    std::vector<Tehai_Estimator_Element> cal_teev (
        const int bakaze, const int jikaze, const Fuuro_Vector& fuuro,
        const int tehai_max_num_in, const Hai_Array& nokori_in, const std::array<bool, 38>& minogashi_in
    );

    public:
    std::vector<Tehai_Estimator_Element> cal_teev_with_prob (
        const Moves& game_record, const Game_State& game_state, const int my_pid, const int target,
        const int tehai_max_num_in, const Hai_Array& nokori_in, const std::array<bool, 38>& minogashi_in
    );

    void normalize2(
        const Fuuro_Vector& fuuro, const float tenpai_prob, const float normal_tenpai_prob, const std::array<float, 3> somete_tenpai_prob,
        std::vector<Tehai_Estimator_Element>& teev
    );
};

std::array<std::array<float, 100>, 38> cal_hai_prob_from_teev(const std::vector<Tehai_Estimator_Element>& teev, const bool is_tsumo, const bool is_now);

class Machi_Coeff {
	public:
	Machi_Coeff();
    std::array<float, 4> katachi_prob;
    std::array<float, 38> tanki_coeff;
	std::array<float, 38> syabo_coeff;
    std::array<std::array<float, 7>, 3> ryanmen_coeff;
    std::array<std::array<float, 9>, 3> kanchan_coeff;
    std::array<std::array<float, 2>, 3> penchan_coeff;
    
	void init_coeff(const int my_pid);
    void set_katachi_prob(const int my_pid);
    void safe_flag_to_coeff(const std::array<bool, 38>& safe_flag);
	void visible_to_coeff(const Hai_Array& visible_all, const Hai_Array& visible);
    void sute_br_to_coeff(const Game_State& game_state, const int my_pid, const int target);
    void ratio_tas_to_coeff(const Game_State& game_state, const int my_pid, const int target);
	void std_coeff();
};

std::array<std::array<float, 100>, 38> cal_hai_prob_from_machi_coeff(
    const Game_State& game_state, const Machi_Coeff& machi_coeff, const std::array<float, 100>& hanfu_weight, const bool is_tsumo
);

std::array<float, 100> cal_agari_hanfu_prob(const std::array<std::array<float, 100>, 38>& hai_prob);
Hai_Array sum_hai_array(const Hai_Array& hai_array1, const Hai_Array& hai_array2);
Hai_Array cal_nokori_kind_array(const Hai_Array& visible_kind);
std::array<bool, 38> get_sute_kind_flag(const Kawa& kawa);

class Tenpai_Estimator_Simple {
	public:
	double tenpai_prob; // 現在のテンパイ確率
	std::array<std::array<float, 100>, 38> hai_tsumo_prob; // ツモアガリすると仮定したときの各牌、ハン、フの確率。
	std::array<std::array<float, 100>, 38> hai_ron_prob; // ロンアガリすると仮定したときの各牌、ハン、フの確率。
    std::array<std::array<float, 100>, 38> hai_ron_prob_now; // 今捨てる牌によりロンアガリすると仮定したときの各牌、ハン、フの確率。
	//double tsumo_prob[14][12];
	//double ron_prob[14][12];
	//double hai_ron_prob_sum[38], hai_ron_prob_sum_now[38];

    Tenpai_Estimator_Simple();
    void set_tenpai2(const Moves& game_record, const Game_State& game_state, const int my_pid, const int target);
    void set_tenpai_estimator(const Moves& game_record, const Game_State& game_state, const int my_pid, const int target, const Tactics& tactics);
};

std::array<float, 4> get_tenpai_prob_array (const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator);
std::array<std::array<float, 100>, 4> cal_agari_hanfu_prob_array(const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator, const bool is_tsumo);

std::pair<std::array<std::array<float, 38>, 4>, std::array<std::array<float, 38>, 4>> cal_houjuu_hai_prob_value(
	const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator, const std::array<std::array<std::array<float, 100>, 4>, 4>& kyoku_end_pt_exp,
	const int my_pid, const bool is_now
);

std::pair<std::array<float, 38>, std::array<float, 38>> cal_total_houjuu_hai_prob_value (
	const int my_pid, const std::array<Tenpai_Estimator_Simple, 4>& tenpai_estimator,
	const std::array<std::array<float, 38>, 4>& houjuu_hai_prob, const std::array<std::array<float, 38>, 4>& houjuu_hai_value
);
