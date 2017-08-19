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
        method("GET"),
        url("127.0.0.1:8080"),
        stream(true)
      );

  /*

  if (res.has_chunked_response()) {
    std::cout << "Response has chunked response\n";
    while (res.get_chunk_response_block().fill_in_next_chunk(sess.transport())) {
      std::cout << res.get_chunk_response_block().get_chunk_body() << std::endl;
    }
  }
  */

  auto it = res.chunk_iter(sess.transport());
  auto it_end = res.chunk_end(sess.transport());

  while (it != it_end) {
    std::cout << *it << std::endl;
    ++it;
  }
}


int main() {
  test_session_basic();
  return 0;
}
