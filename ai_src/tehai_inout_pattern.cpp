#include "tehai_inout_pattern.hpp"

extern const bool out_console;

Tehai_Inout_Pattern::Tehai_Inout_Pattern(){
	reset();
}

void Tehai_Inout_Pattern::reset(){
	hai_in_pattern.erase(hai_in_pattern.begin(), hai_in_pattern.end());
	hai_out_pattern.erase(hai_out_pattern.begin(), hai_out_pattern.end());
	priority = 0.0;
}

bool Tehai_Inout_Pattern::operator<(const Tehai_Inout_Pattern& rhs) const{
	if(priority<rhs.priority){
		return true;
	}else{
		return false;
	}
}

void Tehai_Inout_Pattern::out_info(){
	if (!out_console) return;

	std::cout << "hai_in:";
	for(int i=0;i<hai_in_pattern.size();i++){
		std::cout << hai_in_pattern[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "hai_out:";
	for(int i=0;i<hai_out_pattern.size();i++){
		std::cout << hai_out_pattern[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "priority:" << priority << std::endl;
}
