// Copyright 2021 Dan10022002 dan10022002@mail.ru

#include <downloader.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <regex>

void URL_parse(parameters& tmp_page, std::string& tmp_URL){
  size_t protocol_find = tmp_URL.find(':');
  tmp_page._protocol = tmp_URL.substr(0, protocol_find);
  size_t tmp_find = tmp_URL.find('/', protocol_find + 3);
  if (tmp_find != std::string::npos) {
    tmp_page._host = tmp_URL.substr(protocol_find + 3,
                                    tmp_find - protocol_find - 3);
    tmp_page._target = tmp_URL.substr(tmp_find,
                                      tmp_URL.size() - tmp_find + 1);
  } else {
    tmp_page._host = tmp_URL.substr(tmp_find + 3,
                                    tmp_URL.size() - protocol_find - 2);
    tmp_page._target = "/";
  }
}

void downloader::download_http(parameters&& tmp_page){
  boost::asio::io_service _service;
  boost::asio::ip::tcp::resolver _resolver{_service};
  boost::beast::tcp_stream _stream{_service};

  boost::beast::flat_buffer _buffer;
  _stream.connect(_resolver.resolve(tmp_page._host, "80"));

  boost::beast::http::request<boost::beast::http::string_body> _request {
      boost::beast::http::verb::get, tmp_page._target, 1.1
  };
  _request.set(boost::beast::http::field::host, tmp_page._host);
  _request.set(boost::beast::http::field::user_agent,
               BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(_stream, _request);

  boost::beast::http::response<boost::beast::http::string_body> _response;

  boost::beast::http::read(_stream, _buffer, _response);

  boost::beast::error_code _error;
  _stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            _error);

  tmp_page._page = _response.body();
  page_queue.push(std::move(tmp_page));

  if (_error == boost::asio::error::eof) {
    _error.assign(0, _error.category());
  }
}

void downloader::download_https(parameters&& tmp_page){
  boost::asio::io_service _service;
  boost::asio::ip::tcp::resolver _resolver{_service};
  boost::asio::ssl::context _context{boost::asio::ssl::context::sslv23_client};
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _stream{_service,
                  _context};

  boost::beast::flat_buffer _buffer;

  auto const results = _resolver.resolve(tmp_page._host, "443");
  boost::asio::connect(_stream.next_layer(), results.begin(), results.end());
  _stream.handshake(boost::asio::ssl::stream_base::client);

  boost::beast::http::request<boost::beast::http::string_body> _request {
      boost::beast::http::verb::get, tmp_page._target, 1.1
  };
  _request.set(boost::beast::http::field::host, tmp_page._host);
  _request.set(boost::beast::http::field::user_agent,
               BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(_stream, _request);

  boost::beast::http::response<boost::beast::http::string_body> _response;

  boost::beast::http::read(_stream, _buffer, _response);

  boost::beast::error_code _error;
  _stream.shutdown(_error);

  tmp_page._page = _response.body();
  page_queue.push(std::move(tmp_page));

  if (_error == boost::asio::error::eof) {
    _error.assign(0, _error.category());
  }
}

void downloader::download_page(){
  if (!URL_queue.empty()) {
    std::string tmp_URL = URL_queue.front()._URL;
    std::regex _regex("(^http[s]?://.*)");
    if (!regex_match(tmp_URL.begin(), tmp_URL.end(), _regex)) {
      URL_queue.pop();
      return;
    }
    parameters tmp_page;
    URL_parse(tmp_page, tmp_URL);
    tmp_page._depth = URL_queue.front()._depth;
    if (tmp_page._protocol == "http") {
      download_http(std::move(tmp_page));
    }
    if (tmp_page._protocol == "https") {
      download_https(std::move(tmp_page));
    }
    URL_queue.pop();
  }
}
