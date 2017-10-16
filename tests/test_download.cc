#include <iostream>
#include <map>
#include "hypertext/http_free_funcs.hpp"
#include "hypertext/parameters.hpp"
#include "hypertext/basic_auth.hpp"

namespace ht = hypertext;
using namespace ht::parameters;

void test_download()
{
  auto res = ht::download_file(
        "https://static.pexels.com/photos/34950/pexels-photo.jpg");
  (void)res;
  return;
}

int main() {
  test_download();
  return 0;
}
