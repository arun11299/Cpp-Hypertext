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
types::response session<TransportAdapter>::request(Args&&... args)
{
  static_assert(are_parameters<Args...>{},
      "Not all arguments passed models Parameter concept");

  request_parameters rparams{std::forward<Args>(args)...};

  auto req = prepare_request(rparams);

  hypertext::url::url_view uview{rparams.url.get()};
  BOOST_ASSERT_MSG (uview.success(), "URL parsing failed");
  url_view_ = std::move(uview);

  if (url_view_.scheme() == urlp::Scheme::HTTPS) {
    return transport_.send_secure(req, url_view_.host(), url_view_.port(),
                                  (rparams.stream ? *rparams.stream : false),
                                  rparams.verify);
  }

  return transport_.send(req, url_view_.host(), url_view_.port(), 
                         (rparams.stream ? *rparams.stream : false));
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
prepare_request(const request_parameters& p)
{
  types::request request;
  assert (p.method);

  // Update the headers
  if (p.req_headers) {
    for (auto& f : *p.req_headers) {
      headers_.insert(f.name(), f.value());
    }
  }

  // TODO: Fill it up at a proper place
  //headers_.insert(beast::http::field::host, "www.example.com:80");

  // copy the headers
  static_cast<types::request_header&>(request) = headers_;

  request.method(*(p.method));
  request.target("/");
  request.version = 11;

  request.prepare_payload();

  return request;
}

} // END namespace hypertext

#endif
