#ifndef CPP_HT_BASIC_AUTH_HPP
#define CPP_HT_BASIC_AUTH_HPP

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
    : username_(username.data())
    , password_(password.data())
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
  std::string username_;
  std::string password_;

};

} // END namespace auth
} // END namespace hypertext

#include "hypertext/impl/basic_auth.ipp"

#endif
