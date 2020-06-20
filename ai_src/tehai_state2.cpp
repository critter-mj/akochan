#include "tehai_state2.hpp"

extern const bool out_console;

Tehai_State2::Tehai_State2(){
	reset();
}

Tehai_State2::Tehai_State2(const Hai_Array& tehai, const Fuuro_Vector& fuuro){
	reset_with(tehai, fuuro);
}

Tehai_State2& Tehai_State2::operator=(const Tehai_State2& rhs){
	if(this != &rhs){
		for(int i=0;i<3;i++){
			chi_int[i] = rhs.chi_int[i];
		}
		for(int i=0;i<4;i++){
			pon_kan_int[i] = rhs.pon_kan_int[i];
		}
		reach_aka_int = rhs.reach_aka_int;
	}
	return *this;
}

bool Tehai_State2::operator==(const Tehai_State2& rhs) const{
  for(int i=0;i<3;i++){
    if(chi_int[i] != rhs.chi_int[i]){
      return false;
    }
  }
  for(int i=0;i<4;i++){
    if(pon_kan_int[i] != rhs.pon_kan_int[i]){
      return false;
    }
  }
  if(reach_aka_int != rhs.reach_aka_int){
    return false;
  }
	return true;
}

bool Tehai_State2::operator!=(const Tehai_State2& rhs) const {
	return !(*this==rhs);
}

void Tehai_State2::reset(){
	for(int c=0;c<3;c++){
		chi_int[c] = 0;
		set_aka_inside(c,0);
		set_aka_outside(c,0);
	}
	for(int c=0;c<4;c++){
		pon_kan_int[c] = 0;
	}
}

void Tehai_State2::reset_fuuro(){
	for(int c=0;c<3;c++){
		chi_int[c] = 0;
		set_aka_outside(c,0);
	}
	for(int c=0;c<4;c++){
		pon_kan_int[c] = 0;
	}
}

void Tehai_State2::set_fuuro(const Fuuro_Vector& fuuro) {
    reset_fuuro();
    for (const auto& elem : fuuro) {
		if (elem.type != FT_ANKAN) {
			if (elem.hai % 10 == 0 && 10 <= elem.hai) {
				set_aka_outside(elem.hai/10 - 1, 1);
			}
		}
		for (int j = 0; j < elem.consumed.size(); j++) {
			if (elem.consumed[j]%10 == 0 && 10 <= elem.consumed[j]) {
				set_aka_outside(elem.consumed[j]/10 - 1, 1);
			}
		}
		if (elem.type == FT_CHI) {
			add_one_fuuro(elem.type, std::min({haikind(elem.consumed[0]), haikind(elem.consumed[1]), haikind(elem.hai)}));
		} else {
			add_one_fuuro(elem.type, haikind(elem.consumed[0]));
		}
	}
}

void Tehai_State2::reset_with(const Hai_Array& tehai, const Fuuro_Vector& fuuro) {
    for(int i=0;i<3;i++){
        chi_int[i] = 0;
    }
    for(int i=0;i<4;i++){
		pon_kan_int[i] = 0;
	}
	reach_aka_int = 0;

	for(int hc=0;hc<3;hc++){
		if(tehai[10*(hc+1)]==1){
			reach_aka_int += 1 << (hc+1);
		}
	}
	set_fuuro(fuuro);
}

void Tehai_State2::set_aka_inside(int color, int flag){
	uint32_t bit = 1 << (color+1);
	if(flag==1){
		reach_aka_int = reach_aka_int | bit;
	}else{
		reach_aka_int = reach_aka_int & (~bit);
	}
}

void Tehai_State2::set_aka_outside(int color, int flag){
	uint32_t bit = 1 << (color+4);
	if(flag==1){
		reach_aka_int = reach_aka_int | bit;
	}else{
		reach_aka_int = reach_aka_int & (~bit);
	}
}

void Tehai_State2::add_chi(int smallest_haikind){
	chi_int[smallest_haikind/10] += 1 << (3*(smallest_haikind%10-1));
}

void Tehai_State2::delete_chi(int smallest_haikind) {
	chi_int[smallest_haikind/10] -= 1 << (3*(smallest_haikind%10-1));
}

void Tehai_State2::add_pon(int pon_haikind){
	pon_kan_int[pon_haikind/10] += 1 << (3*(pon_haikind%10-1));
}

void Tehai_State2::delete_pon(int pon_haikind){
	pon_kan_int[pon_haikind/10] -= 1 << (3*(pon_haikind%10-1));
}

void Tehai_State2::add_ankan(int ankan_haikind){
	pon_kan_int[ankan_haikind/10] += 1 << (3*(ankan_haikind%10-1)+1);
}

void Tehai_State2::delete_ankan(int ankan_haikind) {
	pon_kan_int[ankan_haikind/10] -= 1 << (3*(ankan_haikind%10-1)+1);
}

void Tehai_State2::add_minkan(int minkan_haikind){
	pon_kan_int[minkan_haikind/10] += 1 << (3*(minkan_haikind%10-1)+2);
}

void Tehai_State2::delete_minkan(int minkan_haikind){
	pon_kan_int[minkan_haikind/10] -= 1 << (3*(minkan_haikind%10-1)+2);
}

void Tehai_State2::add_one_fuuro(const Fuuro_Type fuuro_type, const int smallest_haikind) {
	if (fuuro_type == FT_CHI) {
		add_chi(smallest_haikind);
	} else if (fuuro_type == FT_PON) {
		add_pon(smallest_haikind);
	} else if (fuuro_type == FT_ANKAN) {
		add_ankan(smallest_haikind);
	} else if (fuuro_type == FT_DAIMINKAN || fuuro_type == FT_KAKAN) {
		add_minkan(smallest_haikind);
	}
}

void Tehai_State2::delete_one_fuuro(const Fuuro_Type fuuro_type, int smallest_haikind) {
	if (fuuro_type == FT_CHI) {
		delete_chi(smallest_haikind);
	} else if (fuuro_type == FT_PON) {
		delete_pon(smallest_haikind);
	} else if (fuuro_type == FT_ANKAN) {
		delete_ankan(smallest_haikind);
	} else if (fuuro_type == FT_DAIMINKAN || fuuro_type == FT_KAKAN) {
		delete_minkan(smallest_haikind);
	}
}

int Tehai_State2::get_chi_num(const int smallest_haikind) const {
	return ( chi_int[smallest_haikind/10] >> (3*(smallest_haikind%10-1)) ) & 7;
}

int Tehai_State2::get_pon_num(const int haikind) const {
	return ( pon_kan_int[haikind/10] >> (3*(haikind%10-1)) ) & 1;
}

int Tehai_State2::get_ankan_num(const int haikind) const {
	return ( pon_kan_int[haikind/10] >> (3*(haikind%10-1)+1) ) & 1;
}

int Tehai_State2::get_minkan_num(const int haikind) const {
	return ( pon_kan_int[haikind/10] >> (3*(haikind%10-1)+2) ) & 1;
}

int Tehai_State2::get_aka_in_side(const int color) const {
	return ( reach_aka_int >> (color+1) ) & 1;
}

int Tehai_State2::get_aka_out_side(const int color) const {
	return ( reach_aka_int >> (color+4) ) & 1;
}

int Tehai_State2::get_haikind_num(const int haikind) const {
	assert(haikind%10 != 0);
	int res = 0;
	res += 3 * get_pon_num(haikind);
	res += 4 * get_minkan_num(haikind);
	res += 4 * get_ankan_num(haikind);
	if (haikind < 30) {
		if (haikind%10 <= 7) {
			res += get_chi_num(haikind);
		}
		if (2 <= haikind%10 && haikind%10 <= 8) {
			res += get_chi_num(haikind-1);
		}
		if (3 <= haikind%10) {
			res += get_chi_num(haikind-2);
		}
	}
	return res;
}

Fuuro_Vector Tehai_State2::get_fuuro(const Tehai_State2& present_tehai_state) const {
	Fuuro_Vector fuuro;
	for(int c=0;c<3;c++){
		for(int j=1;j<=7;j++){
			for(int n=0;n<present_tehai_state.get_chi_num(c*10+j);n++){
				Fuuro_Elem elem;
				elem.type = FT_CHI;
				elem.hai = c*10+j;
				elem.consumed.push_back(c*10+j+1);
				elem.consumed.push_back(c*10+j+2);
				fuuro.push_back(elem);
			}
		}
	}
	for(int hai=0;hai<38;hai++){
		if(hai%10!=0){
			for(int n=0;n<present_tehai_state.get_pon_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_PON;
				elem.hai = hai;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
			for(int n=0;n<present_tehai_state.get_ankan_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_ANKAN;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
			for(int n=0;n<present_tehai_state.get_minkan_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_DAIMINKAN;
				elem.hai = hai;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
		}
	}
	for(int c=0;c<3;c++){
		if(present_tehai_state.get_aka_out_side(c)==1){
			int flag=0;
			for(int fn = 0; fn < fuuro.size(); fn++) {
				if (fuuro[fn].type != FT_ANKAN && fuuro[fn].hai == c*10 + 5 && flag == 0) {
					fuuro[fn].hai += 5;
					flag = 1;
				}
				for (int i = 0; i < fuuro[fn].consumed.size(); i++) {
					if (fuuro[fn].consumed[i] == c*10 + 5 && flag == 0) {
						fuuro[fn].consumed[i] += 5;
						flag = 1;
					}
				}
			}
			assert(flag==1);
		}
	}

	const int present_fuuro_num = fuuro.size();
	for(int c=0;c<3;c++){
		for(int j=1;j<=7;j++){
			for(int n=0;n<get_chi_num(c*10+j) - present_tehai_state.get_chi_num(c*10+j);n++){
				Fuuro_Elem elem;
				elem.type = FT_CHI;
				elem.hai = c*10+j;
				elem.consumed.push_back(c*10+j+1);
				elem.consumed.push_back(c*10+j+2);
				fuuro.push_back(elem);
			}
		}
	}
	for(int hai=0;hai<38;hai++){
		if(hai%10!=0){
			for(int n=0;n<get_pon_num(hai) - present_tehai_state.get_pon_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_PON;
				elem.hai = hai;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
			for(int n=0;n<get_ankan_num(hai) - present_tehai_state.get_ankan_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_ANKAN;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
			for(int n=0;n<get_minkan_num(hai) - present_tehai_state.get_minkan_num(hai);n++){
				Fuuro_Elem elem;
				elem.type = FT_DAIMINKAN;
				elem.hai = hai;
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				elem.consumed.push_back(hai);
				fuuro.push_back(elem);
			}
		}
	}

	for(int c=0;c<3;c++){
		if(get_aka_out_side(c) - present_tehai_state.get_aka_out_side(c)==1){
			int flag=0;
			for (int fn = present_fuuro_num; fn < fuuro.size(); fn++) {
				if (fuuro[fn].type != FT_ANKAN && fuuro[fn].hai == c*10 + 5 && flag == 0) {
					fuuro[fn].hai += 5;
					flag = 1;
				}
				for (int i = 0; i < fuuro[fn].consumed.size(); i++) {
					if (fuuro[fn].consumed[i] == c*10 + 5 && flag == 0) {
						fuuro[fn].consumed[i] += 5;
						flag = 1;
					}
				}
			}
			assert(flag==1);
		}
	}
	return fuuro;
}

void Tehai_State2::get_fuuro(int fuuro[4][6], const Tehai_State2& present_tehai_state) const {
	for(int i=0;i<4;i++){
		for(int j=0;j<6;j++){
			fuuro[i][j] = 0;
		}
	}
	int fuuro_num = 0;
	for(int c=0;c<3;c++){
		for(int j=1;j<=7;j++){
			for(int n=0;n<present_tehai_state.get_chi_num(c*10+j);n++){
				fuuro[fuuro_num][0] = 1;
				fuuro[fuuro_num][2] = c*10+j;
				fuuro[fuuro_num][3] = c*10+j+1;
				fuuro[fuuro_num][4] = c*10+j+2;
				fuuro_num++;
			}
		}
	}
	for(int hai=0;hai<38;hai++){
		if(hai%10!=0){
			for(int n=0;n<present_tehai_state.get_pon_num(hai);n++){
				fuuro[fuuro_num][0] = 2;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro_num++;
			}
			for(int n=0;n<present_tehai_state.get_ankan_num(hai);n++){
				fuuro[fuuro_num][0] = 4;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro[fuuro_num][5] = hai;
				fuuro_num++;
			}
			for(int n=0;n<present_tehai_state.get_minkan_num(hai);n++){
				fuuro[fuuro_num][0] = 3;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro[fuuro_num][5] = hai;
				fuuro_num++;
			}
		}
	}
	for(int c=0;c<3;c++){
		if(present_tehai_state.get_aka_out_side(c)==1){
			int flag=0;
			for(int fn=0;fn<fuuro_num;fn++){
				for(int i=2;i<6;i++){
					if(fuuro[fn][i]==c*10+5 && flag==0){
						fuuro[fn][i] += 5;
						flag = 1;
					}
				}
			}
			assert(flag==1);
		}
	}

	int present_fuuro_num = fuuro_num;
	for(int c=0;c<3;c++){
		for(int j=1;j<=7;j++){
			for(int n=0;n<get_chi_num(c*10+j) - present_tehai_state.get_chi_num(c*10+j);n++){
				fuuro[fuuro_num][0] = 1;
				fuuro[fuuro_num][2] = c*10+j;
				fuuro[fuuro_num][3] = c*10+j+1;
				fuuro[fuuro_num][4] = c*10+j+2;
				fuuro_num++;
			}
		}
	}
	for(int hai=0;hai<38;hai++){
		if(hai%10!=0){
			for(int n=0;n<get_pon_num(hai) - present_tehai_state.get_pon_num(hai);n++){
				fuuro[fuuro_num][0] = 2;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro_num++;
			}
			for(int n=0;n<get_ankan_num(hai) - present_tehai_state.get_ankan_num(hai);n++){
				fuuro[fuuro_num][0] = 4;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro[fuuro_num][5] = hai;
				fuuro_num++;
			}
			for(int n=0;n<get_minkan_num(hai) - present_tehai_state.get_minkan_num(hai);n++){
				fuuro[fuuro_num][0] = 3;
				fuuro[fuuro_num][2] = hai;
				fuuro[fuuro_num][3] = hai;
				fuuro[fuuro_num][4] = hai;
				fuuro[fuuro_num][5] = hai;
				fuuro_num++;
			}
		}
	}

	for(int c=0;c<3;c++){
		if(get_aka_out_side(c) - present_tehai_state.get_aka_out_side(c)==1){
			int flag=0;
			for(int fn=present_fuuro_num;fn<fuuro_num;fn++){
				for(int i=2;i<6;i++){
					if(fuuro[fn][i]==c*10+5 && flag==0){
						fuuro[fn][i] += 5;
						flag = 1;
					}
				}
			}
			assert(flag==1);
		}
	}

}

void Tehai_State2::out_info(){
	if (!out_console) return;

	std::cout << "chi:";
	for(int i=0;i<3;i++){
		std::cout << chi_int[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "pon_kan:";
	for(int i=0;i<4;i++){
		std::cout << pon_kan_int[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "reach_aka:" << reach_aka_int << std::endl;
}

size_t Tehai_State2::hash_value() const{
	size_t res = reach_aka_int;
  for(int i=0;i<3;i++){
    res ^= chi_int[i];
  }
  for(int i=0;i<4;i++){
    res ^= pon_kan_int[i];
  }
  return res;
}

size_t hash_value(const Tehai_State2& tehai_state2){
	return tehai_state2.hash_value();
}

bool is_same_fuuro(const Tehai_State2& ts1, const Tehai_State2& ts2) {
	for(int hai=1;hai<38;hai++) {
		if (hai%10 != 0) {
			if (ts1.get_pon_num(hai) != ts2.get_pon_num(hai)) {
				return false;
			}
			if (ts1.get_minkan_num(hai) != ts2.get_minkan_num(hai)) {
				return false;
			}
			if (ts1.get_ankan_num(hai) != ts2.get_ankan_num(hai)) {
				return false;
			}
			if (hai%10 <= 7 && ts1.get_chi_num(hai) != ts2.get_chi_num(hai)) {
				return false;
			}
		}
	}
	for (int c=0;c<3;c++) {
		if (ts1.get_aka_out_side(c) != ts2.get_aka_out_side(c)) {
			return false;
		}
	}
	return true;
}

bool is_ts_fuuro_consistent(const Tehai_State2& tehai_state, const Fuuro_Vector& fuuro) {
	Tehai_State2 ts;
	ts.set_fuuro(fuuro);
	return is_same_fuuro(tehai_state, ts);
}