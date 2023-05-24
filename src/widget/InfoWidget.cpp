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
#include "InfoWidget.h"
#include "IcAssert.h"
#include "data/View.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::InfoWidget::InfoWidget(ic::data::View* t_parentView, std::string t_name)
    : m_parentView{ t_parentView }
    , m_name{ std::move(t_name) }
{
    IC_ASSERT(m_parentView, "[InfoWidget::InfoWidget()] Null pointer.")
    IC_ASSERT(!m_name.empty(), "[InfoWidget::InfoWidget()] Invalid name.")

    IC_LOG_DEBUG("[InfoWidget::InfoWidget()] Create InfoWidget {}.", m_name);
}

ic::widget::InfoWidget::~InfoWidget() noexcept
{
    IC_LOG_DEBUG("[InfoWidget::~InfoWidget()] Destruct InfoWidget.");
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void ic::widget::InfoWidget::SetPosition(const float t_x, const float t_y)
{
    m_posX = t_x;
    m_posY = t_y;
}

void ic::widget::InfoWidget::SetSize(const float t_x, const float t_y)
{
    m_sizeX = t_x;
    m_sizeY = t_y;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::widget::InfoWidget::Render() const
{
    IC_ASSERT(m_posX >= 0 && m_posY >= 0, "[InfoWidget::Render()] Invalid window position.")
    IC_ASSERT(m_sizeX > 0 && m_sizeY > 0, "[InfoWidget::Render()] Invalid window size.")

    ImGui::SetNextWindowPos({ m_posX, m_posY });
    ImGui::SetNextWindowSize({ m_sizeX, m_sizeY });

    ImGui::Begin(
        (std::string("##Info").append(m_name)).c_str(),
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    if (!m_parentView->currentSelectedPath.empty())
    {
        if (std::filesystem::is_directory(m_parentView->currentSelectedPath))
        {
            ImGui::TextUnformatted(std::string("/").append(m_parentView->currentSelectedPath.filename().string()).c_str());
        }
        else
        {
            ImGui::TextUnformatted(m_parentView->currentSelectedPath.filename().string().c_str());
        }
    }

    ImGui::End();
}
