#ifndef CPP_HT_HTTP_FREE_FUNCS_HPP
#define CPP_HT_HTTP_FREE_FUNCS_HPP

#include "hypertext/types.hpp"
#include "hypertext/session.hpp"
#include "hypertext/type_traits.hpp"
#include "hypertext/asio_transport_adapter.hpp"

namespace hypertext {

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
typename TransportAdapter::result_type 
get(Args&&... args)
{
  session<TransportAdapter> sess;
  return sess.get(std::forward<Args>(args)...);
}

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
typename TransportAdapter::result_type
post(Args&&... args)
{
  session<TransportAdapter> sess;
  return sess.post(std::forward<Args>(args)...);
}

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
typename TransportAdapter::result_type
put();

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
typename TransportAdapter::result_type
delete_();

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
typename TransportAdapter::result_type 
head();

/*
 */
template <typename Stream, typename TransportAdapter>
Stream& operator<<(Stream& os, types::response<TransportAdapter>& resp);


/*
 * If file_name not set, the URL file name would be taken as the
 * file name.
 */
template <typename TransportAdapter = adapter::asio_transport>
typename TransportAdapter::result_type
download_file(beast::string_view url, 
              boost::optional<beast::string_view> file_name = boost::none);

}

#include "hypertext/impl/http_free_funcs.ipp"

#endif
