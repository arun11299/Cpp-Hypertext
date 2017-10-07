#include <iostream>
#include "hypertext/session.hpp"
#include "hypertext/asio_transport_adapter.hpp"
#include "hypertext/parameters.hpp"

namespace ht = hypertext;

void test_ssl_basic()
{
  ht::session<ht::adapter::asio_transport> sess;
  using namespace ht::parameters;

  auto res = sess.request(
        method("GET"),
        url("https://www.example.com"),
        verify(true)
      );

  for (auto& kv : res.header()) {
    std::cout << kv.name_string() << std::endl;
  }
}


int main() {
  test_ssl_basic();
  return 0;
}
