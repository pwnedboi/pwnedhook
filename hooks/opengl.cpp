/*  opengl.cpp
 *
 *
 */
#include "common.h"
#include "util_sdl.h"

#include "imgui.h"
#include "imgui_impl_sdl_gl2.h"

#include "menu.h"

static uintptr_t  orig_swap_window = NULL;
static uintptr_t* swap_window      = nullptr;

menu_t g_menu;

/*
 *
 *  The actual hook function
 */
void swap_window_hook(SDL_Window* window)
{
    print_hook();
    
    static SDL_GLContext context = NULL;
    static SDL_GLContext orig_context = SDL_GL_GetCurrentContext();
    static void (*orig_swap_window_func)(SDL_Window*)= (void(*)(SDL_Window*))(orig_swap_window);
 
    if(!context)
    {
        context = SDL_GL_CreateContext(window);
        
        ImGui::CreateContext();
        
        ImGui_ImplSdlGL2_Init(window);
        
        g_menu.init();
    }
    
    SDL_GL_MakeCurrent(window, context);
    
    /*
     *  disable vsync
     */
    if(SDL_GL_SetSwapInterval(0) < 0)
        pwnedhook("Unable to disable vsync!");
    
    ImGui_ImplSdlGL2_NewFrame(window);
    
    if(set.menu.open)
    {
        g_menu.render();
        g_menu.move();
    }
    
    // draw the mouse
    ImGui::GetIO().MouseDrawCursor = set.menu.open;
    
    
    ImGui::Render();
    ImGui_ImplSdlGL2_RenderDrawData(ImGui::GetDrawData());
    orig_swap_window_func(window);
    SDL_GL_MakeCurrent(window, orig_context);
    glFlush();
}

/*
 *
 *  Returns the mach header of the given module
 */
void* get_module_header(const std::string& module)
{
    unsigned int count = _dyld_image_count();
    
    for (int i = 0; i < count; i++)
    {
        mach_header* header = (mach_header*)_dyld_get_image_header(i);
        
        std::string _name = std::string(_dyld_get_image_name(i));
        
        if (_name.empty())
            continue;
        
        string name = string(find_if(_name.rbegin(), _name.rend(), [=](char ch) { return ch == '/'; }).base(), _name.end());
        
        if (name != module)
            continue;
        
        struct stat sb;
        
        if (stat(_name.c_str(), &sb))
            break;
        
        return header;
    }
}

uintptr_t get_abs_addr(uintptr_t addr, uintptr_t ptr, uintptr_t start_offset, uintptr_t size)
{
    uintptr_t sig_addr = ptr + start_offset;
    uintptr_t file_offset = sig_addr - addr;
    uintptr_t offset = *reinterpret_cast<uint32_t*>(sig_addr);
    return addr + (offset + file_offset) + size;
}

/*
 *
 *  Apply the hook
 */
void open_gl_hook()
{
    uintptr_t swap_window_func  = (uintptr_t)dlsym(RTLD_DEFAULT, "SDL_GL_SwapWindow");
    uintptr_t sdl_lib           = (uintptr_t)get_module_header("libSDL2-2.0.0.dylib");
    swap_window                 = (uintptr_t*)get_abs_addr(sdl_lib, swap_window_func, 0xF, 0x4);
    orig_swap_window            = *swap_window;
    *swap_window                = (uintptr_t)&swap_window_hook;
}
