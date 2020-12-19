#include "IO.h"

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

static const std::unordered_map<SDLHelper::key_code, size_t> KEYMAP = {
        {SDLK_x, 0x00},
        {SDLK_1, 0x01},
        {SDLK_2, 0x02},
        {SDLK_3, 0x03},
        {SDLK_q, 0x04},
        {SDLK_w, 0x05},
        {SDLK_e, 0x06},
        {SDLK_a, 0x07},
        {SDLK_s, 0x08},
        {SDLK_d, 0x09},
        {SDLK_z, 0x0a},
        {SDLK_c, 0x0b},
        {SDLK_4, 0x0c},
        {SDLK_r, 0x0d},
        {SDLK_f, 0x0e},
        {SDLK_v, 0x0f}
};

const size_t FONT_SPRITE_SIZE = 5;

IO::IO(const std::string &windowName)
        : _window{}, _renderer{}, _texture{}, _bitmap{0}, _keys{false}, _exit_flag{false}
{
    static bool SDLInitted = false;
    if (!SDLInitted)
    {
        try
        {
            SDLHelper::init(SDL_INIT_VIDEO);
        } catch (const std::runtime_error &e)
        {
            //the server would never be able to init and use SDL properly
            std::cout << "[!] Could not initialize SDL, Pwn Lion can review based on beeps alone anyway" << std::endl;
            _sdl_disabled = true;
            return;
        }
    }

    _window = SDLHelper::create_window(
            windowName,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
    );

    _renderer = SDLHelper::create_renderer(
            _window.get(),
            0
    );

    SDLHelper::render_set_logical_size(
            _renderer.get(),
            WINDOW_WIDTH,
            WINDOW_HEIGHT
    );

    _texture = SDLHelper::create_texture(
            _renderer.get(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            PIXEL_WIDTH,
            PIXEL_HEIGHT
    );
}

IO::~IO()
{
    if (_sdl_disabled) return;
    SDLHelper::quit();
}

void IO::beep()
{
    //Extremely lazy beep
    putchar(0x7);
}

void IO::clear()
{
    if (_sdl_disabled) return;
    _bitmap.fill(false);
}

void IO::draw()
{
    if (_sdl_disabled) return;
    int pixels[_bitmap.size()];
    std::transform(_bitmap.cbegin(), _bitmap.cend(), pixels,
                   [](bool b) { return b ? /*std::rand()*/ 0xffffffff : 0xff000000; }); //Convert binary pixel to

    SDLHelper::update_texture(
            _texture.get(),
            nullptr,
            pixels,
            PIXEL_WIDTH * sizeof(pixels[0])
    );

    SDLHelper::render_clear(_renderer.get());
    SDLHelper::render_copy(_renderer.get(), _texture.get(), nullptr, nullptr);
    SDLHelper::render_present(_renderer.get());
}

void IO::pollEvents()
{
    if (_sdl_disabled) return;
    SDLHelper::event event;

    while (SDLHelper::poll_event(&event))
    {
        if (event.type == SDL_QUIT)
        {
            _exit_flag = true;
        } else if (event.type == SDL_KEYDOWN)
        {
            if (KEYMAP.contains(event.key.keysym.sym))
            {
                _keys.at(KEYMAP.at(event.key.keysym.sym)) = true;
            }
        } else if (event.type == SDL_KEYUP)
        {
            if (KEYMAP.contains(event.key.keysym.sym))
            {
                _keys.at(KEYMAP.at(event.key.keysym.sym)) = false;
            }
        }
    }
}

std::array<bool, NUM_PIXELS> &IO::getBitmap()
{
    return _bitmap;
}

bool IO::isPressed(byte key) const
{
    if (_sdl_disabled) return false;

    if (key < _keys.size())
    {
        return _keys[key];
    }

    return false;
}

std::optional<byte> IO::getPressedKey()
{
    if (_sdl_disabled) return std::nullopt;

    for (auto keyIndex = 0; keyIndex < _keys.size(); keyIndex++)
    {
        if (_keys[keyIndex])
        {
            _keys[keyIndex] = false;
            return keyIndex;
        }
    }

    return std::nullopt;
}

bool IO::getExitFlag() const
{
    return _exit_flag;
}

