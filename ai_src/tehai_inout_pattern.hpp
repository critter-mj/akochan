#pragma once

#include "../share/include.hpp"

class Tehai_Inout_Pattern{
	public:
	Tehai_Inout_Pattern();
	std::vector<int> hai_in_pattern;
	std::vector<int> hai_out_pattern;
	double priority;

	void reset();
	bool operator<(const Tehai_Inout_Pattern& rhs) const;
	void out_info();
};
