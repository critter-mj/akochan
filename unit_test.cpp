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

void check_hora_points(){
    std::array<int, 4> ten_move;

    // ron
    int hora_actor = 0;
    int hora_target = 1;
    int hora_point = 10;
    int hora_base_point = 8;
    ten_move = ten_move_hora(0, 1, hora_point);

    assert(ten_move[0] == hora_point + hora_base_point); // actor
    assert(ten_move[1] == -(hora_point + hora_base_point)); // target
    assert(ten_move[2] == -hora_base_point);
    assert(ten_move[3] == -hora_base_point);

    // tsumo
    hora_actor = 2;
    hora_target = 2;
    hora_point = 30;
    hora_base_point = 8;
    ten_move = ten_move_hora(0, 1, hora_point);

    assert(ten_move[0] == -(hora_point + hora_base_point)); 
    assert(ten_move[1] == -(hora_point + hora_base_point));
    assert(ten_move[2] == (hora_point + hora_base_point) * 3); // actor & target
    assert(ten_move[3] == -(hora_point + hora_base_point));

}


void exec_unit_test(){
    check_lessor_honors_and_knitted_0();
    check_lessor_honors_and_knitted_1();
    check_hora_points();
}

