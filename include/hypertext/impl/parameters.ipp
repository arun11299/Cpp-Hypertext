#ifndef CPP_HT_IMPL_PARAMETERS_IPP
#define CPP_HT_IMPL_PARAMETERS_IPP

#include <utility>
#include "hypertext/type_traits.hpp"

namespace hypertext {
namespace parameters {

beast::http::verb method_param::get() const noexcept
{
  return method_;
}

headers_param::headers_param(request_header h)
  : headers_(std::move(h))
{
}

request_header headers_param::get()
{
  return std::move(headers_);
}

template <typename AuthConceptT>
auth_param<AuthConceptT>::auth_param(AuthConceptT&& ac)
  : auth_(std::forward<AuthConceptT>(ac))
{
}

template <typename AuthConceptT>
AuthConceptT auth_param<AuthConceptT>::get()
{
  return std::move(auth_);
}

std::chrono::milliseconds timeout_param::get() const noexcept
{
  return timeout_;
}


// Parameter creator function implementation
// --------------------------------------------------------------

method_param
method(beast::http::verb meth)
{
  return method_param{meth};
}

headers_param
headers(
    const std::initializer_list<std::pair<beast::string_view, beast::string_view>>& kv)
{
  request_header rheader;

  for (auto elem : kv) {
    rheader.set(elem.first, elem.second);
  }

  return headers_param{std::move(rheader)};
}

template <typename HeaderConceptT>
headers_param
headers(HeaderConceptT&& hc)
{
  static_assert(is_header_compatible<typename std::decay<HeaderConceptT>::type>{},
      "Type does not match the requirements for a header dictionary");

  hypertext::request_header rheader;

  for (auto elem : hc) {
    rheader.set(elem.first, elem.second);
  }

  return headers_param{std::move(rheader)};
}

template <typename AuthConceptT>
auth_param<typename std::decay<AuthConceptT>::type>
auth(AuthConceptT&& ac)
{
  static_assert(is_authorization<typename std::decay<AuthConceptT>::type>{},
      "Type does not match the requirements for authorization");

  return auth_param<typename std::decay<AuthConceptT>::type>{std::forward<AuthConceptT>(ac)};
}

timeout_param
timeout(std::chrono::milliseconds ms)
{
  return timeout_param{ms};
}

timeout_param
timeout(std::chrono::seconds s)
{
  return timeout_param{std::chrono::duration_cast<std::chrono::milliseconds>(s)};
}

} // END namespace parameters
} // END namespace hypertext

#endif
