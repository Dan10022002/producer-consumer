// Copyright 2021 Dan10022002 dan10022002@mail.ru

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include <queue.h>
#include <downloader.h>
#include <string>

struct URL{
  std::string _URL;
  int _depth;
};

static Queue<URL> URL_queue;
static Queue<std::string> write_queue;

class parser{
 public:
  parser() = delete;
  static void parse();
};

#endif //INCLUDE_PARSER_HPP_
