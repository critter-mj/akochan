#include "yaku_dist.hpp"

int yakuhai_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const int yakuhai, const Game_State& game_state){
    for(const Fuuro_Elem f : fuuro) {
        if (f.consumed[0] == yakuhai) {
            return 0;
        }
    }
    if (3 <= tehai[yakuhai]) {
        return 0;
    }
    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    if (2 <= hai_visible_all[yakuhai]) {
        return 4; // とりあえず。
    } else {
        return 3 - tehai[yakuhai];
    }
}

int honitsu_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro){
    int dist = 13;
    int flags[3];
    for(int i=0;i<3;i++){
        flags[i] = 1;
    }
    for (Color_Type color = (Color_Type)0; color < CT_JIHAI; ++color){
        for(const Fuuro_Elem f : fuuro) {
            if(hai_color(f.consumed[0]) != color && hai_color(f.consumed[0]) != CT_JIHAI) {
                flags[color] = 0;
                break;
            }
        }
        if (flags[color]) {
            int other_num = 0;
            for (int c2 = 0; c2 < 3; c2++) {
                if (c2 != color) {
                    for (int i = 1; i <= 9; i++) {
                        other_num += tehai[10*c2 + i];
                    }
                }
            }
            dist = std::min(dist, other_num);
        }
    }
    return dist;
}

int tanyao_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro){
    for(const Fuuro_Elem f : fuuro) {
        if (f.type == FT_CHI) {
            if (hai_yaochuu(f.hai) != NT_CHUJAN || hai_yaochuu(f.consumed[0]) != NT_CHUJAN || hai_yaochuu(f.consumed[1]) != NT_CHUJAN) {
                return 13;
            }
        } else if (hai_yaochuu(f.consumed[0]) != NT_CHUJAN) {
            return 13;
        }
    }
    int dist = 0;
    for (int hai = 0; hai < 38; hai++) {
        if(tehai[hai] > 0 && hai_yaochuu(hai) != NT_CHUJAN) {
            dist += tehai[hai];
        }
    }
    return dist;
}



int chanta_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state){
    // ヘッドは面倒なので数えない。
    for (const Fuuro_Elem f : fuuro) {
        if (f.type == FT_CHI) {
            if (hai_yaochuu(f.hai) == NT_CHUJAN && hai_yaochuu(f.consumed[0]) == NT_CHUJAN && hai_yaochuu(f.consumed[1]) == NT_CHUJAN) {
                return false;
            }
        } else if (hai_yaochuu(f.consumed[0]) == NT_CHUJAN) {
            return false;
        } 
    }

    int dist = 0;
    for(int c = 0; c < 3; c++) {
        dist += tehai[10*c+4] + tehai[10*c+5] + tehai[10*c+6];
    }

    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);
    for(int c = 0; c < 3; c++) {
        dist += std::max(0, tehai[10*c+2] - std::min(tehai[10*c+1], 4 - hai_visible_all_kind[10*c+3]));
        dist += std::max(0, tehai[10*c+3] - std::min(tehai[10*c+1], 4 - hai_visible_all_kind[10*c+2]));

        dist += std::max(0, tehai[10*c+8] - std::min(tehai[10*c+9], 4 - hai_visible_all_kind[10*c+7]));
        dist += std::max(0, tehai[10*c+7] - std::min(tehai[10*c+9], 4 - hai_visible_all_kind[10*c+8]));
    }

    return dist;
}

int toitoi_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state) {
    int dist = 13;
    for (const Fuuro_Elem f : fuuro) {
        if (f.type == FT_CHI) {
            return dist;
        }
    }
    dist -= fuuro.size()*3;
    int kotu_cand = fuuro.size();
    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);

    for(int hai=0;hai<38;hai++){
        if (3 <= tehai[hai]) {
            dist -= 3;
            kotu_cand++;
        }
    }
    for(int hai = 0; hai < 38; hai++) {
        if (tehai[hai] == 2 && hai_visible_all_kind[hai] < 2 && kotu_cand < 4) {
            dist -= 2;
            kotu_cand++;
        }
    }
    return dist;
}

int sanshikidoukou_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state) {
    int kotu[38];

    for (int hai = 0; hai < 38; hai++) {
        if (tehai[hai] == 3) {
            kotu[hai] = 1;
        } else {
            kotu[hai] = 0;
        }
    }

    for (const Fuuro_Elem f : fuuro) {
        if (f.type == FT_PON || f.type == FT_ANKAN || f.type == FT_DAIMINKAN || f.type == FT_KAKAN) {
            kotu[haikind(f.consumed[0])]++;
        }
    }

    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);
    int dist = 9;
    for (int i = 1; i <= 9; i++) {
        int tmp_dist = 9;
        for (int c = 0; c < 3; c++) {
            const int hai = 10*c + i;
            if (kotu[hai] == 1) {
                tmp_dist -= 3;
            } else if (hai_visible_all_kind[hai] < 2) {
                tmp_dist -= tehai[hai];
            } else {
                tmp_dist += 9;
            }
        }
        dist = std::min(dist, tmp_dist);
    }
    return dist;
}

int sanshiki_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state) {
    int syuntu[30] = {};
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            syuntu[std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]})]++;
        }
    }
    int dist = 9;
    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);
    for (int i = 1; i <= 7; i++) {
        int tmp_dist = 9;
        for (int c = 0; c < 3; c++) {
            const int hai = 10*c + i;
            if (0 < syuntu[hai]) {
                tmp_dist -= 3;
            } else {
                for (int j = 0; j < 3; j++) {
                    if (0 < tehai[hai+j]) {
                        tmp_dist--;
                    } else if (hai_visible_all_kind[hai] == 4) {
                        tmp_dist += 9;
                    }
                }
            }
        }
        dist = std::min(dist, tmp_dist);
    }
    return dist;
}

int ittsuu_dist(const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state) {
    int have[30] = {}; 
    for (int fn = 0; fn < fuuro.size(); fn++) {
        if (fuuro[fn].type == FT_CHI) {
            const int smallest = std::min({fuuro[fn].hai, fuuro[fn].consumed[0], fuuro[fn].consumed[1]});
            if (((smallest % 10) % 3) == 1) {
                have[smallest] = 1;
                have[smallest + 1] = 1;
                have[smallest + 2] = 1;
            }
        }
    }
    int dist = 9;
    const Hai_Array hai_visible_all = get_hai_visible_all(game_state);
    const Hai_Array hai_visible_all_kind = haikind(hai_visible_all);
    for (int c = 0; c < 3; c++) {
        int tmp_dist = 9;
        for (int i = 1; i <= 9; i++) {
            const int hai = 10*c + i;
            if (have[hai] == 1 || 0 < tehai[hai]) {
                tmp_dist--;
            } else if (hai_visible_all_kind[hai] == 4) {
                tmp_dist += 9;
            }
        }
        dist = std::min(dist, tmp_dist);
    }
    return dist;
}

// anko_num_count, kan_num_count

double calc_yaku_dist(const int my_pid, const Hai_Array& tehai, const Fuuro_Vector& fuuro, const Game_State& game_state) {
    double dist = 13.0;
    const int bakaze_dist = yakuhai_dist(tehai, fuuro, 31 + game_state.bakaze, game_state);
    if (bakaze_dist <= 1) {
        dist = std::min(dist, 0.5 * bakaze_dist);
    }

    const int jikaze_dist = yakuhai_dist(tehai, fuuro, 31 + game_state.player_state[my_pid].jikaze, game_state);
    if (jikaze_dist <= 1) {
        dist = std::min(dist, 0.5 * jikaze_dist);
    }

    for(int hai = 35; hai < 38; hai++) {
        const int tmp_dist = yakuhai_dist(tehai, fuuro, hai, game_state);
        if (tmp_dist <= 1) {
            dist = std::min(dist, 0.5 * tmp_dist);
        }
    }
    dist = std::min(dist, 0.6 * honitsu_dist(tehai, fuuro));
    dist = std::min(dist, 0.6 * tanyao_dist(tehai, fuuro));
    dist = std::min(dist, (double)chanta_dist(tehai, fuuro, game_state));
    dist = std::min(dist, (double)toitoi_dist(tehai, fuuro, game_state));
    dist = std::min(dist, (double)sanshikidoukou_dist(tehai, fuuro, game_state));
    dist = std::min(dist, (double)sanshiki_dist(tehai, fuuro, game_state));
    dist = std::min(dist, (double)ittsuu_dist(tehai, fuuro, game_state));

    return dist;
}
