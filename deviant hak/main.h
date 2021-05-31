#pragma once
#include "c_baseentity.h"
class localplayer_t
{
	friend bool operator==(const localplayer_t& lhs, void* rhs);
public:
	localplayer_t() : m_local(nullptr) {}

	operator bool() const { return *m_local != nullptr; }
	operator C_BasePlayer*() const { return *m_local; }

	C_BasePlayer* operator->() { return *m_local; }

private:
	C_BasePlayer * * m_local;
};

extern localplayer_t localplayer;
class deviant_t
{
public:
	// misc stuff
	WNDPROC m_wndproc;
	HWND m_hwnd;
	hook_t* m_device;
	hook_t* m_vguipanel_hook;
	hook_t* m_clientmode;
	hook_t* m_netchannel;
	hook_t* m_renderview;
	hook_t* m_isurface;
	HINSTANCE m_instance;
	localplayer_t localplayer;
	vgui::HFont visualsfont;
	vgui::HFont weapfont;
	c_usercmd usercmd;
};

extern deviant_t deviant;
