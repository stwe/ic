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

#include <memory>
#include <unordered_set>
#include "Entries.h"
#include "Comparator.h"

namespace ic::widget
{
    class ViewWidget;
    class InfoWidget;
}

namespace ic::data
{
    enum class ViewType
    {
        LEFT, RIGHT, NONE
    };

    class View
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The type of this View.
         */
        ViewType viewType{ ViewType::NONE };

        /**
         * Files and directories relative to the currentPath.
         */
        Entries<PathComparator> entries;

        /**
         * Path to the current working directory.
         */
        std::filesystem::path currentPath{ std::filesystem::current_path() };

        /**
         * The file or directory currently selected with a single mouse click.
         */
        std::filesystem::path currentSelectedPath;

        /**
         * Is set when the entries need to be updated.
         */
        bool dirty{ false };

        /**
         * All entries that were selected with the mouse.
         */
        std::unordered_set<std::filesystem::path> selectedEntries;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        View() = delete;

        explicit View(ViewType t_viewType);

        View(const View& t_other) = delete;
        View(View&& t_other) noexcept = delete;
        View& operator=(const View& t_other) = delete;
        View& operator=(View&& t_other) noexcept = delete;

        ~View() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPosition(float t_x, float t_y) const;
        void SetSize(float t_x, float t_y) const;
        void SetInfoPosition(float t_x, float t_y) const;
        void SetInfoSize(float t_x, float t_y) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * To render the entries.
         */
        std::unique_ptr<widget::ViewWidget> m_viewWidget;

        /**
         * To render information about the entries.
         */
        std::unique_ptr<widget::InfoWidget> m_infoWidget;

        //-------------------------------------------------
        // Listeners
        //-------------------------------------------------

        void AppendListeners();
    };
}
