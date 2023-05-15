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

#include <imgui_internal.h>
#include "App.h"
#include "Window.h"
#include "Renderer.h"
#include "IcAssert.h"
#include "vendor/imgui/imgui_impl_sdl2.h"
#include "vendor/imgui/imgui_stdlib.h"

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

    m_window = std::make_unique<Window>(
        "ic",
        INI.Get<int>("window", "width"),
        INI.Get<int>("window", "height")
    );

#if defined(_WIN64) && defined(_MSC_VER)

    for (const auto drive : fs::get_available_drive_letters())
    {
        std::string label(1, drive);
        root_paths.emplace(label.append(":\\"));

        IC_LOG_DEBUG("[App::Init()] Found {} drive letters.", root_paths.size());
    }

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 9)

    root_paths.emplace("/");

#else
    #error Unsupported platform or compiler!
#endif

    IC_ASSERT(!root_paths.empty(), "[App::Init()] Invalid number of root paths.")

    m_entriesLeft = fs::read_from(m_currentPathLeft);
    m_entriesRight = fs::read_from(m_currentPathRight);

    IC_LOG_DEBUG("[App::Init()] The app was successfully initialized.");
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void ic::App::Render()
{
    RenderMainMenu();
    RenderLeft();
    RenderRight();
    RenderLeftInfo();
    RenderRightInfo();
    RenderMainMenuButtons();

    // todo: tmp code; example dialog
    if(m_render_dialog)
    {
        ImGui::OpenPopup("Create Directory");

        if(ImGui::BeginPopupModal("Create Directory", &m_render_dialog, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("##newPath", &m_newPathStr);

            if(ImGui::Button("Close"))
            {
                m_render_dialog = false;

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Create"))
            {
                std::filesystem::path p;
                if (m_currentSide == Side::LEFT)
                {
                    p = m_currentPathLeft / m_newPathStr;
                }
                else
                {
                    p = m_currentPathRight / m_newPathStr;
                }

                if (!std::filesystem::exists(p))
                {
                    if (std::filesystem::create_directory(p))
                    {
                        IC_LOG_DEBUG("[App::Render()] Directory {} created successfully.", p.filename().string());
                        m_entriesLeft = fs::read_from(m_currentPathLeft);
                        m_entriesRight = fs::read_from(m_currentPathRight);
                    }
                    else
                    {
                        IC_LOG_DEBUG("[App::Render()] Failed to create directory.");
                    }
                }
                else
                {
                    IC_LOG_DEBUG("[App::Render()] Directory already exists.");
                }

                m_render_dialog = false;

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

void ic::App::RenderMainMenu()
{
    ImGui::PushStyleColor(ImGuiCol_Border, Window::title_bg_color);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, Window::title_bg_color);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, Window::text_color);

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Window::alpha * 0.5f);
        if (ImGui::BeginMenu("Left"))
        {
            if (ImGui::MenuItem("File listing", "CTRL+G"))
            {
                IC_LOG_INFO("[App::RenderMainMenu()] Left - File listing");
            }
            if (ImGui::MenuItem("Quick view", "CTRL+Q")) {}
            if (ImGui::MenuItem("Info", "CTRL+I")) {}
            if (ImGui::MenuItem("Tree", "CTRL+T")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Rescan", "CTRL+R")) {}
            ImGui::EndMenu();
        }
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();

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

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Window::alpha * 0.5f);
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
                IC_LOG_INFO("[App::RenderMainMenu()] Right - File listing");
            }
            if (ImGui::MenuItem("Quick view", "CTRL+Q")) {}
            if (ImGui::MenuItem("Info", "CTRL+I")) {}
            if (ImGui::MenuItem("Tree", "CTRL+T")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Rescan", "CTRL+R")) {}
            ImGui::EndMenu();
        }
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor(3);
}

void ic::App::RenderLeft()
{
    ImGui::SetNextWindowPos({ 0.0f, ImGui::GetFrameHeight() });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 5.0f) });

    ImGui::Begin(
        "##left",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

#if defined(_WIN64) && defined(_MSC_VER)
    for (const auto drive : fs::get_available_drive_letters())
    {
        if (std::string label(1, drive); ImGui::Button(label.c_str()))
        {
            m_currentPathLeft = std::filesystem::path(label.append(":\\"));
            m_lastClickedLeft = PathClick{};
            entries = fs::read_from(m_currentPathLeft);
        }

        ImGui::SameLine();
    }
#endif

    ImGui::NewLine();

    renderer::render_view(
        Side::LEFT,
        m_currentSide == Side::LEFT,
        m_currentPathLeft,
        m_lastClickedLeft,
        m_entriesLeft,
        m_selectedFileIdsLeft,
        m_selectedDirectoryIdsLeft
    );

    // click on UP-DIR
    if (m_lastClickedLeft.id == 0)
    {
        // update current path if double click
        if (m_lastClickedLeft.doubleClick)
        {
            m_currentPathLeft = m_currentPathLeft.parent_path();
            m_lastClickedLeft = PathClick{};

            m_selectedFileIdsLeft.clear();
            m_selectedDirectoryIdsLeft.clear();

            m_entriesLeft = fs::read_from(m_currentPathLeft);
        }

        // reset clicked
        m_lastClickedLeft = PathClick{};
    }

    // click on other -> DOWN-DIR
    if (m_lastClickedLeft.id > 0 && m_lastClickedLeft.doubleClick && std::filesystem::is_directory(m_lastClickedLeft.path))
    {
        m_currentPathLeft = m_lastClickedLeft.path;
        m_lastClickedLeft = PathClick{};

        m_selectedFileIdsLeft.clear();
        m_selectedDirectoryIdsLeft.clear();

        m_entriesLeft = fs::read_from(m_currentPathLeft);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused())
    {
        m_currentSide = Side::LEFT;
    }

    ImGui::End();
}

void ic::App::RenderRight()
{
    ImGui::SetNextWindowPos({ static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 5.0f) });

    ImGui::Begin(
        "##right",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

#if defined(_WIN64) && defined(_MSC_VER)
    for (const auto drive : fs::get_available_drive_letters())
    {
        if (std::string label(1, drive); ImGui::Button(label.c_str()))
        {
            m_currentPathRight = std::filesystem::path(label.append(":\\"));
            m_lastClickedRight = PathClick{};
            entries = fs::read_from(m_currentPathRight);
        }

        ImGui::SameLine();
    }
#endif

    ImGui::NewLine();

    renderer::render_view(
        Side::RIGHT,
        m_currentSide == Side::RIGHT,
        m_currentPathRight,
        m_lastClickedRight,
        m_entriesRight,
        m_selectedFileIdsRight,
        m_selectedDirectoryIdsRight
    );

    // click on UP-DIR
    if (m_lastClickedRight.id == 0)
    {
        // update current path if double click
        if (m_lastClickedRight.doubleClick)
        {
            m_currentPathRight = m_currentPathRight.parent_path();
            m_lastClickedRight = PathClick{};

            m_selectedFileIdsRight.clear();
            m_selectedDirectoryIdsRight.clear();

            m_entriesRight = fs::read_from(m_currentPathRight);
        }

        // reset clicked
        m_lastClickedRight = PathClick{};
    }

    // click on other
    if (m_lastClickedRight.id > 0 && m_lastClickedRight.doubleClick && std::filesystem::is_directory(m_lastClickedRight.path))
    {
        m_currentPathRight = m_lastClickedRight.path;
        m_lastClickedRight = PathClick{};

        m_selectedFileIdsRight.clear();
        m_selectedDirectoryIdsRight.clear();

        m_entriesRight = fs::read_from(m_currentPathRight);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused())
    {
        m_currentSide = Side::RIGHT;
    }

    ImGui::End();
}

void ic::App::RenderLeftInfo()
{
    ImGui::SetNextWindowPos({ 0.0f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 4.0f) });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() * 2.0f });

    ImGui::Begin(
        "##infoLeft",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    renderer::render_clicked_path_info(m_lastClickedLeft);

    ImGui::End();
}

void ic::App::RenderRightInfo()
{
    ImGui::SetNextWindowPos({ static_cast<float>(m_window->width) * 0.5f, static_cast<float>(m_window->height) - (ImGui::GetFrameHeight() * 4.0f) });
    ImGui::SetNextWindowSize({ static_cast<float>(m_window->width) * 0.5f, ImGui::GetFrameHeight() * 2.0f });

    ImGui::Begin(
        "##infoRight",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings
    );

    renderer::render_clicked_path_info(m_lastClickedRight);

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

    if (ImGui::Button("Help"))
    {
        ImGui::OpenPopup("##HelpScreen");
    }
    if (ImGui::BeginPopupModal("##HelpScreen", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("This is the main help screen for ic.");
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Window::alpha * 0.5f);

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

    ImGui::PopItemFlag();
    ImGui::PopStyleVar();



    if (m_currentSide == Side::NONE)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Window::alpha * 0.5f);
    }
    if (ImGui::Button("MkDir") && m_currentSide != Side::NONE)
    {
        m_render_dialog = true;
    }
    ImGui::SameLine();
    if (m_currentSide == Side::NONE)
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }



    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Window::alpha * 0.5f);

    if (ImGui::Button("Delete")) {}
    ImGui::SameLine();
    if (ImGui::Button("PullDn")) {}
    ImGui::SameLine();

    ImGui::PopItemFlag();
    ImGui::PopStyleVar();

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
