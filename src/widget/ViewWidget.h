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

#include <imgui.h>
#include <filesystem>

namespace ic::data
{
    class View;
}

namespace ic::widget
{
    class ViewWidget
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ViewWidget() = delete;

        explicit ViewWidget(data::View* t_parentView);

        ViewWidget(const ViewWidget& t_other) = delete;
        ViewWidget(ViewWidget&& t_other) noexcept = delete;
        ViewWidget& operator=(const ViewWidget& t_other) = delete;
        ViewWidget& operator=(ViewWidget&& t_other) noexcept = delete;

        ~ViewWidget() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPosition(float t_x, float t_y);
        void SetSize(float t_x, float t_y);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        data::View* m_parentView{ nullptr };

        float m_posX{ -1.0f };
        float m_posY{ -1.0f };
        float m_sizeX{ -1.0f };
        float m_sizeY{ -1.0f };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void RenderHeader();
        void RenderFirstRow() const;
        void RenderRows() const;

#if defined(_WIN64) && defined(_MSC_VER)
        void RenderDriveLetters() const;
#endif

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        [[nodiscard]] bool RenderDirectory(const std::filesystem::path& t_path) const;
        void RenderFile(const std::filesystem::path& t_path) const;

        void PushSymlinkColor(const std::filesystem::path& t_path) const;
        void PushHiddenColor(const std::filesystem::path& t_path) const;
        void PushDefaultColor(const std::filesystem::path& t_path) const;

        [[nodiscard]] bool DirectoryDispatchEvents(const std::filesystem::path& t_path) const;
        void FileDispatchEvents(const std::filesystem::path& t_path) const;

        static void RenderAccessDenied(const std::filesystem::path& t_path, const std::string& t_prefix);
        static void RenderAccessDenied(const char* t_pathStr, ImVec4 t_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    };
}
