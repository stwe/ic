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

#include <string>
#include <filesystem>

namespace ic::application
{
    class Util
    {
    public:
        //-------------------------------------------------
        // Utils
        //-------------------------------------------------

        [[nodiscard]] static bool IsRootDirectory(const std::filesystem::path& t_path);
        [[nodiscard]] static std::string ToZeroLead(const std::string& t_time);
        [[nodiscard]] static std::string LastWriteTimeToStr(const std::filesystem::file_time_type& t_fileTime);
        [[nodiscard]] static std::string GetHumanReadableSize(unsigned long long t_bytes);
        [[nodiscard]] static std::string FloatToString(float t_val);

        //-------------------------------------------------
        // Win64
        //-------------------------------------------------

#if defined(_WIN64) && defined(_MSC_VER)
        [[nodiscard]] static std::string WstringConv(const std::filesystem::path& t_path);
        [[nodiscard]] static bool IsJunctionDirectory(const std::wstring_view& t_path);
        [[nodiscard]] static bool IsHiddenDirectory(const std::wstring_view& t_path);
        [[nodiscard]] static bool IsAccessDenied(const std::wstring_view& t_path);
        [[nodiscard]] static std::vector<char> GetAvailableDriveLetters();
#endif

        //-------------------------------------------------
        // Linux
        //-------------------------------------------------

#if defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
        [[nodiscard]] static bool IsHidden(const std::filesystem::path& t_path);
#endif

    protected:

    private:

    };
}
