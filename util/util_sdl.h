/*
 * util_sdl.h
 */
#pragma once
#define GL_SILENCE_DEPRECATION

#include <sys/types.h>
#include <mach/mach.h>
#include <mach/error.h>
#include <mach-o/dyld.h>
#include <mach/vm_types.h>
#include <mach-o/getsect.h>

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>

Uint32 SDLCALL SDL_GetTicks(void);
Uint32 SDLCALL SDL_GetWindowFlags(SDL_Window* window);
Uint32 SDLCALL SDL_GetMouseState(int* x, int* y);
Uint64 SDLCALL SDL_GetPerformanceFrequency(void);
Uint64 SDLCALL SDL_GetPerformanceCounter(void);
const Uint8* SDLCALL SDL_GetKeyboardState(int* numkeys);
int SDLCALL SDL_ShowCursor(int toggle);
int SDLCALL SDL_SetClipboardText(const char* text);
int SDLCALL SDL_GL_MakeCurrent(SDL_Window* window, SDL_GLContext context);
int SDLCALL SDL_CaptureMouse(SDL_bool enabled);
int SDLCALL SDL_PollEvent(SDL_Event* event);
int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);
int SDLCALL SDL_SetRelativeMouseMode(SDL_bool enabled);
int SDLCALL SDL_Init(Uint32 flags);
int SDLCALL SDL_GL_SetSwapInterval(int interval);
void SDLCALL SDL_GetWindowSize(SDL_Window* window, int* w, int* h);
void SDLCALL SDL_WarpMouseInWindow(SDL_Window* window, int x, int y);
void SDLCALL SDL_GL_GetDrawableSize(SDL_Window* window, int* w, int* h);
void SDLCALL SDL_SetCursor(SDL_Cursor* cursor);
void SDLCALL SDL_FreeCursor(SDL_Cursor* cursor);
char* SDLCALL SDL_GetClipboardText(void);
SDL_bool SDLCALL SDL_SetHint(const char* name, const char* value);
SDL_Keymod SDLCALL SDL_GetModState(void);
SDL_Cursor* SDLCALL SDL_CreateSystemCursor(SDL_SystemCursor id);
SDL_GLContext SDLCALL SDL_GL_GetCurrentContext(void);
SDL_GLContext SDLCALL SDL_GL_CreateContext(SDL_Window* window);
