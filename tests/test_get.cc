#include <iostream>
#include "hypertext/http_free_funcs.hpp"
#include "hypertext/parameters.hpp"
#include "hypertext/basic_auth.hpp"

namespace ht = hypertext;
using namespace ht::parameters;

void test_simple_get()
{
  auto res = ht::get(url("https://10.110.4.4:8008/ha/geographic/config-state"),
                     auth(ht::auth::HTTPBasicAuth("admin", "admin")),
                     verify(false));

  std::cout << "status_code = " << res.status_code << '\n';
  std::cout << "Response = " << res.response << '\n';
  return;
}

int main() {
  test_simple_get();
  return 0;
}
