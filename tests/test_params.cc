#include <iostream>
#include "hypertext/parameters.hpp"

void headers_test()
{
  /*
  using namespace hypertext::parameters;
  auto h1 = headers({{"a", "b"}, {"c", "d"}, {"e", "f"}});
  for (auto e : h1.headers_) {
    std::cout << e.name_string() << '\n';
  }
  */

  beast::http::request_header<> h;
  h.set("a", "b");
  auto h2 = std::move(h);

  for (auto e : h2) {
    std::cout << e.name_string() << '\n';
  }
}

int main() {
  headers_test();
  return 0;
}
