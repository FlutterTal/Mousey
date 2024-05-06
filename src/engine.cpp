/******************************************************************************/
/* Copyright (c) 2024 Taliesin Perscilla "FlutterTal" Ambroise                */
/******************************************************************************/
/* This software is provided ‘as-is’, without any express or implied          */
/* warranty. In no event will the authors be held liable for any damages      */
/* arising from the use of this software.                                     */
/*                                                                            */
/* Permission is granted to anyone to use this software for any purpose,      */
/* including commercial applications, and to alter it and redistribute it     */
/* freely, subject to the following restrictions:                             */
/*                                                                            */
/* 1. The origin of this software must not be misrepresented; you must not    */
/* claim that you wrote the original software. If you use this software       */
/* in a product, an acknowledgment in the product documentation would be      */
/* appreciated but is not required.                                           */
/*                                                                            */
/* 2. Altered source versions must be plainly marked as such, and must not be */
/* misrepresented as being the original software.                             */
/*                                                                            */
/* 3. This notice may not be removed or altered from any source               */
/* distribution.                                                              */
/******************************************************************************/

#include "engine.h"
#include "core.h"
#include <unistd.h>
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_sound.h>
#include <fstream>

Engine Engine::singleton;

Engine::Engine() {
    if(access("project.yaml", F_OK) == -1) {
        YAML::Emitter emitter;
        emitter << YAML::BeginDoc;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "window";
        emitter << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "title" << YAML::Value << "Untitled";
        emitter << YAML::Key << "icon" << YAML::Value << YAML::Null;
        emitter << YAML::Key << "width" << YAML::Value << "800";
        emitter << YAML::Key << "height" << YAML::Value << "600";
        emitter << YAML::Key << "resizable" << YAML::Value << "false";
        emitter << YAML::Key << "always_on_top" << YAML::Value << "false";
        emitter << YAML::Key << "borderless" << YAML::Value << "false";
        emitter << YAML::Key << "fullscreen" << YAML::Value << "false";
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
        emitter << YAML::EndDoc;
        std::ofstream fout("project.yaml");
        fout << emitter.c_str();
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! (%s)", SDL_GetError());
        return;
    }

    if(!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_image! (%s)", IMG_GetError());
        SDL_Quit();
        return;
    }

    if(TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf! (%s)", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return;
    }

    if(Sound_Init() == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_sound! (%s)", Sound_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    YAML::Node project = YAML::LoadFile("project.yaml");
    YAML::Node window_node = project["window"];
    std::string title = window_node["title"].as<std::string>();
    std::string icon = window_node["icon"].as<std::string>();
    int w = window_node["width"].as<int>();
    int h = window_node["height"].as<int>();
    bool resizable = window_node["resizable"].as<bool>();
    bool always_on_top = window_node["always_on_top"].as<bool>();
    bool borderless = window_node["borderless"].as<bool>();
    bool fullscreen = window_node["fullscreen"].as<bool>();
    window = new Window(title.c_str(), w, h, icon.c_str(), resizable, always_on_top, borderless, fullscreen);
}

Engine::~Engine() {
    delete window;
    Sound_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Engine * Engine::get_singleton() {
    return &Engine::singleton;
}

void Engine::run() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    current_keyboard_state = (uint8_t *)calloc(SDL_NUM_SCANCODES, sizeof(uint8_t));
    previous_keyboard_state = (uint8_t *)calloc(SDL_NUM_SCANCODES, sizeof(uint8_t));
    vm.call_func_without_return("initialize");
    uint64_t last_frame_time = SDL_GetTicks64();
    while(!window->should_close()) {
        event.poll();
        uint64_t current_time = SDL_GetTicks64();
        double dt = (current_time - last_frame_time) / 1000.0;
        vm.call_func_without_return("update", dt);
        accumulator += dt;
        while(accumulator >= fixed_dt) {
            vm.call_func_without_return("physics_update", fixed_dt);
            accumulator -= fixed_dt;
        }

        memcpy(previous_keyboard_state, current_keyboard_state, SDL_NUM_SCANCODES);
        previous_mouse_state = current_mouse_state;
        last_frame_time = current_time;
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int w, h;
        SDL_GetWindowSize(window->get_window(), &w, &h);
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        vm.call_func_without_return("render");
        window->swap();
    }

    free(current_keyboard_state);
    free(previous_keyboard_state);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}