#if !defined(TCC_PARSER_QI_ERROR_HANDLER_HPP)
#define TCC_PARSER_QI_ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

namespace tcc {

template <typename Iterator>
struct ErrorHandler {
  template <typename, typename, typename>
  struct result {
    using type = void;
  };

  ErrorHandler(Iterator f, Iterator l, std::ostream& o) : first(f), last(l), out(o) {}

  template <typename Message, typename What>
  void operator()(Message const& message, What const& what, Iterator err_pos) const {
    int line;
    Iterator line_start = get_pos(err_pos, line);
    if (err_pos != last) {
      out << message << what << " line " << line << ':' << std::endl;
      out << get_line(line_start) << std::endl;
      for (; line_start != err_pos; ++line_start) out << ' ';
      out << '^' << std::endl;
    } else {
      out << "Unexpected end of file. ";
      out << message << what << " line " << line << std::endl;
    }
  }

  Iterator get_pos(Iterator err_pos, int& line) const {
    line = 1;
    Iterator i = first;
    Iterator line_start = first;
    while (i != err_pos) {
      bool eol = false;
      if (i != err_pos && *i == '\r')  // CR
      {
        eol = true;
        line_start = ++i;
      }
      if (i != err_pos && *i == '\n')  // LF
      {
        eol = true;
        line_start = ++i;
      }
      if (eol)
        ++line;
      else
        ++i;
    }
    return line_start;
  }

  std::string get_line(Iterator err_pos) const {
    Iterator i = err_pos;
    // position i to the next EOL
    while (i != last && (*i != '\r' && *i != '\n')) ++i;
    return std::string(err_pos, i);
  }

  Iterator first;
  Iterator last;
  std::vector<Iterator> iters;
  std::ostream& out;
};
}  // namespace tcc

#endif
