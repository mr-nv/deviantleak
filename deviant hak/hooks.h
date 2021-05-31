#pragma once
#include "common.h"
#include "security\xorstr.h"
using fn_present = long( __stdcall* )( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
using fn_reset = long( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
using fn_painttraverse = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
typedef void(__stdcall* fn_framestagenotify)(ClientFrameStage_t);
typedef int(__thiscall* fn_sendatagram)(INetChannel*, void*);
typedef void(__thiscall *fn_sceneend)(void*);
using fn_create_move = bool(__stdcall*)(float, c_usercmd*);
typedef void(__thiscall* fn_onscreensizechanged)(ISurface*, int, int);
//extern fn_onscreensizechanged originalOnScreenSizeChanged;

namespace hooked {
	void initialize();
	long __stdcall present( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
	long __stdcall reset( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
	long __stdcall wndproc( const HWND, UINT, WPARAM, LPARAM );
	bool __stdcall create_move(float input_sample_time, c_usercmd* cmd);
	void  __stdcall framestagenotify(ClientFrameStage_t curStage);
	void __stdcall painttraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	int __fastcall senddatagram(INetChannel* thisptr, void* edx, void* datagram);
	void __fastcall scene_end(void* thisptr, void* edx);
	void __stdcall onscreensizechanged(int oldWidth, int oldHeight);
};