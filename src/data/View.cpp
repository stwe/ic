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

#include <eventpp/utilities/argumentadapter.h>
#include "View.h"
#include "IcAssert.h"
#include "application/Application.h"
#include "widget/ViewWidget.h"
#include "widget/InfoWidget.h"
#include "vendor/magic/magic_enum.hpp"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::data::View::View(ViewType t_viewType)
    : viewType{ t_viewType }
{
    IC_ASSERT(viewType != ViewType::NONE, "[View::View()] Invalid view type.")

    IC_LOG_DEBUG("[View::View()] Create View. The type is {}.", std::string(magic_enum::enum_name(viewType)));

    m_viewWidget = std::make_unique<widget::ViewWidget>(this);
    m_infoWidget = std::make_unique<widget::InfoWidget>(this);

    AppendListeners();

    dirty = true; // todo read event
}

ic::data::View::~View() noexcept
{
    IC_LOG_DEBUG("[View::~View()] Destruct View.");
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void ic::data::View::SetPosition(const float t_x, const float t_y) const
{
    m_viewWidget->SetPosition(t_x, t_y);
}

void ic::data::View::SetSize(const float t_x, const float t_y) const
{
    m_viewWidget->SetSize(t_x, t_y);
}

void ic::data::View::SetInfoPosition(const float t_x, const float t_y) const
{
    m_infoWidget->SetPosition(t_x, t_y);
}

void ic::data::View::SetInfoSize(const float t_x, const float t_y) const
{
    m_infoWidget->SetSize(t_x, t_y);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::data::View::Update()
{
    if (dirty)
    {
        for (const auto& entry : std::filesystem::directory_iterator(currentPath))
        {
            entries.filesAndDirs.emplace(entry.path());
        }

        dirty = false;
    }
}

void ic::data::View::Render() const
{
    m_viewWidget->Render();
    m_infoWidget->Render();
}

//-------------------------------------------------
// Listeners
//-------------------------------------------------

void ic::data::View::AppendListeners()
{
    application::Application::event_dispatcher.appendListener(
        event::IcEventType::UP_DIR,
        eventpp::argumentAdapter<void(const event::UpDirEvent&)>(
            [this](const event::UpDirEvent& t_event) {
                if (!t_event.path.empty() && t_event.viewType == viewType)
                {
                    currentPath = t_event.path.parent_path();
                    currentSelectedPath.clear();
                    entries.filesAndDirs.clear();
                    dirty = true;
                    application::Application::current_view_type = viewType;
                    IC_LOG_DEBUG("[View::AppendListeners()] Event type UP_DIR for view type {}.", std::string(magic_enum::enum_name(viewType)));
                }
            })
    );

    application::Application::event_dispatcher.appendListener(
        event::IcEventType::IN_DIR,
        eventpp::argumentAdapter<void(const event::InDirEvent&)>(
            [this](const event::InDirEvent& t_event) {
                if (!t_event.path.empty() && t_event.viewType == viewType)
                {
                    currentPath = t_event.path;
                    currentSelectedPath.clear();
                    entries.filesAndDirs.clear();
                    dirty = true;
                    application::Application::current_view_type = viewType;
                    IC_LOG_DEBUG("[View::AppendListeners()] Event type IN_DIR for view type {}.", std::string(magic_enum::enum_name(viewType)));
                }
            })
    );

    application::Application::event_dispatcher.appendListener(
        event::IcEventType::SELECT_PATH,
        eventpp::argumentAdapter<void(const event::SelectPathEvent&)>(
            [this](const event::SelectPathEvent& t_event) {
                if (!t_event.path.empty() && t_event.viewType == viewType)
                {
                    currentSelectedPath = t_event.path;
                    application::Application::current_view_type = viewType;
                    IC_LOG_DEBUG("[View::AppendListeners()] Event type SELECT_PATH for view type {}.", std::string(magic_enum::enum_name(viewType)));
                }
            })
    );

    application::Application::event_dispatcher.appendListener(
        event::IcEventType::CHANGE_ROOT_PATH,
        eventpp::argumentAdapter<void(const event::ChangeRootPathEvent&)>(
            [this](const event::ChangeRootPathEvent& t_event) {
                if (!t_event.path.empty() && t_event.viewType == viewType)
                {
                    currentPath = t_event.path.parent_path();
                    currentSelectedPath.clear();
                    entries.filesAndDirs.clear();
                    dirty = true;
                    application::Application::current_view_type = viewType;
                    IC_LOG_DEBUG("[View::AppendListeners()] Event type CHANGE_ROOT_PATH for view type {}.", std::string(magic_enum::enum_name(viewType)));
                }
            })
    );
}
