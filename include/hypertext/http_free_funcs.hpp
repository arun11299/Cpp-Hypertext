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
types::result_type get(Args&&... args)
{
  session<TransportAdapter> sess;
  return sess.get(std::forward<Args>(args)...);
}

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
types::result_type post(Args&&... args)
{
  session<TransportAdapter> sess;
  return sess.post(std::forward<Args>(args)...);
}

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
types::result_type put();

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
types::result_type delete_();

/*
 */
template <typename TransportAdapter = adapter::asio_transport,
          typename... Args>
types::result_type head();

}

#endif
