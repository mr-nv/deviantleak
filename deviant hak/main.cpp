#include "common.h"
#include "hooks.h"
#include "interfaces.h"
#include "main.h"
#include "netvar.h"
#include "features\chams.h"
#include "features\glow.h"
void cheat_thread( );
deviant_t deviant;
bool __stdcall DllMain( HINSTANCE instance, unsigned long reason, void *reserved ) {
	DisableThreadLibraryCalls( instance );

	if ( reason == 1 ) {
		deviant.m_instance = instance;
		CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE ) cheat_thread, 0, 0, 0 );
	}

	return true;
}


void cheat_thread( ) {

	while ( !(deviant.m_hwnd = FindWindowA(xorstr("Valve001"), 0 ) ) ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	while ( !GetModuleHandleA(xorstr("serverbrowser.dll") ) ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}
	ctx::initialize();
	netvar = new netvar_t();
	netvar->get();
	chams = new Chams;
	deviant.m_wndproc = ( WNDPROC ) SetWindowLongA(deviant.m_hwnd, -4, ( long ) hooked::wndproc );

	auto device = **reinterpret_cast< void*** >( memory::occurence(xorstr("shaderapidx9.dll"), xorstr("88 56 10" )) + 17 );
	auto clientmode = **(uintptr_t***)((*(uintptr_t**)ctx::chlclient)[10] + 0x5);

	deviant.m_device = new hook_t( device );
	deviant.m_device->hook_index( hooked::reset, 16 );
	deviant.m_device->hook_index( hooked::present, 17 );
	deviant.m_vguipanel_hook = new hook_t((uintptr_t**)ctx::vguipanel);
	deviant.m_vguipanel_hook->hook_index(hooked::painttraverse,41);
	deviant.m_clientmode = new hook_t(clientmode);
	deviant.m_clientmode->hook_index(hooked::create_move, 24);
	deviant.m_clientmode->hook_index(hooked::framestagenotify, 36);
	deviant.m_renderview = new hook_t(ctx::renderview);
	deviant.m_renderview->hook_index(hooked::scene_end, 9);
	deviant.m_isurface = new hook_t(ctx::isurface);
	deviant.m_isurface->hook_index(hooked::onscreensizechanged, 116);
	while (!ctx::engine->IsInGame() || !ctx::engine->IsConnected() || !ctx::clientstate->m_NetChannel)
		Sleep(1000);
		deviant.m_netchannel = new hook_t((uintptr_t**)ctx::clientstate->m_NetChannel);
		deviant.m_netchannel->hook_index(hooked::senddatagram, 48);
	
	// signal initialization 
	Beep( 495, 100 );

	// wait for uninjection
	while ( !GetAsyncKeyState( VK_END ) ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	SetWindowLongPtr(deviant.m_hwnd, GWL_WNDPROC, ( LONG_PTR )deviant.m_wndproc );

	// unload
	chams->~Chams();
	glow::get().clear();
	deviant.m_device->unhook_index( 16 );
	deviant.m_device->unhook_index( 17 );
	deviant.m_device->unhook_index( 41 );
	deviant.m_device->unhook_index( 24 );
	deviant.m_device->unhook_index( 36 );
	deviant.m_device->unhook_index( 9 );
	std::this_thread::sleep_for( std::chrono::seconds( 2 ) );

	FreeLibraryAndExitThread(deviant.m_instance, 0 );
}