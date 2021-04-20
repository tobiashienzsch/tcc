#include "ast.hpp"
#include "parser.hpp"

#include <boost/iostreams/device/mapped_file.hpp> // for mmap

#include <cstdlib>  // for EXIT_SUCCESS
#include <iostream> // for std::cout

namespace bio = boost::iostreams;

auto main(int argc, char **argv) -> int {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " path/to/source.tcc\n";
    std::exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
  }

  auto mmap = bio::mapped_file(argv[1], bio::mapped_file::readonly);
  auto p = parser{std::string_view{mmap.const_data(), mmap.size()}};
  auto ast = p.generate_ast();

  p.print_diagnostics(std::cout);
  ast_utils::pretty_print(std::cout, *ast);

  return EXIT_SUCCESS;
}