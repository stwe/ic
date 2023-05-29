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
#include <filesystem>
#include "BottomMenuWidget.h"
#include "IcAssert.h"
#include "application/Application.h"
#include "vendor/imgui/imgui_stdlib.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::BottomMenuWidget::BottomMenuWidget(data::View* t_parentLeftView, data::View* t_parentRightView)
    : m_parentLeftView{ t_parentLeftView }
    , m_parentRightView{ t_parentRightView }
{
    IC_ASSERT(m_parentLeftView, "[BottomMenuWidget::BottomMenuWidget()] Null pointer.")
    IC_ASSERT(m_parentRightView, "[BottomMenuWidget::BottomMenuWidget()] Null pointer.")

    IC_LOG_DEBUG("[BottomMenuWidget::BottomMenuWidget()] Create BottomMenuWidget.");
}

ic::widget::BottomMenuWidget::~BottomMenuWidget() noexcept
{
    IC_LOG_DEBUG("[BottomMenuWidget::~BottomMenuWidget()] Destruct BottomMenuWidget.");
}

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
        ImGui::Text("Shift + Left mouse button to select file or directory.");
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
        ImGui::OpenPopup("##bottomModalMkDir");
    }
    if (ImGui::BeginPopupModal("##bottomModalMkDir", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Create a new directory.");

        static std::string newPathStr;
        ImGui::InputText("##bottomModalMkDirNewPath", &newPathStr);

        // close
        if (ImGui::Button("Close##bottomModalMkDirClose", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if(ImGui::Button("Create##bottomModalMkDirCreate"))
        {
            // todo: event

            std::filesystem::path p;
            if (application::Application::current_view_type == data::ViewType::LEFT)
            {
                p = m_parentLeftView->currentPath / newPathStr;
            }
            else
            {
                p = m_parentRightView->currentPath / newPathStr;
            }

            if (!std::filesystem::exists(p))
            {
                if (std::filesystem::create_directory(p))
                {
                    IC_LOG_DEBUG("[BottomMenuWidget::Render()] Directory {} created successfully.", p.filename().string());

                    application::Application::event_dispatcher.dispatch(
                        event::IcEventType::DIRTY,
                        event::DirtyEvent(m_parentLeftView->currentPath, data::ViewType::LEFT)
                    );

                    application::Application::event_dispatcher.dispatch(
                        event::IcEventType::DIRTY,
                        event::DirtyEvent(m_parentRightView->currentPath, data::ViewType::RIGHT)
                    );

                    newPathStr.clear();
                }
                else
                {
                    IC_LOG_WARN("[BottomMenuWidget::Render()] Failed to create directory {}.", p.filename().string());
                }
            }
            else
            {
                IC_LOG_WARN("[BottomMenuWidget::Render()] Directory {} already exists.", p.filename().string());
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
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
