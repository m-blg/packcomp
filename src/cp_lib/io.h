

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "string.h"


namespace cp {

inline void print(u32 value) {
    printf("%u", value);
}
inline void print(u64 value) {
    printf("%lu", value);
}
inline void print(i32 value) {
    printf("%i", value);
}
inline void print(i64 value) {
    printf("%li", value);
}
inline void print(f32 value) {
    printf("%f", value);
}
inline void print(f64 value) {
    printf("%lf", value);
}
inline void print(char value) {
    printf("%c", value);
}
inline void print(bool value) {
    printf("%i", value);
}
inline void print(const char* value) {
    printf("%s", value);
}

template <typename t_c_arg, typename... t_rest_args>
void print(t_c_arg c_arg, t_rest_args... rest_args) {
    print(c_arg);
    print(rest_args...);
}

//template <typename... Args>
//void print(Args... args) {
    //print(args...);
//}

template <class list_t>
void print(list_t list) {
    print('[');
    for (auto it = begin(list); it != end(list); it++) {
        print(*it);
        print(", ");
    }
    print(']');
}


template <typename T>
void print(dbuff<T> self) {
    print('[');
    for (auto it = begin(self); it != end(self); it++) {
        print(*it);
        print(", ");
    }
    print(']');
}

template <typename... Ts>
inline void println(Ts... x) { print(x..., "\n"); }

template <typename T>
void read_array(dbuff<T> buffer, FILE* file) {
    fread(buffer.buffer, sizeof(T), cap(&buffer), file);
}

template <typename buff_t>
void write(buff_t b, const char* file_name) {
    FILE* file = fopen(file_name, "w");
    if (file == null) {
        // set error flag
        return;
    }

    fwrite(beginp(b), sizeof(typename buff_t::type), len(b), file);
    fclose(file);
}
}