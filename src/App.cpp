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

#include "App.h"
#include "Log.h"
#include "Window.h"
#include "FileSystem.h"
#include "Renderer.h"
#include "vendor/imgui/imgui_impl_sdl2.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::App::App()
{
    IC_LOG_DEBUG("[App::App()] Create App.");
}

ic::App::~App() noexcept
{
    IC_LOG_DEBUG("[App::~App()] Destruct App.");
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void ic::App::Run()
{
    IC_LOG_DEBUG("[App::Run()] Starts running app.");

    Init();
    AppLoop();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void ic::App::Init()
{
    IC_LOG_DEBUG("[App::Init()] Initializing app...");

    m_window = std::make_unique<Window>("ic", 1024, 768);

    IC_LOG_DEBUG("[App::Init()] The app was successfully initialized.");
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void ic::App::Render()
{
    ImGui::Begin(
        "##app",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    RenderMainMenu();
    RenderLeft();
    RenderRight();
    RenderMainMenuButtons();

    //ImGui::ShowDemoWindow();

    ImGui::End();
}

void ic::App::RenderMainMenu()
{
    ImGui::PushStyleColor(ImGuiCol_Border, Window::TITLE_BG_COLOR);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, Window::TITLE_BG_COLOR);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, Window::TEXT_COLOR);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Left"))
        {
            if (ImGui::MenuItem("File listing", "CTRL+G"))
            {
                Log::IC_LOG_INFO("Left - File listing");
            }
            if (ImGui::MenuItem("Quick view", "CTRL+Q")) {}
            if (ImGui::MenuItem("Info", "CTRL+I")) {}
            if (ImGui::MenuItem("Tree", "CTRL+T")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Rescan", "CTRL+R")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit", "F10"))
            {
                SDL_Event event;
                event.type = SDL_QUIT;
                SDL_PushEvent(&event);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Command"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Right"))
        {
            if (ImGui::MenuItem("File listing", "CTRL+G"))
            {
                Log::IC_LOG_INFO("Right - File listing");
            }
            if (ImGui::MenuItem("Quick view", "CTRL+Q")) {}
            if (ImGui::MenuItem("Info", "CTRL+I")) {}
            if (ImGui::MenuItem("Tree", "CTRL+T")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Rescan", "CTRL+R")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor(3);
}

void ic::App::RenderLeft()
{
    ImGui::SetNextWindowPos({ 0.0f, ImGui::GetFrameHeight() });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 3.0f) });

    ImGui::Begin(
        "##left",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    // todo
    static int selectedIndex = -1;
    renderer::render_table(
        m_currentPathLeft,
        fs::read_from(m_currentPathLeft),
        &selectedIndex,
        m_lastClickedFileLeft
    );

    // click on UP-DIR
    if (selectedIndex == 0)
    {
        m_currentPathLeft = m_currentPathLeft.parent_path();
        m_lastClickedFileLeft = std::filesystem::path();
        selectedIndex = -1;
    }

    // click on other
    if (selectedIndex > 0)
    {
        auto res = *std::next(fs::read_from(m_currentPathLeft).begin(), selectedIndex-1);
        if (std::filesystem::is_directory(res))
        {
            m_currentPathLeft = res;
            m_lastClickedFileLeft = std::filesystem::path();
        }
        if (std::filesystem::is_regular_file(res))
        {
            m_lastClickedFileLeft = res;
        }

        selectedIndex = -1;
    }

    ImGui::Separator();

    ImGui::End();
}

void ic::App::RenderRight()
{
    ImGui::SetNextWindowPos({ static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 3.0f) });

    ImGui::Begin(
        "##right",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    // todo
    static int selectedIndex = -1;
    renderer::render_table(
        m_currentPathRight,
        fs::read_from(m_currentPathRight),
        &selectedIndex,
        m_lastClickedFileRight
    );

    // click on UP-DIR
    if (selectedIndex == 0)
    {
        m_currentPathRight = m_currentPathRight.parent_path();
        m_lastClickedFileRight = std::filesystem::path();
        selectedIndex = -1;
    }

    // click on other
    if (selectedIndex > 0)
    {
        auto res = *std::next(fs::read_from(m_currentPathRight).begin(), selectedIndex-1);
        if (std::filesystem::is_directory(res))
        {
            m_currentPathRight = res;
            m_lastClickedFileRight = std::filesystem::path();
        }
        if (std::filesystem::is_regular_file(res))
        {
            m_lastClickedFileRight = res;
        }
        selectedIndex = -1;
    }

    ImGui::Separator();

    ImGui::End();
}

void ic::App::RenderMainMenuButtons()
{
    ImGui::SetNextWindowPos({ 0.0f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 2.0f) });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width), ImGui::GetFrameHeight() * 2.0f });

    ImGui::Begin(
        "##bottom",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    if (ImGui::Button("Help")) {}
    ImGui::SameLine();
    if (ImGui::Button("Menu")) {}
    ImGui::SameLine();
    if (ImGui::Button("View")) {}
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {}
    ImGui::SameLine();
    if (ImGui::Button("Copy")) {}
    ImGui::SameLine();
    if (ImGui::Button("Move")) {}
    ImGui::SameLine();
    if (ImGui::Button("MkDir")) {}
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {}
    ImGui::SameLine();
    if (ImGui::Button("PullDn")) {}
    ImGui::SameLine();
    if (ImGui::Button("Quit"))
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }

    ImGui::End();
}

//-------------------------------------------------
// App loop
//-------------------------------------------------

void ic::App::AppLoop()
{
    IC_LOG_DEBUG("[App::AppLoop()] Starting the app loop.");

    bool loop{ true };
    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_QUIT:
                loop = false;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    m_window->ChangeSize(event.window.data1, event.window.data2);
                    break;
                default:;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                case SDLK_F10:
                    loop = false;
                    break;
                default:;
                }
                break;
            }
        }

        m_window->StartFrame();
        Render();
        m_window->EndFrame();
    }

    IC_LOG_DEBUG("[App::AppLoop()] The app loop has ended.");
}
