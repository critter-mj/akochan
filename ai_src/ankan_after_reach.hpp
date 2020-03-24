#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"

class Ankan_After_Reach {

public:
	bool check(Hai_Array& tehai,int tumohai);//チェック

	//以下全て作業用
private:
	int kouseichk[38];
	int tumo_syurui;
	bool taatu;
	bool jyantou;
	void chk_kousei(int i, int *work, int *arg); // 元々先頭の引数が int kouseichk[] であったが、private変数のkouseichk[38]しか入っておらず、Wshadowに引っ掛かるので削除した。
	bool riichi_tyuuren_kinsi(int tumohai);
};