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
#include "Window.h"
#include "Log.h"
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

    root_paths.emplace("/");

    m_window = std::make_unique<Window>(
        "ic",
        INI.Get<int>("window", "width"),
        INI.Get<int>("window", "height")
    );

    m_left = std::make_unique<data::View>(data::ViewType::LEFT);

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
    m_left->Update();
}

void ic::application::Application::Render() const
{
    widget::MainMenuWidget::Render();

    m_left->SetPosition(0.0f, ImGui::GetFrameHeight());
    m_left->SetSize(static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 5.0f));
    m_left->Render();

    widget::BottomMenuWidget::SetPosition(0.0f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 2.0f));
    widget::BottomMenuWidget::SetSize(static_cast<float>(m_window->width), ImGui::GetFrameHeight() * 2.0f);
    widget::BottomMenuWidget::Render();
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
