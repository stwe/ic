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

#include <imgui_internal.h>
#include <SDL_events.h>
#include <memory>
#include "MainMenuWidget.h"
#include "application/Application.h"
#include "IcAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::MainMenuWidget::MainMenuWidget(data::View* t_parentLeftView, data::View* t_parentRightView)
    : m_parentLeftView{ t_parentLeftView }
    , m_parentRightView{ t_parentRightView }
{
    IC_ASSERT(m_parentLeftView, "[MainMenuWidget::MainMenuWidget()] Null pointer.")
    IC_ASSERT(m_parentRightView, "[MainMenuWidget::MainMenuWidget()] Null pointer.")

    IC_LOG_DEBUG("[MainMenuWidget::MainMenuWidget()] Create MainMenuWidget.");
}

ic::widget::MainMenuWidget::~MainMenuWidget() noexcept
{
    IC_LOG_DEBUG("[MainMenuWidget::~MainMenuWidget()] Destruct MainMenuWidget.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::MainMenuWidget::Render() const
{
    if (ImGui::BeginMainMenuBar())
    {
        // left
        if (ImGui::BeginMenu("Left##mainMenuLeft"))
        {
            if (ImGui::MenuItem("Rescan##mainMenuLeftRescan"))
            {
                application::Application::event_dispatcher.dispatch(
                    event::IcEventType::DIRTY,
                    event::DirtyEvent(m_parentLeftView->currentPath, data::ViewType::LEFT)
                );
            }

            ImGui::EndMenu();
        }

        // file
        if (ImGui::BeginMenu("File##mainMenuFile"))
        {
            if (ImGui::MenuItem("Exit##mainMenuExit"))
            {
                auto event{ std::make_unique<SDL_Event>() };
                event->type = SDL_QUIT;
                SDL_PushEvent(event.release());
            }

            ImGui::EndMenu();
        }

        // right
        if (ImGui::BeginMenu("Right##mainMenuRight"))
        {
            if (ImGui::MenuItem("Rescan##mainMenuRightRescan"))
            {
                application::Application::event_dispatcher.dispatch(
                    event::IcEventType::DIRTY,
                    event::DirtyEvent(m_parentRightView->currentPath, data::ViewType::RIGHT)
                );
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
