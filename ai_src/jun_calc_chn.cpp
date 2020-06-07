#include "jun_calc.hpp"

std::array<std::array<float, 4>, 4> calc_jun_prob(const int kyoku, const std::array<int,4>& ten, const int oya) {
	const int oya_first = (12 + oya - kyoku) % 4;
	return calc_jun_prob_end(ten, oya_first);
}