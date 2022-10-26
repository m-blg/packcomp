#include "criterion/criterion.h"

#include "rpmvercmp.cc"

Test(parse_uint, test1) {
    cr_expect(parse_uint(str{""}) == 0, "parsing broken");
    cr_expect(parse_uint(str{"0010"}) == 10, "parsing broken");
    cr_expect(parse_uint(str{"10510"}) == 10510, "parsing broken");
}

Test(is_char_group, test1) {
    cr_expect(is_digit('3'), "broken classification");
    cr_expect(is_digit('0'), "broken classification");
    cr_expect(is_digit('9'), "broken classification");

    cr_expect(is_lc_letter('a'), "broken classification");
    cr_expect(is_lc_letter('b'), "broken classification");
    cr_expect(is_lc_letter('z'), "broken classification");

    cr_expect(is_uc_letter('A'), "broken classification");
    cr_expect(is_uc_letter('B'), "broken classification");
    cr_expect(is_uc_letter('Z'), "broken classification");

    cr_expect(is_alphanum('3'), "broken classification");
    cr_expect(is_alphanum('9'), "broken classification");
    cr_expect(is_alphanum('Z'), "broken classification");
}

Test(strcmp, test1) {
    cr_expect(strcmp("word", "word2") < 0, "");
    cr_expect(strcmp("word2", "word2") == 0, "");
    cr_expect(strcmp("word2", "word") > 0, "");
    cr_expect(str_cmp("word", "word2") < 0, "");
    cr_expect(str_cmp("word2", "word2") == 0, "");
    cr_expect(str_cmp("word2", "word") > 0, "");
}

// Test(try_parse_uint, test1) {
//     size_t n = 0;
//     cr_expect(!try_parse_uint(str{""}, &n, &rest), "parsing broken");
// }

Test(rpmvercmp, test1) {
    cr_expect(rpmvercmp(str{""}, {""}) == 0, "1");
    cr_expect(rpmvercmp(str{"r"}, {""}) > 0, "2");
    cr_expect(rpmvercmp(str{""}, {"r"}) < 0, "3");
    cr_expect(rpmvercmp(str{"r"}, {"r"}) == 0, "4");
    cr_expect(rpmvercmp(str{"1.0010"}, {"1.9"}) > 0, "5");
    cr_expect(rpmvercmp(str{"1.05"}, {"1.5"}) == 0, "6");
    cr_expect(rpmvercmp(str{"1.0"}, {"1"}) > 0, "7");
    cr_expect(rpmvercmp(str{"2.50"}, {"2.5"}) > 0, "8");
    cr_expect(rpmvercmp(str{"fc4"}, {"fc.4"}) == 0, "9");
    cr_expect(rpmvercmp(str{"FC5"}, {"fc4"}) < 0, "10");
    cr_expect(rpmvercmp(str{"2a"}, {"2.0"}) < 0, "11");
    cr_expect(rpmvercmp(str{"1.0"}, {"1.fc4"}) > 0, "12");
    cr_expect(rpmvercmp(str{"3.0.0_fc"}, {"3.0.0.fc"}) == 0, "13");
}

