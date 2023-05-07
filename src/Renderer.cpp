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
#include <algorithm>
#include <unistd.h> // todo if linux
#include "FileSystem.h"
#include "Renderer.h"
#include "Log.h"
#include "Window.h"

void ic::renderer::render_table(
    const std::filesystem::path& t_from,
    const std::set<std::filesystem::path, decltype(ic::fs::path_comparator)*>& t_entries,
    int* t_selected,
    const std::filesystem::path& t_clickedFile
)
{
    unsigned long size{ 0 };

    if (ImGui::BeginTable("##table", 3, ImGuiTableFlags_BordersV))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Modify time", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        if (fs::is_root(t_from))
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);
                if (column == 0)
                {
                    if (ImGui::Selectable("/..", *t_selected == 0))
                    {
                        *t_selected = 0;
                    }
                }
                if (column == 1)
                {
                    ImGui::Text("UP--DIR");
                }
            }
        }

        int i{ 1 };
        for (const auto& entry : t_entries) // row
        {
            ImGui::PushID(i);

            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);

                if (column == 0)
                {
                    if (is_regular_file(entry))
                    {
                        size += std::filesystem::file_size(entry);

                        if (access(entry.string().c_str(), R_OK) == 0) // todo
                        {
                            if (ImGui::Selectable(entry.filename().string().c_str(), *t_selected == i))
                            {
                                *t_selected = i;
                            }
                        }
                        else
                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, Window::WARN_COLOR);
                            ImGui::TextUnformatted(entry.filename().string().c_str());
                            ImGui::PopStyleColor(1);
                        }
                    }
                    else if (std::filesystem::is_directory(entry))
                    {
                        std::string pre = "/";
                        if (access(entry.string().c_str(), R_OK) == 0) // todo
                        {
                            if (std::filesystem::is_symlink(entry))
                            {
                                pre = "~";
                            }
                            if (ImGui::Selectable(pre.append(entry.filename().string()).c_str(), *t_selected == i))
                            {
                                *t_selected = i;
                            }
                        }
                        else
                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, Window::WARN_COLOR);
                            ImGui::TextUnformatted(pre.append(entry.filename().string()).c_str());
                            ImGui::PopStyleColor(1);
                        }
                    }
                }
                if (column == 1)
                {
                    if (is_regular_file(entry))
                    {
                        ImGui::Text("%s", get_human_readable_size(std::filesystem::file_size(entry)).c_str());
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
            ++i;
        }

        ImGui::TableNextRow();
        for (int column = 0; column < 3; column++)
        {
            ImGui::TableSetColumnIndex(column);
            if (column == 0)
            {
                ImGui::Separator();
                if (!t_clickedFile.empty())
                {
                    ImGui::Text("%s", t_clickedFile.filename().string().c_str());
                }
            }
            if (column == 1)
            {
                ImGui::Separator();
                if (!t_clickedFile.empty())
                {
                    ImGui::Text("%s", get_human_readable_size(std::filesystem::file_size(t_clickedFile)).c_str());
                }
                else
                {
                    ImGui::Text("%s", get_human_readable_size(size).c_str());
                }
            }
            if (column == 2)
            {
                ImGui::Separator();
                ImGui::Text("");
            }
        }

        ImGui::EndTable();
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
#else
    ftsys = std::chrono::file_clock::to_sys(t_fileTime);
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
