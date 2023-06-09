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

#include "ViewWidget.h"
#include "IcAssert.h"
#include "application/Application.h"
#include "application/Util.h"
#include "vendor/magic/magic_enum.hpp"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::ViewWidget::ViewWidget(data::View* t_parentView)
    : m_parentView{ t_parentView }
{
    IC_ASSERT(m_parentView, "[ViewWidget::ViewWidget()] Null pointer.")

    IC_LOG_DEBUG("[ViewWidget::ViewWidget()] Create ViewWidget.");
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

    const auto name{ std::string(magic_enum::enum_name(m_parentView->viewType)) };

    ImGui::Begin(
        (std::string("##View").append(name)).c_str(),
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

#if defined(_WIN64) && defined(_MSC_VER)
    RenderDriveLetters();
#endif

    ImGui::NewLine();

    ImGui::TextUnformatted(m_parentView->currentPath.string().c_str());

    if (ImGui::BeginTable((std::string("##").append(name).append("filesTable")).c_str(), 3, ImGuiTableFlags_BordersV))
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

void ic::widget::ViewWidget::RenderHeader() const
{
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Modify time", ImGuiTableColumnFlags_WidthStretch);

    ImGuiStyle& style = ImGui::GetStyle();
    if (m_parentView->viewType == application::Application::current_view_type)
    {
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7f, 0.5f, 1.0f, 1.0f); // purple
    }
    else
    {
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.3f, 0.4f, 0.8f, 1.0f); // blue
    }

    ImGui::TableHeadersRow();
}

void ic::widget::ViewWidget::RenderFirstRow() const
{
    if (!application::Util::IsRootDirectory(m_parentView->currentPath))
    {
        ImGui::TableNextRow();

        for (auto column{ 0 }; column < 3; ++column)
        {
            ImGui::TableSetColumnIndex(column);

            if (column == 0)
            {
                ImGui::PushID(0);
                if (ImGui::Selectable("/..", false, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        application::Application::event_dispatcher.dispatch(
                            event::IcEventType::UP_DIR,
                            event::UpDirEvent(m_parentView->currentPath, m_parentView->viewType)
                        );
                    }
                    else
                    {
                        application::Application::current_view_type = m_parentView->viewType;
                    }
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

#if defined(_WIN64) && defined(_MSC_VER)
void ic::widget::ViewWidget::RenderDriveLetters() const
{
    for (const auto& drive : application::Util::GetAvailableDriveLetters())
    {
        if (std::string label(1, drive); ImGui::Button(label.c_str()))
        {
            application::Application::event_dispatcher.dispatch(
                event::IcEventType::CHANGE_ROOT_PATH,
                event::ChangeRootPathEvent(std::filesystem::path(label.append(":\\")), m_parentView->viewType)
            );
        }
        ImGui::SameLine();
    }
}
#endif

//-------------------------------------------------
// Render
//-------------------------------------------------

bool ic::widget::ViewWidget::RenderDirectory(const std::filesystem::path& t_path) const
{
    std::string pre = "/";

#if defined(_WIN64) && defined(_MSC_VER)
    if (!application::Util::IsAccessDenied(t_path.wstring()) && !application::Util::IsJunctionDirectory(t_path.wstring()))
    {
        if (application::Util::IsHiddenDirectory(t_path.wstring()))
        {
            PushHiddenColor(t_path);
        }
        else
        {
            PushDefaultColor(t_path);
        }

        if (ImGui::Selectable(pre.append(application::Util::WstringConv(t_path)).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            return DirectoryDispatchEvents(t_path);
        }

        ImGui::PopStyleColor(1);
    }
    else
    {
        if(application::Util::IsJunctionDirectory(t_path.wstring()))
        {
            pre = "~";
            RenderAccessDenied(pre.append(application::Util::WstringConv(t_path)).c_str(), ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
        }
        else
        {
            RenderAccessDenied(pre.append(application::Util::WstringConv(t_path)).c_str(), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    if (access(t_path.string().c_str(), R_OK) == 0)
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "~";
            PushSymlinkColor(t_path);
        }
        else if (application::Util::IsHidden(t_path))
        {
            PushHiddenColor(t_path);
        }
        else
        {
            PushDefaultColor(t_path);
        }

        if (ImGui::Selectable(pre.append(t_path.filename().string()).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            return DirectoryDispatchEvents(t_path);
        }

        ImGui::PopStyleColor(1);
    }
    else
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "~";
        }

        RenderAccessDenied(t_path, pre);
    }
#endif

    return false;
}

void ic::widget::ViewWidget::RenderFile(const std::filesystem::path& t_path) const
{
#if defined(_WIN64) && defined(_MSC_VER)
    PushDefaultColor(t_path);

    if (ImGui::Selectable(application::Util::WstringConv(t_path).c_str(), false))
    {
        FileDispatchEvents(t_path);
    }

    ImGui::PopStyleColor(1);
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    std::string pre;
    if (access(t_path.string().c_str(), R_OK) == 0)
    {
        if (std::filesystem::is_symlink(t_path))
        {
            pre = "@";
            PushSymlinkColor(t_path);
        }
        else if (application::Util::IsHidden(t_path))
        {
            PushHiddenColor(t_path);
        }
        else
        {
            PushDefaultColor(t_path);
        }

        if (ImGui::Selectable(pre.append(t_path.filename().string()).c_str(), false))
        {
            FileDispatchEvents(t_path);
        }

        ImGui::PopStyleColor(1);
    }
    else
    {
        RenderAccessDenied(t_path, pre);
    }
#endif
}

void ic::widget::ViewWidget::PushSymlinkColor(const std::filesystem::path& t_path) const
{
    if (m_parentView->selectedEntries.contains(t_path))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
    }
}

void ic::widget::ViewWidget::PushHiddenColor(const std::filesystem::path& t_path) const
{
    if (m_parentView->selectedEntries.contains(t_path))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // grey
    }
}

void ic::widget::ViewWidget::PushDefaultColor(const std::filesystem::path& t_path) const
{
    if (m_parentView->selectedEntries.contains(t_path))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // white
    }
}

bool ic::widget::ViewWidget::DirectoryDispatchEvents(const std::filesystem::path& t_path) const
{
    // double click
    if (ImGui::IsMouseDoubleClicked(0))
    {
        application::Application::event_dispatcher.dispatch(
            event::IcEventType::IN_DIR,
            event::InDirEvent(t_path, m_parentView->viewType)
        );

        ImGui::PopStyleColor(1);

        return true;
    }

    // single click
    application::Application::event_dispatcher.dispatch(
        event::IcEventType::SHOW_PATH_INFO,
        event::ShowPathInfoEvent(t_path, m_parentView->viewType)
    );

    if (ImGui::GetIO().KeyShift)
    {
        application::Application::event_dispatcher.dispatch(
            event::IcEventType::SELECT_PATH,
            event::SelectPathEvent(t_path, m_parentView->viewType)
        );
    }

    ImGui::PopStyleColor(1);

    return false;
}

void ic::widget::ViewWidget::FileDispatchEvents(const std::filesystem::path& t_path) const
{
    application::Application::event_dispatcher.dispatch(
        event::IcEventType::SHOW_PATH_INFO,
        event::ShowPathInfoEvent(t_path, m_parentView->viewType)
    );

    if (ImGui::GetIO().KeyShift)
    {
        application::Application::event_dispatcher.dispatch(
            event::IcEventType::SELECT_PATH,
            event::SelectPathEvent(t_path, m_parentView->viewType)
        );
    }
}

void ic::widget::ViewWidget::RenderAccessDenied(const std::filesystem::path& t_path, const std::string& t_prefix)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // red
    ImGui::TextUnformatted(std::string(t_prefix).append(t_path.filename().string()).c_str());
    ImGui::PopStyleColor(1);
}

void ic::widget::ViewWidget::RenderAccessDenied(const char* t_pathStr, ImVec4 t_color)
{
    ImGui::PushStyleColor(ImGuiCol_Text, t_color);
    ImGui::TextUnformatted(t_pathStr);
    ImGui::PopStyleColor(1);
}
