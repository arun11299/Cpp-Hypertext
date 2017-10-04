#ifndef CPP_HT_AUTH_IPP
#define CPP_HT_AUTH_IPP

#include "hypertext/auth.hpp"

namespace hypertext {
namespace auth {

std::string 
HTTPBasicAuth::encoded_str(types::request& req)
{
  std::string enc_str;
  std::string combine;
  // Additional 1 for the colon(:)
  combine.reserve(username_.size() + password_.size() + 1);
  combine = username_ + ':' + password_;

  //get the number of bytes used after encoding
  size_t len = 
    beast::detail::base64::encoded_size(combine.length());
  enc_str.reserve(len);

  //Encode
  size_t wr = beast::detail::base64::encode(
      enc_str.data(), combine.data(), combine.length())

  return enc_str;
}

} // END namespace auth
} // END namespace hypertext


#endif
