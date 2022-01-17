// Copyright 2021 Dan10022002 dan10022002@mail.ru

#include <downloader.h>
#include <threadpool.h>
#include <boost/program_options.hpp>
#include <fstream>
#include <parser.h>

namespace po = boost::program_options;

int main (int args, char* argv[]) {
  po::options_description desc("Options");
  desc.add_options()
      ("URL", po::value<std::string>(), "HTML page address")
      ("depth", po::value<int>(), "page search depth")
      ("network_threads", po::value<int>(),
          "the number of threads-downloader")
      ("parser_threads", po::value<int>(),
          "the number of threads-parser")
      ("output", po::value<std::string>(), "output file path");

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(args,argv)
                                  .options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);

  std::string _URL;
  int _depth;
  int _network_threads;
  int _parser_threads;
  std::string _output;

  if (vm.count("URL")) {
    _URL = vm["URL"].as<std::string>();
  } else {
    _URL = "https://github.com/";
  }
  if (vm.count("depth")) {
    _depth = vm["depth"].as<int>();
  } else {
    _depth = 1;
  }
  if (vm.count("network_threads")) {
    _network_threads = vm["network_threads"].as<int>();
  } else {
    _network_threads = 1;
  }
  if (vm.count("parser_threads")) {
    _parser_threads = vm["parser_threads"].as<int>();
  } else {
    _parser_threads = 1;
  }
  if (vm.count("output")) {
    _output = vm["output"].as<std::string>();
  } else {
    _output = "output.txt";
  }

  ThreadPool network_pool(_network_threads);
  ThreadPool parser_pool(_parser_threads);
  URL current_URL{_URL, _depth};
  URL_queue.push(std::move(current_URL));

  while ((!URL_queue.empty()) || (!page_queue.empty()) ||
         (URL_queue._counter != 0) ||
         (page_queue._counter != 0)) {
    network_pool.enqueue([] {downloader::download_page();});
    parser_pool.enqueue([] {parser::parse();});
  }

  std::ofstream output_file{_output};
  while (!URL_queue.empty()) {
    output_file << write_queue.front();
    write_queue.pop();
  }
  output_file.close();
  return 0;
}
