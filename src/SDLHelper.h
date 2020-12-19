#pragma once

//IO.h, IO.cpp, SDLHelper.h, SDLHelper.cpp, are almost fully copied, with minor changes, from https://github.com/omerk2511/chip-8 .
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

#include <SDL.h>
#include <memory>
#include <functional>
#include <stdexcept>
#include <string>

namespace SDLHelper
{
    template<typename T>
    using ptr = std::unique_ptr<T, std::function<void(T *)>>;

    using window = SDL_Window;
    using renderer = SDL_Renderer;
    using texture = SDL_Texture;
    using rect = SDL_Rect;
    using event = SDL_Event;
    using key_code = SDL_Keycode;

    void init(int flags);

    void quit();

    std::string get_error();

    ptr<window> create_window(const std::string &title, int x, int y, int w, int h, int flags);

    void destroy_window(window *window);

    ptr<renderer> create_renderer(window *window, int flags);

    void destroy_renderer(renderer *renderer);

    void render_clear(renderer *renderer);

    void render_present(renderer *renderer);

    void render_copy(renderer *renderer, texture *texture, rect *src, rect *dest);

    void render_set_logical_size(renderer *renderer, int w, int h);

    ptr<texture> create_texture(renderer *renderer, int format, int access, int w, int h);

    void destroy_texture(texture *texture);

    void update_texture(texture *texture, rect *rect, const void *pixels, int pitch);

    bool poll_event(SDLHelper::event *event);
}