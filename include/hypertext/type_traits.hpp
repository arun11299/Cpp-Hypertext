#ifndef CPP_HT_TYPE_TRAITS_HPP
#define CPP_HT_TYPE_TRAITS_HPP

#include <type_traits>
#include "hypertext/detail/type_traits.hpp"

namespace hypertext {

/**
 */
template <typename T>
struct is_parameter: detail::is_parameter<T>
{
};

/**
 */
template <typename... T>
struct are_parameters: 
  detail::are_all_parameters<
          detail::all_true<is_parameter<T>::value...>{},
          T...>
{
};

/**
 */
template <typename T>
struct is_header_compatible: detail::is_header_compatible<T>
{
};

/**
 */
template <typename T>
struct is_authorization: detail::is_authorization<T>
{
};

/**
 */
template <typename T>
struct is_transport_adapter: detail::is_transport_adapter<T>
{
};

/**
 */
template <typename T>
struct is_named_arg: detail::is_named_arg<T>
{
};

/**
 */
template <typename T>
struct is_auth_concept: detail::is_auth_concept<T>
{
};

} // END namespace hypertext

#endif
