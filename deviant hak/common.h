#pragma once
#include <Windows.h>
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <Psapi.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <assert.h>

// ================================================
//  d3d9 header & lib imports
// ================================================

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// ================================================
//  util imports
// ================================================

#include "hook.h"

// ================================================
//  util functions
// ================================================

#include "singleton.h"
#include "context.h"
#include "hook.h"
#include "singleton.h"
#include "render.h"
#include "Winbase.h"
#include "sdk\cclientstate.h"
#include "sdk\cinput.h"
#include "sdk\iappsystem.h"
#include "sdk\ibaseclientdll.h"
#include "sdk\icliententity.h"
#include "sdk\icliententitylist.h"
#include "sdk\iclientmode.h"
#include "sdk\iclientnetworkable.h"
#include "sdk\iclientrenderable.h"
#include "sdk\iclientthinkable.h"
#include "sdk\iclientunknown.h"
#include "sdk\icollideable.h"
#include "sdk\iconvar.h"
#include "sdk\icvar.h"
#include "sdk\ienginesound.h"
#include "sdk\ienginetrace.h"
#include "sdk\igameeventmanager.h"
#include "sdk\imaterialsystem.h"
#include "sdk\imdlcache.h"
#include "sdk\imovehelper.h"
#include "sdk\ipanel.h"
#include "sdk\iphysics.h"
#include "sdk\iprediction.h"
#include "sdk\irenderview.h"
#include "sdk\isurface.h"
#include "sdk\ivdebugoverlay.h"
#include "sdk\ivengineclient.h"
#include "sdk\iviewrender.h"
#include "sdk\ivmodelinfoclient.h"
#include "sdk\ivmodelrender.h"
#include "sdk\cusercmd.h"
#include "sdk\globals.h"
#include "sdk\ihandleentity.h"
#include "sdk\studio.h"
#include "misc\color.h"
#include "sdk\clientclass.h"
#include "sdk\recv.h"
#include "sdk\enums.h"
#include "sdk\glowobjmng.hpp"
#include "misc\utlvector.h"
#include "misc\utlmemory.h"
#include "misc\platform.h"
#include "menu\config.h"
namespace memory {
	uintptr_t occurence( std::string module, std::string pattern );
	std::uint8_t* find_pattern(void* module, const char* signature);
	DWORD find_pattern2(std::string moduleName, BYTE* Mask, char* szMask);
//	uintptr_t find_pattern(const uintptr_t& start_address, const uintptr_t& end_address, const char* target_pattern);
};

