#pragma once

//IO.h, IO.cpp, SDLHelper.h, SDLHelper.cpp, are almost fully copied, with some changes, from https://github.com/omerk2511/chip-8 .
//
//MIT License
//
//Copyright (c) 2020 Omer Katz
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//        of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//        copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//        copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "SDLHelper.h"
#include "types.h"
#include <unordered_map>
#include <cstdio>
#include <iostream>
#include <optional>
#include <array>


constexpr auto PIXEL_WIDTH = 64;
constexpr auto PIXEL_HEIGHT = 32;

static constexpr size_t NUM_PIXELS = PIXEL_WIDTH * PIXEL_HEIGHT;

constexpr auto SIZE_MULTIPLIER = 20;

constexpr auto WINDOW_WIDTH = PIXEL_WIDTH * SIZE_MULTIPLIER;
constexpr auto WINDOW_HEIGHT = PIXEL_HEIGHT * SIZE_MULTIPLIER;

static constexpr size_t KEYPAD_SIZE = 16;

extern const size_t FONT_SPRITE_SIZE;

class IO final
{
public:
    explicit IO(const std::string &windowName);

    ~IO();

    static void beep();

    void clear();

    void draw();

    void pollEvents();

    std::array<bool, NUM_PIXELS> &getBitmap();

    [[nodiscard]] bool isPressed(byte key) const;

    std::optional<byte> getPressedKey();

    [[nodiscard]] bool getExitFlag() const;

private:
    SDLHelper::ptr<SDLHelper::window> _window;
    SDLHelper::ptr<SDLHelper::renderer> _renderer;
    SDLHelper::ptr<SDLHelper::texture> _texture;

    std::array<bool, KEYPAD_SIZE> _keys;
    std::array<bool, NUM_PIXELS> _bitmap;
    bool _exit_flag;
    bool _sdl_disabled = false;
};




