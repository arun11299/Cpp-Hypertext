#ifndef CPP_HT_UTILS_CT_STRING_HPP
#define CPP_HT_UTILS_CT_STRING_HPP

#include <type_traits>
#include "hypertext/type_traits.hpp"

/// ATTN: This is mostly incomplete.
// Have not been actually able to make use of it till now.
// TODO: Consider removing it.

namespace hypertext {
namespace util {

constexpr size_t ct_strlen(const char* str)
{
    size_t i = 0;
    while (*str++) i++;
    return i;
}

template <size_t N>
class ct_string
{
public:
  constexpr ct_string(const char* ptr)
  {
    for (size_t i = 0; i < N; i++) {
      buf_[i] = *ptr;
      ++ptr;
    }
    buf_[N] = '\0';
  }
  
  constexpr size_t size() const noexcept
  {
      return N;
  }
  
  constexpr size_t length() const noexcept
  {
     return size();
  }
  
  constexpr char at(int i) const noexcept
  {
    return buf_[i];
  }
  
  constexpr char front() const noexcept
  {
    static_assert (N != 0, "front called on empty string");
    return at(0);
  }
  
  constexpr char back() const noexcept
  {
    static_assert (N != 0, "back called on empty string");
    return at(N - 1);
  }
  
  friend
  constexpr bool operator==(const ct_string<N>& f, const ct_string<N>& s) noexcept
  {
    for (size_t i = 0; i < N; i++) {
      if (f.buf_[i] != s.buf_[i]) return false;
    }
    return true;
  }
  
  friend
  constexpr bool operator!=(const ct_string<N>& f, const ct_string<N>& s) noexcept
  {
    return !(f == s);
  }
  
private:
  //Need to initialize it..else error in 
  // constructor for using uninintialized
  // variable in constexpr context
  char buf_[N + 1]={0,};
};


template <typename NamedArg>
static constexpr auto make_ct_string()
{
  static_assert (is_named_arg<NamedArg>::value, 
      "make_ct_string template parameter must model NamedArg concept");
  ct_string<ct_strlen(NamedArg::value())> cstr{NamedArg::value()};
  return cstr;
}

} // END namespace util
} // END namespace hypertext

#endif
