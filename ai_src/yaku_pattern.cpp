#include "yaku_pattern.hpp"

int yakuhai_check_pattern(int mentu[4][3], int bakazehai, int jikazehai){
	int res = 0;
	for(int i=0;i<4;i++){
		if(35<=mentu[i][0] && mentu[i][0]<=37){
			res += 1;
		}
		if(mentu[i][0]==bakazehai){
			res += 1;
		}
		if(mentu[i][0]==jikazehai){
			res += 1;
		}
	}
	return res;
}

int chinitsu_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in){
	int flags[3];
	for(int i=0;i<3;i++){
		flags[i] = 1;
	}
	for(Color_Type color = CT_MANZU; color <= CT_SOZU; ++color){
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				if(mentu[i][j]==0){
					break;
				}else if(hai_color(mentu[i][j])!=color){
					flags[color] = 0;
					break;
				}
			}
		}
		for(int i=0;i<2;i++){
			if(head[i]==0){
				break;
			}else if(hai_color(head[i])!=color){
				flags[color] = 0;
				break;
			}
		}
		for(int i=0;i<hai_in.size();i++){
			if(hai_color(hai_in[i])!=color){
				flags[color] = 0;
				break;
			}
		}
	}
	return flags[0]+flags[1]+flags[2];
}

int honitsu_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in){
	int flags[3];
	for(int i=0;i<3;i++){
		flags[i] = 1;
	}
	for(Color_Type color = CT_MANZU; color <= CT_SOZU; ++color){
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				if(mentu[i][j]==0){
					break;
				}else if(hai_color(mentu[i][j])!=color && hai_color(mentu[i][j])!=3){
					flags[color] = 0;
					break;
				}
			}
		}
		for(int i=0;i<2;i++){
			if(head[i]==0){
				break;
			}else if(hai_color(head[i])!=color && hai_color(head[i])!=3){
				flags[color] = 0;
				break;
			}
		}
		for(int i=0;i<hai_in.size();i++){
			if(hai_color(hai_in[i])!=color && hai_color(hai_in[i])!=3){
				flags[color] = 0;
				break;
			}
		}
	}
	return flags[0]+flags[1]+flags[2];
}

int tanyao_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in){
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(mentu[i][j]==0){
				break;
			}else if(hai_yaochuu(mentu[i][j])!=0){
				return 0;
			}
		}
	}
	for(int i=0;i<2;i++){
		if(head[i]==0){
			break;
		}else if(hai_yaochuu(head[i])!=0){
			return 0;
		}
	}
	for(int i=0;i<hai_in.size();i++){
		if(hai_yaochuu(hai_in[i])!=0){
			return 0;
		}
	}
	return 1;
}

int toitoi_check_pattern(int mentu[4][3], int head[2], std::vector<int> hai_in){
	int hai_flag[38] = {};
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			hai_flag[mentu[i][j]] = 1;
		}
	}
	for(int i=0;i<2;i++){
		hai_flag[head[i]] = 1;
	}
	for(int i=0;i<hai_in.size();i++){
		hai_flag[hai_in[i]] = 1;
	}
	int counter = 0;
	for(int hai=1;hai<38;hai++){
		if(hai_flag[hai]==1){
			counter++;
		}
	}
	if(counter==5){
		return 1;
	}else{
		return 0;
	}
}
