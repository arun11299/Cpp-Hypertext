#ifndef CPP_HT_RESPONSE_IPP
#define CPP_HT_RESPONSE_IPP

namespace beast = boost::beast;

namespace hypertext {
namespace types {

template <typename Transport, typename CBS>
chunked_response<Transport, CBS>::
chunk_response_block::chunk_response_block(
    std::reference_wrapper<response<Transport>> resp)
  : parent_resp_(resp)
{
  //On Chunk header callback
  on_chunk_header_cb_ =
  [&](uint64_t chunk_size, 
      beast::string_view extensions, 
      beast::error_code& ec)
  {
    if (ec) return;
  
    if (chunk_size > std::numeric_limits<size_t>::max()) {
      ec = beast::http::error::body_limit;
      return;
    }
 
    this->chunk_body_.reserve(static_cast<size_t>(chunk_size));
    this->chunk_body_.clear();
  };

  //Set the on header chunk callback.
  //Reserve the string size based on the
  //chunk length.
  parser_.on_chunk_header(on_chunk_header_cb_);


  //On Chunk body callback
  on_chunk_body_cb_ =
  [&](uint64_t           rem_chunk_size,
      beast::string_view body,
      beast::error_code& ec)
  {
    // This is the last portion of chunk to be read.
    // Set the error code indicating this condition.
    if (rem_chunk_size == body.size()) {
      ec = beast::http::error::end_of_chunk;
    }
  
    this->chunk_body_.append(body.data(), body.size());

    //Return the consumed amount of bytes in this step.
    return body.size();
  };

  //Set the on body chunk callback.
  parser_.on_chunk_body(on_chunk_body_cb_);
}

template <typename Transport, typename CBS>
bool
chunked_response<Transport, CBS>::
chunk_response_block::fill_in_next_chunk(Transport& transport)
{
  //There is nothing more to parse
  if (parser_.is_done()) return false;

  bool read_header = (pbuf_.size() == 0);

  transport.read_next_chunked_body(pbuf_, parser_, ec_);

  if (read_header) {
    auto& parser_msg = parser_.get();
    auto& header = parser_msg.base();
    parent_resp_.get().header() = header;
  }

  if (ec_ == beast::http::error::end_of_chunk) {
    return true;
  }

  return ec_ ? false : true;
}

//////////////////////////////////////////////////////////////////////
//   Chunk Iterator
//////////////////////////////////////////////////////////////////////
/*
template <typename Transport, typename CBS>
bool
operator==(const typename chunked_response<Transport, CBS>::chunk_iterator& a,
           const typename chunked_response<Transport, CBS>::chunk_iterator& b)
{
  return a.finished() == b.finished();
}

template <typename Transport, typename CBS>
bool
operator!=(const typename chunked_response<Transport, CBS>::chunk_iterator& a,
           const typename chunked_response<Transport, CBS>::chunk_iterator& b)
{
  return !(a == b);
}
*/

//////////////////////////////////////////////////////////////////////
//   Response API Implementation
//////////////////////////////////////////////////////////////////////

template <typename TransportAdapter>
auto make_result(response<TransportAdapter> resp)
{
  return result_type<TransportAdapter>{std::move(resp), resp.status_code()};
}

} // END namespace types
} // END namespace hypertext


#endif
