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

#include "Util.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #define NOMINMAX
    #include <codecvt>
    #include <Aclapi.h>
    #include <ranges>
#endif

//-------------------------------------------------
// Utils
//-------------------------------------------------

std::string ic::application::Util::ToZeroLead(const std::string& t_time)
{
    return std::string(2 - std::min(2, static_cast<int>(t_time.length())), '0') + t_time;
}

std::string ic::application::Util::LastWriteTimeToStr(const std::filesystem::file_time_type& t_fileTime)
{
    std::chrono::time_point<std::chrono::system_clock> ftsys;

#if defined(_WIN64) && defined(_MSC_VER)
    ftsys = std::chrono::utc_clock::to_sys(std::chrono::file_clock::to_utc(t_fileTime));
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    ftsys = std::chrono::file_clock::to_sys(t_fileTime);
#endif

    time_t ftt{ std::chrono::system_clock::to_time_t(ftsys) };
    const auto tminfo{ localtime(&ftt) };

    return ToZeroLead(std::to_string(tminfo->tm_mday)).append(".").append(ToZeroLead(std::to_string(tminfo->tm_mon))).append(".").append(std::to_string(tminfo->tm_year + 1900)).append(" ").append(ToZeroLead(std::to_string(tminfo->tm_hour))).append(":").append(ToZeroLead(std::to_string(tminfo->tm_min)));
}

std::string ic::application::Util::GetHumanReadableSize(const unsigned long long t_bytes)
{
    static constexpr float gb{ 1024.0f * 1024.0f * 1024.0f };
    static constexpr float mb{ 1024.0f * 1024.0f };
    static constexpr float kb{ 1024.0f };

    if (auto bytes{ static_cast<float>(t_bytes) }; bytes > gb)
    {
        return FloatToString(bytes / gb).append(" Gb ");
    }
    else if (bytes > mb)
    {
        return FloatToString(bytes / mb).append(" Mb ");
    }
    else if (bytes > kb)
    {
        return FloatToString(bytes / kb).append(" Kb ");
    }

    return FloatToString(static_cast<float>(t_bytes)).append(" B ");
}

std::string ic::application::Util::FloatToString(const float t_val)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << t_val;
    std::string result = oss.str();

    return result;
}

//-------------------------------------------------
// Win64
//-------------------------------------------------

#if defined(_WIN64) && defined(_MSC_VER)
std::string ic::application::Util::WstringConv(const std::filesystem::path& t_path)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(t_path.filename().wstring());
}

bool ic::application::Util::IsJunctionDirectory(const std::wstring_view& t_path)
{
    const auto attributes{ GetFileAttributesW(t_path.data()) };
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool ic::application::Util::IsHiddenDirectory(const std::wstring_view& t_path)
{
    const auto attributes{ GetFileAttributesW(t_path.data()) };
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return (attributes & FILE_ATTRIBUTE_HIDDEN) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool ic::application::Util::IsAccessDenied(const std::wstring_view& t_path)
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

std::vector<char> ic::application::Util::GetAvailableDriveLetters()
{
    const auto drives{ GetLogicalDrives() };

    auto driveLetters = std::views::iota(0u, 26u)
        | std::views::filter([drives](const int t_i) { return (drives & (1 << t_i)) != 0; })
        | std::views::transform([](const int t_i) { return static_cast<char>('A' + t_i); });

    return { driveLetters.begin(), driveLetters.end() };
}
#endif

//-------------------------------------------------
// Linux
//-------------------------------------------------

#if defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
bool ic::application::Util::IsHidden(const std::filesystem::path& t_path)
{
    return !t_path.empty() && t_path.filename().string().front() == '.';
}
#endif
