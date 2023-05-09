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

#if defined(_WIN64) && defined(_MSC_VER)
    #include <io.h>
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    #include <unistd.h>
#else
    #error Unsupported platform or compiler!
#endif

#include "FileSystem.h"
#include "Renderer.h"
#include "App.h"
#include "Window.h"

//-------------------------------------------------
// Render
//-------------------------------------------------

void ic::renderer::render_view(
    const std::filesystem::path& t_from,
    const std::set<std::filesystem::path, decltype(ic::fs::path_comparator)*>& t_entries,
    PathClick& t_pathClick
)
{
    if (ImGui::BeginTable("##filesTable", 3, ImGuiTableFlags_BordersV))
    {
        render_header();
        render_first_row(t_from, t_pathClick);

        int id{ 1 };
        bool click{ false };

        for (const auto& entry : t_entries)
        {
            ImGui::PushID(id);

            ImGui::TableNextRow();

            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);

                if (column == 0)
                {
                    if (is_regular_file(entry))
                    {
                        render_files(entry, &click, t_pathClick, id);
                    }
                    else if (std::filesystem::is_directory(entry))
                    {
                        render_directories(entry, &click, t_pathClick, id);
                    }
                }

                if (column == 1)
                {
                    if (is_regular_file(entry))
                    {
                        ImGui::Text("%s", get_human_readable_size(static_cast<unsigned long>(std::filesystem::file_size(entry))).c_str());
                    }
                    else
                    {
                        ImGui::Text("");
                    }
                }

                if (column == 2)
                {
                    const auto fileTime{ std::filesystem::last_write_time(entry) };
                    ImGui::Text("%s", last_write_time_to_str(fileTime).c_str());
                }
            }

            ImGui::PopID();
            ++id;
        }

        ImGui::EndTable();
    }
}

void ic::renderer::render_header()
{
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Modify time", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();
}

void ic::renderer::render_first_row(const std::filesystem::path& t_path, PathClick& t_pathClick)
{
    if (fs::is_root(t_path))
    {
        ImGui::TableNextRow();

        for (int column = 0; column < 3; column++)
        {
            ImGui::TableSetColumnIndex(column);

            if (column == 0)
            {
                auto click{ t_pathClick.id == 0 };
                add_selectable_field("/..", &click, t_pathClick, std::filesystem::path(), 0);
            }

            if (column == 1)
            {
                ImGui::Text("UP--DIR");
            }
        }
    }
}

void ic::renderer::render_files(const std::filesystem::path& t_path, bool* t_selected, PathClick& t_pathClick, const int t_id)
{
#if defined(_WIN64) && defined(_MSC_VER)
    *t_selected = (t_pathClick.id == t_id);
    add_selectable_field(t_path.filename().string().c_str(), t_selected, t_pathClick, t_path, t_id);
    // todo: symlinks
    // todo: access
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    // todo: access for symlinks + pre
    std::string pre;
    if (access(entry.string().c_str(), R_OK) == 0) // todo
    {
        if (std::filesystem::is_symlink(entry))
        {
            pre = "@";
        }
        if (ImGui::Selectable(pre.append(entry.filename().string()).c_str(), t_pathClick.id == i, ImGuiSelectableFlags_AllowDoubleClick))
        {
            t_pathClick.id = i;
            t_pathClick.path = entry;
            t_pathClick.doubleClick = ImGui::IsMouseDoubleClicked(0);
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Window::warn_color);
        ImGui::TextUnformatted(pre.append(entry.filename().string()).c_str());
        ImGui::PopStyleColor(1);
    }
#else
    #error Unsupported platform or compiler!
#endif
}

void ic::renderer::render_directories(const std::filesystem::path& t_path, bool* t_selected, ic::PathClick& t_pathClick, const int t_id)
{
    std::string pre = "/";
#if defined(_WIN64) && defined(_MSC_VER)
    if (_access(t_path.string().c_str(), 4) == 0)
    {
        auto test{ false };

        try
        {
            if (std::filesystem::is_symlink(t_path))
            {}
        }
        catch (std::filesystem::filesystem_error& e)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, Window::warn_color);
            ImGui::TextUnformatted(pre.append(t_path.filename().string()).c_str());
            ImGui::PopStyleColor(1);
            test = true;
        }

        if (!test)
        {
            *t_selected = (t_pathClick.id == t_id);
            add_selectable_field(pre.append(t_path.filename().string()).c_str(), t_selected, t_pathClick, t_path, t_id);
        }
    }
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    if (access(entry.string().c_str(), R_OK) == 0) // todo
    {
        if (std::filesystem::is_symlink(entry))
        {
            pre = "~";
        }
        if (ImGui::Selectable(pre.append(entry.filename().string()).c_str(), t_pathClick.id == i, ImGuiSelectableFlags_AllowDoubleClick))
        {
            t_pathClick.id  = i;
            t_pathClick.path = entry;
            t_pathClick.doubleClick = ImGui::IsMouseDoubleClicked(0);
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Window::warn_color);
        ImGui::TextUnformatted(pre.append(entry.filename().string()).c_str());
        ImGui::PopStyleColor(1);
    }
#else
    #error Unsupported platform or compiler!
#endif
}

void ic::renderer::render_clicked_path_info(const PathClick& t_pathClick)
{
    if (!t_pathClick.path.empty() && std::filesystem::is_regular_file(t_pathClick.path))
    {
        if (std::filesystem::is_symlink(t_pathClick.path))
        {
            ImGui::Text("%s", std::string("-> ").append(std::filesystem::read_symlink(t_pathClick.path).string()).c_str());
        }
        else
        {
            ImGui::Text("%s", t_pathClick.path.filename().string().c_str());
            ImGui::SameLine();
            ImGui::Text("%s", get_human_readable_size(static_cast<unsigned long>(std::filesystem::file_size(t_pathClick.path))).c_str());
        }
    }
    else if (!t_pathClick.path.empty() && std::filesystem::is_directory(t_pathClick.path))
    {
        if (std::filesystem::is_symlink(t_pathClick.path))
        {
            ImGui::Text("%s", std::string("-> ").append(std::filesystem::read_symlink(t_pathClick.path).string()).c_str());
        }
        else
        {
            ImGui::Text("%s", std::string("/").append(t_pathClick.path.filename().string()).c_str());
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void ic::renderer::add_selectable_field(const char* t_label, bool* t_selected, PathClick& t_pathClick, const std::filesystem::path& t_path, int t_id)
{
    if (ImGui::Selectable(t_label, t_selected, ImGuiSelectableFlags_AllowDoubleClick))
    {
        t_pathClick.id = t_id;
        t_pathClick.path = t_path;
        t_pathClick.doubleClick = ImGui::IsMouseDoubleClicked(0);
    }
}

std::string ic::renderer::to_zero_lead(const std::string& t_time)
{
    return std::string(2 - std::min(2, static_cast<int>(t_time.length())), '0') + t_time;
}

std::string ic::renderer::last_write_time_to_str(const std::filesystem::file_time_type& t_fileTime)
{
    std::chrono::time_point<std::chrono::system_clock> ftsys;

#if defined(_WIN64) && defined(_MSC_VER)
    ftsys = std::chrono::utc_clock::to_sys(std::chrono::file_clock::to_utc(t_fileTime));
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    ftsys = std::chrono::file_clock::to_sys(t_fileTime);
#else
    #error Unsupported platform or compiler!
#endif

    time_t ftt{ std::chrono::system_clock::to_time_t(ftsys) };
    const auto tminfo{ localtime(&ftt) };

    return
        to_zero_lead(std::to_string(tminfo->tm_mday)).
        append(".").
        append(to_zero_lead(std::to_string(tminfo->tm_mon))).
        append(".").
        append(std::to_string(tminfo->tm_year + 1900)).
        append(" ").
        append(to_zero_lead(std::to_string(tminfo->tm_hour))).
        append(":").
        append(to_zero_lead(std::to_string(tminfo->tm_min)));
}

std::string ic::renderer::get_human_readable_size(unsigned long t_bytes)
{
    static constexpr float gb{ 1024.0f * 1024.0f * 1024.0f };
    static constexpr float mb{ 1024.0f * 1024.0f };
    static constexpr float kb{ 1024.0f };

    if(auto bytes{ static_cast<float>(t_bytes) }; bytes > gb)
    {
        return std::to_string(bytes / gb).append(" Gb ");
    }
    else if(bytes > mb)
    {
        return std::to_string(bytes / mb).append(" Mb ");
    }
    else if(bytes > kb)
    {
        return std::to_string(bytes / kb).append(" Kb ");
    }

    return std::to_string(t_bytes).append(" B ");
}
