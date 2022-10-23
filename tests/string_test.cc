#include "cp_lib/string.cc"
#include "cp_lib/io.cc"

using namespace cp;

int main() {
    dstrb b = dstrb_from("test ");
    push_str(&b, str{"string"});
    println(b);
}