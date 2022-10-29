
#pragma once
#include "array.h"

namespace cp {

template <typename T>
using String = Dynamic_Buffer<T>;
using str = String<char>;

template <>
struct Dynamic_Buffer<char> {
    char* buffer;
    size_t cap; // in items

    Dynamic_Buffer() = default;
    Dynamic_Buffer(char* _buffer, size_t _cap);
    Dynamic_Buffer(const char* c_str);

    char& operator[](size_t index) {
        assert(("Index out of range", index < cap));
        return buffer[index];
    }

    typedef char type;
};

template <typename T>
using Dynamic_String_Buffer = Dynamic_Array<T>;
using dstrb = Dynamic_String_Buffer<char>;

template <typename T, size_t t_cap>
using Static_String_Buffer = Static_Array<T, t_cap>;
using sstrb = Static_String_Buffer<char, 32>;



void print_fmt(dbuff<char> self, const char* item_fmt="%c");
void print(dbuff<char> self);

int str_cmp(str s1, str s2);

char*
to_c_str(str s);

dstrb dstrb_from(const char* c_str);
dstrb dstrb_from(str s);

str to_str(dstrb sb);
char* to_c_str(dstrb sb);

str
_t_to_str(dstrb b);
str
_t_to_str(str s);
str
_t_to_str(const char* s);

str to_str(sstrb &sb);

str
find(str s, str token);

void
print(dstrb b);




template <typename T>
void copy(T* dst_ptr, String<T> src) {
    memcpy(dst_ptr, beginp(src), sizeof(T) * len(src));
}

template <typename T>
bool operator==(String<T> f, String<T> s)  {
    if (len(f) != len(s)) {
        return false;
    }

    auto it_f = begin(f);
    auto it_s = begin(s);
    for (; it_f != end(f); it_f++, it_s++) {
        if (*it_f != *it_s) return false;
    }
    return true;
}

template <typename T>
bool operator!=(String<T> f, String<T> s)  {
    return !(f == s);
}


template <typename T>
void 
push_str(Dynamic_String_Buffer<T> *out_sb, str s) {
    size_t total_len = len(*out_sb) + len(s);
    if (out_sb->cap < total_len) {
        reserve(out_sb, total_len);
    }

    // append
    copy(beginp(*out_sb) + len(*out_sb), s);
    out_sb->len += len(s);
}

template <typename T, typename t_token_buff>
void 
push_str_buff(Dynamic_String_Buffer<T> *out_sb, t_token_buff tokens) {
    size_t total_len = len(*out_sb);
    for (auto it = begin(tokens); it != end(tokens); it++) {
        total_len += len(*it);
    }
    if (out_sb->cap < total_len) {
        resize(&out_sb->_dbuff, total_len);
    }

    // append
    for (auto it = begin(tokens); it != end(tokens); it++) {
        copy(beginp(*out_sb) + len(*out_sb), *it);
        out_sb->len += len(*it);
    }
}

// BUG(mb) in nested calls like cat(..., cat(...)) it will leak; I guess we have to use destructors
// or just no support for nested calls
template <typename... Ts>
dstrb
cat(Ts... tokens) {
    auto b = dstrb_from("");
    push_str_buff(&b, pack(_t_to_str(tokens)...));
    return b;
}

template <typename t_token_buff>
dstrb
join(t_token_buff tokens) {
    auto b = dstrb_from("");
    push_str_buff(&b, tokens);
    return b;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~``

template <typename T, size_t t_cap, typename t_token_buff>
void cat(Static_String_Buffer<T, t_cap> *out_sb, t_token_buff tokens) {
    size_t total_len = len(*out_sb);
    for (auto it = begin(tokens); it != end(tokens); it++) {
        total_len += len(*it);
    }
    assert(t_cap >= total_len);

    // append
    for (auto it = begin(tokens); it != end(tokens); it++) {
        copy(beginp(*out_sb) + len(*out_sb), *it);
        out_sb->len += len(*it);
    }
}


template <typename T, typename t_arr>
void split(t_arr *out_tokens, String<T> s, T delim, bool is_empty_ignored=true) {
    String<T> token = { beginp(s), 0 };
    for (auto it = begin(s); it != end(s); it++) {
        if (*it == delim) {
            if (!is_empty_ignored || len(token) != 0)
                push(out_tokens, token);
            token.buffer += token.cap + 1;
            token.cap = 0;
            continue;
        }
        token.cap++;
    }
    push(out_tokens, token);
}

template <typename t_arr, typename t_delim_buff>
void split(t_arr *out_tokens, typename t_arr::type s, t_delim_buff delim_buff, bool is_empty_ignored=true) {
    typename t_arr::type token = { beginp(s), 0 };
    for (auto it = begin(s); it != end(s); it++) {
        for (auto delim = begin(delim_buff); delim != end(delim_buff); delim++) {
            if (*it == *delim) {
                if (!is_empty_ignored || len(token) != 0)
                    push(out_tokens, token);
                token.buffer += token.cap + 1;
                token.cap = 0;
                goto for_end;
            }
        }
        token.cap++;
        for_end: ;
    }
    push(out_tokens, token);
}


template <typename... Ts>
void sprint_fmt(dstrb *self, const char* fmt, Ts... args) {
    size_t max_len = snprintf(null, 0, fmt, args...) + 1; // 1 for \0
    reserve(self, max_len);
    self->len += sprintf(endp(*self), fmt, args...);
}


}

