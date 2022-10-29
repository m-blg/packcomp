#include "cp_lib/algorithm.h"
#include "cp_lib/string.cc"
#include "cp_lib/io.cc"
// #include "json-c/json.h"

template void cp::split<char, cp::Dynamic_Array<cp::Dynamic_Buffer<char> > >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, cp::Dynamic_Buffer<char>, char, bool);
template void cp::resize<char const*>(cp::Dynamic_Buffer<char const*>*, unsigned long);
template void cp::print<char const*, cp::Dynamic_Buffer<char const*>, char const*>(char const*, cp::Dynamic_Buffer<char const*>, char const*);
template void cp::print<char const*, char const*>(char const*, char const*);
template void cp::sprint_fmt<char const*>(cp::Dynamic_Array<char>*, char const*, char const*);
template void cp::sprint_fmt<char const*, char const*>(cp::Dynamic_Array<char>*, char const*, char const*, char const*);
template void cp::push<cp::Dynamic_Buffer<char> >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, cp::Dynamic_Buffer<char>);


template void cp::print_fmt<char>(char*, unsigned long, char const*);
template void cp::copy<cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char> >(cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char>);
template void cp::append<char>(cp::Dynamic_Array<char>*, cp::Dynamic_Array<char>);
template bool cp::operator!=<char const*>(cp::Buffer_Iterator<char const*> const&, cp::Buffer_Iterator<char const*> const&);
template void cp::push<char const*>(cp::Dynamic_Array<char const*>*, char const*);
template void cp::shrink_to_fit<char const*>(cp::Dynamic_Array<char const*>*);
template cp::Dynamic_Buffer<char const*> cp::to_dbuff<char const*>(cp::Dynamic_Array<char const*>);
template char const*& cp::operator*<char const*>(cp::Buffer_Iterator<char const*> const&);
template void cp::operator++<char const*>(cp::Buffer_Iterator<char const*>&, int);
template void cp::operator++<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]>&, int);
template void cp::push_str<char>(cp::Dynamic_Array<char>*, cp::Dynamic_Buffer<char>);
template bool cp::operator!=<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&, cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&);
template void cp::reserve<char>(cp::Dynamic_Array<char>*, unsigned long);
template void cp::print<cp::Dynamic_Buffer<char const*>, char const*>(cp::Dynamic_Buffer<char const*>, char const*);
template void cp::resize<char const*>(char const***, unsigned long*, unsigned long);
template void cp::push<char>(cp::Dynamic_Array<char>*, char);
template cp::Dynamic_Array<char> (&cp::operator*<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&)) [2];
template bool cp::operator!=<char>(cp::Buffer_Iterator<char> const&, cp::Buffer_Iterator<char> const&);
template void cp::resize<cp::Dynamic_Buffer<char> >(cp::Dynamic_Buffer<char>**, unsigned long*, unsigned long);
template void cp::operator++<char>(cp::Buffer_Iterator<char>&, int);
template char& cp::operator*<char>(cp::Buffer_Iterator<char> const&);

template void cp::next<char const*>(cp::Buffer_Iterator<char const*>*, unsigned long);
template bool cp::operator==<char>(cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char>);
template void cp::next<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]>*, unsigned long);
template void cp::resize<char>(cp::Dynamic_Buffer<char>*, unsigned long);
template void cp::next<char>(cp::Buffer_Iterator<char>*, unsigned long);
template void cp::print<char const*>(cp::Dynamic_Buffer<char const*>);
template void cp::resize<char>(char**, unsigned long*, unsigned long);
template void cp::copy<char>(char*, cp::Dynamic_Buffer<char>);

template void cp::init<unsigned char>(cp::Dynamic_Buffer<unsigned char>*, unsigned long);
template void cp::init<char>(cp::Dynamic_Array<char>*, unsigned long);
template void cp::init<cp::Dynamic_Buffer<char> >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, unsigned long);
template void cp::init<char const*>(cp::Dynamic_Array<char const*>*, unsigned long);

template void cp::shut<cp::Dynamic_Array<char> [2]>(cp::Dynamic_Buffer<cp::Dynamic_Array<char> [2]>*);
template void cp::shut<unsigned char>(cp::Dynamic_Buffer<unsigned char>*);
template void cp::shut<void* [2]>(cp::Dynamic_Buffer<void* [2]>*);
template void cp::shut<char>(cp::Dynamic_Array<char>*);
template void cp::shut<char const*>(cp::Dynamic_Buffer<char const*>*);

template void cp::init<char const*>(cp::Dynamic_Buffer<char const*>*, unsigned long);
template void cp::init<void* [2]>(cp::Dynamic_Buffer<void* [2]>*, unsigned long);
template void cp::init<cp::Dynamic_Array<char> [2]>(cp::Dynamic_Buffer<cp::Dynamic_Array<char> [2]>*, unsigned long);
template void cp::init<json_object*>(cp::Dynamic_Buffer<json_object*>*, unsigned long);



// cp::str cp::to_str(cp::Dynamic_Array<char>);
// cp::str cp::to_str(cp::Static_Array<char, 32ul>*, unsigned long);
// cp::str cp::to_str(cp::Static_Array<char, 32ul>&);
// int cp::str_cmp(cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char>);
// const char* cp::to_c_str(cp::Dynamic_Array<char>);
// const char* cp::to_c_str(cp::Dynamic_Buffer<char>);