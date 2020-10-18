#pragma once

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <string>

namespace tcc
{
namespace fs = boost::filesystem;
class File
{
public:
    /**
     * @brief Default constructor. Does not create anything.
     */
    File() noexcept = default;

    /**
     * @brief Create a file from the given absolute \p path.
     */
    explicit File(fs::path path) : path_ {std::move(path)}
    {
        fs::fstream file {};
        file.open(path_, std::ios::out);
        file.close();
    }

    /**
     * @brief Returns true if the file exists.
     */
    [[nodiscard]] auto Exists() const noexcept -> bool
    {
        auto ec           = boost::system::error_code {};
        auto const exists = fs::exists(path_, ec);
        return !ec ? exists : false;
    }

    /**
     * @brief Returns the file size in bytes. -1 if the file doesn't exist or an error ocurred.
     */
    [[nodiscard]] auto SizeInBytes() const noexcept -> std::int64_t
    {
        if (Exists())
        {
            auto ec         = boost::system::error_code {};
            auto const size = fs::file_size(path_, ec);
            return !ec ? size : -1;
        }

        return -1;
    }

    /**
     * @brief Removes the file if it exists.
     */
    auto Remove() noexcept -> bool
    {
        if (Exists())
        {
            auto ec            = boost::system::error_code {};
            auto const removed = fs::remove(path_, ec);
            return !ec ? removed : false;
        }

        return false;
    }

    /**
     * @brief Removes the file if it exists.
     */
    auto LoadAsString() noexcept -> std::string
    {
        if (Exists())
        {
            fs::ifstream file {path_};
            std::string str;

            file.seekg(0, std::ios::end);
            str.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            return str;
        }

        return {};
    }

private:
    fs::path path_;
};

}  // namespace tcc
