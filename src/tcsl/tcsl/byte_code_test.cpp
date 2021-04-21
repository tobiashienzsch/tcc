/**
 * @file byte_code_test.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */
#include "tcsl/byte_code.hpp"

#include <sstream>

#include "catch2/catch.hpp"

TEST_CASE("tcsl: ByteCode ostream", "[tcsl]")
{
    auto const input = {
        tcc::ByteCode::ICONST,  //
        tcc::ByteCode::IADD,    //
        tcc::ByteCode::ISUB,    //
        tcc::ByteCode::IMUL,    //
        tcc::ByteCode::ILT,     //
        tcc::ByteCode::IEQ,     //
        tcc::ByteCode::PRINT,   //
        tcc::ByteCode::STORE,   //
        tcc::ByteCode::LOAD,    //
        tcc::ByteCode::GSTORE,  //
        tcc::ByteCode::GLOAD,   //
        tcc::ByteCode::BR,      //
        tcc::ByteCode::BRT,     //
        tcc::ByteCode::BRF,     //
        tcc::ByteCode::POP,     //
        tcc::ByteCode::CALL,    //
        tcc::ByteCode::RET,     //
        tcc::ByteCode::EXIT,    //
        tcc::ByteCode::HALT     //
    };

    auto stream = std::stringstream {};
    for (auto const& x : input) { stream << x; }
    auto const str = stream.str();

    REQUIRE(str
            == "ICONSTIADDISUBIMULILTIEQPRINTSTORELOADGSTOREGLOADBRBRTBR"
               "FPOPCALLRETEXITHALT");
}