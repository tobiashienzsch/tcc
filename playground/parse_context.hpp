#ifndef TCC_PLAYGROUND_PARSE_CONTEXT_HPP
#define TCC_PLAYGROUND_PARSE_CONTEXT_HPP

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <iterator>
#include <ostream>
#include <vector>

class parse_context {
public:
  auto add_error(const std::string& msg) -> void {
    errors_.push_back(fmt::format("ERROR: {}", msg));
  }

  auto print_errors(std::ostream &out) const -> void {
    std::copy(errors_.begin(), errors_.end(),
              std::ostream_iterator<std::string>(out, "\n"));
  }

  [[nodiscard]] auto has_errors() const noexcept -> bool {
    return !errors_.empty();
  }

private:
  std::vector<std::string> errors_{};
};

#endif // TCC_PLAYGROUND_PARSE_CONTEXT_HPP
