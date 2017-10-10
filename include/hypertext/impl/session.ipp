#ifndef CPP_HT_IMPL_SESSION_IPP
#define CPP_HT_IMPL_SESSION_IPP

#include "beast/http/field.hpp"
#include "beast/http/fields.hpp"
#include "hypertext/session.hpp"
#include "hypertext/version.hpp"
#include "hypertext/type_traits.hpp"

namespace urlp = hypertext::util::url_parser;

namespace hypertext {

template <typename TransportAdapter>
session<TransportAdapter>::session()
{
  static_assert(is_transport_adapter<TransportAdapter>{},
      "Session template type does not model TransportAdapter concept");

  fill_default_headers();
}

template <typename TransportAdapter>
template <typename... Args>
types::result_type session<TransportAdapter>::request(Args&&... args)
{
  static_assert(are_parameters<Args...>{},
      "Not all arguments passed models Parameter concept");

  request_parameters rparams{std::forward<Args>(args)...};

  hypertext::url::url_view uview{rparams.url.get()};
  BOOST_ASSERT_MSG (uview.success(), "URL parsing failed");
  url_view_ = std::move(uview);

  auto req = prepare_request(rparams);

  if (url_view_.scheme() == urlp::Scheme::HTTPS) {
    return transport_.send_secure(req, url_view_.host(), url_view_.port(),
                                  (rparams.stream ? *rparams.stream : false),
                                  rparams.verify,
                                  rparams.cert_file);
  }

  return transport_.send(req, url_view_.host(), url_view_.port(), 
                         (rparams.stream ? *rparams.stream : false));
}

template <typename TransportAdapter>
template <typename... Args>
types::result_type session<TransportAdapter>::get(Args&&... args)
{
  return request(parameters::method("GET"), std::forward<Args>(args)...);
}

template <typename TransportAdapter>
void session<TransportAdapter>::fill_default_headers()
{
  headers_.insert(beast::http::field::user_agent, "cpp-ht-requests");
  headers_.insert(beast::http::field::accept_encoding, "identity");
  headers_.insert(beast::http::field::accept, "*/*");
  headers_.insert(beast::http::field::connection, "keep-alive");
}

template <typename TransportAdapter>
types::request session<TransportAdapter>::
prepare_request(request_parameters& p)
{
  types::request request;
  assert (p.method);

  // Update the headers
  if (p.req_headers) {
    for (auto& f : *p.req_headers) {
      headers_.insert(f.name(), f.value());
    }
  }

  headers_.insert(beast::http::field::host, url_view_.host());

  //Add the authorization field
  if (p.auth) {
    headers_.insert(beast::http::field::authorization, 
                    p.auth.get().get_encoded_str(request));
  }

  //TODO: FIXME Optimize this
  std::string path = url_view_.target().data();
  if (p.params) {
    if (path.back() == '/') path.pop_back();
    path += url_view_.build_query_string(*p.params);
  }

  // copy the headers
  static_cast<types::request_header&>(request) = headers_;

  request.method(*(p.method));
  request.target(path);
  request.version = 11;

  request.prepare_payload();

  return request;
}

} // END namespace hypertext

#endif
