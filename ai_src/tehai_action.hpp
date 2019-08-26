#pragma once

#include "../share/include.hpp"

// 麻雀に現れる合法手のタイプ。数値はそのうち修正予定
enum Action_Type : char {
	AT_TSUMO = 0,
	AT_DAHAI = -1,
	AT_REACH_DECLARE = 19,
	AT_REACH_ACCEPTED = 20, //宣言牌がロンされない場合「リーチ宣言、打牌、リーチ受理、次の行動」の順番を想定しています。ロンされる場合「リーチ宣言、打牌、ロン」で局が終わる想定です。
	AT_CHI_LOW = 11, // 数字の小さい牌をチーする。例:2m,3mを晒して1mをフーロ
	AT_CHI_MIDDLE = 12, // 数字が中間の牌をチーする。
	AT_CHI_HIGH = 13, // 数字の大きい牌をチーする。
	AT_PON = 2, // ポン
	AT_DAIMINKAN = 3, // 大明カン
	AT_ANKAN = 4, // 暗カン
	AT_KAKAN = 5, // 加カン
	AT_CHI_LOW_WITH_AKA = 111, // 赤牌をフーロする牌または晒す牌に含むチー。
	AT_CHI_MIDDLE_WITH_AKA = 112,
	AT_CHI_HIGH_WITH_AKA = 113,
	AT_PON_WITH_AKA = 102,
    AT_DAIMINKAN_WITH_AKA = 103, // 大明カンに赤が含まれるかはルールと牌種から明らかなため、不要かもしれない。
    // 以上は牌譜のログに現在含まれるもの

    AT_TSUMO_AGARI = 50,
	AT_RON_AGARI = 51,
	AT_ANKAN_AND_REACH_DECLARE = 52, // 暗槓とリーチ宣言の前にはリンシャンツモが入るため、この合法手のまとめ方は不自然だが、この行動の期待利得を計算したいケースがあるため定義。
	AT_FUURO_PASS = 53,
	AT_KYUSHUKYUHAI = 54,
    AT_NULL = -100,
};

bool is_pon(Action_Type action_type);
bool is_daiminkan(Action_Type action_type);
bool is_ankan(Action_Type action_type);
bool is_kakan(Action_Type action_type);
bool is_chi_low(Action_Type action_type);
bool is_chi_middle(Action_Type action_type);
bool is_chi_high(Action_Type action_type);
bool is_chi(Action_Type action_type);
bool is_reach_declare(Action_Type action_type);

class Tsumo_Edge {
	public:
	Tsumo_Edge();
	Tsumo_Edge(const int sg, const uint8_t hi, const uint8_t ho, const int dg);

	uint8_t hai_in;
	uint8_t hai_out;
	int src_group;
	int dst_group;
};

class Tehai_Action{
	public:
	Tehai_Action();

	uint8_t hai;
	uint8_t hai_out;
	Action_Type action_type;
	uint8_t dst_group_sub;
	// この変数は、赤牌を含むフーロでも、赤牌のないAction_Typeを利用する
	// 想定される値は、AT_TSUMO, AT_PON, AT_CHI_LOW, AT_CHI_MIDDLE, AT_CHI_HIGH, AT_DAIMINKAN, AT_ANKAN, AT_KAKAN
	// リーチかどうかはdst_groupとdst_group_subを見ればわかるためaction_typeでは区別しない
	// 赤牌を含むフーロかどうかも、dst_groupとdst_group_subを見ればわかるはず(多分)。
	int dst_group;


	void out_info() const;
};
