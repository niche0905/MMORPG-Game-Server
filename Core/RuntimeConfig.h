#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>


namespace Runtime
{
    std::filesystem::path ExecutableDirectory();
    std::filesystem::path ResolveFromExecutable(const std::filesystem::path& path);
    void UseExecutableDirectoryAsWorkingDirectory();

    class IniFile
    {
    private:
        std::unordered_map<std::string, std::string> _values;
        std::filesystem::path _path;
        bool _loaded = false;

    public:
        IniFile() = default;
        explicit IniFile(const std::filesystem::path& path);

        bool Load(const std::filesystem::path& path);
        bool IsLoaded() const;
        const std::filesystem::path& GetPath() const;

        std::string GetString(std::string_view section, std::string_view key, std::string_view fallback) const;
        int GetInt(std::string_view section, std::string_view key, int fallback, int minimum, int maximum) const;
        bool GetBool(std::string_view section, std::string_view key, bool fallback) const;
        std::filesystem::path GetPath(std::string_view section, std::string_view key, const std::filesystem::path& fallback) const;
    };
}

