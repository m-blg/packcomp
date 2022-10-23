
#include "packcomp.cc"






int main() {
    dstrb data1 = dstrb_from("a");
    dstrb data2 = dstrb_from("bc");
    fetch_binary_package_lists_raw("p9", "p10", "x86_64", BRANCH_BINARY_PACKAGES_URL_FMT, &data1, &data2);
}