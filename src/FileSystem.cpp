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

#if defined(_WIN64) && defined(_MSC_VER)
    #include <Windows.h>
    #include <Aclapi.h>
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

bool ic::fs::is_root_directory(const std::filesystem::path& t_path)
{
    return t_path != std::filesystem::path(std::filesystem::current_path().root_path());
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
