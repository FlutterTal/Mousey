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
#include "thirdparty/yaml/yaml_emitter.h"
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_sound.h>

Engine Engine::singleton;

Engine::Engine() {
    if(access("project.yaml", F_OK) == -1) {
        YAMLEmitter emitter;
        emitter.new_block("window");
        const char* keys[] = { "title", "icon", "width", "height", "resizable", "always_on_top", "borderless", "fullscreen" };
        const char* values[] = { "Untitled", "null", "800", "600", "false", "false", "false", "false" };
        for(int i = 0; i < 8; i++)
            emitter.new_scalar(keys[i], values[i]);

        emitter.end_block();
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

    if(!Sound_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_sound! (%s)", Sound_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }

    /* TODO: Add a specialized class for the parser */
    const char* title;
    const char* icon;
    int w, h;
    bool resizable, always_on_top, borderless, fullscreen;
    {
        yaml_parser_t parser;
        yaml_event_t event;
        char title_temp[250];
        char icon_temp[250];
        bool in_window_block = false;
        if(!yaml_parser_initialize(&parser)) {
            fprintf(stderr, "Failed to initialize the parser\n");
            return;
        }

        FILE* file = fopen("project.yaml", "rb");
        if(file == nullptr) {
            perror("Failed to open file");
            return;
        }

        yaml_parser_set_input_file(&parser, file);
        do {
            if(!yaml_parser_parse(&parser, &event)) {
                fprintf(stderr, "Parser error\n");
                yaml_parser_delete(&parser);
                return;
            }

            if(event.type == YAML_SCALAR_EVENT) {
                if(in_window_block) {
                    if(strcmp((char*)event.data.scalar.value, "title") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        strncpy(title_temp, (char*)event.data.scalar.value, sizeof(title_temp) - 1);
                        title_temp[sizeof(title_temp) - 1] = '\0';
                        title = title_temp;
                    } else if(strcmp((char*)event.data.scalar.value, "icon") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        if(strcmp((char*)event.data.scalar.value, "null") != 0) {
                            strncpy(icon_temp, (char*)event.data.scalar.value, sizeof(icon_temp) - 1);
                            icon_temp[sizeof(icon_temp) - 1] = '\0';
                            icon = icon_temp;
                        }
                    } else if(strcmp((char*)event.data.scalar.value, "width") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        w = atoi((char*)event.data.scalar.value);
                    } else if(strcmp((char*)event.data.scalar.value, "height") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        h = atoi((char*)event.data.scalar.value);
                    } else if(strcmp((char*)event.data.scalar.value, "resizable") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        if(strcmp((char*)event.data.scalar.value, "true") == 0)
                            resizable = true;
                        else if(strcmp((char*)event.data.scalar.value, "false") == 0)
                            resizable = false;
                        else
                            perror("Value error");
                    } else if(strcmp((char*)event.data.scalar.value, "always_on_top") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        if(strcmp((char*)event.data.scalar.value, "true") == 0)
                            always_on_top = true;
                        else if(strcmp((char*)event.data.scalar.value, "false") == 0)
                            always_on_top = false;
                        else
                            perror("Value error");
                    } else if(strcmp((char *)event.data.scalar.value, "borderless") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        if(strcmp((char*)event.data.scalar.value, "true") == 0)
                            borderless = true;
                        else if(strcmp((char*)event.data.scalar.value, "false") == 0)
                            borderless = false;
                        else
                            perror("Value error");
                    } else if(strcmp((char *)event.data.scalar.value, "fullscreen") == 0) {
                        yaml_event_delete(&event);
                        yaml_parser_parse(&parser, &event);
                        if(strcmp((char*)event.data.scalar.value, "true") == 0)
                            fullscreen = true;
                        else if(strcmp((char*)event.data.scalar.value, "false") == 0)
                            fullscreen = false;
                        else
                            perror("Value error");
                    }
                } else if(strcmp((char*)event.data.scalar.value, "window") == 0)
                    in_window_block = true;
            }

            if(event.type == YAML_MAPPING_END_EVENT && in_window_block)
                in_window_block = false;

            if(event.type != YAML_STREAM_END_EVENT)
                yaml_event_delete(&event);
        } while(event.type != YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
        yaml_parser_delete(&parser);
        fclose(file);
    }

    window = new Window(title, w, h, icon, resizable, always_on_top, borderless, fullscreen);
}

Engine::~Engine() {
    delete window;
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
        SQFloat dt = (current_time - last_frame_time) / 1000.0;
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
        glViewport(0, 0, 800, 600);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);
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