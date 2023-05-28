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

#include <filesystem>

namespace ic::data
{
    struct PathComparator
    {
        bool operator()(const std::filesystem::path& t_a, const std::filesystem::path& t_b) const
        {
            if (std::filesystem::is_directory(t_a) && !std::filesystem::is_directory(t_b))
            {
                return true;
            }
            else if (!std::filesystem::is_directory(t_a) && std::filesystem::is_directory(t_b))
            {
                return false;
            }

            // Both are either directories or files.
            std::string a = t_a.string();
            std::string b = t_b.string();
            std::transform(a.begin(), a.end(), a.begin(), [](unsigned char c){ return std::tolower(c); });
            std::transform(b.begin(), b.end(), b.begin(), [](unsigned char c){ return std::tolower(c); });

            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    };
}
