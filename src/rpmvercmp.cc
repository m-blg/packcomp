#include "cp_lib/string.cc"
#include "cp_lib/io.cc"
#include "cp_lib/algorithm.cc"



using namespace cp;

size_t 
parse_uint(str s) {
    size_t l = len(s);

    size_t n = 0;
    for (size_t i = 0; i < len(s); i++) {
        n = n*10 + s[i] - '0';
    }

    return n;
}

void 
parse_version(str version, int *major, int *minor, int *revision, int *build) {
    darr<str> tokens; init(&tokens);
    split(&tokens, version, '.');

    *major = (len(tokens) > 0) ? parse_uint(tokens[0]) : -1;
    *minor = (len(tokens) > 1) ? parse_uint(tokens[1]) : -1;
    *revision = (len(tokens) > 2) ? parse_uint(tokens[2]) : -1;
    *build = (len(tokens) > 3) ? parse_uint(tokens[3]) : -1;
}

int 
version_cmp(str ver1, str ver2) {
    int major1, minor1, rev1, build1;
    int major2, minor2, rev2, build2;

    parse_version(ver1, &major1, &minor1, &rev1, &build1);
    parse_version(ver2, &major2, &minor2, &rev2, &build2);

    if (major1 < major2) return -1;
    if (major1 > major2) return 1;
    if (minor1 < minor2) return -1;
    if (minor1 > minor2) return 1;
    if (rev1 < rev2) return -1;
    if (rev1 > rev2) return 1;
    if (build1 < build2) return -1;
    if (build1 > build2) return 1;
    return 0;

}

inline bool is_digit(char c) {
    return ('0' <= c && c <= '9');
}
inline bool is_lc_letter(char c) {
    return ('a' <= c && c <= 'z');
}
inline bool is_uc_letter(char c) {
    return ('A' <= c && c <= 'Z');
}
inline bool is_letter(char c) {
    return is_lc_letter(c) || is_uc_letter(c);
}
inline bool is_alphanum(char c) {
    return is_digit(c) || is_letter(c);
}


bool
try_parse_uint(str s, size_t *n, str *rest) {
    size_t l = len(s);
    if (l == 0 || !is_digit(s[0]))
        return false;

    *n = 0;
    size_t i = 0;
    for (; i < len(s); i++) {
        if (!is_digit(s[i]))
            break;
        *n = *n * 10 + s[i] - '0';
    }

    *rest = drop(s, i);
    return true;
}

bool
try_parse_alph(str s, str *token, str *rest) {
    size_t l = len(s);
    if (l == 0)
        return false;

    size_t i = 0;
    for (; i < len(s); i++) {
        if (!is_letter(s[i]))
            break;
    }

    *token = take(s, i);
    *rest = drop(s, i);
    return true;
}

str
skip_nonalphanum(str s) {
    size_t i = 0;
    for (; i < len(s); i++) {
        if (is_alphanum(s[i]))
            break;
    }
    return drop(s, i);
}

// According to https://fedoraproject.org/wiki/Archive:Tools/RPM/VersionComparison
int 
rpmvercmp(str ver1, str ver2) {
    if (ver1 == ver2)
        return 0;

    while (true) {
        if (len(ver1) == 0)
            return (len(ver2) == 0) ? 0 : -1;
        if (len(ver2) == 0)
            return (len(ver1) == 0) ? 0 : 1;
        
        str token1, token2;
        size_t n1, n2;

        ver1 = skip_nonalphanum(ver1);
        ver2 = skip_nonalphanum(ver2);

        if (try_parse_uint(ver1, &n1, &ver1)) {
            if (try_parse_uint(ver2, &n2, &ver2)) {
                if (n1 == n2)
                    continue;
                
                return (n1 > n2) ? 1 : -1;
            }

            // numeric over alphabetic
            if (try_parse_alph(ver2, &token2, &ver2)) {
                return 1;
            }
        }
        if (try_parse_alph(ver1, &token1, &ver1)) {
            // numeric over alphabetic
            if (try_parse_uint(ver2, &n2, &ver2)) {
                return -1;
            }

            if (try_parse_alph(ver2, &token2, &ver2)) {
                int res = str_cmp(token1, token2);
                if (res == 0)
                    continue;
                return res;
            }
        }

        assert(false && "Bad labels");
    }
}
