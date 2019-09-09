#pragma once

#include "../share/include.hpp"
#include "bit_hai_num.hpp"
#include "tehai_ana.hpp"

class Tehai_Change{

public:
	Tehai_Change();
	void reset();

	Bit_Hai_Num tehai_base;
	int id;
	Tehai_Change& operator=(const Tehai_Change& rhs);
	bool operator==(const Tehai_Change& rhs) const;

	//int is_same_element_exist();

	size_t hash_value() const;
	void out_info() const;

};

size_t hash_value(const Tehai_Change& tc);
