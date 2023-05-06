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

#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #define IC_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    #include <csignal>
    #define IC_DEBUG_BREAK raise(SIGTRAP)
#elif defined(__MINGW64__) && defined(__GNUC__) && (__GNUC__ >= 9)
    #define IC_DEBUG_BREAK __builtin_trap()
#else
    #error Unsupported platform or compiler!
#endif

#ifdef IC_DEBUG_BUILD
    #define IC_ENABLE_ASSERTS
#endif

#ifdef IC_ENABLE_ASSERTS
    #define IC_ASSERT(x, ...)                                                \
        {                                                                    \
            if (!(x))                                                        \
            {                                                                \
                IC_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);          \
                IC_DEBUG_BREAK;                                              \
            }                                                                \
        }
#else
    #define IC_ASSERT(x, ...)
#endif
