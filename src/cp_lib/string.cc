
#pragma once
#include "array.cc"

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


Dynamic_Buffer<char>::Dynamic_Buffer(char* _buffer, size_t _cap)
: buffer(_buffer), cap(_cap){}
Dynamic_Buffer<char>::Dynamic_Buffer(const char* c_str) {
    size_t c_str_len = strlen(c_str);
    buffer = (char*)c_str;
    cap = c_str_len;
}

// template <typename T>
// size_t len(String<T> self) {
//     return cap(self);
// }

template <typename T>
void copy(T* dst_ptr, String<T> src) {
    memcpy(dst_ptr, beginp(src), sizeof(T) * len(src));
}

void print_fmt(dbuff<char> self, const char* item_fmt="%c") {
    print_fmt(self.buffer, self.cap, item_fmt);
}
void print(dbuff<char> self) {
    printf("%.*s", self.cap, self.buffer);
}


// template <typename T>
// bool operator==(String<T> f, String<T> s)  {
//     if (len(f) != len(s)) {
//         return false;
//     } else {
//         auto it_f = begin(f);
//         auto it_s = begin(s);
//         for (; it_f != end(f); it_f++, it_s++) {
//             if (*it_f != *it_s) return false;
//         }
//         return true;
//     }
// }

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

int str_cmp(str s1, str s2) {
    int res = strncmp(s1.buffer, s2.buffer, min(len(s1), len(s2)));

    if (res == 0 && len(s1) != len(s2)) {
        return (len(s1) > len(s2)) ? 1 : -1;
    }

    return res;
}


char*
to_c_str(str s) {
    char* c_str = cp::m_alloc<char>(len(s) + 1);
    memcpy(c_str, s.buffer, len(s) * sizeof(char));
    c_str[len(s)] = '\0';
    return c_str;
}

template <typename T>
using Dynamic_String_Buffer = Dynamic_Array<T>;
using dstrb = Dynamic_String_Buffer<char>;

dstrb dstrb_from(const char* c_str) {
    str s = str{c_str};
    dstrb sb; init(&sb, len(s));
    sb.len = len(s);
    copy(sb._dbuff, s);
    return sb;
}

dstrb dstrb_from(str s) {
    dstrb sb; init(&sb, len(s));
    sb.len = len(s);
    copy(sb._dbuff, s);
    return sb;
}

str to_str(dstrb sb) {
    return {sb.buffer, sb.len};
}
char* to_c_str(dstrb sb) {
    return to_c_str(to_str(sb));
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

str
_t_to_str(dstrb b) {
    return to_str(b);
}
str
_t_to_str(str s) {
    return s;
}
str
_t_to_str(const char* s) {
    return str{s};
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

str
find(str s, str token) {
    char* foundp = strstr(s.buffer, token.buffer); // TODO(mb) bad impl, we don't have \0 at the end generally
    return str{foundp, len(token)};
}


void
print(dstrb b) {
    print(to_str(b));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~``

template <typename T, size_t t_cap>
using Static_String_Buffer = Static_Array<T, t_cap>;
using sstrb = Static_String_Buffer<char, 32>;

str to_str(sstrb &sb) {
    return {sb.buffer, sb.len};
}

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


//size_t str_max_len()

template <typename... Ts>
void sprint_fmt(dstrb *self, const char* fmt, Ts... args) {
    size_t max_len = snprintf(null, 0, fmt, args...) + 1; // 1 for \0
    reserve(self, max_len);
    self->len += sprintf(endp(*self), fmt, args...);
}

using dstr = Dynamic_Array<char>;

void init(dstr *self, const char* c_str) {
    size_t init_cap = strlen(c_str);
    self->buffer = (char*)c_str;
    self->cap = init_cap;
    self->len = init_cap;
}

dstr *cat(dstr *first, dstr second) {
    append(first, second);
    return first;
}

template <size_t append_str_count>
dstr *cat(dstr *first, sbuff<dstr, append_str_count> others) {
    for (auto it = begin(others); it < end(others); it++) 
        append(first, *it);
    return first;
}

//alloc
template <size_t append_str_count>
dstr join(sbuff<dstr, append_str_count> strings) {
    dstr first; init(&first);
    for (auto it = begin(strings); it < end(strings); it++) 
        append(first, it);
    return first;
}


void to_str(sstrb *out_sb, size_t value) {
    out_sb->len = sprintf(out_sb->buffer, "%u", value);
}

}

