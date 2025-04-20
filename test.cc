#include <cassert>

#include "argparser.hh"

int main(int argc, char **argv) {
  ArgParser::ArgParser args{argc, argv};

  assert(args.GetArgs().size() >= 1);
  for (const auto &a : args.GetArgs()) {
    std::clog << a << '\n';
  }

  return 0;
}
