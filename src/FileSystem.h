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

#pragma once

#include <set>
#include <filesystem>

namespace ic::fs
{
    //-------------------------------------------------
    // Read
    //-------------------------------------------------

    bool path_comparator(const std::filesystem::path& t_p1, const std::filesystem::path& t_p2);

    std::set<std::filesystem::path, decltype(fs::path_comparator)*> read_from(const std::filesystem::path& t_path);

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    [[nodiscard]] bool is_root_directory(const std::filesystem::path& t_path, const std::set<std::filesystem::path>& t_rootPaths);

#if defined(_WIN64) && defined(_MSC_VER)

    [[nodiscard]] bool is_junction_directory(const std::wstring_view& t_path);
    [[nodiscard]] bool is_hidden_directory(const std::wstring_view& t_path);
    [[nodiscard]] bool is_access_denied(const std::wstring_view& t_path);
    [[nodiscard]] std::vector<char> get_available_drive_letters();

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)

    [[nodiscard]] bool is_hidden(const std::filesystem::path& t_path);

#endif
}
