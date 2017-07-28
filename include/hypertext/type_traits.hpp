#ifndef CPP_HT_TYPE_TRAITS_HPP
#define CPP_HT_TYPE_TRAITS_HPP

#include <type_traits>
#include "hypertext/detail/type_traits.hpp"

namespace hypertext {

template <typename T>
struct is_parameter: detail::is_parameter<T>
{
};

template <typename T>
struct is_header_compatible: detail::is_header_compatible<T>
{
};

template <typename T>
struct is_authorization: detail::is_authorization<T>
{
};

} // END namespace hypertext

#endif
