#include "cp_lib/algorithm.cc"
#include "cp_lib/string.cc"
#include "cp_lib/io.cc"
// #include "json/json.h"

extern template void cp::split<char, cp::Dynamic_Array<cp::Dynamic_Buffer<char> > >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, cp::Dynamic_Buffer<char>, char, bool);
extern template void cp::resize<char const*>(cp::Dynamic_Buffer<char const*>*, unsigned long);
extern template void cp::print<char const*, cp::Dynamic_Buffer<char const*>, char const*>(char const*, cp::Dynamic_Buffer<char const*>, char const*);
extern template void cp::print<char const*, char const*>(char const*, char const*);
extern template void cp::sprint_fmt<char const*>(cp::Dynamic_Array<char>*, char const*, char const*);
extern template void cp::sprint_fmt<char const*, char const*>(cp::Dynamic_Array<char>*, char const*, char const*, char const*);

extern template void cp::init<unsigned char>(cp::Dynamic_Buffer<unsigned char>*, unsigned long);
extern template void cp::shut<unsigned char>(cp::Dynamic_Buffer<unsigned char>*);
extern template void cp::print_fmt<char>(char*, unsigned long, char const*);
extern template void cp::init<char>(cp::Dynamic_Array<char>*, unsigned long);
extern template void cp::copy<cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char> >(cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char>);
extern template void cp::append<char>(cp::Dynamic_Array<char>*, cp::Dynamic_Array<char>);
extern template void cp::init<cp::Dynamic_Buffer<char> >(cp::Dynamic_Array<cp::Dynamic_Buffer<char> >*, unsigned long);
extern template void cp::init<char const*>(cp::Dynamic_Array<char const*>*, unsigned long);
extern template bool cp::operator!=<char const*>(cp::Buffer_Iterator<char const*> const&, cp::Buffer_Iterator<char const*> const&);
extern template void cp::push<char const*>(cp::Dynamic_Array<char const*>*, char const*);
extern template void cp::shrink_to_fit<char const*>(cp::Dynamic_Array<char const*>*);
extern template cp::Dynamic_Buffer<char const*> cp::to_dbuff<char const*>(cp::Dynamic_Array<char const*>);
extern template char const*& cp::operator*<char const*>(cp::Buffer_Iterator<char const*> const&);
extern template void cp::operator++<char const*>(cp::Buffer_Iterator<char const*>&, int);
extern template bool cp::operator!=<char const*>(cp::Buffer_Iterator<char const*> const&, cp::Buffer_Iterator<char const*> const&);
extern template bool cp::operator==<char>(cp::Dynamic_Buffer<char>, cp::Dynamic_Buffer<char>);
extern template void cp::operator++<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]>&, int);
extern template void cp::push_str<char>(cp::Dynamic_Array<char>*, cp::Dynamic_Buffer<char>);
extern template bool cp::operator!=<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&, cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&);
extern template void cp::reserve<char>(cp::Dynamic_Array<char>*, unsigned long);
extern template void cp::print<cp::Dynamic_Buffer<char const*>, char const*>(cp::Dynamic_Buffer<char const*>, char const*);
extern template void cp::init<char const*>(cp::Dynamic_Buffer<char const*>*, unsigned long);
extern template void cp::shut<void* [2]>(cp::Dynamic_Buffer<void* [2]>*);
extern template void cp::resize<char const*>(char const***, unsigned long*, unsigned long);
extern template void cp::push<char>(cp::Dynamic_Array<char>*, char);
extern template void cp::shut<char>(cp::Dynamic_Array<char>*);
extern template void cp::shut<char const*>(cp::Dynamic_Buffer<char const*>*);
extern template cp::Dynamic_Array<char> (&cp::operator*<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]> const&)) [2];
extern template bool cp::operator!=<char>(cp::Buffer_Iterator<char> const&, cp::Buffer_Iterator<char> const&);
extern template void cp::init<void* [2]>(cp::Dynamic_Buffer<void* [2]>*, unsigned long);
extern template void cp::resize<cp::Dynamic_Buffer<char> >(cp::Dynamic_Buffer<char>**, unsigned long*, unsigned long);
extern template void cp::operator++<char>(cp::Buffer_Iterator<char>&, int);
extern template void cp::init<cp::Dynamic_Array<char> [2]>(cp::Dynamic_Buffer<cp::Dynamic_Array<char> [2]>*, unsigned long);
extern template char& cp::operator*<char>(cp::Buffer_Iterator<char> const&);
extern template void cp::shut<cp::Dynamic_Array<char> [2]>(cp::Dynamic_Buffer<cp::Dynamic_Array<char> [2]>*);
extern template void cp::init<json_object*>(cp::Dynamic_Buffer<json_object*>*, unsigned long);


extern template void cp::next<char const*>(cp::Buffer_Iterator<char const*>*, unsigned long);
extern template void cp::next<cp::Dynamic_Array<char> [2]>(cp::Buffer_Iterator<cp::Dynamic_Array<char> [2]>*, unsigned long);
extern template void cp::resize<char>(cp::Dynamic_Buffer<char>*, unsigned long);
extern template void cp::next<char>(cp::Buffer_Iterator<char>*, unsigned long);
extern template void cp::print<char const*>(cp::Dynamic_Buffer<char const*>);
extern template void cp::resize<char>(char**, unsigned long*, unsigned long);
extern template void cp::copy<char>(char*, cp::Dynamic_Buffer<char>);