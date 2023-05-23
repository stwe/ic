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
#include "BottomMenuWidget.h"
#include "IcAssert.h"

//-------------------------------------------------
// Setter
//-------------------------------------------------

void ic::widget::BottomMenuWidget::SetPosition(const float t_x, const float t_y)
{
    m_pos_x = t_x;
    m_pos_y = t_y;
}

void ic::widget::BottomMenuWidget::SetSize(const float t_x, const float t_y)
{
    m_size_x = t_x;
    m_size_y = t_y;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::BottomMenuWidget::Render()
{
    IC_ASSERT(m_pos_x >= 0 && m_pos_y > 0, "[BottomMenuWidget::Render()] Invalid window position.")
    IC_ASSERT(m_size_x > 0 && m_size_y > 0, "[BottomMenuWidget::Render()] Invalid window size.")

    ImGui::SetNextWindowPos({ m_pos_x, m_pos_y });
    ImGui::SetNextWindowSize({ m_size_x, m_size_y });

    ImGui::Begin(
        "##bottom",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    if (ImGui::Button("Help##bottomHelp"))
    {
        ImGui::OpenPopup("##bottomHelpScreen");
    }
    if (ImGui::BeginPopupModal("##bottomHelpScreen", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("This is the main help screen for ic.");
        if (ImGui::Button("OK##bottomOk", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
    ImGui::SameLine();

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    if (ImGui::Button("Menu##bottomMenu")) {}
    ImGui::SameLine();
    if (ImGui::Button("View##bottomView")) {}
    ImGui::SameLine();
    if (ImGui::Button("Edit##bottomEdit")) {}
    ImGui::SameLine();
    if (ImGui::Button("Copy##bottomCopy")) {}
    ImGui::SameLine();
    if (ImGui::Button("Move##bottomMove")) {}
    ImGui::SameLine();
    ImGui::PopItemFlag();

    if (ImGui::Button("MkDir##bottomMkDir"))
    {
        // todo
    }
    ImGui::SameLine();

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    if (ImGui::Button("Delete##bottomDelete")) {}
    ImGui::SameLine();
    if (ImGui::Button("PullDn##bottomPullDn")) {}
    ImGui::SameLine();
    ImGui::PopItemFlag();

    if (ImGui::Button("Quit##bottomQuit"))
    {
        auto event{ std::make_unique<SDL_Event>() };
        event->type = SDL_QUIT;
        SDL_PushEvent(event.release());
    }

    ImGui::End();
}
