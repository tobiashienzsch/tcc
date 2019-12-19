#pragma once

#include "fmt/format.h"
#include "fmt/ostream.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <cstdlib>

namespace tcvm
{

struct ProgramOptions
{
    static auto Parse(int argc, char** argv, po::variables_map& outputVariableMap) -> bool
    {
        try
        {
            po::options_description desc("tcc: tobante's crappy virtual machine");
            desc.add_options()                           //
                ("help,h", "produce this help message")  //
                ;

            po::store(po::parse_command_line(argc, argv, desc), outputVariableMap);
            po::notify(outputVariableMap);

            if (outputVariableMap.count("help"))
            {
                fmt::print("{}\n", desc);
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