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
#include "application/Util.h"
#include "vendor/magic/magic_enum.hpp"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::widget::InfoWidget::InfoWidget(data::View* t_parentView)
    : m_parentView{ t_parentView }
{
    IC_ASSERT(m_parentView, "[InfoWidget::InfoWidget()] Null pointer.")

    IC_LOG_DEBUG("[InfoWidget::InfoWidget()] Create InfoWidget.");
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
    IC_ASSERT(m_posX >= 0 && m_posY > 0, "[InfoWidget::Render()] Invalid window position.")
    IC_ASSERT(m_sizeX > 0 && m_sizeY > 0, "[InfoWidget::Render()] Invalid window size.")

    ImGui::SetNextWindowPos({ m_posX, m_posY });
    ImGui::SetNextWindowSize({ m_sizeX, m_sizeY });

    const auto name{ std::string(magic_enum::enum_name(m_parentView->viewType)) };

    ImGui::Begin(
        (std::string("##Info").append(name)).c_str(),
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
#if defined(_WIN64) && defined(_MSC_VER)
            ImGui::TextUnformatted(std::string("/").append(application::Util::WstringConv(m_parentView->currentSelectedPath.filename().string())).c_str());
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
            if (std::filesystem::is_symlink(m_parentView->currentSelectedPath))
            {
                ImGui::TextUnformatted(std::string("-> ").append(std::filesystem::read_symlink(m_parentView->currentSelectedPath).string()).c_str());
            }
            else
            {
                ImGui::TextUnformatted(std::string("/").append(m_parentView->currentSelectedPath.filename().string()).c_str());
            }
#endif
        }
        else if (std::filesystem::is_regular_file(m_parentView->currentSelectedPath))
        {
#if defined(_WIN64) && defined(_MSC_VER)
            ImGui::TextUnformatted(application::Util::WstringConv(m_parentView->currentSelectedPath.filename().string()).c_str());
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
            if (std::filesystem::is_symlink(m_parentView->currentSelectedPath))
            {
                ImGui::TextUnformatted(std::string("-> ").append(std::filesystem::read_symlink(m_parentView->currentSelectedPath).string()).c_str());
            }
            else
            {
                ImGui::TextUnformatted(m_parentView->currentSelectedPath.filename().string().c_str());
            }
#endif
        }
    }

    ImGui::End();
}
