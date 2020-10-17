#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "tcc/ir/instruction_set.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
using IRIdentifier = std::string;
using IRRegister   = std::string;
using IRConstant   = std::uint32_t;

class IRArgumentList
{
public:
    using value_type = std::vector<IRRegister>;

    auto begin() noexcept -> value_type::iterator { return data_.begin(); }
    auto begin() const noexcept -> value_type::const_iterator { return data_.begin(); }
    auto cbegin() const noexcept -> value_type::const_iterator { return data_.cbegin(); }

    auto end() noexcept -> value_type::iterator { return data_.end(); }
    auto end() const noexcept -> value_type::const_iterator { return data_.end(); }
    auto cend() const noexcept -> value_type::const_iterator { return data_.cend(); }

    auto push_back(IRRegister const& str) -> void { data_.push_back(str); }
    auto push_back(IRRegister&& str) -> void { data_.push_back(std::forward<IRRegister>(str)); }

    auto size() const noexcept -> value_type::size_type { return data_.size(); }

private:
    value_type data_;
};

struct IRStatement
{
    using Argument         = std::variant<IRConstant, IRRegister, IRArgumentList>;
    using OptionalArgument = std::optional<Argument>;

    tcc::IRByteCode Type;
    bool IsTemporary {true};
    IRRegister Destination;
    Argument First;
    OptionalArgument Second;
};

auto operator<<(std::ostream& out, IRStatement const& data) -> std::ostream&;

using IRStatementList = std::vector<IRStatement>;

struct IRBasicBlock
{
    IRIdentifier Name          = "";
    IRStatementList Statements = {};
};

struct IRFunction
{
    IRIdentifier Name                    = "";
    std::map<std::string, int> Args      = {};
    std::map<std::string, int> Variables = {};
    std::vector<IRBasicBlock> Blocks     = {};
};

auto operator<<(std::ostream& out, IRFunction const& data) -> std::ostream&;

struct IRPackage
{
    IRIdentifier Name                 = "";
    std::vector<IRFunction> Functions = {};
};

auto operator<<(std::ostream& out, IRPackage const& pkg) -> std::ostream&;

}  // namespace tcc