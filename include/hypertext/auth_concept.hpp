#ifndef CPP_HT_AUTH_CONCEPT_HPP
#define CPP_HT_AUTH_CONCEPT_HPP

#include <memory>
#include "hypertext/types.hpp"
#include "hypertext/type_traits.hpp"

namespace hypertext {
namespace auth {

/*
 */
class AuthConcept
{
public: // 'tors
  template <typename T,
            //I am not greedy, but this constructor sure is.
            typename = std::enable_if_t<is_auth_concept<std::decay_t<T>>::value>>
  AuthConcept(T&& impl)
    : data_(std::make_shared<Holder<std::decay_t<T>>>(std::forward<T>(impl)))
  {
  }

  AuthConcept(const AuthConcept& other) = default;
  AuthConcept(AuthConcept&& other) = default;
  AuthConcept& operator=(const AuthConcept& other) = default;
  AuthConcept& operator=(AuthConcept&& other) =default;
  ~AuthConcept() = default;

public:
  /*
   */
  std::string get_encoded_str(types::request& req)
  {
    return data_->encoded_str(req);
  }

private:
  /*
   */
  struct Concept {
    virtual std::string encoded_str(types::request& req) = 0;
    virtual ~Concept() = default;
  };

  /*
   */
  template <typename T>
  struct Holder final: Concept
  {
    using held_type = typename std::decay<T>::type;

    Holder(T&& impl): auth_impl_(std::forward<T>(impl))
    {}

    std::string encoded_str(types::request& req) override
    {
      return auth_impl_.encoded_str(req);
    }

    held_type auth_impl_;
  };

private: // Data Members
  // Offers kind of value semantics
  // and is cheap.
  std::shared_ptr<Concept> data_;
};

} // END namespace auth
} // END namespace hypertext

#endif
