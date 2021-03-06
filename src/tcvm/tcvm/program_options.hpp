/**
 * @file program_options.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include <boost/program_options.hpp>

#include "fmt/format.h"
#include "fmt/ostream.h"
namespace po = boost::program_options;

#include <cstdlib>

namespace tcvm
{

struct ProgramOptions
{
    static auto parse(int argc, char** argv, po::variables_map& outputVariableMap) -> bool
    {
        try
        {
            auto const* text = "tcc: tobante's crappy virtual machine";
            po::options_description desc(text);
            auto options = desc.add_options();
            options("help,h", "produce this help message");
            options("input,i", po::value<std::int64_t>(), "input argument");
            options("file,f", po::value<std::string>(), "binary file path");
            options("version,v", "print version string");

            po::store(po::parse_command_line(argc, argv, desc), outputVariableMap);
            po::notify(outputVariableMap);

            if (outputVariableMap.count("help") != 0)
            {
                fmt::print("{}\n", desc);
                std::exit(0);
            }

            if (outputVariableMap.count("version") != 0)
            {
                fmt::print("{}\n", "0.1.0");
                std::exit(0);
            }
        }
        catch (std::exception& e)
        {
            fmt::print("error: {}\n", e.what());
            return false;
        }
        catch (...)
        {
            fmt::print("Exception of unknown type!\n");
            return false;
        }

        return true;
    }
};

}  // namespace tcvm