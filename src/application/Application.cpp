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

#include "Application.h"
#include "IcAssert.h"
#include "Window.h"
#include "Util.h"
#include "data/View.h"
#include "widget/MainMenuWidget.h"
#include "widget/BottomMenuWidget.h"
#include "vendor/imgui/imgui_impl_sdl2.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::application::Application::Application()
{
    IC_LOG_DEBUG("[Application::Application()] Create Application.");

    current_view_type = data::ViewType::NONE;
}

ic::application::Application::~Application() noexcept
{
    IC_LOG_DEBUG("[Application::~Application()] Destruct Application.");
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void ic::application::Application::Run()
{
    IC_LOG_DEBUG("[Application::Run()] Starts running application.");

    Init();
    Loop();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool ic::application::Application::IsRootDirectory(const std::filesystem::path& t_path)
{
    return root_paths.contains(t_path);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::application::Application::Init()
{
    IC_LOG_DEBUG("[Application::Init()] Initializing application ...");

#if defined(_WIN64) && defined(_MSC_VER)
    for (const auto drive : Util::GetAvailableDriveLetters())
    {
        std::string label(1, drive);
        root_paths.emplace(label.append(":\\"));
    }
    IC_LOG_DEBUG("[Application::Init()] Found {} drive letters.", root_paths.size());
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)
    root_paths.emplace("/");
#else
    #error Unsupported platform or compiler!
#endif

    IC_ASSERT(!root_paths.empty(), "[Application::Init()] Invalid number of root paths.")

    m_window = std::make_unique<Window>(
        "ic",
        INI.Get<int>("window", "width"),
        INI.Get<int>("window", "height")
    );

    m_leftView = std::make_unique<data::View>(data::ViewType::LEFT);
    m_rightView = std::make_unique<data::View>(data::ViewType::RIGHT);

    IC_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void ic::application::Application::HandleSdlEvents(bool& t_running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
        {
            t_running = false;
        }

        if (event.type == SDL_WINDOWEVENT)
        {
            m_window->OnSdlEvent(&event);
        }
    }
}

void ic::application::Application::Update()
{
    m_leftView->Update();
    m_rightView->Update();
}

void ic::application::Application::Render() const
{
    widget::MainMenuWidget::Render();

    m_leftView->SetPosition(0.0f, ImGui::GetFrameHeight());
    m_leftView->SetSize(static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 5.0f));
    m_leftView->SetInfoPosition(0.0f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 4.0f));
    m_leftView->SetInfoSize(static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() * 2.0f);
    m_leftView->Render();

    m_rightView->SetPosition(static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight());
    m_rightView->SetSize(static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 5.0f));
    m_rightView->SetInfoPosition(static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 4.0f));
    m_rightView->SetInfoSize(static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() * 2.0f);
    m_rightView->Render();

    widget::BottomMenuWidget::SetPosition(0.0f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 2.0f));
    widget::BottomMenuWidget::SetSize(static_cast<float>(m_window->width), ImGui::GetFrameHeight() * 2.0f);
    widget::BottomMenuWidget::Render();

#ifdef IC_DEBUG_BUILD
    ImGui::Begin("Debug");

    const auto fps{ static_cast<double>(ImGui::GetIO().Framerate) };
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // red
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0/fps, fps);
    ImGui::PopStyleColor(1);

    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    ImGui::Text("Left");
    ImGui::PopStyleColor(1);
    for (const auto& entry : m_leftView->selectedEntries)
    {
        ImGui::TextUnformatted(entry.filename().string().c_str());
    }

    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    ImGui::Text("Right");
    ImGui::PopStyleColor(1);
    for (const auto& entry : m_rightView->selectedEntries)
    {
        ImGui::TextUnformatted(entry.filename().string().c_str());
    }

    ImGui::End();
#endif
}

//-------------------------------------------------
// Application loop
//-------------------------------------------------

void ic::application::Application::Loop()
{
    IC_LOG_DEBUG("[Application::Loop()] Starting the application loop.");

    bool running{ true };
    while (running)
    {
        HandleSdlEvents(running);

        m_window->StartFrame();
        Update();
        Render();
        m_window->EndFrame();
    }

    IC_LOG_DEBUG("[Application::Loop()] The application loop has ended.");
}
