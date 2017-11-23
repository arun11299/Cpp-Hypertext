#include <iostream>
#include <map>
#include "hypertext/http_free_funcs.hpp"
#include "hypertext/parameters.hpp"
#include "hypertext/basic_auth.hpp"

namespace ht = hypertext;
using namespace ht::parameters;

void test_simple_get()
{
  auto res = ht::get(url("https://api.github.com/events"),
                     verify(false));

  for (auto seq : res.resp.content_iter()) {
    std::cout << boost::asio::buffer_cast<const char*>(seq);
  }
  std::cout << '\n';
  return;
}

void test_query_params()
{
  auto res = ht::get(url("http://httpbin.org/get"),
                     params({ {"key1", "value1"},
                              {"key2", "value2"}
                            }));

  std::cout << res.status_code << std::endl;

  std::unordered_map<std::string, std::string> m;
  m["key1"] = "value1";
  m["key2"] = "value2";

  auto res2 = ht::get(url("http://httpbin.org/get"),
                params(std::move(m)));

  std::cout << res2.status_code << std::endl;
}

void test_rift()
{
  auto res = ht::get(
      url("https://10.110.4.4:8008/api/operational/project/default/vnfr-catalog/vnfr/f416166f-63ac-4f1a-bb70-3ad9f7163164"),
      verify(false),
      auth(ht::auth::HTTPBasicAuth{"admin", "admin"}));

  std::cout << res.status_code << std::endl;
}

int main() {
  //test_simple_get();
  //test_query_params();
  test_rift();
  return 0;
}
