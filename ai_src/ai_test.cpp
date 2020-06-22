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

    std::vector<Tehai_Pattern_Honors_And_Knitted>& vec = tehai_analyzer.pattern_honors_and_knitted_vec[1];
    vec[0].out_info();

    /*
    for (int i = 0; i < 7; i++) {
        std::vector<Tehai_Pattern_Honors_And_Knitted>& vec = tehai_analyzer.pattern_honors_and_knitted_vec[i];
        std::cout << i << " " << vec.size() << std::endl;
        vec[0].out_info();
    }
    */
}

#ifdef WINSTD
__declspec(dllexport)
#endif
void ai_test() {
	test1();
}