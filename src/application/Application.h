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

#include <eventpp/eventdispatcher.h>
#include "event/Event.h"
#include "vendor/ini/ini.h"

namespace ic::data
{
    class View;
}

namespace ic::application
{
    class Window;

    class Application
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static const inih::INIReader INI{ "./config.ini" };
        inline static eventpp::EventDispatcher<event::IcEventType, void(const event::IcEvent&)> event_dispatcher;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Application();

        Application(const Application& t_other) = delete;
        Application(Application&& t_other) noexcept = delete;
        Application& operator=(const Application& t_other) = delete;
        Application& operator=(Application&& t_other) noexcept = delete;

        ~Application() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        [[nodiscard]] static bool IsRootDirectory(const std::filesystem::path& t_path);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static std::set<std::filesystem::path> root_paths;

        std::unique_ptr<Window> m_window;
        std::unique_ptr<data::View> m_left;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void HandleSdlEvents(bool& t_running);
        void Update();
        void Render() const;

        //-------------------------------------------------
        // Application loop
        //-------------------------------------------------

        void Loop();
    };
}
