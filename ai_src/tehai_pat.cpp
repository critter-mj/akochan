#include "tehai_pat.hpp"

extern const bool out_console;

void push_back_all(std::vector<int>& v1, const std::vector<int>& v2){
	for(int i=0;i<v2.size();i++){
		v1.push_back(v2[i]);
	}
}

Tehai_Pattern_Honors_And_Knitted::Tehai_Pattern_Honors_And_Knitted(){}

Tehai_Pattern_Honors_And_Knitted::Tehai_Pattern_Honors_And_Knitted(
	const int perm_id_in, const std::array<bool, 8>& honors_in, const std::array<bool, 10>& knitted_in, const std::vector<int>& remain_in
) {
	perm_id = perm_id_in;
	knitted = knitted_in;
	honors = honors_in;
	remain = remain_in;
}

void Tehai_Pattern_Honors_And_Knitted::reset_all() {
	remain.clear();
	perm_id = -1;
	for (int i = 0; i < 10; i++) {
		knitted[i] = false;
	}
	for (int i = 0; i < 8; i++) {
		honors[i] = false;
	}
}

void Tehai_Pattern_Honors_And_Knitted::cal_shanten() {
	int cnt = 0;
	for (int i = 1; i <= 7; i++) {
		if (honors[i]) {
			cnt++;
		}
	}
	for (int i = 1; i <= 9; i++) {
		if (knitted[i]) {
			cnt++;
		}
	}
	shanten_num = std::max(0, 13 - cnt);
}

void Tehai_Pattern_Honors_And_Knitted::cal_hai_in_pattern(){
	std::vector<int> hai_need;
	const std::array<int, 3>& perm = ALL_PERM[perm_id];
	for (int i = 1; i <= 9; i++) {
		if (!knitted[i]) {
			hai_need.push_back(perm[(i-1)%3] * 10 + i);
		}
	}
	for (int i = 1; i <= 7; i++) {
		if (!honors[i]) {
			hai_need.push_back(30 + i);
		}
	}
	const int num_need = 14 - (16 - hai_need.size()); // 全部で16枚ある牌のうち、14種必要. num_need = hai_need.size() - 2;
	if (0 < num_need) {
		int p = 0;
		std::vector<int> empty;
		do {
			hai_in_pattern.push_back(empty);
			for(int i = 0; i < num_need; i++) {
				hai_in_pattern[p].push_back(hai_need[i]);
			}
			std::sort(hai_in_pattern[p].begin(), hai_in_pattern[p].end());
			p++;
		} while (boost::next_combination(hai_need.begin(), hai_need.begin() + num_need, hai_need.end()));
	}
}

void Tehai_Pattern_Honors_And_Knitted::out_info(){
	if (!out_console) return;

	const std::array<int, 3>& perm = ALL_PERM[perm_id];
	for (int i = 1; i <= 9; i++) {
		if (knitted[i]) {
			std::cout << perm[(i-1)%3] * 10 + i << " ";
		}
	}
	for (int i = 1; i <= 7; i++) {
		if (honors[i]) {
			std::cout << 30 + i << " ";
		}
	}
	std::cout << std::endl;

	printf("-----hai_out---\n");
	for(int j=0;j<remain.size();j++){
		printf("%d ", remain[j]);
	}
	printf("\n");

	printf("-----hai_in----\n");
	for(int i=0;i<hai_in_pattern.size();i++){
		for(int j=0;j<hai_in_pattern[i].size();j++){
			printf("%d ", hai_in_pattern[i][j]);
		}
		printf("\n");
	}
}


Tehai_Pattern_Titoi::Tehai_Pattern_Titoi(){
}

Tehai_Pattern_Titoi::Tehai_Pattern_Titoi(std::vector<int> h, std::vector<int> k1, std::vector<int> k2){
	reset_all();
	head_num = h.size()/2;
	for(int i=0;i<head_num;i++){
		for(int j=0;j<2;j++){
			head[i][j] = h[i*2+j];
		}
	}
	k1_num = k1.size();
	for(int i=0;i<k1_num;i++){
		head[head_num+i][0] = k1[i];
	}
	remain = k2;
}

void Tehai_Pattern_Titoi::reset_all(){
	for(int i=0;i<7;i++){
		for(int j=0;j<2;j++){
			head[i][j] = 0;
		}
	}
	head_num = 0;
	shanten_num = 14 - 1;
	remain.erase(remain.begin(),remain.begin()+remain.size());
}

void Tehai_Pattern_Titoi::cal_shanten(){
	shanten_num = 14 - 1;
	for(int i=0;i<7;i++){
		for(int j=0;j<2;j++){
			if(head[i][j]!=0){
				shanten_num--;
			}
		}
	}
}

void Tehai_Pattern_Titoi::cal_hai_in_pattern(){
	std::vector<int> head_empty;
	int exist_flag;
	for(int hai=0;hai<38;hai++){
		if(hai%10!=0){
			exist_flag = 0;
			for(int i=0;i<head_num+k1_num;i++){
				if(hai==head[i][0]){
					exist_flag=1;
				}
			}
			for(int i=0;i<remain.size();i++){
				if(hai==remain[i]){
					exist_flag=1;
				}
			}
			if(exist_flag==0){
				head_empty.push_back(hai);
			}
		}
	}

	if(shanten_num!=-1){
		int num_empty = 7-head_num-k1_num;
		int p = 0;
		std::vector<int> empty;
		do{
			hai_in_pattern.push_back(empty);
			for(int i=0;i<k1_num;i++){
				hai_in_pattern[p].push_back(head[head_num+i][0]);
			}
			for(int i=0;i<num_empty;i++){
				hai_in_pattern[p].push_back(head_empty[i]);
				hai_in_pattern[p].push_back(head_empty[i]);
			}
			std::sort(hai_in_pattern[p].begin(), hai_in_pattern[p].end());
			p++;
		}while(boost::next_combination(head_empty.begin(), head_empty.begin()+num_empty, head_empty.end()));
	}
}

void Tehai_Pattern_Titoi::out_info(){
	if (!out_console) return;

	for(int i=0;i<7;i++){
		printf("[%d,%d]", head[i][0], head[i][1]);
	}
	printf("\n");

	printf("-----hai_out---\n");
	for(int j=0;j<remain.size();j++){
		printf("%d ", remain[j]);
	}
	printf("\n");

	printf("-----hai_in----\n");
	for(int i=0;i<hai_in_pattern.size();i++){
		for(int j=0;j<hai_in_pattern[i].size();j++){
			printf("%d ", hai_in_pattern[i][j]);
		}
		printf("\n");
	}
}

Tehai_Pattern::Tehai_Pattern(int pid){
	my_pid = pid;
}

//Tehai_Pattern::Tehai_Pattern(int pid, std::vector<int> h, std::vector<int> m, std::vector<int> t, std::vector<int> k1, std::vector<int> k2, int fuuro[4][6], int fuuro_num){
Tehai_Pattern::Tehai_Pattern(int pid, const std::vector<int>& h, const std::vector<int>& m, const std::vector<int>& t, const std::vector<int>& k1, const std::vector<int>& k2, const Tehai_State2& tehai_state, int fuuro_num){
	reset_all();
	my_pid = pid;
	for(int i=0;i<h.size();i++){
		head[i] = h[i];
	}
	int mentu_num = m.size()/3;
	for(int i=0;i<mentu_num;i++){
		for(int j=0;j<3;j++){
			mentu[i][j] = m[i*3+j];
		}
	}
	int tatu_num = t.size()/2;
	for(int i=0;i<tatu_num;i++){
		for(int j=0;j<2;j++){
			mentu[mentu_num+i][j] = t[i*2+j];
		}
	}
	for(int i=0;i<k1.size();i++){
		mentu[mentu_num+tatu_num+i][0] = k1[i];
	}
	int fuuro[4][6];
	Tehai_State2 empty_tehai_state;
	tehai_state.get_fuuro(fuuro, empty_tehai_state);
	for(int fn=0;fn<fuuro_num;fn++){
		mentu[mentu_num+tatu_num+k1.size()+fn][0] = fuuro[fn][2];
		mentu[mentu_num+tatu_num+k1.size()+fn][1] = fuuro[fn][3];
		mentu[mentu_num+tatu_num+k1.size()+fn][2] = fuuro[fn][4];
	}
	remain = k2;
}

void Tehai_Pattern::reset_all(){
	for(int i=0;i<2;i++){
		head[i] = 0;
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			mentu[i][j] = 0;
		}
	}
	shanten_num = 12+2-1;
	remain.erase(remain.begin(),remain.end());
	hai_in_pattern.erase(hai_in_pattern.begin(), hai_in_pattern.end());
	for(int i=0;i<5;i++){
		hai_in_block[i].erase(hai_in_block[i].begin(), hai_in_block[i].end());
	}
}

void Tehai_Pattern::cal_shanten(){
	shanten_num = 12+2-1;
	for(int i=0;i<2;i++){
		if(head[i]!=0){
			shanten_num--;
		}
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(mentu[i][j]!=0){
				shanten_num--;
			}
		}
	}
}

void Tehai_Pattern::cal_hai_in_block(){
	std::vector<int> tmp;
	for(int mi=0;mi<4;mi++){
		if(mentu[mi][0]==0){
			for(int j=0;j<3;j++){
				for(int i=1;i<=7;i++){
					tmp.push_back(j*10+i);
					tmp.push_back(j*10+i+1);
					tmp.push_back(j*10+i+2);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
				for(int i=1;i<=9;i++){
					tmp.push_back(j*10+i);
					tmp.push_back(j*10+i);
					tmp.push_back(j*10+i);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
			}
			for(int i=31;i<38;i++){
				tmp.push_back(i);
				tmp.push_back(i);
				tmp.push_back(i);
				hai_in_block[mi].push_back(tmp);
				tmp.erase(tmp.begin(),tmp.end());
			}
		}else if(mentu[mi][1]==0){
			if(mentu[mi][0]<30){
				if(mentu[mi][0]%10<=7){
					tmp.push_back(mentu[mi][0]+1);
					tmp.push_back(mentu[mi][0]+2);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
				if(2<=mentu[mi][0]%10 && mentu[mi][0]%10<=8){
					tmp.push_back(mentu[mi][0]-1);
					tmp.push_back(mentu[mi][0]+1);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
				if(3<=mentu[mi][0]%10){
					tmp.push_back(mentu[mi][0]-1);
					tmp.push_back(mentu[mi][0]-2);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
			}
			tmp.push_back(mentu[mi][0]);
			tmp.push_back(mentu[mi][0]);
			hai_in_block[mi].push_back(tmp);
			tmp.erase(tmp.begin(),tmp.end());
		}else if(mentu[mi][2]==0){
			if(mentu[mi][0]==mentu[mi][1]){
				tmp.push_back(mentu[mi][0]);
				hai_in_block[mi].push_back(tmp);
				tmp.erase(tmp.begin(),tmp.end());
			}else if(abs(mentu[mi][0]-mentu[mi][1])==2){
				tmp.push_back((mentu[mi][0]+mentu[mi][1])/2);
				hai_in_block[mi].push_back(tmp);
				tmp.erase(tmp.begin(),tmp.end());
			}else if(mentu[mi][1]-mentu[mi][0]==1){
				if(mentu[mi][0]%10>=2){
					tmp.push_back(mentu[mi][0]-1);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
				if(mentu[mi][1]%10<=8){
					tmp.push_back(mentu[mi][1]+1);
					hai_in_block[mi].push_back(tmp);
					tmp.erase(tmp.begin(),tmp.end());
				}
			}
		}else{
			hai_in_block[mi].push_back(tmp);
		}
	}

	if(head[0]==0){
		for(int i=0;i<38;i++){
			if(i%10!=0){
				tmp.push_back(i);
				tmp.push_back(i);
				hai_in_block[4].push_back(tmp);
				tmp.erase(tmp.begin(),tmp.end());
			}
		}
	}else if(head[1]==0){
		tmp.push_back(head[0]);
		hai_in_block[4].push_back(tmp);
		tmp.erase(tmp.begin(),tmp.end());
	}else{
		hai_in_block[4].push_back(tmp);
	}
}

int Tehai_Pattern::is_in_remain(std::vector<int> test){
	for(int i=0;i<test.size();i++){
		for(int j=0;j<remain.size();j++){
			if(test[i]==remain[j]){
				return 1;
			}
		}
	}
	return 0;
}

int Tehai_Pattern::check_hai_in_error (const Hai_Array& tehai_kind, const Fuuro_Vector& fuuro_kind, const int i0, const int i1, const int i2, const int i3, const int i4) {
	if(
		hai_in_block[0][i0].size()+hai_in_block[1][i1].size()+hai_in_block[2][i2].size()+
		hai_in_block[3][i3].size()+hai_in_block[4][i4].size() == 0
	) {
		return 1;
	}
	Hai_Array using_kind_array = using_hai_array(tehai_kind, fuuro_kind);

	int hai_all[38] = {};
	for(int i=0;i<hai_in_block[0][i0].size();i++){
		hai_all[hai_in_block[0][i0][i]]++;
	}
	for(int i=0;i<hai_in_block[1][i1].size();i++){
		hai_all[hai_in_block[1][i1][i]]++;
	}
	for(int i=0;i<hai_in_block[2][i2].size();i++){
		hai_all[hai_in_block[2][i2][i]]++;
	}
	for(int i=0;i<hai_in_block[3][i3].size();i++){
		hai_all[hai_in_block[3][i3][i]]++;
	}
	for(int i=0;i<hai_in_block[4][i4].size();i++){
		hai_all[hai_in_block[4][i4][i]]++;
	}
	for (int hai = 1; hai < 38; hai++) {
		if (using_kind_array[hai] + hai_all[hai]>4){
			return 1;
		}
	}

	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			hai_all[mentu[i][j]]++;
		}
	}
	for(int j=0;j<2;j++){
		hai_all[head[j]]++;
	}
	for(int hai=1;hai<38;hai++){
		if(hai_all[hai]>4){
			return 1;
		}
	}
	return 0;
}

void Tehai_Pattern::cal_hai_in_pattern(const Hai_Array& tehai_kind, const Fuuro_Vector& fuuro_kind){
	int p = 0;
	std::vector<int> empty;
	for(int i0=0;i0<hai_in_block[0].size();i0++){
		if(is_in_remain(hai_in_block[0][i0])==0 && hai_in_block[0][i0].size()!=3){
			for(int i1=0;i1<hai_in_block[1].size();i1++){
				if(is_in_remain(hai_in_block[1][i1])==0 && hai_in_block[1][i1].size()!=3){
					for(int i2=0;i2<hai_in_block[2].size();i2++){
						if(is_in_remain(hai_in_block[2][i2])==0 && hai_in_block[2][i2].size()!=3){
							for(int i3=0;i3<hai_in_block[3].size();i3++){
								if(is_in_remain(hai_in_block[3][i3])==0 && hai_in_block[3][i3].size()!=3){
									for(int i4=0;i4<hai_in_block[4].size();i4++){
										if(is_in_remain(hai_in_block[4][i4])==0 && (hai_in_block[4][i4].size()!=2 || shanten_num<3) ){
											if(check_hai_in_error(tehai_kind, fuuro_kind, i0, i1, i2, i3, i4)==0){
												hai_in_pattern.push_back(empty);
												push_back_all(hai_in_pattern[p], hai_in_block[0][i0]);
												push_back_all(hai_in_pattern[p], hai_in_block[1][i1]);
												push_back_all(hai_in_pattern[p], hai_in_block[2][i2]);
												push_back_all(hai_in_pattern[p], hai_in_block[3][i3]);
												push_back_all(hai_in_pattern[p], hai_in_block[4][i4]);
												std::sort(hai_in_pattern[p].begin(), hai_in_pattern[p].end());
												p++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

int Tehai_Pattern::yaku_check(int nip){
	assert(nip < hai_in_pattern.size());
	if(honitsu_check_pattern(mentu, head, hai_in_pattern[nip])==1){
		return 1;
	}else if(toitoi_check_pattern(mentu, head, hai_in_pattern[nip])==1){
		return 1;
	}

	return 0;
}

double Tehai_Pattern::cal_priority(const Game_State& game_state, const Hai_Array& hai_visible_kind, int nip){
	double priority = 0.0;
	if(honitsu_check_pattern(mentu, head, hai_in_pattern[nip])==1){
		priority += 2.0;
	}
	if(toitoi_check_pattern(mentu, head, hai_in_pattern[nip])==1){
		priority += 2.0;
	}
	if(tanyao_check_pattern(mentu, head, hai_in_pattern[nip])==1){
		priority += 1.0;
	}
	priority += yakuhai_check_pattern(mentu, 31 + game_state.bakaze, 31 + game_state.player_state[my_pid].jikaze);

	int hai_in[38] = {};
	for(int i=0;i<hai_in_pattern[nip].size();i++){
		hai_in[hai_in_pattern[nip][i]]++;
	}

	double prob_score = 1.0;
	for(int hai=0;hai<38;hai++){
		prob_score *= combination(4 - hai_visible_kind[hai], hai_in[hai]);
	}
	priority += prob_score/pow(4,hai_in_pattern[nip].size());

	return priority;
}

void Tehai_Pattern::out_info(){
	if (!out_console) return;

	for(int i=0;i<4;i++){
		printf("[%d,%d,%d]", mentu[i][0], mentu[i][1], mentu[i][2]);
	}
	printf("[%d,%d]", head[0], head[1]);
	printf("\n");

	printf("-----hai_out---\n");
	for(int j=0;j<remain.size();j++){
		printf("%d ", remain[j]);
	}
	printf("\n");

	printf("-----hai_in----\n");
	for(int i=0;i<hai_in_pattern.size();i++){
		for(int j=0;j<hai_in_pattern[i].size();j++){
			printf("%d ", hai_in_pattern[i][j]);
		}
		printf(",\n");
	}
}
