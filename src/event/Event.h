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
#include "data/View.h"

namespace ic::event
{
    //-------------------------------------------------
    // Event types
    //-------------------------------------------------

    enum class IcEventType
    {
        // default
        NONE,

        // events
        UP_DIR,
        IN_DIR,
        SELECT_PATH,
        CHANGE_ROOT_PATH,
        DIRTY
    };

    //-------------------------------------------------
    // Event base class
    //-------------------------------------------------

    struct IcEvent
    {
        std::filesystem::path path;
        data::ViewType viewType{ data::ViewType::NONE };
        IcEventType eventType{ IcEventType::NONE };

        IcEvent() = default;

        IcEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : path{ std::move(t_path) }
            , viewType{ t_viewType }
        {}

        virtual ~IcEvent() noexcept = default;
    };

    //-------------------------------------------------
    // Events
    //-------------------------------------------------

    struct UpDirEvent: IcEvent
    {
        UpDirEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : IcEvent(std::move(t_path), t_viewType)
        {
            eventType = IcEventType::UP_DIR;
        }
    };

    struct InDirEvent: IcEvent
    {
        InDirEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : IcEvent(std::move(t_path), t_viewType)
        {
            eventType = IcEventType::IN_DIR;
        }
    };

    struct SelectPathEvent: IcEvent
    {
        SelectPathEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : IcEvent(std::move(t_path), t_viewType)
        {
            eventType = IcEventType::SELECT_PATH;
        }
    };

    struct ChangeRootPathEvent: IcEvent
    {
        ChangeRootPathEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : IcEvent(std::move(t_path), t_viewType)
        {
            eventType = IcEventType::CHANGE_ROOT_PATH;
        }
    };

    struct DirtyEvent: IcEvent
    {
        explicit DirtyEvent(data::ViewType t_viewType)
            : IcEvent(std::filesystem::current_path(), t_viewType)
        {
            eventType = IcEventType::DIRTY;
        }

        DirtyEvent(std::filesystem::path t_path, data::ViewType t_viewType)
            : IcEvent(std::move(t_path), t_viewType)
        {
            eventType = IcEventType::DIRTY;
        }
    };
}
