#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <cctype>

#include <iostream>
#include <string>
#include <vector>

namespace ArgParser {

class Argument final {
public:
  Argument() = default;
  Argument(std::string);

#ifndef NDEBUG
  friend std::ostream &operator<<(std::ostream &, const Argument &);
#endif

  const std::string &GetField() const { return field; }
  const std::string &GetContent() const { return content; }

  bool IsLong() const { return long_op; }

private:
  bool long_op = true;
  std::string field = "";
  std::string content = "";
};

class ArgParser final {
public:
  ArgParser() = delete;
  ArgParser(int, char **);

#ifndef NDEBUG
  friend std::ostream &operator<<(std::ostream &, const ArgParser &);

  const std::vector<Argument> &GetArgs() { return args; }
#endif

private:
  std::vector<Argument> args;
};

inline Argument::Argument(std::string r) : long_op{}, field{}, content{} {
  if (r.find("--") != r.npos) {
    this->long_op = {true};
    r.erase(0, 2);
  } else {
    this->long_op = {false};
    r.erase(0, 1);
  }

  this->field = {r.substr(0, r.find(' '))};
  r.erase(0, r.find(' '));

  if (!r.empty()) {
    this->content = {r};
  }
}

#ifndef NDEBUG
inline std::ostream &operator<<(std::ostream &rhs, const Argument &lhs) {
  rhs << std::boolalpha << lhs.GetField()
      << (lhs.GetContent().empty() ? "" : " ") << lhs.GetContent() << ' '
      << lhs.IsLong() << std::noboolalpha;

  return rhs;
}
#endif

inline ArgParser::ArgParser(int argc, char **argv) : args{} {
  this->args.reserve(static_cast<decltype(this->args)::size_type>(argc));

  auto i{1};
  while (i != argc) {
    auto dummy{std::string(*(argv + i)) + [&]() -> std::string {
      const char *dummy = *(argv + i + 1);
      if (dummy && std::string(dummy).at(0) != '-') {
        return ' ' + std::string(dummy);
      } else {
        return "";
      }
    }()};

    this->args.emplace_back(dummy);

    if (!*(argv + ++i)) {
      break;
    }
  }

  this->args.shrink_to_fit();
}

#ifndef NDEBUG
inline std::ostream &operator<<(std::ostream &rhs, const ArgParser &lhs) {
  for (const auto &a : lhs.args) {
    rhs << a;
  }

  return rhs;
}
#endif

} // namespace ArgParser

#endif // ARGPARSER_H
