
#include "rpmvercmp.cc"



int main() {
    // rpmvercmp(str{"1.0010"}, {"1.9"}) > 0;
    rpmvercmp(str{"1.05"}, {"1.5"}) == 0;
}