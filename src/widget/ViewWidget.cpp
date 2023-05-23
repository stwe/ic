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
#include "ViewWidget.h"
#include "IcAssert.h"
#include "application/Application.h"
#include "application/Util.h"
#include "data/View.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::ViewWidget::ViewWidget(data::View* t_parentView, std::string t_name)
    : m_parentView{ t_parentView }
    , m_name{ std::move(t_name) }
{
    IC_ASSERT(m_parentView, "[ViewWidget::ViewWidget()] Null pointer.")
    IC_ASSERT(!m_name.empty(), "[ViewWidget::ViewWidget()] Invalid name.")

    IC_LOG_DEBUG("[ViewWidget::ViewWidget()] Create ViewWidget {}.", m_name);
}

ic::widget::ViewWidget::~ViewWidget() noexcept
{
    IC_LOG_DEBUG("[ViewWidget::~ViewWidget()] Destruct ViewWidget.");
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void ic::widget::ViewWidget::SetPosition(const float t_x, const float t_y)
{
    m_posX = t_x;
    m_posY = t_y;
}

void ic::widget::ViewWidget::SetSize(const float t_x, const float t_y)
{
    m_sizeX = t_x;
    m_sizeY = t_y;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::ViewWidget::Render() const
{
    IC_ASSERT(m_posX >= 0 && m_posY >= 0, "[ViewWidget::Render()] Invalid window position.")
    IC_ASSERT(m_sizeX > 0 && m_sizeY > 0, "[ViewWidget::Render()] Invalid window size.")

    ImGui::SetNextWindowPos({ m_posX, m_posY });
    ImGui::SetNextWindowSize({ m_sizeX, m_sizeY });

    ImGui::Begin(
        (std::string("##").append(m_name)).c_str(),
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::NewLine();

    if (ImGui::BeginTable((std::string("##").append(m_name).append("filesTable")).c_str(), 3, ImGuiTableFlags_BordersV))
    {
        RenderHeader();
        RenderFirstRow();
        RenderRows();

        ImGui::EndTable();
    }

    ImGui::End();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void ic::widget::ViewWidget::RenderHeader()
{
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Modify time", ImGuiTableColumnFlags_WidthStretch);

    ImGui::TableHeadersRow();
}

void ic::widget::ViewWidget::RenderFirstRow() const
{
    if (!application::Application::IsRootDirectory(m_parentView->currentPath))
    {
        ImGui::TableNextRow();

        for (auto column{ 0 }; column < 3; ++column)
        {
            ImGui::TableSetColumnIndex(column);

            if (column == 0)
            {
                ImGui::PushID(0);
                if (ImGui::Selectable("/..", false, ImGuiSelectableFlags_AllowDoubleClick) && ImGui::IsMouseDoubleClicked(0))
                {
                    application::Application::event_dispatcher.dispatch(event::IcEventType::UP_DIR, event::UpDirEvent(m_parentView->currentPath));
                }
                ImGui::PopID();
            }

            if (column == 1)
            {
                ImGui::Text("UP--DIR");
            }
        }
    }
}

void ic::widget::ViewWidget::RenderRows() const
{
    auto id{ 1 };
    for (const auto& entry : m_parentView->entries.filesAndDirs)
    {
        ImGui::PushID(id);

        ImGui::TableNextRow();

        for (int column = 0; column < 3; column++)
        {
            ImGui::TableSetColumnIndex(column);

            if (column == 0)
            {
                if (std::filesystem::is_regular_file(entry))
                {
                    RenderFile(entry);
                }
                else if (std::filesystem::is_directory(entry))
                {
                    if (RenderDirectory(entry))
                    {
                        // return if select - m_parentView->entries.filesAndDirs is empty!
                        ImGui::PopID();
                        return;
                    }
                }
            }

            if (column == 1)
            {
                if (std::filesystem::is_regular_file(entry))
                {
                    ImGui::Text("%s", application::Util::GetHumanReadableSize(std::filesystem::file_size(entry)).c_str());
                }
                else
                {
                    ImGui::Text("");
                }
            }

            if (column == 2)
            {
                if (std::filesystem::is_regular_file(entry) || std::filesystem::is_directory(entry))
                {
                    const auto fileTime{ std::filesystem::last_write_time(entry) };
                    ImGui::Text("%s", application::Util::LastWriteTimeToStr(fileTime).c_str());
                }
            }
        }

        ImGui::PopID();
        ++id;
    }
}

//-------------------------------------------------
// Render
//-------------------------------------------------

bool ic::widget::ViewWidget::RenderDirectory(const std::filesystem::path& t_path)
{
    std::string pre = "/";

    if (access(t_path.string().c_str(), R_OK) == 0)
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "~";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
        }
        else if (application::Util::IsHidden(t_path))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // grey
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // white
        }

        if (ImGui::Selectable(pre.append(t_path.filename().string()).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick) && ImGui::IsMouseDoubleClicked(0))
        {
            application::Application::event_dispatcher.dispatch(event::IcEventType::IN_DIR, event::InDirEvent(t_path));
            ImGui::PopStyleColor(1);
            return true;
        }

        ImGui::PopStyleColor(1);
    }
    else
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "~";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // red
        ImGui::TextUnformatted(pre.append(t_path.filename().string()).c_str());
        ImGui::PopStyleColor(1);
    }

    return false;
}

void ic::widget::ViewWidget::RenderFile(const std::filesystem::path& t_path)
{
    std::string pre;
    if (access(t_path.string().c_str(), R_OK) == 0)
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "@";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
        }
        else if (application::Util::IsHidden(t_path))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // grey
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // white
        }

        ImGui::TextUnformatted(pre.append(t_path.filename().string()).c_str());
        ImGui::PopStyleColor(1);
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // red
        ImGui::TextUnformatted(pre.append(t_path.filename().string()).c_str());
        ImGui::PopStyleColor(1);
    }
}