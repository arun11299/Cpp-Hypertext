#ifndef CPP_HT_DETAIL_TYPE_TRAITS
#define CPP_HT_DETAIL_TYPE_TRAITS

#include <string>
#include <type_traits>

namespace hypertext {
namespace detail {

template <typename... T>
struct make_void
{
  using type = void;
};

template <typename... T>
using void_t = typename make_void<T...>::type;


template <bool... B>
struct bool_pack {};

template <bool... B>
using all_true = std::is_same<bool_pack<true, B...>, bool_pack<B..., true>>;

//-------------------------------------------------

template <typename T, typename=void>
struct is_parameter: std::false_type
{
};

template <typename T>
struct is_parameter<T, void_t<
                          decltype(
                              std::declval<T&>().get(),
                              (void)0)
                       >
                   >: std::true_type
{
};

template <bool B, typename... T>
struct are_all_parameters;

template <typename... T>
struct are_all_parameters<true, T...>: std::true_type
{
};


template <typename T, typename=void>
struct is_header_compatible: std::false_type
{
};

template <typename T>
struct is_header_compatible<T, void_t<
                                typename std::enable_if<
                                  std::is_convertible<typename T::key_type, beast::string_view>::value,
                                  void
                                >::type,
                                decltype(
                                    //TODO: Improve the traits check
                                    std::declval<const T&>().operator[](std::declval<const typename T::key_type&>()),
                                    std::declval<const T&>().begin(),
                                    std::declval<const T&>().end(),
                                    (void)0)
                               >
                           >: std::true_type
{
};


template <typename T, typename=void>
struct is_authorization: std::false_type
{
};

//TODO: This is very poor
template <typename T>
struct is_authorization<T, void_t<
                            decltype(
                              std::declval<std::string>() = std::declval<T&>.operator()(),
                              (void)0)
                           >
                       >: std::true_type
{
};

} // END namespace detail
} // END namespace hypertext

#endif
