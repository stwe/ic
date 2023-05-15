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

#include "Window.h"
#include "App.h"
#include "Log.h"
#include "IcException.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_opengl3_loader.h"
#include "vendor/imgui/imgui_impl_sdl2.h"

ic::Window::Window(std::string t_title, const int t_width, const int t_height)
    : width{ t_width }
    , height{ t_height }
    , m_title{ std::move(t_title) }
{
    IC_LOG_DEBUG("[Window::Window()] Create Window.");

    InitSdl();
    ConfigTheme();
    InitImGui();
}

ic::Window::~Window() noexcept
{
    IC_LOG_DEBUG("[Window::~Window()] Destruct Window.");

    CleanUp();
}

void ic::Window::StartFrame() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(sdlWindow);
    ImGui::NewFrame();
}

void ic::Window::EndFrame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(sdlWindow);
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void ic::Window::ChangeSize(const int t_width, const int t_height)
{
    width = t_width;
    height = t_height;
    glViewport(0, 0, width, height);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void ic::Window::InitSdl()
{
    IC_LOG_DEBUG("[Window::InitSdl()] Initializing SDL.");

    if (width < MIN_WIDTH || height < MIN_HEIGHT)
    {
        throw IC_EXCEPTION("[Window::InitSdl()] Invalid window size..");
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw IC_EXCEPTION("[Window::InitSdl()] Unable to initialize SDL.");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    sdlWindow = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!sdlWindow)
    {
        throw IC_EXCEPTION("[Window::InitSdl()] Unable to create the SDL window.");
    }

    m_sdlGlContext = SDL_GL_CreateContext(sdlWindow);
    if (!m_sdlGlContext)
    {
        throw IC_EXCEPTION("[Window::InitSdl()] Unable to create a handle to an OpenGL context.");
    }

    SDL_GL_MakeCurrent(sdlWindow, m_sdlGlContext);

    // enable vsync
    SDL_GL_SetSwapInterval(1);
}

void ic::Window::ConfigTheme()
{
    IC_LOG_DEBUG("[Window::ConfigTheme()] Read color values from the config.ini file.");

    const auto windowBg{ App::INI.GetVector<float>("theme", "window_bg_color") };
    const auto text{ App::INI.GetVector<float>("theme", "text_color") };
    const auto titleBg{ App::INI.GetVector<float>("theme", "title_bg_color") };
    const auto titleBgActive{ App::INI.GetVector<float>("theme", "title_bg_active_color") };
    const auto border{ App::INI.GetVector<float>("theme", "border_color") };
    const auto headerHovered{ App::INI.GetVector<float>("theme", "header_hovered_color") };
    const auto menuBarBg{ App::INI.GetVector<float>("theme", "menu_bar_bg_color") };
    const auto warn{ App::INI.GetVector<float>("theme", "warn_color") };
    const auto clear{ App::INI.GetVector<float>("theme", "clear_color") };
    const auto hidden{ App::INI.GetVector<float>("theme", "hidden_color") };
    const auto symlink{ App::INI.GetVector<float>("theme", "symlink_color") };
    const auto selected{ App::INI.GetVector<float>("theme", "selected_color") };
    const auto tableHeaderBg{ App::INI.GetVector<float>("theme", "table_header_bg_color") };
    const auto tableHeaderActiveBg{ App::INI.GetVector<float>("theme", "table_header_bg_active_color") };

    const std::vector<std::vector<float>> colors{
        windowBg, text, titleBg, titleBgActive, border, headerHovered, menuBarBg, warn, clear, symlink, selected,
        tableHeaderBg, tableHeaderActiveBg
    };

    for (const auto& col : colors)
    {
        if (col.size() != 3)
        {
            throw IC_EXCEPTION("[Window::ConfigTheme()] Invalid color config.");
        }
    }

    if (hidden.size() != 4)
    {
        throw IC_EXCEPTION("[Window::ConfigTheme()] Invalid color config.");
    }

    window_bg_color = ImVec4(windowBg.at(0), windowBg.at(1), windowBg.at(2), 1.0f);
    text_color = ImVec4(text.at(0), text.at(1), text.at(2), 1.0f);
    title_bg_color = ImVec4(titleBg.at(0), titleBg.at(1), titleBg.at(2), 1.0f);
    title_bg_active_color = ImVec4(titleBgActive.at(0), titleBgActive.at(1), titleBgActive.at(2), 1.0f);
    border_color = ImVec4(border.at(0), border.at(1), border.at(2), 1.0f);
    header_hovered_color = ImVec4(headerHovered.at(0), headerHovered.at(1), headerHovered.at(2), 1.0f);
    menu_bar_bg_color = ImVec4(menuBarBg.at(0), menuBarBg.at(1), menuBarBg.at(2), 1.0f);
    warn_color = ImVec4(warn.at(0), warn.at(1), warn.at(2), 1.0f);
    clear_color = ImVec4(clear.at(0), clear.at(1), clear.at(2), 1.0f);
    alpha = App::INI.Get<float>("theme", "alpha");
    hidden_color = ImVec4(hidden.at(0), hidden.at(1), hidden.at(2), hidden.at(3));
    symlink_color = ImVec4(symlink.at(0), symlink.at(1), symlink.at(2), 1.0f);
    selected_color = ImVec4(selected.at(0), selected.at(1), selected.at(2), 1.0f);
    table_header_bg_color = ImVec4(tableHeaderBg.at(0), tableHeaderBg.at(1), tableHeaderBg.at(2), 1.0f);
    table_header_bg_active_color = ImVec4(tableHeaderActiveBg.at(0), tableHeaderActiveBg.at(1), tableHeaderActiveBg.at(2), 1.0f);
}

void ic::Window::InitImGui()
{
    IC_LOG_DEBUG("[Window::InitImGui()] Initializing ImGui.");

    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext())
    {
        throw IC_EXCEPTION("[Window::InitImGui()] Unable to create the ImGui context.");
    }

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = text_color;
    style.Colors[ImGuiCol_WindowBg] = window_bg_color;
    style.Colors[ImGuiCol_TitleBg] = title_bg_color;
    style.Colors[ImGuiCol_TitleBgActive] = title_bg_active_color;
    style.Colors[ImGuiCol_Border] = border_color;
    style.Colors[ImGuiCol_HeaderHovered] = header_hovered_color;
    style.Colors[ImGuiCol_MenuBarBg] = menu_bar_bg_color;
    style.Colors[ImGuiCol_Button] = title_bg_color;
    style.Colors[ImGuiCol_ButtonHovered] = header_hovered_color;
    style.Alpha = alpha;

    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, m_sdlGlContext);

    const char* glslVersion = "#version 130";
    ImGui_ImplOpenGL3_Init(glslVersion);

    glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void ic::Window::CleanUp()
{
    IC_LOG_DEBUG("[Window::CleanUp()] CleanUp window.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_sdlGlContext)
    {
        SDL_GL_DeleteContext(m_sdlGlContext);
    }

    if (sdlWindow)
    {
        SDL_DestroyWindow(sdlWindow);
    }

    SDL_Quit();
}
