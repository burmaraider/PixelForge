#ifndef PF_SDL2_COMMON_H
#define PF_SDL2_COMMON_H

#ifdef PF_SDL2_COMMON_H
#   define PF_COMMON_IMPL
#endif //PF_SDL2_COMMON_H

#include "../common.h"
#include <SDL2/SDL.h>

/* Pixel getter/setter */

PFcolor PF_GetPixel(const void* pixels, PFsizei offset);
void PF_SetPixel(void* pixels, PFsizei offset, PFcolor color);

/* Window management */

typedef struct {
    SDL_Window *sdlWin;
    SDL_Surface *surface;
} Window;

Window Window_Create(const char* title, int x, int y, int w, int h, Uint32 flags);
void Window_Destroy(Window* window);
void Window_Update(Window* window);

/* Clock managment */

typedef struct {
    Uint32 ticksAtLastFrame;
    float deltaTime;
    Uint32 maxFPS;
} Clock;

Clock Clock_Create(Uint32 maxFPS);
void Clock_Begin(Clock* clock);
void Clock_End(Clock* clock);

/* PixelForge context management */

PFctx* PF_InitFromWindow(Window* window);


/* Implementation */

#ifdef PF_SDL2_COMMON_IMPL

/* Pixel getter/setter */

PFcolor PF_GetPixel(const void* pixels, PFsizei offset)
{
    return (PFcolor) {
        ((PFubyte*)pixels)[offset*4 + 2],
        ((PFubyte*)pixels)[offset*4 + 1],
        ((PFubyte*)pixels)[offset*4],
        ((PFubyte*)pixels)[offset*4 + 3]
    };
}

void PF_SetPixel(void* pixels, PFsizei offset, PFcolor color)
{
    ((PFubyte*)pixels)[offset*4] = color.b;
    ((PFubyte*)pixels)[offset*4 + 1] = color.g;
    ((PFubyte*)pixels)[offset*4 + 2] = color.r;
    ((PFubyte*)pixels)[offset*4 + 3] = color.a;
}

/* Window management */

static int windowCounter = 0;

Window Window_Create(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    // Initializing SDL
    if (windowCounter++ == 0 && SDL_VideoInit(NULL) < 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Creating the window
    SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window == NULL)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Getting the window area
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (surface == NULL)
    {
        fprintf(stderr, "Error getting window area: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    return (Window) {
        .sdlWin = window,
        .surface = surface
    };
}

void Window_Destroy(Window* window)
{
    SDL_DestroyWindow(window->sdlWin);
    window->surface = NULL;
    window->sdlWin = NULL;

    if (--windowCounter == 0)
    {
        SDL_VideoQuit();
    }
}

void Window_Update(Window* window)
{
    SDL_UpdateWindowSurface(window->sdlWin);
}

/* Clock managment */

Clock Clock_Create(Uint32 maxFPS)
{
    return (Clock) {
        .ticksAtLastFrame = SDL_GetTicks(),
        .deltaTime = 0.0f,
        .maxFPS = maxFPS
    };
}

void Clock_Begin(Clock* clock)
{
    clock->ticksAtLastFrame = SDL_GetTicks();
}

void Clock_End(Clock* clock)
{
    Uint32 ticksAtThisFrame = SDL_GetTicks();
    Uint32 ticksSinceLastFrame = ticksAtThisFrame - clock->ticksAtLastFrame;
    float targetDeltaTime = 1000.0f / clock->maxFPS;

    if (ticksSinceLastFrame < targetDeltaTime)
    {
        SDL_Delay((Uint32)(targetDeltaTime - ticksSinceLastFrame));
    }

    clock->ticksAtLastFrame = SDL_GetTicks();
    clock->deltaTime = (clock->ticksAtLastFrame - ticksAtThisFrame) / 1000.0f;
}

/* PixelForge context management */

PFctx* PF_InitFromWindow(Window* window)
{
    PFctx *ctx = PF_Init(window->surface->pixels, window->surface->w, window->surface->h);
    pfSetDefaultPixelGetter(PF_GetPixel);
    pfSetDefaultPixelSetter(PF_SetPixel);
    return ctx;
}

#endif //PF_SDL2_COMMON_IMPL
#endif //PF_SDL2_COMMON_H