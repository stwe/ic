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

std::set<std::filesystem::path> ic::fs::read_from(const std::filesystem::path& t_path)
{
    std::set<std::filesystem::path> results;
    for (const auto& entry : std::filesystem::directory_iterator(t_path))
    {
        results.emplace(entry.path());
    }

    return results;
}

bool ic::fs::IsRoot(const std::filesystem::path& t_path)
{
    return t_path != std::filesystem::path(std::filesystem::current_path().root_path());
/*
#if defined(_WIN64) && defined(_MSC_VER)
    return t_path != std::filesystem::path(std::filesystem::current_path().root_path());
#else
    return t_path != std::filesystem::path("/");
#endif
*/
}
