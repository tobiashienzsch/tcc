#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>

// #include <boost/archive/text_iarchive.hpp>
// #include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/serialization.hpp>

#include "fmt/printf.h"

struct ParameterChange {
  float value{};

  template <typename archive>
  void serialize(archive& ar, const unsigned /*version*/) {
    ar& value;
  }
};

struct ParameterMetadata {
  std::map<std::string, std::string> values{};

  template <typename archive>
  void serialize(archive& ar, const unsigned /*version*/) {
    ar& values;
  }
};

struct Message {
  int32_t plugin_id{};
  int32_t parameter_id{};

  ParameterChange change{};
  boost::optional<ParameterMetadata> metadata{};

  template <typename archive>
  void serialize(archive& ar, const unsigned /*version*/) {
    ar& plugin_id;
    ar& parameter_id;
    ar& change;
    ar& metadata;
  }
};

auto main(int, char**) -> int {
  std::stringstream ss;

  {
    auto msg = Message{};
    msg.parameter_id = 143;
    msg.change = ParameterChange{2.9f};
    auto metadata = ParameterMetadata{};
    metadata.values["test"] = "TEST";
    msg.metadata = metadata;
    // boost::archive::text_oarchive out_archive(ss);
    boost::archive::binary_oarchive out_archive(ss, boost::archive::no_header);
    out_archive << msg;
  }

  {
    auto msg = Message{};
    boost::archive::binary_iarchive in_archive(ss, boost::archive::no_header);
    in_archive.get_flags();
    in_archive >> msg;

    fmt::printf("%d\n", msg.parameter_id);
    if (msg.metadata.has_value()) {
      fmt::printf("%s\n", msg.metadata.value().values["test"]);
    }
  }

  return EXIT_SUCCESS;
}