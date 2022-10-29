
#pragma once
#include "string.h"

namespace cp {

Dynamic_Buffer<char>::Dynamic_Buffer(char* _buffer, size_t _cap)
: buffer(_buffer), cap(_cap){}
Dynamic_Buffer<char>::Dynamic_Buffer(const char* c_str) {
    size_t c_str_len = strlen(c_str);
    buffer = (char*)c_str;
    cap = c_str_len;
}

void print_fmt(dbuff<char> self, const char* item_fmt) {
    print_fmt(self.buffer, self.cap, item_fmt);
}
void print(dbuff<char> self) {
    printf("%.*s", self.cap, self.buffer);
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

str to_str(sstrb &sb) {
    return {sb.buffer, sb.len};
}

}

