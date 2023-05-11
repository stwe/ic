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

#pragma once

#include <set>
#include <filesystem>
#include <string>

namespace ic
{
    struct PathClick;
}

namespace ic::renderer
{
    //-------------------------------------------------
    // Render
    //-------------------------------------------------

    void render_view(const std::filesystem::path& t_from, PathClick& t_pathClick);

    void render_header();
    void render_first_row(const std::filesystem::path& t_path, PathClick& t_pathClick);
    void render_file(const std::filesystem::path& t_path, bool* t_selected, PathClick& t_pathClick, int t_id);
    void render_directory(const std::filesystem::path& t_path, bool* t_selected, PathClick& t_pathClick, int t_id);

    void render_clicked_path_info(const PathClick& t_pathClick);

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void add_selectable_field(const char* t_label, bool* t_selected, PathClick& t_pathClick, const std::filesystem::path& t_path, int t_id);
    std::string to_zero_lead(const std::string& t_time);
    std::string last_write_time_to_str(const std::filesystem::file_time_type& t_fileTime);
    std::string get_human_readable_size(unsigned long t_bytes);
    std::string wstring_conv(const std::filesystem::path& t_path);
    std::string float_to_string(float t_val);
}
