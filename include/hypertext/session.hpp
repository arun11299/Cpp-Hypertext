#ifndef CPP_HT_SESSION_HPP
#define CPP_HT_SESSION_HPP

#include <type_traits>

#include "boost/asio.hpp"
#include "hypertext/types.hpp"

namespace hypertext {

/*
 * @class: Session
 * @brief: 
 * @detail:
 */
template <typename TransportAdapter>
class session
{
public:
  using transport_type = typename std::decay<TransportAdapter>::type;

public: // 'tors
  session();

  /// Copying of session objects disabled
  session(const session& other) = delete;
  session& operator=(const session& other) = delete;

  /// Session objects are move constructible
  session(session&& other) = default;
  session& operator=(session&& other) = default;

  ~session() = default;

public: // Exposed APIs
  /*
   */
  types::response send_request();

  /*
   */
  void close();

  /*
   */
  transport_type& transport() noexcept
  {
    return transport_;
  }

  /*
   */
  int64_t requests_sent() const noexcept
  {
    return requests_sent_;
  }

  /*
   */
  bool is_cookie_reuse_enabled() const noexcept
  {
    return use_saved_cookies_;
  }

  /*
   */
  types::request_header& headers() noexcept
  {
    return default_headers_;
  }

private:
  /// The underlying transport mechanism
  // Should model TransportAdapter concept
  transport_type transport_;

  /// Number of requests sent in this session
  int64_t requests_sent_ = 0;

  /// Should cookies be saved and reused on
  // subsequent requests ? Yes, by default
  bool use_saved_cookies_ = true;

  /// Default headers to be used for the session
  types::request_header default_headers_;
};

} // END namespace hypertext

#include "hypertext/impl/session.ipp"

#endif
