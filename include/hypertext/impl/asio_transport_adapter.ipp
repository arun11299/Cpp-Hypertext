#ifndef CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP
#define CPP_HT_IMPL_ASIO_TRANSPORT_ADAPTER_IPP

#include <system_error>
#include <fstream>
#include <exception>

#include "beast/http/read.hpp"
#include "beast/http/write.hpp"

#include <boost/asio/ssl.hpp>
#include <boost/filesystem.hpp>

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace fs = boost::filesystem;

namespace hypertext {
namespace adapter {

asio_transport::asio_transport()
  : sock_(ios_)
{
}

types::result_type asio_transport::send(
    const types::request& req,
    beast::string_view    host,
    uint16_t              port,
    bool                  stream)
{
  if (!is_connected()) {
    connect_to_peer(host, port);
    assert (is_connected());
  }

  auto response = send_impl(sock_, req, stream);

  return {std::move(response), response.status_code()};
}

types::result_type asio_transport::send_secure(
    const types::request& req,
    beast::string_view    host,
    uint16_t              port,
    bool                  stream,
    const boost::optional<
      boost::variant<std::string, bool>>& verify,
    const boost::optional<std::string>& cert_file)
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
      cert_path = boost::get<std::string>(verify.get());
      break;
    case 1:
      cert_ver = boost::get<bool>(verify.get());
      break;
    }
  }

  // Create the required ssl context
  ssl::context ctx{ssl::context::sslv23_client};

  // Wrap the now-connected socket in an SSL ssl_stream
  ssl::stream<tcp::socket&> ssl_stream{sock_, ctx};

  if (cert_ver && *cert_ver) {
    ssl_stream.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
  } else if (cert_ver && !*cert_ver){
    ssl_stream.set_verify_mode(ssl::verify_none);
  }

  if (cert_path) {
    if (fs::is_regular_file(*cert_path)) {
      ctx.load_verify_file(*cert_path);
    } else {
      //FIXME
      //ctx.load_verify_path(*cert_path);
    }
    ssl_stream.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
  }

  if (cert_file) {
    boost::system::error_code ec;

    if (fs::is_regular_file(*cert_file)) {
      boost::system::error_code ec;
      ctx.use_certificate_file(*cert_file, boost::asio::ssl::context::pem, ec);
    } else {
      //TODO: FIXME
    }

    if (ec) throw "Exception";
  }

  // Perform SSL handshaking
  ssl_stream.handshake(ssl::stream_base::client);

  auto response = send_impl(ssl_stream, req, stream);

  return {std::move(response), response.status_code()};
}

template <typename StreamObject>
types::response asio_transport::send_impl(
    StreamObject& sobj,
    const types::request& req,
    bool stream)
{
  beast::http::write(sobj, req);

  beast::flat_buffer buf;
  types::response resp;
 
  if (!stream) {
    beast::http::read(sobj, buf, resp);
  } else {
    //Set the chunk response handler
    resp.set_chunked_response();
  }
 
  return resp;
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
