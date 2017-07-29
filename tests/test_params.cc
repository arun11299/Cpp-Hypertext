#include <iostream>
#include <map>
#include "hypertext/parameters.hpp"

void headers_test()
{
  using namespace hypertext::parameters;
  auto h1 = headers({{"a", "b"}, {"c", "d"}, {"e", "f"}});
  for (auto& e : h1.get()) {
    std::cout << e.name_string() << '\n';
  }

  std::map<std::string, std::string> m;
  m["a"] = "b";
  m["c"] = "d";
  auto h2 = headers(std::move(m));

  for (auto& e : h2.get()) {
    std::cout << e.name_string() << '\n';
  }
}

int main() {
  headers_test();
  return 0;
}
