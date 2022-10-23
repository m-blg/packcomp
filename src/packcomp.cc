#include "curl/curl.h"
#include "cp_lib/string.cc"


using namespace cp;

#define ALL_PKGSET_ARCHS_URL_FMT "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=%s"
#define BRANCH_BINARY_PACKAGES_URL_FMT "https://rdb.altlinux.org/api/export/branch_binary_packages/%s?arch=%s"


size_t
curl_wf_callback(char* data, size_t size, size_t nmemb, void *userp) {
    size_t data_size = size * nmemb;

    dstrb *buffer = (dstrb*) userp;
    push_str(buffer, {data, nmemb});

    return data_size;
}

bool
fetch_binary_package_lists_raw(const char* branch1, const char* branch2, 
    const char* arch, const char* url_fmt, dstrb *data1, dstrb *data2) 
{
    CURL *handles[2];
    CURLM *curlm;

    CURLMsg *msg;
    int msgs_left;

    handles[0] = curl_easy_init();
    handles[1] = curl_easy_init();

    dstrb url1, url2; init(&url1); init(&url2);
    sprint_fmt(&url1, url_fmt, branch1, arch);
    sprint_fmt(&url2, url_fmt, branch2, arch);
    push(&url1, '\0'); 
    push(&url2, '\0');

    curl_easy_setopt(handles[0], CURLOPT_URL, url1.buffer);
    curl_easy_setopt(handles[0], CURLOPT_WRITEFUNCTION, curl_wf_callback);
    curl_easy_setopt(handles[0], CURLOPT_WRITEDATA, (void*)data1);

    curl_easy_setopt(handles[1], CURLOPT_URL, url2.buffer);
    curl_easy_setopt(handles[1], CURLOPT_WRITEFUNCTION, curl_wf_callback);
    curl_easy_setopt(handles[1], CURLOPT_WRITEDATA, (void*)data2);

    curlm = curl_multi_init();

    curl_multi_add_handle(curlm, handles[0]);
    curl_multi_add_handle(curlm, handles[1]);

    int hcount = 1; 
    while (hcount) {
        CURLMcode result = curl_multi_perform(curlm, &hcount);
    
        if (hcount)
            result = curl_multi_poll(curlm, NULL, 0, 1000, NULL);

        if (result != CURLM_OK) break;
    }

    while ((msg = curl_multi_info_read(curlm, &msgs_left))) {
        if(msg->msg == CURLMSG_DONE) {
            if (msg->easy_handle == handles[0]) {
                printf("branch: %s completed with status %d\n", branch1, msg->data.result);
            }
            if (msg->easy_handle == handles[1]) {
                printf("branch: %s completed with status %d\n", branch2, msg->data.result);
            }
        }
    }
    
    curl_multi_remove_handle(curlm, handles[0]);
    curl_multi_remove_handle(curlm, handles[1]);
    curl_easy_cleanup(handles[0]);
    curl_easy_cleanup(handles[1]);
    
    curl_multi_cleanup(curlm);
    
    shut(&url1);
    shut(&url2);
}






