#include "cp_lib/algorithm.cc"
#include "cp_lib/string.cc"
#include "cp_lib/io.cc"

template void cp::split<char, cp::Dynamic_Array<cp::Dynamic_Buffer<char> > >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, cp::Dynamic_Buffer<char>, char, bool);
template void cp::resize<char const*>(cp::Dynamic_Buffer<char const*>*, unsigned long);
template void cp::print<char const*, cp::Dynamic_Buffer<char const*>, char const*>(char const*, cp::Dynamic_Buffer<char const*>, char const*);
template void cp::print<char const*, char const*>(char const*, char const*);
template void cp::sprint_fmt<char const*>(cp::Dynamic_Array<char>*, char const*, char const*);
template void cp::sprint_fmt<char const*, char const*>(cp::Dynamic_Array<char>*, char const*, char const*, char const*);
template void cp::push<cp::Dynamic_Buffer<char> >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, cp::Dynamic_Buffer<char>);