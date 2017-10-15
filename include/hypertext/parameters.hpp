#ifndef CPP_HT_PARAMTERS_HPP
#define CPP_HT_PARAMTERS_HPP

#include <map>
#include <chrono>

#include "boost/variant.hpp"

#include "beast/http/verb.hpp"
#include "beast/core/string.hpp"

#include "hypertext/types.hpp"
#include "hypertext/auth_concept.hpp"

namespace hypertext {
namespace parameters {

using key_value_t = std::map<std::string, std::string>;

// Fwd. decl all the available paremters

/// The HTTP method for the request.
struct method_param;

/// The URL to query
struct url_param;

/// Optional query string to send
struct params_param;

/// The dictionary/map of HTTP headers to be sent.
struct headers_param;

/// The Authorization field.
struct auth_param;

/// How many milli-seconds to wait for the server to send response.
struct timeout_param;

/// Option to enable streaming download
struct stream_param;

/// Either a bool, in which case it controls whether
/// we verify the server's TLS certificate or a string
/// in which case it must point to a CA bundle.
struct verify_param;

/// Option to provide the SSL client cert file (.pem)
struct cert_param;

/// HTTP query string parameters
struct params_param; 

/// HTTP POST data parameters
struct data_param;


/// Method Parameter Definitions
//------------------------------

struct method_param
{
  beast::http::verb get() const noexcept;

  beast::http::verb method_;
};

struct url_param
{
  beast::string_view get() const noexcept;
  std::string url_;
};

struct headers_param
{
  headers_param(types::request_header);
  types::request_header& get();

  types::request_header headers_;
};

struct params_param
{
  params_param(std::map<std::string, std::string>&&);

  std::map<std::string, std::string>& get();

  //TODO: FIXME: I do not like this. Should be more generic
  //FIXME: Almost all usage of std::string can somehow
  //be changed to string_view. Check that!
  std::map<std::string, std::string> params_;
};

struct data_param
{
  data_param(key_value_t&&);
  data_param(beast::string_view);

  boost::variant<key_value_t, beast::string_view>&
  get();

  boost::variant<key_value_t, beast::string_view> data_;
};

struct auth_param
{
  auth_param(auth::AuthConcept);
  auth::AuthConcept& get();

  auth::AuthConcept auth_;
};

struct timeout_param
{
  timeout_param(std::chrono::milliseconds);
  std::chrono::milliseconds get() const noexcept;
  std::chrono::milliseconds timeout_;
};

struct stream_param
{
  stream_param(bool);
  bool get() const noexcept;
  bool stream_ = false;
};

struct verify_param
{
  verify_param(const std::string&);
  verify_param(bool);

  boost::variant<std::string, bool>& get() noexcept;

  boost::variant<std::string, bool> verify_;
};

struct cert_param
{
  beast::string_view get() const noexcept;
  std::string cert_;
};

/// Parameter creators
//-------------------------------

/*
 */
method_param 
method(beast::http::verb);

/*
 */
method_param
method(beast::string_view);

/*
 */
url_param
url(beast::string_view url);

/*
 */
headers_param 
headers(
    const std::initializer_list<
                std::pair<beast::string_view, beast::string_view>
          >&);

/*
 */
template <typename HeaderConceptT>
headers_param
headers(HeaderConceptT&&);

/*
 */
template <typename AuthT>
auth_param 
auth(AuthT&& auth);

/*
 * Uses HTTPBasicAuth.
 * Only offers a syntactical advantage if the
 * user wants to use Basic Authentication.
 */
auth_param
auth(std::pair<std::string, std::string> creds);

/*
 */
timeout_param 
timeout(std::chrono::milliseconds);

/*
 */
timeout_param 
timeout(std::chrono::seconds);

/*
 */
stream_param
stream(bool);

/*
 */
verify_param
verify(const std::string&);

/*
 */
verify_param
verify(bool);

/*
 */
cert_param
cert(beast::string_view cert);

/*
 */
params_param
params(
    const std::initializer_list<
            std::pair<beast::string_view, beast::string_view>
          >&);

/*
 */
template <typename HeaderConceptT>
params_param
params(HeaderConceptT&&);

/*
 */
template <typename HeaderConceptT>
auto
data(HeaderConceptT&& hc) -> std::enable_if_t<
                               is_header_compatible<std::decay_t<HeaderConceptT>>{},
                               data_param
                               >;

/*
 */
data_param
data(
    const std::initializer_list<
            std::pair<beast::string_view, beast::string_view>
          >&);

/*
 */
data_param
data(beast::string_view);

} // END namespace parameters
} // END namespace hypertext

#include "hypertext/impl/parameters.ipp"

#endif
