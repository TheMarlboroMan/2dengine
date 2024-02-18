#pragma once

namespace d2d { namespace tools {

//convert pointers and references to references.
template<typename T> const T& to_ref(const T& _v) {return _v;}
template<typename T> const T& to_ref(T const * _v) {return *_v;}

}}
