//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_WINDOW_H
#define PHYSICS_ENGINE_WINDOW_H

#pragma once
#include <unordered_map>
#include <SDL.h>
#include <iostream>

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_sdl2.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"

class Window {
public:
    enum Keys {
        UP = SDLK_UP,
        DOWN = SDLK_DOWN,
        LEFT = SDLK_LEFT,
        RIGHT = SDLK_RIGHT,
        FORWARD = SDLK_w,
        BACKWARD = SDLK_s,
        ESCAPE = SDLK_ESCAPE,
    };
public:
    Window(std::string windowName);
    ~Window();

    bool init();

    void swap();
    void pollEvent();

    bool getKeyHold(Keys k);
    bool getKeyPress(Keys k);

    void getMouseMotion(int& x, int& y);

    unsigned int getTick() const;

    bool shouldClose() const ;

    int getWidth() const;
    int getHeight() const;

private:
    SDL_Window* m_window;
    SDL_GLContext m_context;

    bool m_shouldClose;
    bool m_shouldResize;
    bool m_mouseClicked;
    int m_width, m_height;

    std::unordered_map<Keys, bool> m_keys;
    int m_mouseX, m_mouseY;
    std::string m_windowName;
};

#endif //PHYSICS_ENGINE_WINDOW_H
