#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>

#include <iostream>

struct Vector2
{
    Vector2(){};
    Vector2(int x, int y)
    {
        this->x = x;
        this->y = y;
    };
    int x = 0;
    int y = 0;
};

struct Camera
{
    Vector2 position;
    Vector2 size;
};

struct Object
{
    Vector2 position;
    Vector2 size = Vector2(50, 50);
};

Vector2 GetMousePosition()
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    return Vector2(x, y);
}

int main(int, char **)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error: %s\n"
                  << SDL_GetError();

        return -1;
    }

    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Viewport", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 950, 550, window_flags);
    bool windowIsOpen = true;

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init();

    std::vector<Object> objects;

    Camera camera;
    camera.position = Vector2(0, 0);

    Object object;
    object.position = Vector2(0, 0);

    objects.push_back(object);

    Object object2;
    object2.position = Vector2(250, 150);
    objects.push_back(object2);

    // ConfigureImGuiStyle();

    bool movingCamera = false;
    Vector2 lastMousePosition;

    while (windowIsOpen)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_QUIT:
                windowIsOpen = false;

                break;
            case SDL_MOUSEWHEEL:

            case SDL_MOUSEBUTTONDOWN:
                lastMousePosition = GetMousePosition();

                if (event.button.button == SDL_BUTTON_RIGHT)
                    movingCamera = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_RIGHT)
                    movingCamera = false;
                break;
            }
        }

        if (movingCamera)
        {
            Vector2 currentMousePosition = GetMousePosition();

            camera.position.x += lastMousePosition.x - currentMousePosition.x;
            camera.position.y += lastMousePosition.y - currentMousePosition.y;

            lastMousePosition = GetMousePosition();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        for (Object obj : objects)
        {
            ImVec2 startPos = ImVec2(obj.position.x - camera.position.x, obj.position.y - camera.position.y);
            ImVec2 endPos = startPos;
            endPos.x += obj.size.x;
            endPos.y += obj.size.y;

            ImDrawList *drawList = ImGui::GetBackgroundDrawList();
            ImU32 fillColor = IM_COL32(255, 255, 255, 255);

            drawList->AddRectFilled(ImVec2(startPos.x, startPos.y), ImVec2(endPos.x, endPos.y), fillColor, 0);
        }

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0, 0, 0, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}