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

#include "keyboard_wrapper.h"
#include "core.h"
#include <SDL2/SDL.h>

enum Key {
    KEY_A = SDLK_a,
    KEY_B = SDLK_b,
    KEY_C = SDLK_c,
    KEY_D = SDLK_d,
    KEY_E = SDLK_e,
    KEY_F = SDLK_f,
    KEY_G = SDLK_g,
    KEY_H = SDLK_h,
    KEY_I = SDLK_i,
    KEY_J = SDLK_j,
    KEY_K = SDLK_k,
    KEY_L = SDLK_l,
    KEY_M = SDLK_m,
    KEY_N = SDLK_n,
    KEY_O = SDLK_o,
    KEY_P = SDLK_p,
    KEY_Q = SDLK_q,
    KEY_R = SDLK_r,
    KEY_S = SDLK_s,
    KEY_T = SDLK_t,
    KEY_U = SDLK_u,
    KEY_V = SDLK_v,
    KEY_W = SDLK_w,
    KEY_X = SDLK_x,
    KEY_Y = SDLK_y,
    KEY_Z = SDLK_z,
    KEY_1 = SDLK_1,
    KEY_2 = SDLK_2,
    KEY_3 = SDLK_3,
    KEY_4 = SDLK_4,
    KEY_5 = SDLK_5,
    KEY_6 = SDLK_6,
    KEY_7 = SDLK_7,
    KEY_8 = SDLK_8,
    KEY_9 = SDLK_9,
    KEY_0 = SDLK_0,
    KEY_ENTER = SDLK_RETURN,
    KEY_ESCAPE = SDLK_ESCAPE,
    KEY_BACKSPACE = SDLK_BACKSPACE,
    KEY_TAB = SDLK_TAB,
    KEY_SPACE = SDLK_SPACE,
    KEY_HYPHEN = SDLK_MINUS,
    KEY_EQUAL = SDLK_EQUALS,
    KEY_LBRACKET = SDLK_LEFTBRACKET,
    KEY_RBRACKET = SDLK_RIGHTBRACKET,
    KEY_BACKSLASH = SDLK_BACKSLASH,
    KEY_SEMICOLON = SDLK_SEMICOLON,
    KEY_COMMA = SDLK_COMMA,
    KEY_PERIOD = SDLK_PERIOD,
    KEY_SLASH = SDLK_SLASH,
    KEY_CAPSLOCK = SDLK_CAPSLOCK,
    KEY_F1 = SDLK_F1,
    KEY_F2 = SDLK_F2,
    KEY_F3 = SDLK_F3,
    KEY_F4 = SDLK_F4,
    KEY_F5 = SDLK_F5,
    KEY_F6 = SDLK_F6,
    KEY_F7 = SDLK_F7,
    KEY_F8 = SDLK_F8,
    KEY_F9 = SDLK_F9,
    KEY_F10 = SDLK_F10,
    KEY_F11 = SDLK_F11,
    KEY_F12 = SDLK_F12,
    KEY_PRINTSCREEN = SDLK_PRINTSCREEN,
    KEY_SCROLLLOCK = SDLK_SCROLLLOCK,
    KEY_PAUSE = SDLK_PAUSE,
    KEY_INSERT = SDLK_INSERT,
    KEY_HOME = SDLK_HOME,
    KEY_PAGEUP = SDLK_PAGEUP,
    KEY_DELETE = SDLK_DELETE,
    KEY_END = SDLK_END,
    KEY_PAGEDOWN = SDLK_PAGEDOWN,
    KEY_RIGHT = SDLK_RIGHT,
    KEY_LEFT = SDLK_LEFT,
    KEY_DOWN = SDLK_DOWN,
    KEY_UP = SDLK_UP,
    KEY_NUMLOCK = SDLK_NUMLOCKCLEAR,
    KEY_NUMPAD_DIVIDE = SDLK_KP_DIVIDE,
    KEY_NUMPAD_MULTIPLY = SDLK_KP_MULTIPLY,
    KEY_NUMPAD_MINUS = SDLK_KP_MINUS,
    KEY_NUMPAD_PLUS = SDLK_KP_PLUS,
    KEY_NUMPAD_ENTER = SDLK_KP_ENTER,
    KEY_NUMPAD_1 = SDLK_KP_1,
    KEY_NUMPAD_2 = SDLK_KP_2,
    KEY_NUMPAD_3 = SDLK_KP_3,
    KEY_NUMPAD_4 = SDLK_KP_4,
    KEY_NUMPAD_5 = SDLK_KP_5,
    KEY_NUMPAD_6 = SDLK_KP_6,
    KEY_NUMPAD_7 = SDLK_KP_7,
    KEY_NUMPAD_8 = SDLK_KP_8,
    KEY_NUMPAD_9 = SDLK_KP_9,
    KEY_NUMPAD_0 = SDLK_KP_0,
    KEY_APPLICATION = SDLK_APPLICATION,
    KEY_NUMPAD_EQUAL = SDLK_KP_EQUALS,
    KEY_F13 = SDLK_F13,
    KEY_F14 = SDLK_F14,
    KEY_F15 = SDLK_F15,
    KEY_F16 = SDLK_F16,
    KEY_F17 = SDLK_F17,
    KEY_F18 = SDLK_F18,
    KEY_F19 = SDLK_F19,
    KEY_F20 = SDLK_F20,
    KEY_F21 = SDLK_F21,
    KEY_F22 = SDLK_F22,
    KEY_F23 = SDLK_F23,
    KEY_F24 = SDLK_F24,
    KEY_EXECUTE = SDLK_EXECUTE,
    KEY_NUMPAD_DECIMAL = SDLK_KP_DECIMAL,
    KEY_LCTRL = SDLK_LCTRL,
    KEY_LSHIFT = SDLK_LSHIFT,
    KEY_LALT = SDLK_LALT,
    KEY_LSYSTEM = SDLK_LGUI,
    KEY_RCTRL = SDLK_RCTRL,
    KEY_RSHIFT = SDLK_RSHIFT,
    KEY_RALT = SDLK_RALT,
    KEY_RSYSTEM = SDLK_RGUI,
};

static SQInteger squirrel_keyboard_isdown(HSQUIRRELVM v) {
    Key key;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&key)))
        return sq_throwerror(v, _SC("Argument 1 not a key"));
    
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    sq_pushbool(v, current_keyboard_state[scancode]);
    return 1;
}

static SQInteger squirrel_keyboard_ispressed(HSQUIRRELVM v) {
    Key key;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&key)))
        return sq_throwerror(v, _SC("Argument 1 not a key"));
    
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    sq_pushbool(v, current_keyboard_state[scancode] && !previous_keyboard_state[scancode]);
    return 1;
}

static SQInteger squirrel_keyboard_isreleased(HSQUIRRELVM v) {
    Key key;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&key)))
        return sq_throwerror(v, _SC("Argument 1 not a key"));
    
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    sq_pushbool(v, !current_keyboard_state[scancode] && previous_keyboard_state[scancode]);
    return 1;
}

void register_keyboard_wrapper(HSQUIRRELVM v) {
    sq_pushstring(v, _SC("Key"), -1);
    sq_newtable(v);
    sq_pushstring(v, _SC("A"), -1);
    sq_pushinteger(v, Key::KEY_A);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("B"), -1);
    sq_pushinteger(v, Key::KEY_B);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("C"), -1);
    sq_pushinteger(v, Key::KEY_C);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("D"), -1);
    sq_pushinteger(v, Key::KEY_D);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("E"), -1);
    sq_pushinteger(v, Key::KEY_E);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F"), -1);
    sq_pushinteger(v, Key::KEY_F);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("G"), -1);
    sq_pushinteger(v, Key::KEY_G);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("H"), -1);
    sq_pushinteger(v, Key::KEY_H);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("I"), -1);
    sq_pushinteger(v, Key::KEY_I);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("J"), -1);
    sq_pushinteger(v, Key::KEY_J);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("K"), -1);
    sq_pushinteger(v, Key::KEY_K);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("L"), -1);
    sq_pushinteger(v, Key::KEY_L);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("M"), -1);
    sq_pushinteger(v, Key::KEY_M);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("N"), -1);
    sq_pushinteger(v, Key::KEY_N);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("O"), -1);
    sq_pushinteger(v, Key::KEY_O);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("P"), -1);
    sq_pushinteger(v, Key::KEY_P);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Q"), -1);
    sq_pushinteger(v, Key::KEY_Q);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("R"), -1);
    sq_pushinteger(v, Key::KEY_R);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("S"), -1);
    sq_pushinteger(v, Key::KEY_S);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("T"), -1);
    sq_pushinteger(v, Key::KEY_T);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("U"), -1);
    sq_pushinteger(v, Key::KEY_U);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("V"), -1);
    sq_pushinteger(v, Key::KEY_V);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("W"), -1);
    sq_pushinteger(v, Key::KEY_W);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("X"), -1);
    sq_pushinteger(v, Key::KEY_X);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Y"), -1);
    sq_pushinteger(v, Key::KEY_Y);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Z"), -1);
    sq_pushinteger(v, Key::KEY_Z);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num1"), -1);
    sq_pushinteger(v, Key::KEY_0);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num2"), -1);
    sq_pushinteger(v, Key::KEY_2);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num3"), -1);
    sq_pushinteger(v, Key::KEY_3);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num4"), -1);
    sq_pushinteger(v, Key::KEY_4);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num5"), -1);
    sq_pushinteger(v, Key::KEY_5);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num6"), -1);
    sq_pushinteger(v, Key::KEY_6);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num7"), -1);
    sq_pushinteger(v, Key::KEY_7);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num8"), -1);
    sq_pushinteger(v, Key::KEY_8);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num9"), -1);
    sq_pushinteger(v, Key::KEY_9);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Num0"), -1);
    sq_pushinteger(v, Key::KEY_0);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Enter"), -1);
    sq_pushinteger(v, Key::KEY_ENTER);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Escape"), -1);
    sq_pushinteger(v, Key::KEY_ESCAPE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Backspace"), -1);
    sq_pushinteger(v, Key::KEY_BACKSPACE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Tab"), -1);
    sq_pushinteger(v, Key::KEY_TAB);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Space"), -1);
    sq_pushinteger(v, Key::KEY_SPACE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Hyphen"), -1);
    sq_pushinteger(v, Key::KEY_HYPHEN);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Equal"), -1);
    sq_pushinteger(v, Key::KEY_EQUAL);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("LeftBracket"), -1);
    sq_pushinteger(v, Key::KEY_LBRACKET);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("RightBracket"), -1);
    sq_pushinteger(v, Key::KEY_RBRACKET);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Backslash"), -1);
    sq_pushinteger(v, Key::KEY_BACKSLASH);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Semicolon"), -1);
    sq_pushinteger(v, Key::KEY_SEMICOLON);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Comma"), -1);
    sq_pushinteger(v, Key::KEY_COMMA);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Period"), -1);
    sq_pushinteger(v, Key::KEY_PERIOD);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Slash"), -1);
    sq_pushinteger(v, Key::KEY_SLASH);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F1"), -1);
    sq_pushinteger(v, Key::KEY_F1);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F2"), -1);
    sq_pushinteger(v, Key::KEY_F2);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F3"), -1);
    sq_pushinteger(v, Key::KEY_F3);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F4"), -1);
    sq_pushinteger(v, Key::KEY_F4);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F5"), -1);
    sq_pushinteger(v, Key::KEY_F5);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F6"), -1);
    sq_pushinteger(v, Key::KEY_F6);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F7"), -1);
    sq_pushinteger(v, Key::KEY_F7);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F8"), -1);
    sq_pushinteger(v, Key::KEY_F8);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F9"), -1);
    sq_pushinteger(v, Key::KEY_F9);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F10"), -1);
    sq_pushinteger(v, Key::KEY_F10);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F11"), -1);
    sq_pushinteger(v, Key::KEY_F11);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F12"), -1);
    sq_pushinteger(v, Key::KEY_F12);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F13"), -1);
    sq_pushinteger(v, Key::KEY_F13);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F14"), -1);
    sq_pushinteger(v, Key::KEY_F14);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F15"), -1);
    sq_pushinteger(v, Key::KEY_F15);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F16"), -1);
    sq_pushinteger(v, Key::KEY_F16);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F17"), -1);
    sq_pushinteger(v, Key::KEY_F17);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F18"), -1);
    sq_pushinteger(v, Key::KEY_F18);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F19"), -1);
    sq_pushinteger(v, Key::KEY_F19);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F20"), -1);
    sq_pushinteger(v, Key::KEY_F20);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F21"), -1);
    sq_pushinteger(v, Key::KEY_F21);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F22"), -1);
    sq_pushinteger(v, Key::KEY_F22);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F23"), -1);
    sq_pushinteger(v, Key::KEY_F23);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("F24"), -1);
    sq_pushinteger(v, Key::KEY_F24);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("CapsLock"), -1);
    sq_pushinteger(v, Key::KEY_CAPSLOCK);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("PrintScreen"), -1);
    sq_pushinteger(v, Key::KEY_PRINTSCREEN);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("ScrollLock"), -1);
    sq_pushinteger(v, Key::KEY_SCROLLLOCK);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Pause"), -1);
    sq_pushinteger(v, Key::KEY_PAUSE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Insert"), -1);
    sq_pushinteger(v, Key::KEY_INSERT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Home"), -1);
    sq_pushinteger(v, Key::KEY_HOME);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("PageUp"), -1);
    sq_pushinteger(v, Key::KEY_PAGEUP);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("PageDown"), -1);
    sq_pushinteger(v, Key::KEY_PAGEDOWN);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Right"), -1);
    sq_pushinteger(v, Key::KEY_RIGHT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Left"), -1);
    sq_pushinteger(v, Key::KEY_LEFT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Down"), -1);
    sq_pushinteger(v, Key::KEY_DOWN);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Up"), -1);
    sq_pushinteger(v, Key::KEY_UP);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumLock"), -1);
    sq_pushinteger(v, Key::KEY_NUMLOCK);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadDivide"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_DIVIDE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadMultiply"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_MULTIPLY);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadMinus"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_MINUS);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadPlus"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_PLUS);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadEqual"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_EQUAL);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadEnter"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_ENTER);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("NumpadDecimal"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_DECIMAL);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad1"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_1);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad2"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_2);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad3"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_3);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad4"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_4);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad5"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_5);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad6"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_6);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad7"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_7);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad8"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_8);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad9"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_9);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Numpad0"), -1);
    sq_pushinteger(v, Key::KEY_NUMPAD_0);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Application"), -1);
    sq_pushinteger(v, Key::KEY_APPLICATION);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Execute"), -1);
    sq_pushinteger(v, Key::KEY_EXECUTE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("LeftCtlr"), -1);
    sq_pushinteger(v, Key::KEY_LCTRL);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("LeftShift"), -1);
    sq_pushinteger(v, Key::KEY_LSHIFT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("LeftAlt"), -1);
    sq_pushinteger(v, Key::KEY_LALT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("LeftSystem"), -1);
    sq_pushinteger(v, Key::KEY_LSYSTEM);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("RightCtlr"), -1);
    sq_pushinteger(v, Key::KEY_RCTRL);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("RightShift"), -1);
    sq_pushinteger(v, Key::KEY_RSHIFT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("RightAlt"), -1);
    sq_pushinteger(v, Key::KEY_RALT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("RightSystem"), -1);
    sq_pushinteger(v, Key::KEY_RSYSTEM);
    sq_newslot(v, -3, SQFalse);
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("Keyboard"), -1);
    sq_newtable(v);

    sq_pushstring(v, _SC("is_down"), -1);
    sq_newclosure(v, squirrel_keyboard_isdown, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("is_pressed"), -1);
    sq_newclosure(v, squirrel_keyboard_ispressed, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("is_released"), -1);
    sq_newclosure(v, squirrel_keyboard_isreleased, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);
}