#include "pch.h"
#include "RuntimeConfig.h"

#include <charconv>
#include <cctype>
#include <stdexcept>


namespace
{
    std::string Trim(std::string value)
    {
        auto is_space = [](unsigned char ch) { return std::isspace(ch) != 0; };

        value.erase(value.begin(), std::find_if(value.begin(), value.end(), [&](char ch) {
            return not is_space(static_cast<unsigned char>(ch));
        }));
        value.erase(std::find_if(value.rbegin(), value.rend(), [&](char ch) {
            return not is_space(static_cast<unsigned char>(ch));
        }).base(), value.end());

        return value;
    }

    std::string ToLower(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return value;
    }

    std::string MakeKey(std::string_view section, std::string_view key)
    {
        return ToLower(Trim(std::string(section))) + "." + ToLower(Trim(std::string(key)));
    }
}


std::filesystem::path Runtime::ExecutableDirectory()
{
    std::wstring buffer(32768, L'\0');
    DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (length == 0 or length >= buffer.size()) {
        return std::filesystem::current_path();
    }

    buffer.resize(length);
    return std::filesystem::path(buffer).parent_path();
}

std::filesystem::path Runtime::ResolveFromExecutable(const std::filesystem::path& path)
{
    if (path.is_absolute()) return path.lexically_normal();
    return (ExecutableDirectory() / path).lexically_normal();
}

void Runtime::UseExecutableDirectoryAsWorkingDirectory()
{
    std::filesystem::current_path(ExecutableDirectory());
}

Runtime::IniFile::IniFile(const std::filesystem::path& path)
{
    Load(path);
}

bool Runtime::IniFile::Load(const std::filesystem::path& path)
{
    _values.clear();
    _path = path;
    _loaded = false;

    std::ifstream input(path);
    if (not input.is_open()) return false;

    std::string section;
    std::string line;
    int line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;
        line = Trim(line);

        if (line.empty() or line.starts_with(';') or line.starts_with('#')) continue;

        if (line.front() == '[' and line.back() == ']') {
            section = Trim(line.substr(1, line.size() - 2));
            continue;
        }

        const size_t separator = line.find('=');
        if (separator == std::string::npos) {
            throw std::runtime_error("Invalid config line " + std::to_string(line_number) + " in " + path.string());
        }

        std::string key = Trim(line.substr(0, separator));
        std::string value = Trim(line.substr(separator + 1));
        if (key.empty()) {
            throw std::runtime_error("Empty config key on line " + std::to_string(line_number) + " in " + path.string());
        }

        _values[MakeKey(section, key)] = value;
    }

    _loaded = true;
    return true;
}

bool Runtime::IniFile::IsLoaded() const
{
    return _loaded;
}

const std::filesystem::path& Runtime::IniFile::GetPath() const
{
    return _path;
}

std::string Runtime::IniFile::GetString(std::string_view section, std::string_view key, std::string_view fallback) const
{
    auto found = _values.find(MakeKey(section, key));
    return found == _values.end() ? std::string(fallback) : found->second;
}

int Runtime::IniFile::GetInt(std::string_view section, std::string_view key, int fallback, int minimum, int maximum) const
{
    std::string value = GetString(section, key, "");
    if (value.empty()) return fallback;

    int result = 0;
    auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);
    if (error != std::errc{} or end != value.data() + value.size() or result < minimum or result > maximum) {
        throw std::runtime_error("Invalid integer config value: " + MakeKey(section, key));
    }

    return result;
}

bool Runtime::IniFile::GetBool(std::string_view section, std::string_view key, bool fallback) const
{
    std::string value = ToLower(GetString(section, key, ""));
    if (value.empty()) return fallback;
    if (value == "true" or value == "1" or value == "yes" or value == "on") return true;
    if (value == "false" or value == "0" or value == "no" or value == "off") return false;

    throw std::runtime_error("Invalid boolean config value: " + MakeKey(section, key));
}

std::filesystem::path Runtime::IniFile::GetPath(std::string_view section, std::string_view key, const std::filesystem::path& fallback) const
{
    std::filesystem::path value = GetString(section, key, fallback.string());
    return ResolveFromExecutable(value);
}

