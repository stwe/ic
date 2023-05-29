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

namespace ic::data
{
    class View;
}

namespace ic::widget
{
    class BottomMenuWidget
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BottomMenuWidget() = delete;

        explicit BottomMenuWidget(data::View* t_parentLeftView, data::View* t_parentRightView);

        BottomMenuWidget(const BottomMenuWidget& t_other) = delete;
        BottomMenuWidget(BottomMenuWidget&& t_other) noexcept = delete;
        BottomMenuWidget& operator=(const BottomMenuWidget& t_other) = delete;
        BottomMenuWidget& operator=(BottomMenuWidget&& t_other) noexcept = delete;

        ~BottomMenuWidget() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPosition(float t_x, float t_y);
        void SetSize(float t_x, float t_y);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        data::View* m_parentLeftView{ nullptr };
        data::View* m_parentRightView{ nullptr };

        float m_pos_x{ -1.0f };
        float m_pos_y{ -1.0f };
        float m_size_x{ -1.0f };
        float m_size_y{ -1.0f };
    };
}
