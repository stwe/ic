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

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::MainMenuWidget::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
        // left
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        if (ImGui::BeginMenu("Left##mainMenuLeft"))
        {
            ImGui::EndMenu();
        }
        ImGui::PopItemFlag();

        // file
        if (ImGui::BeginMenu("File##mainMenuFile"))
        {
            if (ImGui::MenuItem("Exit", "F10"))
            {
                auto event{ std::make_unique<SDL_Event>() };
                event->type = SDL_QUIT;
                SDL_PushEvent(event.release());
            }
            ImGui::EndMenu();
        }

        // right
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        if (ImGui::BeginMenu("Right##mainMenuRight"))
        {
            ImGui::EndMenu();
        }
        ImGui::PopItemFlag();

        ImGui::EndMainMenuBar();
    }
}
