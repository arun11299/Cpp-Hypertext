#include <iostream>
#include <map>
#include "hypertext/http_free_funcs.hpp"
#include "hypertext/parameters.hpp"
#include "hypertext/basic_auth.hpp"

namespace ht = hypertext;
using namespace ht::parameters;

void test_simple_post()
{
  auto res = ht::post(url("http://httpbin.org/post"),
                      data({{"key1", "value1"}}));

  for (auto seq : res.resp.content_iter()) {
    std::cout << boost::asio::buffer_cast<const char*>(seq);
  }
  std::cout << '\n';
  return;
}

void test_json_post()
{
  std::string d = "{\"some\": \"data\"}";
  auto res = ht::post(url("https://api.github.com/some/endpoint"),
                      data(d));

  auto text = res.resp.content();

  std::cout << text << std::endl;
}

int main() {
  test_simple_post();
  test_json_post();
  return 0;
}
