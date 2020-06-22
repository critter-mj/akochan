#include "main.hpp"

void check_lessor_honors_and_knitted_0(){
    Hai_Array tehai = {};
    Fuuro_Vector fuuro;

    tehai[2] = 1; tehai[5] = 1; tehai[8] = 1;
    tehai[11] = 1; tehai[14] = 1;
    tehai[23] = 1; tehai[29] = 1;
    tehai[31] = 1; tehai[32] = 1; tehai[33] = 1; tehai[34] = 1; tehai[35] = 1; tehai[36] = 1;

    Tenpai_Info tenpai_info = cal_tenpai_info(0, 0, tehai, fuuro);
    assert_with_out(tenpai_info.honors_and_knitted_shanten_num == 0, "check_lessor_honors_and_knitted 0");
    assert_with_out(tenpai_info.agari_vec.size() == 3, "check_lessor_honors_and_knitted 1");

    for (const auto& agari : tenpai_info.agari_vec) {
        std::cout << "agari_hai:" << agari.hai << std::endl;
        if (agari.hai == 37) {
            assert_with_out(agari.result_tsumo.greater_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 2");
            assert_with_out(agari.result_ron.greater_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 3");
            assert_with_out(!agari.result_tsumo.lesser_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 4");
            assert_with_out(!agari.result_ron.lesser_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 5");
            assert_with_out(agari.result_tsumo.calc_point() == 24, "check_lessor_honors_and_knitted 6");
            assert_with_out(agari.result_ron.calc_point() == 24, "check_lessor_honors_and_knitted 7");
        } else {
            assert_with_out(!agari.result_tsumo.greater_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 8");
            assert_with_out(!agari.result_ron.greater_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 9");
            assert_with_out(agari.result_tsumo.lesser_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 10");
            assert_with_out(agari.result_ron.lesser_honors_and_knitted_tiles, "check_lessor_honors_and_knitted 11");
            assert_with_out(agari.result_tsumo.calc_point() == 12, "check_lessor_honors_and_knitted 12");
            assert_with_out(agari.result_ron.calc_point() == 12, "check_lessor_honors_and_knitted 13");
        }
    }
    std::cout << "check_lessor_honors_and_knitted_0 done" << std::endl;
}

void check_lessor_honors_and_knitted_1(){
    Hai_Array tehai = {};
    Fuuro_Vector fuuro;

    tehai[2] = 1; tehai[5] = 1; tehai[8] = 1;
    tehai[11] = 1; tehai[14] = 1;
    tehai[23] = 2;
    tehai[31] = 1; tehai[32] = 1; tehai[33] = 1; tehai[34] = 1; tehai[35] = 1; tehai[36] = 1;

    Tenpai_Info tenpai_info = cal_tenpai_info(0, 0, tehai, fuuro);
    assert_with_out(tenpai_info.honors_and_knitted_shanten_num == 13, "check_lessor_honors_and_knitted 0");
    assert_with_out(tenpai_info.agari_vec.size() == 0, "check_lessor_honors_and_knitted 1");

    std::cout << "check_lessor_honors_and_knitted_1 done" << std::endl;
}

void exec_unit_test(){
    check_lessor_honors_and_knitted_0();
    check_lessor_honors_and_knitted_1();
}