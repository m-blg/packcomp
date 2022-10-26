#include "curl/curl.h"
#include "json-c/json.h"

#include "cp_lib/string.cc"
#include "cp_lib/io.cc"
#include "cp_lib/algorithm.cc"

#include "packcomp.h"
#include "rpmvercmp.cc"


using namespace cp;

int g_packcomp_verbose;

static size_t
curl_wf_callback(char *data, size_t size, size_t nmemb, void *userp) {
    size_t data_size = size * nmemb;

    dstrb *buffer = (dstrb*) userp;
    push_str(buffer, {data, nmemb});

    return data_size;
}

// bool
// curl_multi_get_request(dbuff<const char*> urls, dbuff<dstrb*> buffers);

bool
curl_two_get_requests(const char *url1, const char *url2, dstrb *data1, dstrb *data2) 
{
    CURL *handles[2];
    CURLM *curlm;

    CURLMsg *msg;
    int msgs_left;

    handles[0] = curl_easy_init();
    handles[1] = curl_easy_init();

    curl_easy_setopt(handles[0], CURLOPT_URL, url1);
    curl_easy_setopt(handles[0], CURLOPT_WRITEFUNCTION, curl_wf_callback);
    curl_easy_setopt(handles[0], CURLOPT_WRITEDATA, (void*)data1);

    curl_easy_setopt(handles[1], CURLOPT_URL, url2);
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
                printf("request 1 completed with status %d\n", msg->data.result);
            }
            if (msg->easy_handle == handles[1]) {
                printf("request 2 completed with status %d\n", msg->data.result);
            }
        }
    }
    
    curl_multi_remove_handle(curlm, handles[0]);
    curl_multi_remove_handle(curlm, handles[1]);
    curl_easy_cleanup(handles[0]);
    curl_easy_cleanup(handles[1]);
    
    curl_multi_cleanup(curlm);
    
    return true;
}

bool
fetch_binary_package_lists_raw(const char *branch1, const char *branch2, 
    const char *arch, dstrb *data1, dstrb *data2) 
{
    dstrb url1, url2; init(&url1); init(&url2);
    sprint_fmt(&url1, BRANCH_BINARY_PACKAGES_URL_FMT, branch1, arch);
    sprint_fmt(&url2, BRANCH_BINARY_PACKAGES_URL_FMT, branch2, arch);
    // push(&url1, '\0'); 
    // push(&url2, '\0');

    bool result = curl_two_get_requests(url1.buffer, url2.buffer, data1, data2);
    
    shut(&url1);
    shut(&url2);

    return result;
}


bool
fetch_binary_package_lists_raw_multiarch(const char *branch1, const char *branch2, 
    dbuff<const char*> archs, dbuff<dstrb[2]> *data_buffers) 
{
    assert(len(archs) == len(*data_buffers));

    if (g_packcomp_verbose) {
        println("Downloading branch binary package lists");
    }


    dbuff<CURL*[2]> handles; init(&handles, len(*data_buffers));
    CURLM *curlm;

    CURLMsg *msg;
    int msgs_left;

    dbuff<dstrb[2]> urls; init(&urls, len(*data_buffers));


    curlm = curl_multi_init();
    if (!curlm) {
        fprintf(stderr, "curl init failed\n");
        curl_easy_cleanup(curlm);
        shut(&handles);
        shut(&urls);
        return false;
    }


    for (int i = 0; i < len(*data_buffers); i++) 
    {
        auto arch = archs[i];

        handles[i][0] = curl_easy_init();
        handles[i][1] = curl_easy_init();

        init(&urls[i][0]); init(&urls[i][1]);
        sprint_fmt(&urls[i][0], BRANCH_BINARY_PACKAGES_URL_FMT, branch1, arch);
        sprint_fmt(&urls[i][1], BRANCH_BINARY_PACKAGES_URL_FMT, branch2, arch);
        // push(&urls[i][0], '\0'); 
        // push(&urls[i][1], '\0');

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

    if (g_packcomp_verbose) {
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


void
print_error_response(json_object *jo) {
    println(json_object_to_json_string(jo));
}

dbuff<json_object*>
package_compare(const char *branch1, const char *branch2, 
    dbuff<const char*> archs, json_object*(*compare_lmd)(json_object*,json_object*,const char*)) 
{
    dbuff<dstrb[2]> text_buffers; 
    init(&text_buffers, len(archs));
    for (auto it = begin(text_buffers); it != end(text_buffers); it++) {
        init(&(*it)[0]);
        init(&(*it)[1]);
    }

    if (!fetch_binary_package_lists_raw_multiarch(branch1, branch2, archs, &text_buffers))
        return {};

    dbuff<json_object*> results; 
    init(&results, len(archs));
    for (int i = 0; i < len(archs); i++) {

        json_object *list[2];
        for (int j = 0; j < 2; j++) {
            push(&text_buffers[i][j], '\0');
            auto jo = json_tokener_parse(text_buffers[i][j].buffer);

            json_object *length = json_object_object_get(jo, "length");
            if (length == null) { 
                print_error_response(jo);
                return {}; 
            }
            
            json_object_object_get_ex(jo, "packages", &list[j]);
            assert(json_object_get_int(length) == json_object_array_length(list[j]));
        }

        results[i] = compare_lmd(list[0], list[1], archs[i]);
    }

    return results;
}

json_object*
compare_sorted(json_object *a1, json_object *a2, const char* arch)  {
    size_t a1_len = json_object_array_length(a1);
    size_t a2_len = json_object_array_length(a2);

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
            if (rpmvercmp(str{item1_ver}, str{item2_ver}) > 0) {
                json_object_array_add(o3, item1);
            }
            i1 += 1;
            i2 += 1;
        }
    }

    auto o = json_object_new_object();

    json_object_object_add(o, "arch", json_object_new_string(arch));
    json_object_object_add(o, "length1", 
        json_object_new_uint64(json_object_array_length(o1)));
    json_object_object_add(o, "length2",
        json_object_new_uint64(json_object_array_length(o2)));
    json_object_object_add(o, "length3",
        json_object_new_uint64(json_object_array_length(o3)));
    json_object_object_add(o, "packages1", o1);
    json_object_object_add(o, "packages2", o2);
    json_object_object_add(o, "packages3", o3);

    return o;
}


bool
get_common_archs(const char *branch1, const char *branch2, dbuff<const char*> *archs) 
{
    if (g_packcomp_verbose) {
        println("Requesting arch lists");
    }

    dstrb url1, url2; init(&url1); init(&url2);
    sprint_fmt(&url1, ALL_PKGSET_ARCHS_WITH_SRC_COUNT, branch1);
    sprint_fmt(&url2, ALL_PKGSET_ARCHS_WITH_SRC_COUNT, branch2);
    // push(&url1, '\0'); 
    // push(&url2, '\0');

    dstrb data1, data2;
    init(&data1); init(&data2);
    
    bool result = curl_two_get_requests(url1.buffer, url2.buffer, &data1, &data2);

    if (!result) {
        shut(&data1); shut(&data2);
        shut(&url1); shut(&url2);
        return false;
    }


    auto json1 = json_tokener_parse(data1.buffer);
    auto l1 = json_object_get_int(json_object_object_get(json1, "length"));
    auto json1_archs = json_object_object_get(json1, "archs");

    dbuff<const char*> b1; init(&b1, l1);
    for (int i = 0; i < l1; i++) {
        b1[i] = json_object_get_string(json_object_object_get(json_object_array_get_idx(json1_archs, i), "arch"));
    }

    auto json2 = json_tokener_parse(data2.buffer);
    auto l2 = json_object_get_int(json_object_object_get(json2, "length"));
    auto json2_archs = json_object_object_get(json2, "archs");

    darr<const char*> b2; init(&b2, l2);
    for (int i = 0; i < l2; i++) {
        auto arch = json_object_get_string(json_object_object_get(json_object_array_get_idx(json2_archs, i), "arch"));
        if (find(begin(b1), end(b1), arch, 
            [](auto s1, auto s2) {return strcmp(s1, s2) == 0;}) != end(b1)) 
        {
            push(&b2, arch);
        }
    }
    shrink_to_fit(&b2);
    *archs = to_dbuff(b2);

    shut(&data1); shut(&data2);
    shut(&url1); shut(&url2);

    return true;

}


dbuff<json_object*>
package_compare_all_archs(const char *branch1, const char *branch2) {
    dbuff<const char*> archs;
    if (!get_common_archs(branch1, branch2, &archs))
        return {};
    
    auto res = package_compare(branch1, branch2, archs, compare_sorted);
    shut(&archs);
    return res;
}
