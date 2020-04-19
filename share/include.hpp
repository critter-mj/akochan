#pragma once

#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <numeric>
#include <set>
//#include <unordered_map>
//#include <unordered_set>
#include <boost/container/static_vector.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
//#include <unistd.h>

#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mutex>
#include <thread>

#include <string>

#include <boost/asio.hpp>
#include <memory>
#include <boost/bind.hpp>

//#ifdef USE_GTK

//#include <gtk/gtk.h>
//#include <gdk/gdk.h>

//#endif

#ifdef WINSTD
#include <windows.h>
#include <wingdi.h>
#include <direct.h>

#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "json11.hpp"

#ifdef WINOLD

namespace std {
    template < typename T > std::string to_string( const T& n ) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

#endif

#define ENABLE_ENUM_OPERATORS(T)                                    \
    inline T   operator +  (const T   lhs, const T   rhs) { return T(int(lhs) + int(rhs)); } \
    inline T   operator -  (const T   lhs, const T   rhs) { return T(int(lhs) - int(rhs)); } \
    inline T   operator *  (const int lhs, const T   rhs) { return T(lhs      * int(rhs)); } \
    inline T   operator *  (const T   lhs, const int rhs) { return T(int(lhs) *     rhs ); } \
    inline T   operator -  (const T   lhs               ) { return T(-int(lhs)          ); } \
    inline T&  operator += (      T&  lhs, const T   rhs) { return lhs = lhs        + rhs; } \
    inline T&  operator -= (      T&  lhs, const T   rhs) { return lhs = lhs        - rhs; } \
    inline T&  operator *= (      T&  lhs, const int rhs) { return lhs = T(int(lhs) * rhs); } \
    inline T&  operator ++ (      T&  lhs               ) { return lhs = T(int(lhs) + 1  ); } \
    inline T&  operator -- (      T&  lhs               ) { return lhs = T(int(lhs) - 1  ); } \
    inline T   operator /  (const T   lhs, const int rhs) { return T(int(lhs) / rhs); } \
    inline int operator /  (const T   lhs, const T   rhs) { return int(lhs) / int(rhs); } \
    inline T&  operator /= (      T&  lhs, const int rhs) { return lhs = T(int(lhs) / rhs); }

void assert_with_out(const bool true_condition, const std::string& comment);
bool str_starts_with(const std::string& str, const std::string& pre);
std::vector<std::string> str_split(const std::string& str, const char del);
bool check_openable_file(const std::string& file_name);
json11::Json load_json_from_file(const std::string& file_name);
std::vector<json11::Json> load_json_vec_from_file(const std::string& file_name);
void dump_json_to_file(const json11::Json& json, const std::string& file_name);
void dump_json_vec_to_file(const std::vector<json11::Json>& json_vec, const std::string& file_name);
std::string json_vec_to_str(const std::vector<json11::Json>& json_vec);
std::string json_vec2d_to_str(const std::vector<std::vector<json11::Json>>& json_vec2d);
std::vector<std::string> get_files_path(const std::string& dir_name);
void make_dir(const std::string& dir_name);
