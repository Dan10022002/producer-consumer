// Copyright 2021 Dan10022002 dan10022002@mail.ru

#ifndef INCLUDE_DOWNLOADER_HPP_
#define INCLUDE_DOWNLOADER_HPP_

#include <queue.hpp>
#include <parser.hpp>
#include <string>

struct parameters{
  std::string _host;
  std::string _target;
  std::string _page;
  std::string _protocol;
  int _depth;
};

class downloader{
  static void download_http(parameters&& page);
  static void download_https(parameters&& page);
 public:
  static Queue<parameters> page_queue;

  downloader() = delete;
  static void download_page();
};

#endif //INCLUDE_DOWNLOADER_HPP_