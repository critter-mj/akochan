#include "ai_test.hpp"

void test1() {
    Game_State game_state;
    const int my_pid = 0;
    Hai_Array tehai = {};
    Fuuro_Vector fuuro;

    tehai[2] = 1; tehai[5] = 1; tehai[8] = 1;
    tehai[11] = 1; tehai[14] = 1;
    tehai[23] = 2;
    tehai[31] = 1; tehai[32] = 1; tehai[33] = 1; tehai[34] = 1; tehai[35] = 1; tehai[36] = 1;

    Tehai_Analyzer tehai_analyzer;
	tehai_analyzer.reset_tehai_analyzer_with(tehai, fuuro);

    tehai_analyzer.pattern_flag = 1;
	tehai_analyzer.analyze_tenpai(my_pid, game_state);
    std::cout << "shanten_num:" << tehai_analyzer.get_shanten_num() << std::endl;

    std::vector<Tehai_Pattern_Honors_And_Knitted>& vec = tehai_analyzer.pattern_honors_and_knitted_vec[1];
    vec[0].out_info();

    Tehai_Calculator tehai_calculator;
	tehai_calculator.reset(my_pid, false);
	tehai_calculator.fuuro_cand_hai = 0;
	tehai_calculator.get_effective(game_state, tehai_analyzer);
	tehai_calculator.tehai_all_num = tehai_analyzer.get_tehai_num() + tehai_analyzer.get_fuuro_num()*3;

    std::cout << "tehai_all_num:" << tehai_calculator.tehai_all_num << std::endl;

    for (int hai = 0; hai < 38; hai++) {
        if (tehai_calculator.effective_flag[hai] == 1) {
            std::cout << "effective:" << hai << std::endl;
        }
    }
    
    /*
	tehai_calculator.set_candidates3_single_thread(
		game_state, tehai_analyzer, tehai_analyzer_af,
		std::max({tehai_analyzer.mentu_change_num_max, tehai_analyzer.titoi_change_num_max, tehai_analyzer_af.mentu_change_num_max}), tactics
	);
    */
}

#ifdef WINSTD
__declspec(dllexport)
#endif
void ai_test() {
	test1();
}