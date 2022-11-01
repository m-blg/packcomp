#pragma once

#include "json-c/json.h"
#ifndef PACKCOMP_SOURSE
#include "cp_lib/extern_cp_lib.h"
#else
#include "cp_lib/instance_cp_lib.cc"
#endif

extern int g_packcomp_verbose;

#define ALL_PKGSET_ARCHS_WITH_SRC_COUNT "https://rdb.altlinux.org/api/site/all_pkgset_archs_with_src_count?branch=%s"
#define BRANCH_BINARY_PACKAGES_URL_FMT "https://rdb.altlinux.org/api/export/branch_binary_packages/%s?arch=%s"


/**
 * Performs two get requests asynchronously
 *
 * @param url1 url containing get request
 * @param url2 url containing get request
 * @param data1 output string buffer for url1 *Should be preinitiallized*
 * @param data2 output string buffer for url2 *Should be preinitiallized*
 * 
 * @returns true on success
 */
bool
curl_two_get_requests(const char *url1, const char *url2, cp::dstrb *data1, cp::dstrb *data2);


/**
 * Fetches available architectures for two branches, returns common architectures
 *
 * @param branch1 name of the first branch
 * @param branch1 name of the second branch
 * @param archs output list of common architectures *Should NOT be preinitiallized*
 * 
 * @returns true on success
 */
bool
get_common_archs(const char *branch1, const char *branch2, cp::dbuff<const char*> *archs);

/**
 * Downloads binary package lists for two branches asynchronously
 * Uses BRANCH_BINARY_PACKAGES_URL_FMT to form get request links
 *
 * @param branch1 name of the first branch
 * @param branch1 name of the second branch
 * @param arch architecture package lists for which to be downloaded
 * @param data1 output string buffer for branch1 *Should be preinitiallized*
 * @param data2 output string buffer for branch2 *Should be preinitiallized*
 * 
 * @returns true on success
 */
bool
fetch_binary_package_lists_raw(const char *branch1, const char *branch2, 
    const char *arch, cp::dstrb *data1, cp::dstrb *data2);

/**
 * Downloads binary package lists for two branches for each specified architecture asynchronously 
 * Uses BRANCH_BINARY_PACKAGES_URL_FMT to form get request links
 * 
 * @param branch1 name of the first branch
 * @param branch1 name of the second branch
 * @param archs list of architectures package lists for which to be downloaded
 * @param data_buffers output string buffers, each item is a pair of output buffers for two branches *Should be preinitiallized*
 * 
 * @returns true on success
 */
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