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

beast::string_view url_param::get() const noexcept
{
  return url_;
}

headers_param::headers_param(types::request_header h)
  : headers_(std::move(h))
{
}

types::request_header headers_param::get()
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

timeout_param::timeout_param(std::chrono::milliseconds s)
  : timeout_(s)
{
}

std::chrono::milliseconds timeout_param::get() const noexcept
{
  return timeout_;
}

stream_param::stream_param(bool b)
  : stream_(b)
{
}

bool stream_param::get() const noexcept
{
  return stream_;
}

verify_param::verify_param(const std::string& path)
  : verify_(path)
{
}

verify_param::verify_param(bool v)
  : verify_(v)
{
}

boost::variant<std::string, bool>
verify_param::get() const
{
  return verify_;
}

// Parameter creator function implementation
// --------------------------------------------------------------

method_param
method(beast::http::verb meth)
{
  return method_param{meth};
}

method_param
method(beast::string_view meth)
{
  return method(beast::http::string_to_verb(meth));
}

url_param
url(beast::string_view url)
{
  return url_param{url};
}

headers_param
headers(
    const std::initializer_list<std::pair<beast::string_view, beast::string_view>>& kv)
{
  types::request_header rheader;

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

  hypertext::types::request_header rheader;

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

stream_param
stream(bool b)
{
  return stream_param{b};
}

verify_param
verify(const std::string& path)
{
  return verify_param{path};
}

verify_param
verify(bool v)
{
  return verify_param{v};
}

namespace literals {

auto operator "" _verb(const char* s, size_t l)
{
  return method(beast::string_view{s, l});
}

auto operator "" _url(const char* s, size_t l)
{
  return url(beast::string_view{s, l});
}

}


} // END namespace parameters
} // END namespace hypertext

#endif
