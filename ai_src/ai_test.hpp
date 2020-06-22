#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"
#include "../share/agari_ten.hpp"
#include "../share/make_move.hpp"

#include "mjutil.hpp"
#include "jun_calc.hpp"
#include "tehai_action.hpp"
#include "tehai_cal.hpp"
#include "exp_values.hpp"

void test1();

#ifdef WINSTD
__declspec(dllexport)
#endif
void ai_test();