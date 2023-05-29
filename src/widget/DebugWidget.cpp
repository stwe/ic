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

#include <imgui.h>
#include "DebugWidget.h"
#include "application/Application.h"
#include "vendor/magic/magic_enum.hpp"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::DebugWidget::Render(const application::Application* t_application)
{
    ImGui::Begin("Debug");

    // FPS

    const auto fps{ static_cast<double>(ImGui::GetIO().Framerate) };
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // red
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0/fps, fps);
    ImGui::PopStyleColor(1);

    ImGui::Separator();

    // Current view type

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
    ImGui::Text("Current view type: %s", std::string(magic_enum::enum_name(application::Application::current_view_type)).c_str());
    ImGui::PopStyleColor(1);

    ImGui::Separator();

    // Left selected

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    ImGui::Text("Left");
    ImGui::PopStyleColor(1);
    for (const auto& entry : t_application->leftView->selectedEntries)
    {
        ImGui::TextUnformatted(entry.filename().string().c_str());
    }

    ImGui::Separator();

    // Right selected

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    ImGui::Text("Right");
    ImGui::PopStyleColor(1);
    for (const auto& entry : t_application->rightView->selectedEntries)
    {
        ImGui::TextUnformatted(entry.filename().string().c_str());
    }

    ImGui::End();
}
