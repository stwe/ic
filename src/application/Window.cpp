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
#include "Log.h"
#include "IcException.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_opengl3_loader.h"
#include "vendor/imgui/imgui_impl_sdl2.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

ic::application::Window::Window(std::string t_title, const int t_width, const int t_height)
    : width{ t_width }
    , height{ t_height }
    , m_title{ std::move(t_title) }
{
    IC_LOG_DEBUG("[Window::Window()] Create Window.");

    InitSdl();
    InitImGui();
}

ic::application::Window::~Window() noexcept
{
    IC_LOG_DEBUG("[Window::~Window()] Destruct Window.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void ic::application::Window::StartFrame() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(sdlWindow);
    ImGui::NewFrame();
}

void ic::application::Window::EndFrame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(sdlWindow);
}

void ic::application::Window::OnSdlEvent(const SDL_Event* t_event)
{
    if (t_event->type == SDL_WINDOWEVENT && t_event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        OnSdlWindowResized(t_event->window.data1, t_event->window.data2);
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void ic::application::Window::InitSdl()
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
    SDL_GL_SetSwapInterval(1);
}

void ic::application::Window::InitImGui()
{
    IC_LOG_DEBUG("[Window::InitImGui()] Initializing ImGui.");

    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext())
    {
        throw IC_EXCEPTION("[Window::InitImGui()] Unable to create the ImGui context.");
    }

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, m_sdlGlContext);

    const char* glslVersion = "#version 130";
    ImGui_ImplOpenGL3_Init(glslVersion);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//-------------------------------------------------
// Event handler
//-------------------------------------------------

void ic::application::Window::OnSdlWindowResized(const int t_width, const int t_height)
{
    width = t_width;
    height = t_height;

    glViewport(0, 0, width, height);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void ic::application::Window::CleanUp()
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
