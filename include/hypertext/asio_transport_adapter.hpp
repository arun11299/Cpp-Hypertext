#ifndef CPP_HT_ASIO_TRANSPORT_ADAPTER_HPP
#define CPP_HT_ASIO_TRANSPORT_ADAPTER_HPP

#include <memory>

#include "boost/asio.hpp"
#include <boost/asio/ssl.hpp>
#include "boost/beast/core/string.hpp"
#include "hypertext/types.hpp"

using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;

namespace hypertext {
namespace adapter {

/*
 */
class asio_transport
{
public: // typedefs
  using result_type = types::result_type<asio_transport>;

public: // 'tors
  asio_transport();
  asio_transport(const asio_transport&) = delete;
  asio_transport& operator=(const asio_transport&) = delete;
  ~asio_transport();

public: // Exposed APIs
  /*
   */
  boost::asio::io_service& get_io_service() noexcept
  {
    return ios_;
  }

  /*
   */
  bool is_connected() const noexcept
  {
    return is_connected_;
  }

  /*
   */
  result_type send(const types::request& req,
                   beast::string_view    host, 
                   uint16_t              port,
                   bool                  stream);

  /*
   */
  result_type send_secure(const types::request& req,
                          beast::string_view    host,
                          uint16_t              port,
                          bool                  stream,
                          const boost::optional<
                                 boost::variant<std::string, bool>>& verify,
                          const boost::optional<std::string>& cert_file);

  /*
   */
  template <typename DynamicBuffer>
  void read_next_chunked_body(DynamicBuffer& buf, //parser buffer
                              types::emptybody_parser& p,
                              beast::error_code& ec);

  /*
   */
  void close();


private: // Private implementation details
  /*
   */
  template <typename StreamObject>
  types::response<asio_transport> 
  send_impl(StreamObject&         sobj,
            const types::request& req,
            bool                  stream);

  /*
   */
  void connect_to_peer(beast::string_view host,
                       uint16_t           port);

  /*
   */
  template <typename DynamicBuffer>
  void read_chunked_response(types::response<asio_transport>& response,
                             DynamicBuffer&   buf);

  /*
   */
  template <typename StreamObj, typename DynamicBuffer>
  void read_next_chunked_body_impl(StreamObj& sobj,
                                   DynamicBuffer& buf, //parser buffer
                                   types::emptybody_parser& p,
                                   beast::error_code& ec);

private:
  /// ASIO IO service event loop
  boost::asio::io_service ios_;

  /// The TCP socket
  boost::asio::ip::tcp::socket sock_;

  /// The SSL wrapper over TCP socket
  //FIXME unique_ptr here because I could not make in_place work
  //with optional
  std::unique_ptr<boost::asio::ssl::stream<tcp::socket&>> ssl_stream_;
  //boost::optional<boost::asio::ssl::stream<tcp::socket&>> ssl_stream_;

  /// Check if connection is already established
  bool is_connected_ = false;
};

} // END namespace adapter
} // END namespace hypertext

#include "hypertext/impl/asio_transport_adapter.ipp"

#endif
