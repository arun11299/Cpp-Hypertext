#ifndef CPP_HT_AUTH_HPP
#define CPP_HT_AUTH_HPP

#include <string>

#include "beast/core/detail/base64.hpp"
#include "hypertext/types.hpp"

namespace hypertext {
namespace auth {

/**
 */
class HTTPBasicAuth
{
public: // 'tors
  HTTPBasicAuth(beast::string_view username,
                beast::string_view password)
    : username_(username)
    , password_(password)
  {}
  HTTPBasicAuth(const HTTPBasicAuth&) = default;
  HTTPBasicAuth& operator=(const HTTPBasicAuth&) = default;
  ~HTTPBasicAuth() = default;

public: // Exposed APIs
  /*
   */
  std::string encoded_str(types::request& req);

private:
  /// The user name.
  beast::string_view username_;
  beast::string_view password_;

};

} // END namespace auth
} // END namespace hypertext

#include "hypertext/impl/auth.ipp"

#endif
