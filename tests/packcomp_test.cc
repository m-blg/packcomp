
#include "packcomp.cc"


void test1() {
    dstrb data1 = dstrb_from("a");
    dstrb data2 = dstrb_from("bc");
    fetch_binary_package_lists_raw("p9", "p10", "x86_64", BRANCH_BINARY_PACKAGES_URL_FMT, &data1, &data2);
}

bool
test2() {
    sbuff<const char*, 1> archs = pack("x86_64");
    auto results = package_compare("p9", "p10", to_dbuff(&archs), BRANCH_BINARY_PACKAGES_URL_FMT, compare_sorted);
    
    for (auto it = begin(results); it != end(results); it++) {
        json_object_to_file("tests/test_results.txt", *it);
    }
    return true;
}


const char* test_json1 = 
"["
"{\"name\": \"gambas-gb-markdown\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"},"
"{\"name\": \"gambas-gb-media\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"}"
",]";
const char* test_json2 = 
"["
"{\"name\": \"gambas-gb-markdown\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"},"
"{\"name\": \"gambas-gb-media\", \"epoch\": 0, \"version\": \"3.17.3\", \"release\": \"alt1\", \"arch\": \"aarch64\", \"disttag\": \"sisyphus+304238.300.3.1\", \"buildtime\": 1658747740, \"source\": \"gambas\"}"
",]";

void
test3() {
    auto j1 = json_tokener_parse(test_json1);
    auto j2 = json_tokener_parse(test_json2);
    auto j3 = compare_sorted(j1, j2);
    println(json_object_to_json_string(j3));
}


int main() {
    test2();
}