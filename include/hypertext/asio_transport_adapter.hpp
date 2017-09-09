#ifndef CPP_HT_ASIO_TRANSPORT_ADAPTER_HPP
#define CPP_HT_ASIO_TRANSPORT_ADAPTER_HPP

#include "boost/asio.hpp"
#include "beast/core/string.hpp"
#include "hypertext/types.hpp"

namespace hypertext {
namespace adapter {

/*
 */
class asio_transport
{
public: // 'tors
  asio_transport();
  asio_transport(const asio_transport&) = delete;
  asio_transport& operator=(const asio_transport&) = delete;
  ~asio_transport() = default;

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
  types::response send(const types::request& req,
                       beast::string_view    host, 
                       uint16_t              port,
                       bool                  stream);

  /*
   */
  types::response send_secure(const types::request& req,
                              beast::string_view    host,
                              uint16_t              port,
                              bool                  stream,
                              boost::optional<
                                boost::variant<std::string, bool>> verify);

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
  void connect_to_peer(beast::string_view host,
                       uint16_t           port);

  /*
   */
  template <typename DynamicBuffer>
  void read_chunked_response(types::response& response,
                             DynamicBuffer&   buf);

private:
  /// ASIO IO service event loop
  boost::asio::io_service ios_;

  /// The TCP socket
  boost::asio::ip::tcp::socket sock_;

  /// Check if connection is already established
  bool is_connected_ = false;
};

} // END namespace adapter
} // END namespace hypertext

#include "hypertext/impl/asio_transport_adapter.ipp"

#endif
