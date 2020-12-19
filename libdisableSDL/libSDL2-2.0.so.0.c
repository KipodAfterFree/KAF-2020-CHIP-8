__attribute__ ((visibility("default"))) int SDL_Init(int flags) {
    return 0x1337;
}

void SDL_Quit() {}

const char *SDL_GetError() { return "AAA"; }

void *SDL_CreateWindow(const char *title,
                       int x, int y, int w,
                       int h, unsigned int flags) { return 0; }

void SDL_DestroyWindow(void *window) {}

void *SDL_CreateRenderer(void *window, int index, unsigned int flags) { return 0; }

void SDL_DestroyRenderer(void *renderer) {}

int SDL_RenderClear(void *renderer) { return 0x41414141; }

void SDL_RenderPresent(void *renderer) {}

int SDL_RenderCopy(void *renderer,
                   void *texture,
                   const void *srcrect,
                   const void *dstrect) { return 0x41414141; }

int SDL_RenderSetLogicalSize(void *renderer, int w, int h) { return 0x41414141; }

void *SDL_CreateTexture(void *renderer,
                        unsigned int format,
                        int access, int w,
                        int h) { return 0; }

void SDL_DestroyTexture(void *text) {}

int SDL_UpdateTexture(void *texture,
                      const void *rect,
                      const void *pixels, int pitch) { return 0x41414141; }

int SDL_PollEvent(void *event) { return 0; }