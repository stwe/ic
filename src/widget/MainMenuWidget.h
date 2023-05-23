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

namespace ic::widget
{
    class MainMenuWidget
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MainMenuWidget(const MainMenuWidget& t_other) = delete;
        MainMenuWidget(MainMenuWidget&& t_other) noexcept = delete;
        MainMenuWidget& operator=(const MainMenuWidget& t_other) = delete;
        MainMenuWidget& operator=(MainMenuWidget&& t_other) noexcept = delete;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        static void Render();

    protected:

    private:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MainMenuWidget() = default;
        ~MainMenuWidget() noexcept = default;
    };
}