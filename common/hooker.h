/*
 * hooker.h
 */
#pragma once

void cheat_init();
void cheat_start();

#define SIG_ANIM_STATE_OFFSET (u_char*)"\x48\x8B\xBB\x00\x00\x00\x00\x48\x85\xFF\x74\x41\xE8\x00\x00\x00\x00\x4C"
#define MSK_ANIM_STATE_OFFSET "xxx????xxxxxx????x"

#define SIG_CLIENTMODE (u_char*)"\x48\x8B\xB7\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x5D\xE9"
#define MSK_CLIENTMODE "xxx????xxx????xx"

#define SIG_GLOBALVARS (u_char*)"\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\x00\xF3\x0F\x10\x00\x00\xF3\x0F\x11\x83\x00\x00\x00\x00"
#define MSK_GLOBALVARS "xxx????xxxxxx??xxxx????"

#define SIG_CLIENTSTATE (u_char*)"\x55\x48\x89\xE5\x48\x8B\x00\x00\x00\x00\x00\x48\x83\x00\x00\x5D\xC3\x66\x66\x66\x66\x66\x66\x2E\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define MSK_CLIENTSTATE "xxxxxx?????xx??xxxxxxxxxxxxxxxxx"

#define SIG_SENDPACKET (u_char*)"\x41\xB5\x00\x84\xC0\x74\x11"
#define MSK_SENDPACKET "xx?xxxx"

#define SIG_GAMERULES // PatternScanner->GetPointer("client_panorama.dylib", "49 89 FE 48 8D 05 ? ? ? ? 48 8B 38 48 8B 07", 0x6) + 0x4

#define SIG_SDL_MANAGER // PatternScanner->GetProcedure("launcher.dylib", "53 48 8B 1D ? ? ? ? 48 85 DB 75 1C") - 0x6
