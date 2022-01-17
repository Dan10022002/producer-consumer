// Copyright 2021 Dan10022002 dan10022002@mail.ru

#ifndef INCLUDE_DOWNLOADER_H_
#define INCLUDE_DOWNLOADER_H_

#include <queue.h>
#include <parser.h>
#include <string>

struct parameters{
  std::string _host;
  std::string _target;
  std::string _page;
  std::string _protocol;
  int _depth;
};

static Queue<parameters> page_queue;

class downloader{
 public:
  static void download_http(parameters&& page);
  static void download_https(parameters&& page);

  downloader() = delete;
  static void download_page();
};

#endif //INCLUDE_DOWNLOADER_H_
