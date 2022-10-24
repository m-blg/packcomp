
// #include "stdio.h"
// #include "stdlib.h"
// #include "assert.h"
#include "cp_lib/io.cc"

#include "json-c/json.h"

const char* test_json = 
"["
"{\"name\": \"gambas-gb-markdown\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"},"
"{\"name\": \"gambas-gb-media\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"}"
",]";

bool
test1() {
    json_object *list = json_tokener_parse(test_json);
    if (!list) {
        fprintf(stderr, "parsing failed");
        return false;
    }
    size_t n = json_object_array_length(list);
    for (size_t i = 0; i < n; i++) {
        json_object *item = json_object_array_get_idx(list, i);
        json_object *name; 
        json_object_object_get_ex(item, "name", &name);
        printf("%s\n", json_object_get_string(name));
    }
    json_object_put(list);
    return true;
}


bool
test2() {
    // FILE *file = fopen("tests/test_json2.txt", "r");
    // size_t fsize = cp::file_size(file);
    // char* json_text = cp::m_alloc<char>(fsize+1);
    // fread(json_text, 1, fsize, file);
    // json_text[fsize] = '\0';
    // fclose(file);
    auto json_text = cp::read("tests/test_json2.txt");
    char* json_text_raw = cp::to_c_str(json_text);

    puts("start");
    json_object *list =  json_tokener_parse(json_text_raw);
    puts("end");
    if (!list) {
        fprintf(stderr, "parsing failed");
        free(json_text_raw);
        shut(&json_text);
        return false;
    }
    // size_t n = json_object_array_length(list);
    // for (size_t i = 0; i < n; i++) {
    //     json_object *item = json_object_array_get_idx(list, i);
    //     json_object *name; 
    //     json_object_object_get_ex(item, "name", &name);
    //     printf("%s\n", json_object_get_string(name));
    // }

    json_object_put(list);
    free(json_text_raw);
    shut(&json_text);

    return true;
}

int
main() {
    assert(test2());
}