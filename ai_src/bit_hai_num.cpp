#include "bit_hai_num.hpp"

extern const bool out_console;

uint32_t bit_to_num(uint32_t bit, int x){
	return (bit >> (3*(x-1))) & 7;
}

Bit_Hai_Num::Bit_Hai_Num(){
  reset();
}

void Bit_Hai_Num::reset(){
  for(int i=0;i<4;i++){
    color[i] = 0;
  }
  size = 0;
}

Bit_Hai_Num& Bit_Hai_Num::operator=(const Bit_Hai_Num& rhs){
	if(this != &rhs){
		for(int c=0;c<4;c++){
			color[c] = rhs.color[c];
		}
		size = rhs.size;
	}
	return *this;
}

bool Bit_Hai_Num::operator==(const Bit_Hai_Num& rhs) const{
  for(int i=0;i<4;i++){
    if(color[i]!=rhs.color[i]){
      return false;
    }
  }
  return true;
}

bool Bit_Hai_Num::operator!=(const Bit_Hai_Num& rhs) const {
	return !(*this==rhs);
}

int Bit_Hai_Num::get_size() const {
  return size;
}

int Bit_Hai_Num::count_hai(const int hai) const {
  //assert(hai%10 != 0);
  return bit_to_num(color[hai/10],hai%10);
}

void Bit_Hai_Num::add_hai(const int hai) {
  //assert(hai%10 != 0);
  color[hai/10] += 1 << ((hai%10 - 1)*3);
  size++;
}

void Bit_Hai_Num::delete_hai(const int hai) {
  //assert(hai%10 != 0);
  color[hai/10] -= 1 << ((hai%10 - 1)*3);
  size--;
}

void Bit_Hai_Num::set_from_array38(const int tehai[38]){
  reset();
  for(int i=0;i<3;i++){
    for(int j=1;j<=9;j++){
      color[i] += tehai[10*i+j] << ((j-1)*3);
      size += tehai[10*i+j];
    }
    color[i] += tehai[10*i+10] << ((5-1)*3);
    size += tehai[10*i+10];
  }
  for (int j=1;j<=7;j++) {
    color[3] += tehai[30+j] << ((j-1)*3);
    size += tehai[30+j];
  }
}

void Bit_Hai_Num::insert_to_array38(Hai_Array& tehai) const {
    tehai[0] = 0;
    for (int c=0;c<3;c++) {
      for (int j=1;j<=9;j++) {
        tehai[10*c+j] = count_hai(10*c+j);
      }
      tehai[10*c+10] = 0;
    }
    for (int j=1;j<=7;j++) {
      tehai[30+j] = count_hai(30+j);
    }
}

void Bit_Hai_Num::out_info() const{
  if (!out_console) return;
  
  for(int hai=0;hai<38;hai++){
    if(hai%10!=0){
      for(int i=0;i<count_hai(hai);i++){
        std::cout << hai << " ";
      }
    }
  }
  std::cout << std::endl;
}

size_t Bit_Hai_Num::hash_value() const{
  size_t res = 0;
  for(int i=0;i<4;i++){
    res ^= color[i] * (i+1);
  }
  return res;
}

size_t hash_value(const Bit_Hai_Num& bit_hai_num){
  return bit_hai_num.hash_value();
}
