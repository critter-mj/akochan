#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"

int yakuhai_check_pattern(int mentu[4][3], int bakazehai, int jikazehai);
int chinitsu_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in);
int honitsu_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in);
int tanyao_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in);
int toitoi_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in);
