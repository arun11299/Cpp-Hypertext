#ifndef CPP_HT_HTTP_FREE_FUNCS_IPP
#define CPP_HT_HTTP_FREE_FUNCS_IPP

#include <string>
#include <fstream>
#include <iostream>

#include "hypertext/url.hpp"
#include "hypertext/exceptions.hpp"
#include "hypertext/http_free_funcs.hpp"

#include "boost/filesystem.hpp"

namespace beast = boost::beast;

namespace hypertext {

namespace { // Anonymous namespace

/*
 */
template <typename Stream, typename TransportAdapter>
Stream& handle_regular_response(
    Stream& os, 
    types::response<TransportAdapter>& resp)
{
  auto cb_seq = resp.content_iter();

  for (auto buf : cb_seq) {
    //TODO: FIXME: Need stream concept ?
    os.write(boost::asio::buffer_cast<const char*>(buf),
             boost::asio::buffer_size(buf));
  }

  return os;
}

/*
 */
template <typename Stream, typename TransportAdapter>
Stream& handle_chunked_response(
    Stream& os, 
    types::response<TransportAdapter>& resp)
{
  auto chunk_resp = resp.chunk_response();
  for (const auto& chunk : chunk_resp) {
    os << chunk;
  }

  return os;
}

} // END anonymous namespace


template <typename Stream, typename TransportAdapter>
Stream& operator<< (
    Stream& os, 
    types::response<TransportAdapter>& resp)
{
  return resp.has_chunked_response()
    ? handle_chunked_response(os, resp)
    : handle_regular_response(os, resp)
    ;
}


template <typename TransportAdapter = adapter::asio_transport>
typename TransportAdapter::result_type
download_file_impl(beast::string_view url,
                   const std::string& file,
                   bool verify)
{
  namespace para = parameters;

  //Streaming needs a session object to have greater lifetime.
  session<adapter::asio_transport> sess;

  auto result = sess.request(
                      para::method("GET"),
                      para::url(url), 
                      para::headers({{"Accept-Encoding", "gzip, deflate"}}),
                      //para::stream(true),
                      para::verify(verify));

  /*BOOST_ASSERT_MSG (result.resp.has_chunked_response(),
      "Response is not for streaming");*/

  std::ofstream out{file, std::ios::binary};
  if (!out) {
    std::string exp = std::string{"Failed to write to "} + file.data();
    throw FileError(std::move(exp));
  }

  out << result.resp;
  return result;
}

template <typename TransportAdapter>
typename TransportAdapter::result_type
download_file(beast::string_view url, 
              boost::optional<beast::string_view> file_name)
{
  if (!url.length()) {
    throw UnexpectedURLFormat{"Invalid URL provided."};
  }
  // It does not make sense to add a trailing '/'
  // if the resource to be downloaded is a file.
  if (url.back() == '/') {
    std::string msg = std::string{url.data()} + " has / at the end. "
      "Trailing / no expected for file download URL.";
    throw UnexpectedURLFormat{std::move(msg)};
  }

  std::string fname;

  if (!file_name) {
    url::url_view uview{url};
    beast::string_view rname = uview.resource_name();

    if (!rname.length()) {
      throw UnexpectedURLFormat{"Resource name not found in URL"};
    }
    fname = rname.data();
  }

  BOOST_ASSERT_MSG (fname.length(), "File name to be saved could not be determined.");

  //FIXME: Verify is hard coded!!
  return download_file_impl(url, fname, false);
}

}

#endif
