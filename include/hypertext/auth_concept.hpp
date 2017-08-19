#ifndef CPP_HT_AUTH_CONCEPT_HPP
#define CPP_HT_AUTH_CONCEPT_HPP

#include <memory>
#include "hypertext/types.hpp"

namespace hypertext {
namespace auth {

/*
 */
class AuthConcept
{
public: // 'tors
  template <typename T>
  AuthConcept(T&& impl)
    : data_(std::make_shared<T>(std::forward<T>(impl)))
  {
  }

private:
  /*
   */
  struct Concept {
    virtual std::string encoded_str(types::request& req);
    virtual ~Concept() = default;
  };

  template <typename T>
  struct Holder final: Concept
  {
    using held_type = typename std::decay<T>::type;

    Holder(T&& impl): auth_impl_(std::forward<T>(impl))
    {}

    std::string encoded_str(types::request& req) override;

    held_type auth_impl_;
  };

private: // Data Members
  // Offers kind of value semantics
  // and is cheap.
  std::shared_ptr<const Holder> data_;
};

} // END namespace auth
} // END namespace hypertext

#endif
