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

#include <memory>
#include <filesystem>
#include "vendor/ini/ini.h"

namespace ic
{
    class Window;

    class App
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static const inih::INIReader INI{ "./config.ini" };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        App();

        App(const App& t_other) = delete;
        App(App&& t_other) noexcept = delete;
        App& operator=(const App& t_other) = delete;
        App& operator=(App&& t_other) noexcept = delete;

        ~App() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::unique_ptr<Window> m_window;
        std::filesystem::path m_currentPathLeft{ std::filesystem::path("/") };
        std::filesystem::path m_currentPathRight{ std::filesystem::current_path() };
        std::filesystem::path m_lastClickedLeft;
        std::filesystem::path m_lastClickedRight;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render();

        void RenderMainMenu();
        void RenderLeft();
        void RenderRight();
        void RenderLeftInfo();
        void RenderRightInfo();
        void RenderMainMenuButtons();

        //-------------------------------------------------
        // App loop
        //-------------------------------------------------

        void AppLoop();
    };
}
