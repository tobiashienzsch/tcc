#pragma once

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <string>

namespace tcc
{
namespace fs = boost::filesystem;

/**
 * @brief Handle to a file on the filesystem.
 *
 * @todo Use memory mapped file for loading string.
 *
 * https://stackoverflow.com/questions/17925051/fast-textfile-reading-in-c/17925143#17925143
 */
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
    File(fs::path path, bool createIfNotExists = false) : path_ {std::move(path)}
    {
        if (createIfNotExists)
        {
            fs::fstream file {};
            file.open(path_, std::ios::out);
            file.close();
        }
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
    [[nodiscard]] auto LoadAsString() noexcept -> std::string
    {
        if (Exists())
        {
            // open file
            fs::ifstream file {path_, std::ios_base::in};
            file.unsetf(std::ios::skipws);  // No white space skipping!

            // reserve file size
            std::string str;
            file.seekg(0, std::ios::end);
            str.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            // copy to string
            str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            return str;
        }

        return {};
    }

private:
    fs::path path_;
};

}  // namespace tcc
