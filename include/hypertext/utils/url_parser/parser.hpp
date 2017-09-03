#include <string>
#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/io.hpp>

namespace x3 = boost::spirit::x3;


namespace hypertext { namespace util {
namespace url_parser {

/*
 * HTTP URL request scheme component.
 */
enum class Scheme
{
  HTTP,
  HTTPS,
};

static std::string scheme_str(Scheme sch)
{
  switch(sch) {
  case Scheme::HTTP: return "HTTP";
  case Scheme::HTTPS: return "HTTPS";
  default: return "UNKNOWN";
  };

  BOOST_ASSERT_MSG(0, "CODE NOT REACHED");
}

namespace ast {

/*
 */
struct host_name
{
  std::string host_;
  boost::optional<unsigned short> port_;
};

struct url
{
  Scheme scheme_;
  host_name hostname_;
};

} // END namespace ast


namespace grammar {
  //Get the X3 parsers
  using x3::lexeme;
  using x3::alpha;
  using x3::alnum;
  using x3::ascii::char_;
  using x3::ascii::string;
  using x3::lit;
  using x3::short_;

  /*
   * Parser creator helper
   */
  template <typename T>
  auto as = [](auto p) { return x3::rule<struct _, T>{} = as_parser(p); };

  /// String parser
  auto str_ = [](const char* lit) { return x3::string(lit); };

  /*
   * A URL for http/https consists of 3/4 components:
   * 1. Scheme
   * 2. Host
   * 3. Path
   * 4. Query String
   */
  
  //Scheme parser rule
  const x3::rule<class scheme_p, Scheme> scheme = "scheme-parser";

  //Hostname parser rule
  const x3::rule<class hostname_p, ast::host_name> hostname = "hostname-parser";

  //URL parser rule
  const x3::rule<class url_p, ast::url> url = "url-parser";

  auto on_scheme = [](auto& ctx) {
    std::string& scheme = x3::_attr(ctx);

    if (scheme == "http") {
      x3::_val(ctx) = Scheme::HTTP;
    } else if (scheme == "https") {
      x3::_val(ctx) = Scheme::HTTPS;
    }

  };

  ///Parses the scheme component of the URL
  const auto scheme_def = str_("https") [on_scheme]
                        | str_("http")  [on_scheme]
                        ;

  //Host parsing
  auto domain = as<std::string>( char_ >> *(alnum | char_('.')) );

  auto octet = as<std::string>( char_ >> -char_ >> -char_ );
  auto dot = as<char>(char_('.'));

  auto ipv4_address = as<std::string>(octet > dot > octet > dot > octet > dot > octet);

  //TODO: IPV6 address
  const auto hostname_def = (domain | ipv4_address) >> -( ':' > short_ );

  const auto url_def = scheme >> lit("://") >> hostname;

  BOOST_SPIRIT_DEFINE (url);
  BOOST_SPIRIT_DEFINE (scheme);
  BOOST_SPIRIT_DEFINE (hostname);
}

} // END namespace url_parser
} // END namespace util
} // END namespace hypertext

BOOST_FUSION_ADAPT_STRUCT(
  hypertext::util::url_parser::ast::host_name,
  (std::string, host_),
  (boost::optional<unsigned short>, port_)
);

BOOST_FUSION_ADAPT_STRUCT(
  hypertext::util::url_parser::ast::url,
  (hypertext::util::url_parser::Scheme, scheme_),
  (hypertext::util::url_parser::ast::host_name, hostname_)
);
