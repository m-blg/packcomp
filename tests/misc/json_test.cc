
#include "criterion/criterion.h"
#include "cp_lib/io.cc"

#include "json-c/json.h"

const char* test_json = 
"["
"{\"name\": \"gambas-gb-markdown\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"},"
"{\"name\": \"gambas-gb-media\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"}"
",]";

Test(json_tests, test1) {
    json_object *list = json_tokener_parse(test_json);
    cr_expect_not(!list, "parsing failed");

    size_t n = json_object_array_length(list);
    for (size_t i = 0; i < n; i++) {
        json_object *item = json_object_array_get_idx(list, i);
        json_object *name; 
        json_object_object_get_ex(item, "name", &name);
        printf("%s\n", json_object_get_string(name));
    }
    json_object_put(list);
}


Test(json_tests, test2) {
    auto json_text = cp::read("tests/test_json2.txt");
    char* json_text_raw = cp::to_c_str(json_text);

    puts("start");
    json_object *list =  json_tokener_parse(json_text_raw);
    puts("end");
    if (!list) {
        free(json_text_raw);
        shut(&json_text);
        cr_expect(false, "parsing failed");
    }

    json_object_put(list);
    free(json_text_raw);
    shut(&json_text);
}