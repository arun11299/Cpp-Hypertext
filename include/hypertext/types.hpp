#ifndef CPP_HT_TYPES_HPP
#define CPP_HT_TYPES_HPP

#include <chrono>
#include <string>
#include <iterator>
#include <functional>

#include <boost/assert.hpp>

#include "beast/http/error.hpp"
#include "beast/http/parser.hpp"
#include "beast/http/message.hpp"
#include "beast/http/empty_body.hpp"
#include "beast/http/string_body.hpp"
#include "beast/http/dynamic_body.hpp"
#include "beast/core/flat_buffer.hpp"

namespace hypertext {
namespace types {

using request_header   = beast::http::request_header<>;
using response_header  = beast::http::response_header<>;
using emptybody_parser = beast::http::parser<false, beast::http::empty_body>;

struct in_place_construct_t {};

//Fwd Decl
class response;

/*
 */
class request:
  public beast::http::request<beast::http::string_body>
{
public: // 'tors
  request() = default;
  request(const request&) = default;
  request(request&&) = default;
  request& operator=(const request&) = default;
  request& operator=(request&&) = default;
  ~request() = default;

public: // Exposed APIs
  /*
   */
  const request_header& header() const noexcept
  {
    return (*this);
  }

  /*
   */
  request_header& header() noexcept
  {
    return (*this);
  }
  
};

/*
 *
 */
template <typename TransportAdapter, typename ChunkBodyStore>
class chunked_response
{
public: //'tors
  /*
   */
  chunked_response(response& resp, TransportAdapter& transport)
    : parent_resp_(resp)
    , transport_(transport)
  {
    crb_.emplace(parent_resp_);
  }

  chunked_response(const chunked_response&) = delete;
  chunked_response(chunked_response&&) = default;
  chunked_response& operator=(const chunked_response&) = delete;
  chunked_response& operator=(chunked_response&&) = default;
  ~chunked_response() = default;

public: // Internal classes
  /*
   */
  class chunk_response_block
  {
  public: // typedefs
    using storage_type = ChunkBodyStore;

  public: // 'tors
    chunk_response_block(std::reference_wrapper<response>);
    chunk_response_block(const chunk_response_block&) = delete;
    chunk_response_block(chunk_response_block&&) = default;
    chunk_response_block& operator=(const chunk_response_block&) = delete;
    chunk_response_block& operator=(chunk_response_block&&) = default;

  public: // Exposed APIs
    /*
     */
    emptybody_parser&
    parser() noexcept { return parser_; }

    /*
     */
    beast::flat_buffer&
    pbuf() noexcept { return pbuf_; }

    /*
     */
    beast::error_code
    get_error() const noexcept { return ec_; }

    /*
     */
    beast::string_view
    get_chunk_body() noexcept { return {chunk_body_}; }

    /*
     */
    ChunkBodyStore&
    get_storage() noexcept { return chunk_body_; }

    /*
     */
    bool fill_in_next_chunk(TransportAdapter& transport);

  private: // Data members (chunk_response_block)
    /// Storage for storing the chunk body
    storage_type chunk_body_;
    /// The dynamic buffer used for parsing
    beast::flat_buffer pbuf_;
    /// HTTP parser
    emptybody_parser parser_;
    /// The error code
    beast::error_code ec_;

    /// The original response
    std::reference_wrapper<response> parent_resp_;

    //Chunk decoding callbacks
    std::function<
        void(uint64_t,
             beast::string_view,
             beast::error_code&)> on_chunk_header_cb_;
 
    std::function<
        size_t(uint64_t,
               beast::string_view,
               beast::error_code&)> on_chunk_body_cb_;

  };

  /*
   */
  class chunk_iterator
  {
  public: // iterator typedefs
    using value_type        = beast::string_view;
    using pointer           = chunk_response_block*;
    using reference         = typename std::add_lvalue_reference<chunk_response_block>::type;
    using difference_type   = size_t;
    using iterator_category = std::input_iterator_tag;

    /*
     * https://stackoverflow.com/questions/35165828/
     */
    class post_increment_proxy
    {
    public:
      post_increment_proxy(const typename chunk_response_block::storage_type& s)
        : value_(s)
      {}
      post_increment_proxy(const post_increment_proxy&) = default;
      post_increment_proxy& operator=(const post_increment_proxy&) = default;

    public:
      typename chunk_response_block::storage_type&
      operator*() const 
      {
        return value_;
      }

    private:
      typename chunk_response_block::storage_type value_;
    };

  public: // 'tors
    chunk_iterator(chunk_response_block& crb, TransportAdapter& transport)
      : crb_ref_(crb)
      , transport_(transport)
    {}

    chunk_iterator(bool finish)
      : finished_(finish)
    {}

    chunk_iterator(const chunk_iterator&) = default;
    chunk_iterator& operator=(const chunk_iterator&) = default;

  public: // Input Iterator Requirements
    /*
     */
    value_type operator*() const noexcept
    {
      return crb_ref_.get().get_chunk_body();
    }

    /*
     */
    pointer operator->() const noexcept
    {
      return &(crb_ref_.get());
    }

    /*
     */
    chunk_iterator& operator++() noexcept
    {
      finished_ = !(crb_ref_.get().fill_in_next_chunk(transport_.get()));
      return *this;
    }

    /*
     */
    post_increment_proxy operator++(int) noexcept
    {
      post_increment_proxy result{crb_ref_.get().get_storage()};
      finished_ = !(crb_ref_.get().fill_in_next_chunk(transport_.get()));
      return result;
    }

    /*
     */
    bool finished() const noexcept { return finished_; }

    /// Friends
    friend bool operator==(const chunk_iterator& a, const chunk_iterator& b)
    {
      return a.finished() == b.finished();
    }
    friend bool operator!=(const chunk_iterator& a, const chunk_iterator& b)
    {
      return !(a == b);
    }

  private:
    ///
    bool finished_ = false;
    ///
    boost::optional<chunk_response_block&> crb_ref_;
    ///
    boost::optional<TransportAdapter&> transport_;
  };

public: // Exposed APIs
  /*
   */
  chunk_iterator begin()
  {
    return chunk_iterator{crb_.get(), transport_.get()};
  }

  /*
   */
  chunk_iterator end()
  {
    return chunk_iterator{true};
  }


private: // Data Members (chunked_response)
  ///
  std::reference_wrapper<response> parent_resp_;
  ///
  std::reference_wrapper<TransportAdapter> transport_;
  ///
  boost::optional<chunk_response_block> crb_;
};


/*
 */
class response: 
  public beast::http::response<beast::http::dynamic_body>
{
public: // 'tors
  response() = default;
  response(const response&) = delete;
  response(response&&) = default;
  response& operator=(const response&) = delete;
  response& operator=(response&&) = default;

public:
  
public: // Exposed APIs
  /*
   */
  const response_header& header() const noexcept
  {
    return (*this);
  }

  /*
   */
  response_header& header()
  {
    return (*this);
  }

  /*
   */
  void set_chunked_response() noexcept
  {
    has_chunked_response_ = true;
  }

  /*
   */
  bool has_chunked_response() const noexcept
  {
    return has_chunked_response_;
  }

  /*
   */
  template <typename Transport>
  chunked_response<Transport, std::string>
  chunk_response(Transport& transport)
  {
    BOOST_ASSERT_MSG(has_chunked_response(), 
        "Response does not have any chunked body");
    return chunked_response<Transport, std::string>{*this, transport};
  }

  /*
   */
  std::chrono::milliseconds elapsed_time() const noexcept
  {
    return elapsed_time_;
  }

private:
  ///
  std::chrono::milliseconds elapsed_time_;
  ///
  bool has_chunked_response_ = false;
};


} // END namespace hypertext
} // END namespace hypertext

#include "hypertext/impl/response.ipp"

#endif
