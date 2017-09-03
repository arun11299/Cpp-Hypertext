#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include "parser.hpp"

namespace urlp = hypertext::util::url_parser;
namespace x3 = boost::spirit::x3;


int main() {
  using x3::space;

  std::string input{"http"};
  urlp::Scheme sch;

  bool res = phrase_parse(input.begin(),
                          input.end(),
                          urlp::grammar::scheme,
                          space,
                          sch);

  switch (sch) {
  case urlp::Scheme::HTTP:
    std::cout << "Parsed HTTP\n";
    break;
  case urlp::Scheme::HTTPS:
    std::cout << "Parsed HTTPS\n";
  default:
    std::cout << "No match\n";
  };

  std::vector<std::string> inputs = {
    "www.example.com",
    "10.96.4.5",
    "10.96.4.3:8008"
  };

  for (auto& inp: inputs) {
    urlp::ast::host_name hn;
    res = phrase_parse(inp.begin(),
                       inp.end(),
                       urlp::grammar::hostname,
                       space,
                       hn);
    std::cout << "Host name = " << hn.host_ << std::endl;
    if (hn.port_) std::cout << "Port = " << hn.port_ << std::endl;
  }

  input = "http://www.example.com:8888/a/b/c";
  urlp::ast::url u;
  res = phrase_parse(input.begin(),
                     input.end(),
                     urlp::grammar::url,
                     space,
                     u);

  std::cout << u.hostname_.host_ << ":" << u.hostname_.port_
    << std::endl;

  return 0;
}
