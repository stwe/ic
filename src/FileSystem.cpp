// This file is part of the IC project.
//
// Copyright (c) 2023. stwe <https://github.com/stwe/ic>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#include "FileSystem.h"
#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #include <Aclapi.h>
    #include <ranges>
#endif

//-------------------------------------------------
// Read
//-------------------------------------------------

bool ic::fs::path_comparator(const std::filesystem::path& t_p1, const std::filesystem::path& t_p2)
{
    if (std::filesystem::is_directory(t_p1) && !std::filesystem::is_directory(t_p2))
    {
        return true;
    }
    else if (!std::filesystem::is_directory(t_p1) && std::filesystem::is_directory(t_p2))
    {
        return false;
    }

    // Both are either directories or files.
    return t_p1.string() < t_p2.string();
}

std::set<std::filesystem::path, decltype(ic::fs::path_comparator)*> ic::fs::read_from(const std::filesystem::path& t_path)
{
    IC_LOG_DEBUG("[read_from] read files and directories ...");

    std::set<std::filesystem::path, decltype(fs::path_comparator)*> results(&fs::path_comparator);
    for (const auto& entry : std::filesystem::directory_iterator(t_path))
    {
        results.emplace(entry.path());
    }

    return results;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool ic::fs::is_root_directory(const std::filesystem::path& t_path, const std::set<std::filesystem::path>& t_rootPaths)
{
    return t_rootPaths.contains(t_path);
}

bool ic::fs::is_junction_directory(const std::wstring_view& t_path)
{
    const auto attributes{ GetFileAttributesW(t_path.data()) };
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool ic::fs::is_hidden_directory(const std::wstring_view& t_path)
{
    const auto attributes{ GetFileAttributesW(t_path.data()) };
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return (attributes & FILE_ATTRIBUTE_HIDDEN) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool ic::fs::is_access_denied(const std::wstring_view& t_path)
{
    PSECURITY_DESCRIPTOR sd{ nullptr };

    const auto dwRes{ GetNamedSecurityInfoW(
        t_path.data(),
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &sd
    ) };

    return dwRes != ERROR_SUCCESS;
}

std::vector<char> ic::fs::get_available_drive_letters()
{
    const auto drives{ GetLogicalDrives() };

    auto driveLetters = std::views::iota(0u, 26u)
        | std::views::filter([drives](const int t_i) { return (drives & (1 << t_i)) != 0; })
        | std::views::transform([](const int t_i) { return static_cast<char>('A' + t_i); });

    return { driveLetters.begin(), driveLetters.end() };
}
