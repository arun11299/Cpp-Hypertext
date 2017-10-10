#ifndef CPP_HT_BASIC_AUTH_IPP
#define CPP_HT_BASIC_AUTH_IPP

#include "hypertext/basic_auth.hpp"
#include "hypertext/utils/ct_string.hpp"

#include "beast/core/detail/base64.hpp"

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
  enc_str.resize(len);

  //Encode
  size_t wr = beast::detail::base64::encode(
      &enc_str[0], &combine[0], combine.length());
  (void)wr;

  return "Basic " + enc_str;
}

} // END namespace auth
} // END namespace hypertext


#endif
