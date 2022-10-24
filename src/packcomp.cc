#include "curl/curl.h"
#include "json-c/json.h"

#include "cp_lib/string.cc"
#include "cp_lib/io.cc"


using namespace cp;

#define ALL_PKGSET_ARCHS_URL_FMT "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=%s"
#define BRANCH_BINARY_PACKAGES_URL_FMT "https://rdb.altlinux.org/api/export/branch_binary_packages/%s?arch=%s"


static size_t
curl_wf_callback(char *data, size_t size, size_t nmemb, void *userp) {
    size_t data_size = size * nmemb;

    dstrb *buffer = (dstrb*) userp;
    push_str(buffer, {data, nmemb});

    return data_size;
}

bool
fetch_binary_package_lists_raw(const char *branch1, const char *branch2, 
    const char *arch, const char *url_fmt, dstrb *data1, dstrb *data2) 
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
    if (!curlm) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curlm);
        return false;
    }

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

    return true;
}


bool
fetch_binary_package_lists_raw_multiarch(const char *branch1, const char *branch2, 
    dbuff<const char*> archs, const char *url_fmt, dbuff<dstrb[2]> *data_buffers) 
{
    assert(len(archs) == len(*data_buffers));

    dbuff<CURL*[2]> handles; init(&handles, len(*data_buffers));
    CURLM *curlm;

    CURLMsg *msg;
    int msgs_left;

    dbuff<dstrb[2]> urls; init(&urls, len(*data_buffers));


    curlm = curl_multi_init();
    if (!curlm) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curlm);
        return false;
    }


    for (int i = 0; i < len(*data_buffers); i++) 
    {
        auto arch = archs[i];

        handles[i][0] = curl_easy_init();
        handles[i][1] = curl_easy_init();

        init(&urls[i][0]); init(&urls[i][1]);
        sprint_fmt(&urls[i][0], url_fmt, branch1, arch);
        sprint_fmt(&urls[i][1], url_fmt, branch2, arch);
        push(&urls[i][0], '\0'); 
        push(&urls[i][1], '\0');

        curl_easy_setopt(handles[i][0], CURLOPT_URL, urls[i][0].buffer);
        curl_easy_setopt(handles[i][0], CURLOPT_WRITEFUNCTION, curl_wf_callback);
        curl_easy_setopt(handles[i][0], CURLOPT_WRITEDATA, (void*)&(*data_buffers)[i][0]);

        curl_easy_setopt(handles[i][1], CURLOPT_URL, urls[i][1].buffer);
        curl_easy_setopt(handles[i][1], CURLOPT_WRITEFUNCTION, curl_wf_callback);
        curl_easy_setopt(handles[i][1], CURLOPT_WRITEDATA, (void*)&(*data_buffers)[i][1]);

        curl_multi_add_handle(curlm, handles[i][0]);
        curl_multi_add_handle(curlm, handles[i][1]);
    }

    int hcount = 1; 
    while (hcount) {
        CURLMcode result = curl_multi_perform(curlm, &hcount);
    
        if (hcount)
            result = curl_multi_poll(curlm, NULL, 0, 1000, NULL);

        if (result != CURLM_OK) break;
    }

    while ((msg = curl_multi_info_read(curlm, &msgs_left))) {
        if(msg->msg == CURLMSG_DONE) {
            for (int i = 0; i < len(archs); i++) {
                if (msg->easy_handle == handles[i][0]) {
                    printf("branch: %s, arhc: %s completed with status %d\n", branch1, archs[i], msg->data.result);
                }
                if (msg->easy_handle == handles[i][1]) {
                    printf("branch: %s, arhc: %s completed with status %d\n", branch2, archs[i], msg->data.result);
                }
            }
        }
    }
    
    for (int i = 0; i < len(*data_buffers); i++) 
    {
        curl_multi_remove_handle(curlm, handles[i][0]);
        curl_multi_remove_handle(curlm, handles[i][1]);
        curl_easy_cleanup(handles[i][0]);
        curl_easy_cleanup(handles[i][1]);

        shut(&urls[i][0]);
        shut(&urls[i][1]);
    }
    curl_multi_cleanup(curlm);

    return true;
}

dbuff<json_object*>
package_compare(const char *branch1, const char *branch2, 
    dbuff<const char*> archs, const char *url_fmt, json_object*(*compare_lmd)(json_object*,json_object*)) 
{
    dbuff<dstrb[2]> text_buffers; 
    init(&text_buffers, len(archs));
    for (auto it = begin(text_buffers); it != end(text_buffers); it++) {
        init(&(*it)[0]);
        init(&(*it)[1]);
    }

    if (!fetch_binary_package_lists_raw_multiarch(branch1, branch2, archs, url_fmt, &text_buffers))
        return {};

    dbuff<json_object*> results; 
    init(&results, len(archs));
    for (int i = 0; i < len(archs); i++) {

        json_object *list[2];
        for (int j = 0; j < 2; j++) {
            push(&text_buffers[i][j], '\0');
            auto jo = json_tokener_parse(text_buffers[i][j].buffer);
            json_object *length = json_object_object_get(jo, "length");
            json_object_object_get_ex(jo, "packages", &list[j]);
            assert(json_object_get_int(length) == json_object_array_length(list[j]));
        }

        results[i] = compare_lmd(list[0], list[1]);
    }

    return results;
}

u32 parse_uint(str s) {
    u32 l = (u32)len(s);

    int n = 0;
    for (int i = 0; i < len(s); i++) {
        n = n*10 + s[i] - '0';
    }

    return n;
}

void parse_version(str version, int *major, int *minor, int *revision, int *build) {
    darr<str> tokens; init(&tokens);
    split(&tokens, version, '.');

    *major = (len(tokens) > 0) ? parse_uint(tokens[0]) : -1;
    *minor = (len(tokens) > 1) ? parse_uint(tokens[1]) : -1;
    *revision = (len(tokens) > 2) ? parse_uint(tokens[2]) : -1;
    *build = (len(tokens) > 3) ? parse_uint(tokens[3]) : -1;
}

int version_cmp(str ver1, str ver2) {
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

json_object*
compare_sorted(json_object *a1, json_object *a2)  {
    size_t a1_len = json_object_array_length(a1);
    size_t a2_len = json_object_array_length(a2);
    // if (a1 > a2) {
    //     swap(&a1, &a2);
    //     swap(&a1_len, &a2_len);
    // }

    auto o1 = json_object_new_array();
    auto o2 = json_object_new_array();
    auto o3 = json_object_new_array();

    size_t i1 = 0;
    size_t i2 = 0;
    while (i1 < a1_len && i2 < a2_len) {
        auto item1 = json_object_array_get_idx(a1, i1);
        auto item2 = json_object_array_get_idx(a2, i2);
        
        auto item1_name = json_object_get_string(json_object_object_get(item1, "name"));
        auto item2_name = json_object_get_string(json_object_object_get(item2, "name"));

        auto item1_ver = json_object_get_string(json_object_object_get(item1, "version"));
        auto item2_ver = json_object_get_string(json_object_object_get(item2, "version"));

        int cmp = strcmp(item1_name, item2_name);
        if (cmp < 0) {
            json_object_array_add(o1, item1);
            i1 += 1;
            continue;
        } else if (cmp > 0) {
            json_object_array_add(o2, item2);
            i2 += 1;
            continue;
        } else {
            if (version_cmp(str{item1_ver}, str{item2_ver}) > 0) {
                json_object_array_add(o3, item1);
            }
            i1 += 1;
            i2 += 1;
        }
    }

    auto o = json_object_new_object();
    json_object_object_add(o, "branch1 only packages length", 
        json_object_new_uint64(json_object_array_length(o1)));
    json_object_object_add(o, "branch2 only packages length",
        json_object_new_uint64(json_object_array_length(o2)));
    json_object_object_add(o, "ver1 > ver2 packages length",
        json_object_new_uint64(json_object_array_length(o3)));
    json_object_object_add(o, "branch1 only packages", o1);
    json_object_object_add(o, "branch2 only packages", o2);
    json_object_object_add(o, "ver1 > ver2 packages", o3);

    return o;
}


