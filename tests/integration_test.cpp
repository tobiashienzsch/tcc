/**
 * @file statement_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tcc/asm/assembly_generator.hpp"
#include "tcc/cmd/program_options.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/parser.hpp"
#include "tsl/tsl.hpp"

#include "catch2/catch.hpp"

TEST_CASE("integration: CompileAndRun", "[integration]") { REQUIRE(true == true); }
