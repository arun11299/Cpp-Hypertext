#include <type_traits>
#include <typeinfo>
#include <iostream>

template <typename... T>
struct make_void
{
  using type = void;
};

template <typename... T>
using void_t = typename make_void<T...>::type;


template <typename T, typename=void>
struct is_named_arg: std::false_type
{
};

template <typename T>
struct is_named_arg<T, void_t<
                        typename std::enable_if<
                            std::is_same<decltype(T::value()), char const*>::value,
                            void>::type
                       >
                   >: std::true_type
{
};

struct NamedArg {
    static constexpr const char* cvalue() { return "arun"; }
};


int main() {
    static_assert (is_named_arg<NamedArg>::value, "Nope!");
    return 0;
}
