#ifndef CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP
#define CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP

#include <system_error>

#include "beast/http/read.hpp"
#include "beast/http/write.hpp"

#include <boost/asio/ssl.hpp>

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

namespace hypertext {
namespace adapter {

asio_transport::asio_transport()
  : sock_(ios_)
{
}

types::response asio_transport::send(
    const types::request& req,
    beast::string_view    host,
    uint16_t              port,
    bool                  stream)
{
  if (!is_connected()) {
    connect_to_peer(host, port);
    assert (is_connected());
  }

  beast::http::write(sock_, req);

  beast::flat_buffer buf;
  types::response resp;

  if (!stream) {
    beast::http::read(sock_, buf, resp);
  } else {
    //Set the chunk response handler
    resp.set_chunked_response();
  }

  return resp;
}

types::response asio_transport::send_secure(
    const types::request& req,
    beast::string_view    host,
    uint16_t              port,
    bool                  stream,
    boost::optional<
      boost::variant<std::string, bool>> verify)
{
  // Create the TCP connection
  if (!is_connected()) {
    connect_to_peer(host, port);
    assert (is_connected());
  }

  boost::optional<std::string> cert_path;
  boost::optional<bool> cert_ver;

  if (verify) {
    switch(verify.get().which()) {
    case 0:
      cert_path = boost::get<0>(verify.get());
      break;
    case 1:
      cert_ver = boost::get<1>(verify.get());
      break;
    }
  }

  // Create the required ssl context
  ssl::context ctx{ssl::context::sslv23_client};

  // Wrap the now-connected socket in an SSL stream
  ssl::stream<tcp::socket&> stream{sock_, ctx};

  if (cert_ver && *cert_ver) {
    stream.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
  } else if (cert_ver && !*cert_ver){
    stream.set_verify_mode(ssl::verify_none);
  }

  if (cert_path) {
  }

  // Perform SSL handshaking
  stream.handshake(ssl::stream_base::client);
}

template <typename DynamicBuffer>
void asio_transport::read_next_chunked_body(
    DynamicBuffer& buf,
    types::emptybody_parser& parser,
    beast::error_code& ec)
{
  //Read the header if the routine is being
  //called for the first time.
  if (buf.size() == 0) {
    beast::http::read_header(sock_, buf, parser);
  }

  //Read as much data we can from the stream
  beast::http::read(sock_, buf, parser, ec);
}

void asio_transport::connect_to_peer(
    beast::string_view host, uint16_t port)
{
  boost::asio::ip::tcp::resolver r{ios_};
  boost::asio::connect(
      sock_, 
      r.resolve(
        boost::asio::ip::tcp::resolver::query{host.to_string(), std::to_string(port)}
      ));

  is_connected_ = true;

  return;
}

void asio_transport::close()
{
}

} // END namespace adapter
} // END namespace hypertext

#endif
