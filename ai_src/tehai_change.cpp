#include "tehai_change.hpp"

extern const bool out_console;

Tehai_Change::Tehai_Change(){
	reset();
}

void Tehai_Change::reset(){
	tehai_base.reset();
}

Tehai_Change& Tehai_Change::operator=(const Tehai_Change& rhs){
	if(this != &rhs){
		tehai_base = rhs.tehai_base;
		id = rhs.id;
	}
	return *this;
}

bool Tehai_Change::operator==(const Tehai_Change& rhs) const{
	for(int c=0;c<4;c++){
		if(tehai_base.color[c] != rhs.tehai_base.color[c]){
			return false;
		}
	}
	return true;
}

void Tehai_Change::out_info() const{
	if (!out_console) return;

	std::cout << "id:" << id << std::endl;
	tehai_base.out_info();
}

size_t Tehai_Change::hash_value() const{
	/*
	size_t res = 0;
	for(int c=0;c<4;c++){
		res ^= tehai_base.color[c];
	}
	return res;
	*/
	return tehai_base.hash_value();
}

size_t hash_value(const Tehai_Change& tc){
	return tc.hash_value();
}
