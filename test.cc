#include <cassert>

#include <iostream>

#include "argparser.hh"

auto main(int argc, char **argv) -> int {
  arg_parser::arg_parser args{argc, argv};

  assert(not args.get_args().empty());
  for (const auto &arg : args.get_args()) {
    std::clog << arg << '\n';
  }

  return 0;
}
