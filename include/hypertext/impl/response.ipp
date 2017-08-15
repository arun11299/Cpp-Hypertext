#ifndef CPP_HT_RESPONSE_IPP
#define CPP_HT_RESPONSE_IPP

namespace hypertext {
namespace types {

template <typename CBS>
response::chunk_response_block<CBS>::
chunk_response_block(in_place_construct_t)
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

template <typename CBS>
template <typename TransportAdapter>
bool
response::chunk_response_block<CBS>::
fill_in_next_chunk(TransportAdapter& transport)
{
  //There is nothing more to parse
  if (parser_.is_done()) return false;

  transport.read_next_chunked_body(pbuf_, parser_, ec_);

  if (ec_ == beast::http::error::end_of_chunk) {
    return true;
  }

  return ec_ ? false : true;
}




//////////////////////////////////////////////////////////////////////
//   Response API Implementation
//////////////////////////////////////////////////////////////////////


} // END namespace types
} // END namespace hypertext


#endif
