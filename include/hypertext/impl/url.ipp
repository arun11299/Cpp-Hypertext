#ifndef CPP_HT_IMPL_URL_IPP
#define CPP_HT_IMPL_URL_IPP

#include "hypertext/url.hpp"
#include <iostream>

namespace x3 = boost::spirit::x3;
namespace urlp = hypertext::util::url_parser;
namespace beast = boost::beast;

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

std::string url_view::build_query_string(
     const std::map<std::string, std::string>& params) const
{
  if (!params.size()) return {};
  std::string query{'?'};

  for (const auto& p : params) {
    query += (p.first + '=' + p.second + '&');
  }
  query.pop_back();

  return query;
}

beast::string_view url_view::resource_name() const
{
  beast::string_view tgt = target();

  size_t pos = tgt.rfind('/');
  if (pos == std::string::npos) return {};

  return tgt.substr(pos + 1);
}


} // END namespace url
} // END namespace hypertext

#endif
