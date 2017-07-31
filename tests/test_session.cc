#include <iostream>
#include "hypertext/session.hpp"
#include "hypertext/asio_transport_adapter.hpp"
#include "hypertext/parameters.hpp"

namespace ht = hypertext;

void test_session_basic()
{
  ht::session<ht::adapter::asio_transport> sess;
  using namespace ht::parameters;
  auto res = sess.request(
        method(beast::http::verb::get),
        url("www.example.com")
      );
  std::cout << res << std::endl;
}


int main() {
  test_session_basic();
  return 0;
}
