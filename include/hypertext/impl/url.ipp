#ifndef CPP_HT_IMPL_URL_IPP
#define CPP_HT_IMPL_URL_IPP

#include "hypertext/url.hpp"

namespace x3 = boost::spirit::x3;
namespace urlp = hypertext::util::url_parser;

namespace hypertext {
namespace url {

url_view::url_view(beast::string_view url)
  : url_(url)
{
  using x3::space;

  //ATTN: Throw an exception if parsing fails
  //TODO: Store more information about the failure
  parse_success_ = phrase_parse(url_.begin(),
                          url_.end(),
                          urlp::grammar::url,
                          space,
                          ast_url_);
}


} // END namespace url
} // END namespace hypertext

#endif
