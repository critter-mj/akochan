#pragma once

#include "../share/include.hpp"

enum JUN_EST_TYPE {
    JET_DEFAULT,
    JET_INSTANT,
    JET_LAST_HOUJUU_MODIFY
};

class Tactics{
public:
    Tactics();

    int tegawari_num[7]; // シャンテン数ごとに、手替わりを何枚まで考えるかを設定
    JUN_EST_TYPE jun_est_type; // 順位推定として何を使うか
    bool do_houjuu_discount; // 相手のテンパイ率が高くないの場合に、放銃確率を意図的に小さくするかどうか
    bool do_speed_modify; // 早い手の評価値を意図的に上げるかどうか
    bool use_larger_at_cal_tenpai_after; // cal_tenpai_afterにおいて、推定値が元のテンパイ率を下回った場合に、元のテンパイ率を用いるかどうか
    bool betaori_compare_at_2fuuro; // べた降りの判定を相手が2フーロ以上の時に行うかどうか。
    bool do_kan; // カンをするかどうか
    bool do_kyushukyuhai; // 九種九牌をするかどうか
    bool use_nn_keiten_estimator; // 形式テンパイ確率の推定にchainerモデルを使うかどうか。
    bool use_nn_kyoku_result_target_estimator; // 誰からあがるかの推定にchainerモデルを使うかどうか。
    bool use_nn_kyoku_result_other_agari_estimator; // 誰があがるかの推定にchainerモデルを使うかどうか。
    bool use_ori_choice_exception; // 相手からの攻撃が無い場合に、calc_DPで降りを無視するかどうか。
    bool use_new_agari_ml;
    bool use_agari_coeff_old;
    bool use_agari_coeff_tp;
    bool use_agari_coeff_fp;
    bool modify_always;
    bool use_rule_base_at_mentu5_titoi3_shanten;
    bool use_cn_max_addition_if_honitsu; // ホンイツ手を余分に列挙するかどうか。
    bool use_reach_daten_data_flag;
    bool do_tsumo_prob_dora_shift;
    bool use_no_fuuro_data_flag;
    bool use_new_other_end_prob;
    bool use_fuuro_choice_simple;
    bool use_tenpai_prob_other_zero_fuuro_exception;
    bool use_ori_choice_exception_near_lose; // 後がない時に降りを無視するかどうか。
    bool use_reach_tenpai_prob_other_if_other_reach; // 誰かがリーチをしている時に、reach_tenpai_prob_otherを使うかどうか。
    bool use_han_shift_at_fuuro_estimation; // 副露手推定で見えない打点上昇を計算するか。
    bool use_han_shift_at_fuuro_estimation2; // 副露手推定でドラ依存の見えない打点上昇を計算するか。
    bool use_new_tenpai_est_tmp; // テンパイ推定
    bool use_ratio_tas_to_coeff; // メンツごとの待ちの回帰分析
    bool jun_calc_bug; 
    int use_yama_ratio_kawa_num; // 山読みをはじめる河の枚数。
    int use_dp_last_tsumo_num; // tsumo_num_DPがこの値より小さい時、邪魔ポンなどを考慮。
    int inclusive_sn_always; // 包括方策1人麻雀を利用するシャンテン数
    int inclusive_sn_other_reach; // 他家がリーチしている場合に包括方策1人麻雀を利用するシャンテン数
    int inclusive_sn_fp; // フーロ手番で包括方策1人麻雀を利用するシャンテン数
    int inclusive_sn_fp_other_reach; // 他家がリーチしているフーロ手番で包括方策1人麻雀を利用するシャンテン数
    int reach_regression_mode_default; // calc_DPにおいて、リーチに関する種々の確率推定を使うかどうか。現状0か1
    int reach_regression_mode_other_reach; // 他家がリーチしている場合にcalc_DPにおいて、リーチに関する種々の確率推定を使うかどうか。
    double reach_regression_coeff_other_reach; // 他家がリーチしている場合にcalc_DPにおいて利用する、ロン確率に関する係数。
    double norisk_ratio_if_other_reach; // 現在の危険牌が、将来的に通っているとする確率。
    double dora_fuuro_coeff; // calc_DPにおいてドラを副露できる確率の補正係数。
    double other_end_prob_max; // calc_DPにおける、other_end_probの最大値。
    double reach_tenpai_prob_other_coeff; // reach_tenpai_prob_otherの係数
    double other_end_prob_coeff_if_other_reach; // 他家からリーチが来ているときに、other_end_probにかける係数。
    double last_tedashi_neighbor_coeff; // 最終手出し付近の両面待ちにかける係数。
    double gukei_est_coeff; // 愚形待ち推定にかける係数。
    double last_effective_ratio; // 終盤の有効牌ツモ率にかける係数。
    int tsumo_num_DP_max_not_menzen; // calc_DPでのtsumo_num_DPの最大値。
    int fuuro_num_max; // 列挙する手牌の最大フーロ回数（現在の節点からの相対差）
    int cn_max_addition; // 通常のcn_maxに加えて変換を考える数。この値を設定するだけでは何も変わらず、どのような手配の手替わりを増やすかは別途設定の必要がある。
    int enumerate_restriction; // 列挙する手牌の数の制限。制限しない場合-1にする。列挙される手牌の数がこの値を大きく超えることはない。
    int enumerate_restriction_fp; // フーロ局面で列挙する手牌の数の制限。制限しない場合-1にする。列挙される手牌の数がこの値を大きく超えることはない。
    int tsumo_enumerate_always; // 制限を考えずに次の変換枚数のツモ手牌列挙を行う閾値となるサイズ。制限しない場合-1
    int tsumo_enumerate_fuuro_restriction; // フーロ前提のツモ手牌を列挙する際の閾値。-1ならば無制限
    int tsumo_enumerate_restriction; // 列挙するツモ手牌の数の制限。列挙される手牌の数がこの値を大きく超えることはない。

    int tsumo_enumerate_additional_maximum; // cn_max_additionの手牌を考える際の最大数
    int tsumo_enumerate_additional_minimum; // cn_max_additionの手牌を考える際の十分条件
    double tsumo_enumerate_additional_priority; // cn_max_additionの手牌を考える際のpriorityの閾値

    std::array<std::array<double, 12>, 14> haufu_weight_tsumo;
    std::array<std::array<double, 12>, 14> haufu_weight_ron;

    void set_default();
    void set_light();
    void set_zero_first();
};

int cal_titoi_change_num_max(const int titoi_shanten_num, const int mentu_shanten_num);
bool cal_dp_flag(const int shanten_num, const int fuuro_agari_shanten_num, const bool is_other_reach_declared, const bool is_fuuro_phase, const Tactics& tactics);
bool judge_kan(const int shanten_num, const bool is_other_reach_declared);