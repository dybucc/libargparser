#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace arg_parser {

class argument final {
public:
  argument() = default;
  explicit argument(std::string /*r*/);

#ifndef NDEBUG
  friend auto operator<<(std::ostream & /*rhs*/, const argument & /*lhs*/)
      -> std::ostream &;
#endif

  auto get_field() const -> const std::string & { return field; }
  auto get_content() const -> const std::string & { return content; }

  auto is_long() const -> bool { return long_op; }

private:
  bool long_op = true;
  std::string field;
  std::string content;
};

class arg_parser final {
public:
  arg_parser() = delete;
  arg_parser(int /*argc*/, char ** /*argv*/);

#ifndef NDEBUG
  friend auto operator<<(std::ostream & /*rhs*/, const arg_parser & /*lhs*/)
      -> std::ostream &;

  auto get_args() -> const std::vector<argument> & { return args; }
#endif

private:
  std::vector<argument> args;
};

inline argument::argument(std::string cmd) : long_op{} {
  if (cmd.find("--") not_eq std::string::npos) {
    this->long_op = {true};
    cmd.erase(0, 2);
  } else if (cmd.find('-') not_eq std::string::npos) {
    this->long_op = {false};
    cmd.erase(0, 1);
  } else {
    throw std::invalid_argument("Invalid argument format: " + cmd +
                                " [in file " + __FILE__ + ", line " +
                                std::to_string(__LINE__) + "]");
  }

  this->field = {cmd.substr(0, cmd.find(' '))};
  cmd.erase(0, cmd.find(' ') not_eq std::string::npos ? cmd.find(' ') + 1
                                                      : std::string::npos);

  if (not cmd.empty()) {
    this->content = {cmd};
  }
}

#ifndef NDEBUG
inline auto operator<<(std::ostream &rhs, const argument &lhs)
    -> std::ostream & {
  rhs << std::boolalpha << lhs.get_field()
      << (lhs.get_content().empty() ? "" : " ") << lhs.get_content() << ' '
      << lhs.is_long() << std::noboolalpha;

  return rhs;
}
#endif

inline arg_parser::arg_parser(int argc, char **argv) {
  // Safe, conservative preset not representative of reality
  this->args.reserve(static_cast<decltype(this->args)::size_type>(argc - 1));

  auto index{1};
  while (index not_eq argc) {
    auto dummy{std::string{*(argv + index)} + [&]() -> std::string {
      std::string dummy = *(argv + index + 1) ? *(argv + index + 1) : "";
      if (not dummy.empty() and dummy.at(0) not_eq '-') {
        return ' ' + dummy;
      }
      return "";
    }()};

    if (dummy.find(' ') not_eq std::string::npos) {
      ++index;
    }

    this->args.emplace_back(dummy);

    if (*(argv + ++index) == nullptr) {
      break;
    }
  }

  this->args.shrink_to_fit();
}

#ifndef NDEBUG
inline auto operator<<(std::ostream &rhs, const arg_parser &lhs)
    -> std::ostream & {
  for (const auto &arg : lhs.args) {
    rhs << arg;
  }

  return rhs;
}
#endif

} // namespace arg_parser

#endif // ARGPARSER_H
