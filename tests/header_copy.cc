#include <iostream>
#include "beast/http.hpp"
using namespace boost;


int main() {
  beast::http::request_header<> h;
  h.method(beast::http::verb::get);
  std::cout << h.method_string() << std::endl;

  beast::http::request_header<> h2;
  h2 = h;
  std::cout << h2.method_string() << std::endl;
  return 0;
}
