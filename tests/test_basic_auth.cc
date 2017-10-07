#include <iostream>
#include "hypertext/session.hpp"
#include "hypertext/asio_transport_adapter.hpp"
#include "hypertext/parameters.hpp"
#include "hypertext/basic_auth.hpp"

namespace ht = hypertext;

void test_basic_auth()
{
  ht::session<ht::adapter::asio_transport> sess;
  using namespace ht::parameters;

  auto res = sess.request(
        method("GET"),
        url("https://api.github.com/user"),
        auth(ht::auth::HTTPBasicAuth{"arun11299", "sharpbrain99211"}),
        verify(false)
      );

  std::cout << "Status : " << res.result() << std::endl;

  for (auto& kv : res.header()) {
    std::cout << kv.name_string() << std::endl;
  }
}


int main() {
  test_basic_auth();
  return 0;
}
