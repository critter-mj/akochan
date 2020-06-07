#include "tehai_group.hpp"

Fuuro_Type fuuro_action_type_to_fuuro_type(const int fuuro_action_type) {
	if (11 <= fuuro_action_type && fuuro_action_type <= 13) {
		return FT_CHI;
	} else {
		return (Fuuro_Type)fuuro_action_type;
	}
}

Tehai_Location::Tehai_Location() {
	data = 0;
}

Tehai_Location::Tehai_Location(const int first, const int second) {
	assert(first < 256);
	assert(second < 256 * 256 * 256);
	data = (uint32_t(first) << 24) + uint32_t(second);
}

int Tehai_Location::get_first() const {
	return int(data >> 24);
}

int Tehai_Location::get_second() const {
	return int(data & 0x00FFFFFF);
}

int loc_intvec(const std::vector<int>& vec, const int num) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == num) {
			return i;
		}
	}
	return -1;
}

Tehai_Group::Tehai_Group(){
	reset();
}

void Tehai_Group::reset(){
	ta_loc.clear();
	tehai_change.reset();
}

void Tehai_Group::set_tav_init(
	const Bit_Hai_Num& tehai_bit_original,
	const Tehai_State2& ts,
	const int fuuro_cand_hai,
	boost::unordered_map<Tehai_State2, int>& ts_map,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
){
	ta_loc.clear();
	int aka_in_choice[3] = {};
	int aka_out_choice[3] = {};
	std::vector<int> hai_in_tmp, hai_out_tmp;

	for (int hai=1; hai<38; hai++) {
		if (hai%10 != 0) {
			for (int i=0; i < std::max(tehai_change.tehai_base.count_hai(hai)-tehai_bit_original.count_hai(hai),0); i++) {
				hai_in_tmp.push_back(hai);
			}
			for (int i=0; i < std::max(tehai_bit_original.count_hai(hai)-tehai_change.tehai_base.count_hai(hai),0); i++) {
				hai_out_tmp.push_back(hai);
			}
		}
	}

	for (int hc = 0; hc < 3; hc++) {
		if (std::count(hai_in_tmp.begin(), hai_in_tmp.end(), hc*10+5) != 0 && fuuro_cand_hai == hc*10 + 10) {
			aka_in_choice[hc] = 2;
		} else {
			aka_in_choice[hc] = 1;
		}
		if (ts.get_aka_in_side(hc) == 0 || std::count(hai_out_tmp.begin(), hai_out_tmp.end(), hc*10 + 5) == 0) {
			// 赤5を出す選択が無い場合
			aka_out_choice[hc] = 1;
		} else if (tehai_bit_original.count_hai(hc*10+5) > std::count(hai_out_tmp.begin(), hai_out_tmp.end(), hc*10 + 5) && ts.get_aka_in_side(hc) == 1) {
			// 黒5と赤5を出す選択両方がある場合
			aka_out_choice[hc] = 2;
		} else if(tehai_bit_original.count_hai(hc*10+5) == std::count(hai_out_tmp.begin(), hai_out_tmp.end(), hc*10 + 5) && ts.get_aka_in_side(hc) == 1) {
			// 赤5を出す選択しかない場合
			aka_out_choice[hc] = 1;
			hai_out_tmp[loc_intvec(hai_out_tmp, hc*10+5)] = (hc+1)*10;
		} else {
			assert(false);
		}
	}

	for(int aic0=0;aic0<aka_in_choice[0];aic0++){
		if(aic0==1){
			hai_in_tmp[loc_intvec(hai_in_tmp, 5)] = 10;
		}
		for(int aic1=0;aic1<aka_in_choice[1];aic1++){
			if(aic1==1){
				hai_in_tmp[loc_intvec(hai_in_tmp, 15)] = 20;
			}
			for(int aic2=0;aic2<aka_in_choice[2];aic2++){
				if(aic2==1){
					hai_in_tmp[loc_intvec(hai_in_tmp, 25)] = 30;
				}

				for(int aoc0=0;aoc0<aka_out_choice[0];aoc0++){
					if(aoc0==1){
						hai_out_tmp[loc_intvec(hai_out_tmp, 5)] = 10;
					}
					for(int aoc1=0;aoc1<aka_out_choice[1];aoc1++){
						if(aoc1==1){
							hai_out_tmp[loc_intvec(hai_out_tmp, 15)] = 20;
						}
						for(int aoc2=0;aoc2<aka_out_choice[2];aoc2++){
							if(aoc2==1){
								hai_out_tmp[loc_intvec(hai_out_tmp, 25)] = 30;
							}
							Tehai_Analyzer_Basic tehai_analyzer;

							cal_tav[thread_num].push_back(tehai_analyzer);
							const int loc_second_new = cal_tav[thread_num].size() - 1;
							cal_tav[thread_num][loc_second_new].reset_tehai_analyzer_basic_with2(tehai_bit_original, ts);
							for(int i=0;i<hai_in_tmp.size();i++){
								cal_tav[thread_num][loc_second_new].add_hai(hai_in_tmp[i]);
							}
							for(int i=0;i<hai_out_tmp.size();i++){
								cal_tav[thread_num][loc_second_new].delete_hai(hai_out_tmp[i]);
							}
							ta_loc.push_back(Tehai_Location(thread_num, loc_second_new));

							const int gn_new = ta_loc.size()-1;
							ts_map[cal_tav[thread_num][loc_second_new].tehai_state] = gn_new;

							if(aoc2==1){
								hai_out_tmp[loc_intvec(hai_out_tmp, 30)] = 25;
							}
						}
						if(aoc1==1){
							hai_out_tmp[loc_intvec(hai_out_tmp, 20)] = 15;
						}
					}
					if(aoc0==1){
						hai_out_tmp[loc_intvec(hai_out_tmp, 10)] = 5;
					}
				}

				if(aic2==1){
					hai_in_tmp[loc_intvec(hai_in_tmp, 30)] = 25;
				}
			}
			if(aic1==1){
				hai_in_tmp[loc_intvec(hai_in_tmp, 20)] = 15;
			}
		}
		if(aic0==1){
			hai_in_tmp[loc_intvec(hai_in_tmp, 10)] = 5;
		}
	}
}

void Tehai_Group::add_fuuro_child(
	Tehai_Analyzer_Basic& tehai_analyzer,
	int fuuro_cand[38], const int fuuro_cand_copy[38], int fuuro_num_begin, int fuuro_num_end, int fuuro_must, int kan_cand[38],
	boost::unordered_map<Tehai_State2, int>& ts_map,
	//std::unordered_map<Tehai_State, int, Tehai_State_Hash>& ts_map,
	const int fuuro_type, int hai0, int hai1, int hai2, int hai3,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
){
	if (cal_tav[thread_num].size() >= MAX_TA_NUM_PER_THREAD - BUF_FOR_REACH_PER_THREAD ||
		ta_loc.size() >= MAX_TA_NUM_PER_GROUP - BUF_FOR_REACH_PER_GROUP
	) {
		// 何かアラートを出すべき？
		return;
	}

	assert(fuuro_type != FT_ANKAN);
	assert(fuuro_type != FT_DAIMINKAN);
	
	tehai_analyzer.delete_hai(hai0);
	tehai_analyzer.delete_hai(hai1);
	tehai_analyzer.delete_hai(hai2);
	
	if((hai0%10==0 && hai0>0) || (hai1%10==0 && hai1>0) || (hai2%10==0 && hai2>0) || (hai3%10==0 && hai3>0)){
		// add_fuuro_childはカンの生成には使わないようにリファクタリング中なので、将来的にhai3は無くなる予定
		tehai_analyzer.tehai_state.set_aka_inside((hai0-1)/10,0);
		tehai_analyzer.tehai_state.set_aka_outside((hai0-1)/10,1);
	}
	//tehai_analyzer.tehai_state.add_one_fuuro(fuuro_type, smallest_haikind3(hai0,hai1,hai2));
	tehai_analyzer.tehai_state.add_one_fuuro(fuuro_action_type_to_fuuro_type(fuuro_type), std::min({haikind(hai0), haikind(hai1), haikind(hai2)}));
	tehai_analyzer.add_fuuro_num(1);
	if(tehai_analyzer.get_fuuro_num() - cal_tav[ta_loc[0].get_first()][ta_loc[0].get_second()].get_fuuro_num() >= fuuro_num_begin){
		if(ts_map.find(tehai_analyzer.tehai_state) == ts_map.end()){
			ts_map[tehai_analyzer.tehai_state] = ta_loc.size();

			cal_tav[thread_num].push_back(tehai_analyzer);
			const int loc_second_new = cal_tav[thread_num].size() - 1;
			ta_loc.push_back(Tehai_Location(thread_num, loc_second_new));
		}
	}
	fuuro_cand[hai0]--;
	add_fuuro(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, cal_tav, thread_num);
	fuuro_cand[hai0]++;

	tehai_analyzer.reduce_fuuro_num(1);
	tehai_analyzer.tehai_state.delete_one_fuuro(fuuro_action_type_to_fuuro_type(fuuro_type), std::min({haikind(hai0), haikind(hai1), haikind(hai2)}));
	if((hai0%10==0 && hai0>0) || (hai1%10==0 && hai1>0) || (hai2%10==0 && hai2>0) || (hai3%10==0 && hai3>0)){
		// add_fuuro_childはカンの生成には使わないようにリファクタリング中なので、将来的にhai3は無くなる予定
		tehai_analyzer.tehai_state.set_aka_inside((hai0-1)/10,1);
		tehai_analyzer.tehai_state.set_aka_outside((hai0-1)/10,0);
	}
	tehai_analyzer.add_hai(hai0);
	tehai_analyzer.add_hai(hai1);
	tehai_analyzer.add_hai(hai2);
	
}

// add_fuuroに渡すtehai_anayzerは参照の方が良いと思われるが、assertで落ちるためとりあえず値渡しにしておく。
// assertで落ちる原因は探るべきだが、手牌情報をビットで渡す方式のリファクタリングが進めば、そもそも値渡しで十分になるかもしれない。
void Tehai_Group::add_fuuro(
	Tehai_Analyzer_Basic tehai_analyzer,
	int fuuro_cand[38], const int fuuro_cand_copy[38], int fuuro_num_begin, int fuuro_num_end, int fuuro_must, int kan_cand[38],
	boost::unordered_map<Tehai_State2, int>& ts_map,
	//std::unordered_map<Tehai_State, int, Tehai_State_Hash>& ts_map,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
){
	if((tehai_analyzer.get_fuuro_num() - tehai_analyzer.get_ankan_num()) - (cal_tav[ta_loc[0].get_first()][ta_loc[0].get_second()].get_fuuro_num() - cal_tav[ta_loc[0].get_first()][ta_loc[0].get_second()].get_ankan_num()) >= fuuro_num_end){
		return;
	}else if(fuuro_must!=0 && tehai_analyzer.get_fuuro_num() - cal_tav[ta_loc[0].get_first()][ta_loc[0].get_second()].get_fuuro_num() > 0){
		//if(tehai_analyzer.fuuro[tav[0].fuuro_num][2] != fuuro_must){
		if(fuuro_cand[fuuro_must] == fuuro_cand_copy[fuuro_must]){
			return;
		}
	}

	for(int hai=0;hai<38;hai++){
		if(fuuro_cand[hai]>0 && tehai_analyzer.count_hai(hai)>=3){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 2, hai, hai, hai, 0, cal_tav, thread_num);
		}
		if(fuuro_cand[hai]>0 && hai%10==5 && hai<30 && tehai_analyzer.count_hai(hai)>=2 && tehai_analyzer.count_hai(hai+5)==1){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 2, hai, hai, hai+5, 0, cal_tav, thread_num);
		}
		if(fuuro_cand[hai]>0 && hai%10==0 && 0<hai && tehai_analyzer.count_hai(hai)==1 && tehai_analyzer.count_hai(hai-5)>=2){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 2, hai, hai-5, hai-5, 0, cal_tav, thread_num);
		}
	}

	int hai;
	for(int hai_c=0;hai_c<3;hai_c++){
		for(int hai_n=1;hai_n<=7;hai_n++){
			hai = hai_c*10 + hai_n;
			if(fuuro_cand[hai]>0 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai+1)>0 && tehai_analyzer.count_hai(hai+2)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 11, hai, hai+1, hai+2, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==3 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai+1)>0 && tehai_analyzer.count_hai(hai+7)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 11, hai, hai+1, hai+7, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==4 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai+6)>0 && tehai_analyzer.count_hai(hai+2)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 11, hai, hai+6, hai+2, 0, cal_tav, thread_num);
			}
		}
		if(fuuro_cand[hai_c*10+10]>0 && tehai_analyzer.count_hai(hai_c*10+10)>0 && tehai_analyzer.count_hai(hai_c*10+6)>0 && tehai_analyzer.count_hai(hai_c*10+7)>0){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 11, hai_c*10+10, hai_c*10+6, hai_c*10+7, 0, cal_tav, thread_num);
		}
	}

	for(int hai_c=0;hai_c<3;hai_c++){
		for(int hai_n=2;hai_n<=8;hai_n++){
			hai = hai_c*10 + hai_n;
			if(fuuro_cand[hai]>0 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai-1)>0 && tehai_analyzer.count_hai(hai+1)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 12, hai, hai-1, hai+1, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==4 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai-1)>0 && tehai_analyzer.count_hai(hai+6)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 12, hai, hai-1, hai+6, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==6 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai+4)>0 && tehai_analyzer.count_hai(hai+1)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 12, hai, hai+4, hai+1, 0, cal_tav, thread_num);
			}
		}
		if(fuuro_cand[hai_c*10+10]>0 && tehai_analyzer.count_hai(hai_c*10+10)>0 && tehai_analyzer.count_hai(hai_c*10+4)>0 && tehai_analyzer.count_hai(hai_c*10+6)>0){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 12, hai_c*10+10, hai_c*10+4, hai_c*10+6, 0, cal_tav, thread_num);
		}
	}

	for(int hai_c=0;hai_c<3;hai_c++){
		for(int hai_n=3;hai_n<=9;hai_n++){
			hai = hai_c*10 + hai_n;
			if(fuuro_cand[hai]>0 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai-2)>0 && tehai_analyzer.count_hai(hai-1)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 13, hai, hai-2, hai-1, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==6 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai-2)>0 && tehai_analyzer.count_hai(hai+4)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 13, hai, hai-2, hai+4, 0, cal_tav, thread_num);
			}
			if(fuuro_cand[hai]>0 && hai%10==7 && tehai_analyzer.count_hai(hai)>0 && tehai_analyzer.count_hai(hai+3)>0 && tehai_analyzer.count_hai(hai-1)>0){
				add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 13, hai, hai+3, hai-1, 0, cal_tav, thread_num);
			}
		}
		if(fuuro_cand[hai_c*10+10]>0 && tehai_analyzer.count_hai(hai_c*10+10)>0 && tehai_analyzer.count_hai(hai_c*10+3)>0 && tehai_analyzer.count_hai(hai_c*10+4)>0){
			add_fuuro_child(tehai_analyzer, fuuro_cand, fuuro_cand_copy, fuuro_num_begin, fuuro_num_end, fuuro_must, kan_cand, ts_map, 13, hai_c*10+10, hai_c*10+3, hai_c*10+4, 0, cal_tav, thread_num);
		}
	}
}

void Tehai_Group::add_ankan(
	const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
) {
	const int tav_size_tmp = ta_loc.size();
	for (int gn = 0; gn < tav_size_tmp; gn++) {
		for (int hai = 1; hai < 38; hai++) {
			if (hai%10 != 0 && kan_cand[hai] == 1) {
				// カンの派生手牌を考える際、派生する前の手牌では対象となる牌種がちょうど3枚である必要がある。
				// haiが4枚ある手牌の暗刻を槓子に置き換えてしまうと、haiが最終的に5枚になってしまうため不可能
				const int ta_loc_gn_first = ta_loc[gn].get_first();
				const int ta_loc_gn_second = ta_loc[gn].get_second();
				if (cal_tav[ta_loc_gn_first][ta_loc_gn_second].count_hai(hai) == 3 && cal_tav[ta_loc_gn_first][ta_loc_gn_second].using_haikind_num(hai) == 3) {
					int hai2 = hai;
					if (cal_tav[ta_loc_gn_first][ta_loc_gn_second].count_hai(hai) == 2) {
						assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_aka_in_side(hai/10) == 1);
						hai2 += 5;
						// 元々手牌に赤5がある場合に対応させる。赤の処理をどこで行うことが最適か不明なため将来的に不要となる可能性あり。
						// 黒5が手牌に3枚あった場合、ここで追加させるのは赤5の無い暗槓であることに注意。
					}
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_ankan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].get_fuuro_num() < 4);

					if (cal_tav[thread_num].size() >= MAX_TA_NUM_PER_THREAD - BUF_FOR_REACH_PER_THREAD ||
						ta_loc.size() >= MAX_TA_NUM_PER_GROUP - BUF_FOR_REACH_PER_GROUP
					) {
						// 何かアラートを出すべき？
						return;
					}

					cal_tav[thread_num].push_back(cal_tav[ta_loc_gn_first][ta_loc_gn_second]);
					const int loc_second_new = cal_tav[thread_num].size() - 1;
					cal_tav[thread_num][loc_second_new].delete_hai(hai);
					cal_tav[thread_num][loc_second_new].delete_hai(hai);
					cal_tav[thread_num][loc_second_new].delete_hai(hai2);
					cal_tav[thread_num][loc_second_new].add_ankan(hai, hai2);
					cal_tav[thread_num][loc_second_new].set_kan_changed_flag(1);
					ta_loc.push_back(Tehai_Location(thread_num, loc_second_new));

					const int gn_new = ta_loc.size()-1;
					assert(ts_map.find(cal_tav[thread_num][loc_second_new].tehai_state) == ts_map.end());
					ts_map[cal_tav[thread_num][loc_second_new].tehai_state] = gn_new;

					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_ankan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].get_fuuro_num() < 4);
				}
			}
		}
	}
}

void Tehai_Group::add_daiminkan(
	const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
) {
	const int tav_size_tmp = ta_loc.size();
	for (int gn = 0; gn < tav_size_tmp; gn++) {
		const int ta_loc_gn_first = ta_loc[gn].get_first();
		const int ta_loc_gn_second = ta_loc[gn].get_second();

		for (int hai = 1; hai < 38; hai++) {
			if (hai%10 != 0 && kan_cand[hai] == 1) {
				// カンの派生手牌を考える際、派生する前の手牌では対象となる牌種がちょうど3枚である必要がある。
				// haiが4枚ある手牌の暗刻を槓子に置き換えてしまうと、haiが最終的に5枚になってしまうため不可能
				if (cal_tav[ta_loc_gn_first][ta_loc_gn_second].count_hai(hai) == 3 && cal_tav[ta_loc_gn_first][ta_loc_gn_second].using_haikind_num(hai) == 3) {
					int hai2 = hai;
					if (cal_tav[ta_loc_gn_first][ta_loc_gn_second].count_hai(hai) == 2) {
						assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_aka_in_side(hai/10) == 1);
						hai2 += 5;
						// 元々手牌に赤5がある場合に対応させる。赤の処理をどこで行うことが最適か不明なため将来的に不要となる可能性あり。
						// 黒5が手牌に3枚あった場合、ここで追加させるのは赤5の無い大明槓であることに注意。
					}
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_ankan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].get_fuuro_num() < 4);

					if (cal_tav[thread_num].size() >= MAX_TA_NUM_PER_THREAD - BUF_FOR_REACH_PER_THREAD ||
						ta_loc.size() >= MAX_TA_NUM_PER_GROUP - BUF_FOR_REACH_PER_GROUP
					) {
						// 何かアラートを出すべき？
						return;
					}

					cal_tav[thread_num].push_back(cal_tav[ta_loc_gn_first][ta_loc_gn_second]);
					const int loc_second_new = cal_tav[thread_num].size() - 1;
					cal_tav[thread_num][loc_second_new].delete_hai(hai);
					cal_tav[thread_num][loc_second_new].delete_hai(hai);
					cal_tav[thread_num][loc_second_new].delete_hai(hai2);
					cal_tav[thread_num][loc_second_new].add_daiminkan(hai, hai2);
					cal_tav[thread_num][loc_second_new].set_kan_changed_flag(1);
					ta_loc.push_back(Tehai_Location(thread_num, loc_second_new));

					const int gn_new = ta_loc.size()-1;
					assert(ts_map.find(cal_tav[thread_num][loc_second_new].tehai_state) == ts_map.end());
					ts_map[cal_tav[thread_num][loc_second_new].tehai_state] = gn_new;

					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_ankan_num(hai) == 0);
					assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].get_fuuro_num() < 4);
				}
			}
		}
	}
}

void Tehai_Group::add_kakan(
	const int kan_cand[38], boost::unordered_map<Tehai_State2, int>& ts_map,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	const int thread_num
) {
	int tav_size_tmp = ta_loc.size();
	for (int gn=0;gn<tav_size_tmp;gn++) {
		const int ta_loc_gn_first = ta_loc[gn].get_first();
		const int ta_loc_gn_second = ta_loc[gn].get_second();
		for (int hai = 1; hai < 38; hai++) {
			if (hai%10 != 0 && cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 1 && kan_cand[hai] == 1) {
				assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);

				if (cal_tav[thread_num].size() >= MAX_TA_NUM_PER_THREAD - BUF_FOR_REACH_PER_THREAD ||
					ta_loc.size() >= MAX_TA_NUM_PER_GROUP - BUF_FOR_REACH_PER_GROUP
				) {
					// 何かアラートを出すべき？
					return;
				}

				if(cal_tav[ta_loc_gn_first][ta_loc_gn_second].count_hai(hai)==0 && cal_tav[ta_loc_gn_first][ta_loc_gn_second].using_haikind_num(hai) == 3){
					cal_tav[thread_num].push_back(cal_tav[ta_loc_gn_first][ta_loc_gn_second]);
					const int loc_second_new = cal_tav[thread_num].size() - 1;
					cal_tav[thread_num][loc_second_new].change_pon_to_kakan(hai, hai);
					cal_tav[thread_num][loc_second_new].set_kan_changed_flag(1);
					ta_loc.push_back(Tehai_Location(thread_num, loc_second_new));

					const int gn_new = ta_loc.size()-1;
					assert(ts_map.find(cal_tav[thread_num][loc_second_new].tehai_state) == ts_map.end());
					ts_map[cal_tav[thread_num][loc_second_new].tehai_state] = gn_new;
				}
				assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_pon_num(hai) == 1);
				assert(cal_tav[ta_loc_gn_first][ta_loc_gn_second].tehai_state.get_minkan_num(hai) == 0);
			}
		}
	}
}

void Tehai_Group::analyze_all_tenpai(
	const int my_pid, const Game_State& game_state,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav
) {
	for(int gn=0;gn<ta_loc.size();gn++) {
		cal_tav[ta_loc[gn].get_first()][ta_loc[gn].get_second()].analyze_tenpai(my_pid, game_state);
	}
}

void Tehai_Group::analyze_all_agari(
	const int my_pid, const Game_State& game_state,
	std::array<boost::container::static_vector<Tehai_Analyzer_Basic, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& cal_tav,
	boost::container::static_vector<Agari_Calc, MAX_AGARI_NUM_PER_THREAD>& agari_graph,
	//boost::container::static_vector<Agari_Basic, 10>* graph_agariv_cn
	std::array<std::array<std::array<int, 3>, MAX_TA_NUM_PER_THREAD>, CAL_NUM_THREAD>& agari_graph_loc,
	const int thread_num
) {
	for (int gn = 0; gn < ta_loc.size(); gn++) {
		const int ta_loc_gn_first = ta_loc[gn].get_first();
		const int ta_loc_gn_second = ta_loc[gn].get_second();
		agari_graph_loc[ta_loc_gn_first][ta_loc_gn_second][0] = thread_num;
		agari_graph_loc[ta_loc_gn_first][ta_loc_gn_second][1] = agari_graph.size();
		//cal_tav[ta_loc_gn_first][ta_loc_gn_second].analyze_tehai(my_pid, graph_agariv_cn[gn]);
		cal_tav[ta_loc_gn_first][ta_loc_gn_second].analyze_tehai(my_pid, game_state, agari_graph);
		agari_graph_loc[ta_loc_gn_first][ta_loc_gn_second][2] = agari_graph.size();
	}
}
