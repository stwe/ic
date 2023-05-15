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
#include "FileSystem.h"
#include "vendor/ini/ini.h"

namespace ic
{
    class Window;

    struct PathClick
    {
        int id{ -1 };
        std::filesystem::path path{ std::filesystem::path() };
        bool doubleClick{ false };
    };

    enum class Side
    {
        LEFT, RIGHT, NONE
    };

    class App
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static const inih::INIReader INI{ "./config.ini" };
        inline static std::set<std::filesystem::path> root_paths;

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

        std::filesystem::path m_currentPathLeft{ std::filesystem::current_path() };
        std::filesystem::path m_currentPathRight{ std::filesystem::current_path() };

        PathClick m_lastClickedLeft;
        PathClick m_lastClickedRight;

        std::set<int> m_selectedFileIdsLeft;
        std::set<int> m_selectedFileIdsRight;

        std::set<int> m_selectedDirectoryIdsLeft;
        std::set<int> m_selectedDirectoryIdsRight;

        inline static bool m_render_dialog{ false }; // todo: tmp

        Side m_currentSide{ Side::NONE };
        std::string m_newPathStr; // todo: tmp

        std::set<std::filesystem::path, decltype(fs::path_comparator)*> m_entriesLeft;
        std::set<std::filesystem::path, decltype(fs::path_comparator)*> m_entriesRight;

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
