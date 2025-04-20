#ifndef ARGPARSER_H
#define ARGPARSER_H

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
  } else if (r.find('-') != r.npos) {
    this->long_op = {false};
    r.erase(0, 1);
  } else {
    // TODO exception handling
  }

  this->field = {r.substr(0, r.find(' '))};
  r.erase(0, r.find(' ') != r.npos ? r.find(' ') + 1 : r.npos);

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
  // Safe, conservative preset not representative of reality
  this->args.reserve(static_cast<decltype(this->args)::size_type>(argc - 1));

  auto i{1};
  while (i != argc) {
    auto dummy{std::string(*(argv + i)) + [&]() -> std::string {
      std::string dummy = *(argv + i + 1) ? *(argv + i + 1) : "";
      if (!dummy.empty() && dummy.at(0) != '-') {
        return ' ' + dummy;
      } else {
        return "";
      }
    }()};

    if (dummy.find(' ') != dummy.npos) {
      ++i;
    }

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
