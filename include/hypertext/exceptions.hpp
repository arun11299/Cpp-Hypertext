#ifndef CPP_HT_EXCEPTIONS_HPP
#define CPP_HT_EXCEPTIONS_HPP

#include <string>
#include <utility>
#include <exception>
#include <type_traits>

namespace hypertext {

/*
 */
class UnexpectedURLFormat final: public std::runtime_error
{
public:
  UnexpectedURLFormat(const char* msg)
    : std::runtime_error(msg)
    , msg_(msg)
  {
  }

  template <typename T,
            typename = std::enable_if_t<std::is_convertible<std::decay_t<T>, std::string>{}>
            >
  UnexpectedURLFormat(T&& msg)
    : std::runtime_error(msg.c_str()) //FIXME:
    , msg_(std::forward<T>(msg)) 
  {
  }

  const char* what() const noexcept override
  {
    return msg_.c_str();
  }

private:
  std::string msg_;
};


/*
 */
class FileError final: public std::runtime_error
{
public:
  FileError(const char* msg)
    : std::runtime_error(msg)
    , msg_(msg)
  {
  }

  template <typename T,
            typename = std::enable_if_t<std::is_convertible<std::decay_t<T>, std::string>{}>
            >
  FileError(T&& msg)
    : std::runtime_error(msg.c_str())
    , msg_(std::forward<T>(msg))
  {
  }

  const char* what() const noexcept override
  {
    return msg_.c_str();
  }

private:
  std::string msg_;
};


} // END namespace hypertext

#endif 
