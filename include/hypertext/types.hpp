#ifndef CPP_HT_TYPES_HPP
#define CPP_HT_TYPES_HPP

#include <chrono>

#include "beast/http/message.hpp"
#include "beast/http/dynamic_body.hpp"

namespace hypertext {
namespace types {

using request_header = beast::http::request_header<>;

/*
 */
class response: 
  public beast::http::response<beast::http::dynamic_body>
{
public:
  //TODO: 'tors

public: // Exposed APIs
  /*
   */
  std::chrono::milliseconds elapsed_time() const noexcept
  {
    return elapsed_time_;
  }

private:
  std::chrono::milliseconds elapsed_time_ = 0;
};

} // END namespace hypertext
} // END namespace hypertext

#endif
