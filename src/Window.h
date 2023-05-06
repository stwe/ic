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

#include <SDL.h>
#include <imgui.h>
#include <string>

namespace ic
{
    class Window
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr auto WINDOW_BG_COLOR{ ImVec4(0.0f, 0.0f, 0.66f, 1.00f) };
        static constexpr auto TEXT_COLOR{ ImVec4(1.0f, 1.0f, 1.0f, 1.00f) };
        static constexpr auto TITLE_BG_COLOR{ ImVec4(0.0f, 0.66f, 0.66f, 1.00f) };
        static constexpr auto COLOR_TITLE_BG_ACTIVE{ ImVec4(0.66f, 0.66f, 0.66f, 1.00f) };
        static constexpr auto BORDER_COLOR{ ImVec4(0.3f, 0.3f, 0.3f, 1.00f) };
        static constexpr auto HEADER_HOVERED_COLOR{ ImVec4(0.3f, 0.3f, 0.3f, 1.0f) };
        static constexpr auto MENU_BAR_BG_COLOR{ ImVec4(0.0f, 0.66f, 0.66f, 1.0f) };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        SDL_Window* sdlWindow{ nullptr };
        int width;
        int height;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Window() = delete;

        Window(std::string t_title, int t_width, int t_height);

        Window(const Window& t_other) = delete;
        Window(Window&& t_other) noexcept = delete;
        Window& operator=(const Window& t_other) = delete;
        Window& operator=(Window&& t_other) noexcept = delete;

        ~Window() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void StartFrame() const;
        void EndFrame() const;

        //-------------------------------------------------
        // Helper / Event handler
        //-------------------------------------------------

        void ChangeSize(int t_width, int t_height);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        SDL_GLContext m_sdlGlContext{ nullptr };
        std::string m_title;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitSdl();
        void InitImGui();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
