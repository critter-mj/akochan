#pragma once

#include "../share/include.hpp"
#include "../share/types.hpp"

uint32_t bit_to_num(uint32_t bit, int x);

class Bit_Hai_Num {
private:

public:
    uint32_t color[4];
    int size;
    Bit_Hai_Num& operator=(const Bit_Hai_Num& rhs);
    bool operator==(const Bit_Hai_Num& rhs) const;
    bool operator!=(const Bit_Hai_Num& rhs) const;
    Bit_Hai_Num();
    void reset();

    int get_size() const;
    int count_hai(const int hai) const;

    void add_hai(const int hai);
    void delete_hai(const int hai);

    void set_from_array38(const int tehai[38]);
    void insert_to_array38(Hai_Array& tehai) const;

    void out_info() const;
    size_t hash_value() const;
};

size_t hash_value(const Bit_Hai_Num& bit_hai_num);
