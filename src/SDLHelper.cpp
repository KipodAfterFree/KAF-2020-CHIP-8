#include "SDLHelper.h"

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

static constexpr int SDL_SUCCESS = 0;

static constexpr int SDL_FIRST_RENDERER = -1;

void SDLHelper::init(int flags)
{
    if (::SDL_Init(flags) != SDL_SUCCESS)
    {
        throw std::runtime_error("Could not initialize SDL: " + SDLHelper::get_error());
    }
}

void SDLHelper::quit()
{
    ::SDL_Quit();
}

std::string SDLHelper::get_error()
{
    return ::SDL_GetError();
}

SDLHelper::ptr<SDLHelper::window>
SDLHelper::create_window(const std::string &title, int x, int y, int w, int h, int flags)
{
    auto window = ::SDL_CreateWindow(
            title.c_str(),
            x, y, w, h,
            flags
    );

    if (!window)
    {
        throw std::runtime_error("Could not create a SDL window: " + SDLHelper::get_error());
    }

    return SDLHelper::ptr<SDLHelper::window>(window, SDLHelper::destroy_window);
}

void SDLHelper::destroy_window(window *window)
{
    ::SDL_DestroyWindow(window);
}

SDLHelper::ptr<SDLHelper::renderer> SDLHelper::create_renderer(window *window, int flags)
{
    auto renderer = ::SDL_CreateRenderer(
            window,
            SDL_FIRST_RENDERER,
            flags
    );

    if (!renderer)
    {
        throw std::runtime_error("Could not create a SDL renderer: " + SDLHelper::get_error());
    }

    return SDLHelper::ptr<SDLHelper::renderer>(renderer, SDLHelper::destroy_renderer);
}

void SDLHelper::destroy_renderer(renderer *renderer)
{
    ::SDL_DestroyRenderer(renderer);
}

void SDLHelper::render_clear(renderer *renderer)
{
    auto status = ::SDL_RenderClear(renderer);

    if (status != SDL_SUCCESS)
    {
        throw std::runtime_error("Could not clear renderer: " + SDLHelper::get_error());
    }
}

void SDLHelper::render_present(renderer *renderer)
{
    ::SDL_RenderPresent(renderer);
}

void SDLHelper::render_copy(renderer *renderer, texture *texture, rect *src, rect *dest)
{
    auto status = ::SDL_RenderCopy(
            renderer,
            texture,
            src,
            dest
    );

    if (status != SDL_SUCCESS)
    {
        throw std::runtime_error("Could not copy a portion of the texture to the renderer: " + SDLHelper::get_error());
    }
}

void SDLHelper::render_set_logical_size(renderer *renderer, int w, int h)
{
    auto status = ::SDL_RenderSetLogicalSize(
            renderer,
            w, h
    );

    if (status != SDL_SUCCESS)
    {
        throw std::runtime_error("Could not set renderer's logical size: " + SDLHelper::get_error());
    }
}

SDLHelper::ptr<SDLHelper::texture> SDLHelper::create_texture(renderer *renderer, int format, int access, int w, int h)
{
    auto texture = ::SDL_CreateTexture(
            renderer,
            format,
            access,
            w, h
    );

    if (!texture)
    {
        throw std::runtime_error("Could not create a texture: " + SDLHelper::get_error());
    }

    return SDLHelper::ptr<SDLHelper::texture>(texture, SDLHelper::destroy_texture);
}

void SDLHelper::destroy_texture(texture *texture)
{
    ::SDL_DestroyTexture(texture);
}

void SDLHelper::update_texture(texture *texture, rect *rect, const void *pixels, int pitch)
{
    auto status = ::SDL_UpdateTexture(
            texture,
            rect,
            pixels,
            pitch
    );

    if (status != SDL_SUCCESS)
    {
        throw std::runtime_error("Could not update texture: " + SDLHelper::get_error());
    }
}

bool SDLHelper::poll_event(SDLHelper::event *event)
{
    return ::SDL_PollEvent(event);
}