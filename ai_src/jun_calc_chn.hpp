#pragma once

#include "../share/include.hpp"

#include "mjutil.hpp"
#include "jun_calc.hpp"

std::array<std::array<float, 4>, 4> calc_jun_prob_chn(const int kyoku, const std::array<int,4>& ten, const int oya);
