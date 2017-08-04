#ifndef CPP_HT_SESSION_HPP
#define CPP_HT_SESSION_HPP

#include <type_traits>

#include "boost/asio.hpp"
#include "boost/optional.hpp"

#include "hypertext/types.hpp"
#include "hypertext/parameters.hpp"

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
  template <typename... Args>
  types::response request(Args&&... args);

  /*
   */
  template <typename... Args>
  types::response get(Args&&... args);

  /*
   */
  template <typename... Args>
  types::response post(Args&&... args);

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
    return headers_;
  }

private: // Private data structures

  struct request_parameters
  {
    template <typename... Args>
    request_parameters(Args&&... args)
    {
      set_param(std::forward<Args>(args)...);
    }

    boost::optional<beast::http::verb> method;
    boost::optional<beast::string_view> url;
    boost::optional<types::request_header> req_headers;
    boost::optional<std::chrono::milliseconds> timeout;
    boost::optional<bool> stream;

  private:
    template <typename... Args>
    void set_param(parameters::method_param&& m, Args&&... args)
    {
      method = m.get();
      set_param(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void set_param(parameters::url_param&& u, Args&&... args)
    {
      url = u.get();
      set_param(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void set_param(parameters::headers_param&& h, Args&&... args)
    {
      req_headers = std::move(h.get());
      set_param(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void set_param(parameters::timeout_param&& t, Args&&... args)
    {
      timeout = t.get();
      set_param(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void set_param(parameters::stream_param&& s, Args&&... args)
    {
      stream = s.get();
      set_param(std::forward<Args>(args)...);
    }

    void set_param()
    {
      return;
    }
  };

private: // Private implementations
  /*
   */
  void fill_default_headers();

  /*
   */
  types::request prepare_request(const request_parameters&);

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
  types::request_header headers_;
};

} // END namespace hypertext

#include "hypertext/impl/session.ipp"

#endif