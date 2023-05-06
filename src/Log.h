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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//-------------------------------------------------
// Log
//-------------------------------------------------

namespace ic
{
    class Log
    {
    public:
        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        static void Init()
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");

            m_logger = spdlog::stdout_color_mt("IC");

#ifdef IC_DEBUG_BUILD
            m_logger->set_level(spdlog::level::trace);
#else
            logger->set_level(spdlog::level::info);
#endif
        }

        inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_logger; }

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static std::shared_ptr<spdlog::logger> m_logger;
    };

#define IC_LOG_TRACE(...)   Log::GetLogger()->trace(__VA_ARGS__)
#define IC_LOG_DEBUG(...)   Log::GetLogger()->debug(__VA_ARGS__)
#define IC_LOG_INFO(...)    Log::GetLogger()->info(__VA_ARGS__)
#define IC_LOG_WARN(...)    Log::GetLogger()->warn(__VA_ARGS__)
#define IC_LOG_ERROR(...)   Log::GetLogger()->error(__VA_ARGS__)
#define IC_LOG_CRITICAL(...)   Log::GetLogger()->critical(__VA_ARGS__)
}
