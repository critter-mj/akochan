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
	void chk_kousei(int kouseichk[], int i, int *work, int *arg);
	bool riichi_tyuuren_kinsi(int tumohai);
};