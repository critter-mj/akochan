#include "calc_agari.hpp"

Agari_Info calc_agari(
    const int bakaze_hai, const int jikaze_hai,
    const Hai_Array& tehai, const Hai_Array& tehai_tate_cut, const Hai_Array& tehai_tmp, const Fuuro_Vector& fuuro,
    const int machi_hai, const Machi_Type machi_type, const bool titoi_flag
) {
    Hai_Array tehai_tate;
    for(int hai = 0; hai < 38; hai++) {
        tehai_tate[hai] = tehai[hai] - tehai_tate_cut[hai];
    }
    
    Agari_Info agari_info;
    agari_info.hai = machi_hai;

    int peko_num, kan_num, anko_num, anko3_flag, anko4_flag; // 暗刻役はツモかロンかで変わりうるので、フラグを整数で持つ。0 : 役にならない、1 : ツモなら役になる。2 : 必ず役になる。
    bool menzen_flag, bakaze_flag, jikaze_flag, sangen_flag[3], kaze_flag[4], pinfu_flag;
    bool chinitsu_flag, honitsu_flag, tanyao_flag, junchan_flag, chanta_flag, toitoi_flag;
    bool peko1_flag, peko2_flag, kan3_flag, kan4_flag, sanshikidoujun_flag, sanshikidoukou_flag, ittsuu_flag; // to do doukou
    bool daisangen_flag, syousangen_flag, chinrou_flag, honrou_flag, chuuren_flag, suitsu_flag, daisushi_flag, syousushi_flag;

    menzen_flag = menzen_check(fuuro);
    chinitsu_flag = chinitsu_check(tehai, fuuro);
    honitsu_flag = chinitsu_flag ? false : honitsu_check(tehai, fuuro);
    tanyao_flag = tanyao_check(tehai, fuuro, machi_hai);
    honrou_flag = honrou_check(tehai, fuuro, machi_hai);
    suitsu_flag = suitsu_check(tehai, fuuro);

    if (titoi_flag) {
        bakaze_flag = false;
        jikaze_flag = false;
        sangen_flag[0] = false;
        sangen_flag[1] = false;
        sangen_flag[2] = false;
        pinfu_flag = false;
        junchan_flag = false;
        chanta_flag = false;
        toitoi_flag = false;
        peko1_flag = false;
        peko2_flag = false;
        sanshikidoujun_flag = false;
        sanshikidoukou_flag = false;
        ittsuu_flag = false;
        anko3_flag = 0;
        anko4_flag = 0;
        kan3_flag = false;
        kan4_flag = false;
        daisangen_flag = false;
        syousangen_flag = false;
        chinrou_flag = false;
        chuuren_flag = false;
        syousushi_flag = false;
        daisushi_flag = false;
    } else {
        for(int i = 0; i < 3; i++) {
            sangen_flag[i] = yakuhai_check(35 + i, tehai_tate, fuuro, machi_hai, machi_type);
        }
        for(int i = 0; i < 4; i++) {
            kaze_flag[i] = yakuhai_check(31 + i, tehai_tate, fuuro, machi_hai, machi_type);
        }
        bakaze_flag = kaze_flag[bakaze_hai - 31];
        jikaze_flag = kaze_flag[jikaze_hai - 31];
        pinfu_flag = pinfu_check(tehai_tate, fuuro, machi_type, bakaze_hai, jikaze_hai);

        junchan_flag = junchan_check(tehai, tehai_tate, fuuro, tehai_tmp, machi_hai);
        chanta_flag = junchan_flag ? false : chanta_check(tehai, tehai_tate, fuuro, tehai_tmp, machi_hai);

        toitoi_flag = toitoi_check(tehai_tate_cut, fuuro, tehai_tmp, machi_type);

        peko_num = peko_num_count(tehai_tate_cut, fuuro, tehai_tmp, machi_hai, machi_type);
        peko2_flag = (peko_num == 2);
        peko1_flag = (peko_num == 1);
        
        sanshikidoujun_flag = sanshikidoujun_check(tehai_tate_cut, fuuro, tehai_tmp, machi_hai, machi_type);
        sanshikidoukou_flag = sanshikidoukou_check(tehai_tate, fuuro, machi_hai, machi_type);
        ittsuu_flag = ittsuu_check(tehai_tate_cut, fuuro, tehai_tmp, machi_hai, machi_type);

        anko_num = anko_num_count(tehai_tate, fuuro);
        if (anko_num < 2) {
            anko4_flag = 0;
            anko3_flag = 0;
        } else if (anko_num == 2) {
            anko4_flag = 0;
            if (machi_type == MT_SHABO) {
                anko3_flag = 1;
            } else {
                anko3_flag = 0;
            }
        } else if (anko_num == 3) {
            anko3_flag = 2;
            if (machi_type == MT_SHABO) {
                anko4_flag = 1;
            } else {
                anko4_flag = 0;
            }
        } else if(anko_num == 4) {
            anko4_flag = 2;
            anko3_flag = 0;
        }

        kan_num = kan_num_count(fuuro);
        kan3_flag = (kan_num == 3);
        kan4_flag = (kan_num == 4);

        daisangen_flag = false;
        syousangen_flag = false;
        if (sangen_flag[0] && sangen_flag[1] && sangen_flag[2]) {
            daisangen_flag = true;
        } else {
            for(int i = 0; i < 3; i++) {
                if (sangen_flag[(i+1)%3] && sangen_flag[(i+2)%3] && (tehai_tate[35+i] == 2 || machi_hai == 35 + i)) {
                    syousangen_flag = true;
                }
            }
        }

        daisushi_flag = false;
        syousushi_flag = false;
        if (kaze_flag[0] && kaze_flag[1] && kaze_flag[2] && kaze_flag[3] == 4) {
            daisushi_flag = true;
        } else {
            for (int i = 0; i < 4; i++) {
                if (kaze_flag[(i+1)%4] && kaze_flag[(i+2)%4] && kaze_flag[(i+3)%4] && (tehai_tate[31+i] == 2 || machi_hai == 31+i)) {
                    syousushi_flag = true;
                }
            }
        }

        chuuren_flag = chinitsu_flag && chuuren_check(tehai, fuuro, machi_hai);
        chinrou_flag = junchan_flag && toitoi_flag;
        if (chinrou_flag) {
            honrou_flag = false;
        }
    }

    int han_tsumo = 0;
    int han_ron = 0;
    if (chinrou_flag || daisangen_flag || kan4_flag || chuuren_flag || suitsu_flag || daisushi_flag || syousushi_flag || anko4_flag==2) {
        han_tsumo = 13;
        han_ron = 13;
    } else {
        if (bakaze_flag) { han_tsumo++; }
        if (jikaze_flag) { han_tsumo++; }
        if (sangen_flag[0]) { han_tsumo++; }
        if (sangen_flag[1]) { han_tsumo++; }
        if (sangen_flag[2]) { han_tsumo++; }
        if (pinfu_flag) { han_tsumo++; }
        if (titoi_flag) { han_tsumo += 2; }
        if (tanyao_flag) { han_tsumo++; }
        if (toitoi_flag) { han_tsumo += 2; }
        if (peko1_flag) { han_tsumo++; }
        if (peko2_flag) { han_tsumo += 3; }
        if (syousangen_flag) { han_tsumo += 2; }
        if (honrou_flag) { han_tsumo += 2; }
        if (kan3_flag) { han_tsumo += 2; }
        if (sanshikidoukou_flag) { han_tsumo += 2; }
        if (chinitsu_flag) { han_tsumo += menzen_flag ? 6 : 5; }
        if (honitsu_flag) { han_tsumo += menzen_flag ? 3 : 2; }
        if (junchan_flag) { han_tsumo += menzen_flag ? 3 : 2; }
        if (chanta_flag) { han_tsumo += menzen_flag ? 2 : 1; }
        if (sanshikidoujun_flag) { han_tsumo += menzen_flag ? 2 : 1; }
        if (ittsuu_flag) { han_tsumo += menzen_flag ? 2 : 1; }
        if (anko3_flag == 2) { han_tsumo += 2; }
        
        han_ron = han_tsumo;
        if (anko4_flag == 1) {
            han_tsumo = 13;
        } else if(anko3_flag == 1) {
            han_tsumo += 2;
        }
        if (menzen_flag) { han_tsumo++; }
    }

    agari_info.han_tsumo = han_tsumo;
    agari_info.han_ron = han_ron;

    std::pair<int, int> fu_pair = calc_fu(
        bakaze_hai, jikaze_hai,
        tehai, tehai_tate_cut, tehai_tate, tehai_tmp, fuuro,
        machi_hai, machi_type, menzen_flag, pinfu_flag, titoi_flag
    );
    agari_info.fu_tsumo = fu_pair.first;
    agari_info.fu_ron = fu_pair.second;
    return agari_info;
}