#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"

class Tehai_State2{
private:
  uint32_t chi_int[3];
  uint32_t pon_kan_int[4];
  uint32_t reach_aka_int;

public:
	Tehai_State2();
	Tehai_State2(const Hai_Array& tehai, const Fuuro_Vector& fuuro);
  void reset();
  void reset_fuuro();
  void reset_with(const Hai_Array& tehai, const Fuuro_Vector& fuuro);

	Tehai_State2& operator=(const Tehai_State2& rhs);
	bool operator==(const Tehai_State2& rhs) const;
  bool operator!=(const Tehai_State2& rhs) const;

  void set_aka_inside(int color, int flag);
  void set_aka_outside(int color, int flag);

  void set_fuuro(const Fuuro_Vector& fuuro);

  void add_chi(int smallest_haikind);
  void delete_chi(int smallest_haikind);
  void add_pon(int pon_haikind);
  void delete_pon(int pon_haikind);
  void add_ankan(int ankan_haikind);
  void delete_ankan(int ankan_haikind);
  void add_minkan(int minkan_haikind);
  void delete_minkan(int minkan_haikind);
  void add_one_fuuro(const Fuuro_Type fuuro_type, const int smallest_haikind);
  void delete_one_fuuro(const Fuuro_Type fuuro_type, int smallest_haikind);

  int get_chi_num(const int smallest_haikind) const;
  int get_pon_num(const int haikind) const;
  int get_ankan_num(const int haikind) const;
  int get_minkan_num(const int haikind) const;
  int get_aka_in_side(const int color) const;
  int get_aka_out_side(const int color) const;
  int get_haikind_num(const int haikind) const;

  Fuuro_Vector get_fuuro(const Tehai_State2& present_tehai_state) const;
  void get_fuuro(int fuuro[4][6], const Tehai_State2& present_tehai_state) const;

	void out_info();
	size_t hash_value() const;

};

size_t hash_value(const Tehai_State2& tehai_state2);

bool is_same_fuuro(const Tehai_State2& ts1, const Tehai_State2& ts2);
bool is_ts_fuuro_consistent(const Tehai_State2& tehai_state, const Fuuro_Vector& fuuro);
