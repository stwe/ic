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
#include "Renderer.h"
#include "Log.h"

void ic::renderer::render_table(
    const std::filesystem::path& t_from,
    const std::set<std::filesystem::path>& t_entries,
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

        if (t_from != std::filesystem::path("/"))
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

                        if (ImGui::Selectable(entry.filename().c_str(), *t_selected == i))
                        {
                            *t_selected = i;
                        }
                    }
                    else if (std::filesystem::is_directory(entry))
                    {
                        if (std::string sl{ "/" }; ImGui::Selectable(sl.append(entry.filename()).c_str(), *t_selected == i))
                        {
                            *t_selected = i;
                        }
                    }
                }
                if (column == 1)
                {
                    if (is_regular_file(entry))
                    {
                        ImGui::Text("%ju", std::filesystem::file_size(entry));
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
                    ImGui::Text("%ju kb", std::filesystem::file_size(t_clickedFile) / 1024);
                }
                else
                {
                    ImGui::Text("%lu mb", size / 1024 / 1024);
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

std::string ic::renderer::last_write_time_to_str(const std::filesystem::file_time_type& t_fileTime)
{
    const std::time_t cftime{ std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(t_fileTime)) };
    const auto tminfo{ localtime(&cftime) };

    return std::to_string(tminfo->tm_mday).append(".").append(std::to_string(tminfo->tm_mon)).append(".").append(std::to_string(tminfo->tm_year + 1900)).
        append(" ").append(std::to_string(tminfo->tm_hour)).append(":").append(std::to_string(tminfo->tm_min));
}
