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

#include <string>

namespace ic::data
{
    class View;
}

namespace ic::widget
{
    class InfoWidget
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        InfoWidget() = delete;

        explicit InfoWidget(data::View* t_parentView, std::string t_name);

        InfoWidget(const InfoWidget& t_other) = delete;
        InfoWidget(InfoWidget&& t_other) noexcept = delete;
        InfoWidget& operator=(const InfoWidget& t_other) = delete;
        InfoWidget& operator=(InfoWidget&& t_other) noexcept = delete;

        ~InfoWidget() noexcept;

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
        std::string m_name;

        float m_posX{ -1.0f };
        float m_posY{ -1.0f };
        float m_sizeX{ -1.0f };
        float m_sizeY{ -1.0f };
    };
}
