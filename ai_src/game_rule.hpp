#pragma once

#include "../share/include.hpp"

class Game_Rule{
	public:
	Game_Rule();
	int jun_pt[4];
	int aka_flag;
	int kyoku_first; // 東南戦は0、東風戦は4に設定。それ以外にも対応可能。
	int hai_num_all;
	int ten_init[4]; // 将来的に、初期の点数をプレイヤごとに変えて対戦を始める場合を想定して配列にする。

	void set_tenhou_hanchan();
	void set_mjfg();
	void set_self_match();

	int next_player(const int pid, const int arg) const;
};
