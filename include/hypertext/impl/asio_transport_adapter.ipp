#ifndef CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP
#define CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP

#include <string>
#include <iostream>

#include "beast/core/flat_buffer.hpp"
#include "beast/http/read.hpp"
#include "beast/http/write.hpp"

namespace hypertext {
namespace adapter {

asio_transport::asio_transport()
  : sock_(ios_)
{
}

types::response asio_transport::send(
    const types::request& req,
    beast::string_view host,
    uint16_t port)
{
  if (!is_connected()) {
    connect_to_peer(host, port);
    assert (is_connected());
  }

  beast::http::write(sock_, req);

  beast::flat_buffer b;
  types::response resp;
  beast::http::read(sock_, b, resp);

  return resp;
}

void asio_transport::connect_to_peer(
    beast::string_view host, uint16_t port)
{
  boost::asio::ip::tcp::resolver r{ios_};
  boost::asio::connect(
      sock_, 
      r.resolve(
        boost::asio::ip::tcp::resolver::query{host.to_string(), std::to_string(port)}));

  is_connected_ = true;

  return;
}

void asio_transport::close()
{
}

} // END namespace adapter
} // END namespace hypertext

#endif
