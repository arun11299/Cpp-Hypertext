#ifndef CPP_HT_URL_HPP
#define CPP_HT_URL_HPP

#include "boost/beast/core/string.hpp"
#include "hypertext/utils/url_parser/parser.hpp"

namespace urlp = hypertext::util::url_parser;
namespace x3 = boost::spirit::x3;
namespace beast = boost::beast;

namespace hypertext {
namespace url {
/*
 */
class url_view
{
public: // 'tors
  url_view() = default;
  url_view(beast::string_view url);
  url_view(const url_view&) = default;
  url_view& operator=(const url_view&) = default;
  ~url_view() = default;

public: //Exposed APIs
  /*
   */
  bool failed() const noexcept
  {
    return !parse_success_;
  }

  /*
   */
  bool success() const noexcept
  {
    return parse_success_;
  }

  /*
   */
  urlp::Scheme scheme() const noexcept
  {
    return ast_url_.scheme_;
  }

  /*
   */
  std::string scheme_str() const
  { 
    return urlp::scheme_str(ast_url_.scheme_);
  }

  /*
   */
  std::string host() const noexcept
  {
    return ast_url_.hostname_.host_;
  }

  /*
   */
  unsigned short port() const noexcept
  {
    if (ast_url_.hostname_.port_) {
      return ast_url_.hostname_.port_.get();
    } else {
      switch(scheme()) {
      case urlp::Scheme::HTTP:
        return 80;
      case urlp::Scheme::HTTPS:
        return 443;
      default:
        BOOST_ASSERT_MSG(0, "Invalid Scheme");
      };
    }
    BOOST_ASSERT_MSG(0, "CODE NOT REACHED");
  }

  /*
   */
  beast::string_view target() const
  {
    return ast_url_.target_path_.length()
      ? ast_url_.target_path_
      : "/"
      ;
  }

  /*
   */
  std::string build_query_string(
      const std::map<std::string, std::string>& params) const;

  /*
   */
  beast::string_view resource_name() const;

private:
  /// The URL provided by the caller
  std::string url_;
  /// If URL parsing is succes,
  // it is set to true
  bool parse_success_;
  /// Parsed spirit AST
  urlp::ast::url ast_url_;
};

}
} // END namespace hypertext

#include "hypertext/impl/url.ipp"


#endif
