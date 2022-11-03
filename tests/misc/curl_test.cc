#include <curl/curl.h>
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"


bool
test1() {
    CURL *curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curl);
        return false;
    }

    // curl_easy_setopt(curl, CURLOPT_URL, "https://rdb.altlinux.org/api/license");
    curl_easy_setopt(curl, CURLOPT_URL, 
        "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus");

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "download error: %s\n", curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
    return true;
}

size_t
curl_wf_callback(char* data, size_t size, size_t nmemb, void *userp) {
    size_t data_size = size * nmemb;

    size_t *count = (size_t*) userp;
    *count += data_size;

    return data_size;
}

bool
test2() {
    CURL *curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curl);
        return false;
    }

    // curl_easy_setopt(curl, CURLOPT_URL, "https://rdb.altlinux.org/api/license");
    curl_easy_setopt(curl, CURLOPT_URL, 
        "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_wf_callback);

    size_t data_size = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&data_size);

    bool res = true;
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "download error: %s\n", curl_easy_strerror(result));
        res = false;
    }

    if (res)
        printf("data size: %lu\n", data_size);

    curl_easy_cleanup(curl);
    return res;
}


size_t
curl_wf_callback_wfile(char* data, size_t size, size_t nmemb, void *userp) {
    size_t data_size = size * nmemb;

    FILE *f = *(FILE**) userp;
    fwrite(data, size, nmemb, f);

    return data_size;
}

bool
test3() {
    CURL *curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curl);
        return false;
    }

    // curl_easy_setopt(curl, CURLOPT_URL, "https://rdb.altlinux.org/api/license");
    curl_easy_setopt(curl, CURLOPT_URL, 
        "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus?arch=x86_64");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_wf_callback_wfile);

    FILE *file = fopen("tests/test_json2.txt", "w");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&file);

    bool res = true;
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "download error: %s\n", curl_easy_strerror(result));
        res = false;
    }

    fclose(file);
    curl_easy_cleanup(curl);
    return res;
}

int
main() {
    assert(test3());
}
