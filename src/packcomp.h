#pragma once

extern int g_packcomp_verbose;

bool
curl_two_get_requests(const char *url1, const char *url2, cp::dstrb *data1, cp::dstrb *data2);

bool
fetch_binary_package_lists_raw(const char *branch1, const char *branch2, 
    const char *arch, cp::dstrb *data1, cp::dstrb *data2);
bool
fetch_binary_package_lists_raw_multiarch(const char *branch1, const char *branch2, 
    cp::dbuff<const char*> archs, cp::dbuff<cp::dstrb[2]> *data_buffers);

cp::dbuff<json_object*>
package_compare(const char *branch1, const char *branch2, 
    cp::dbuff<const char*> archs, json_object*(*compare_lmd)(json_object*,json_object*,const char*));

cp::dbuff<json_object*>
package_compare_all_archs(const char *branch1, const char *branch2);

json_object*
compare_sorted(json_object *a1, json_object *a2, const char* arch);

bool
get_common_archs(const char *branch1, const char *branch2, cp::dbuff<const char*> *archs);