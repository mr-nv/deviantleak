#pragma once
#include "common.h"
#include "sdk\cclientstate.h"
#include "security/xorstr.h"
namespace ctx
{
	extern CGlobalVarsBase* globals;
	extern IVEngineClient* engine;
	extern IBaseClientDLL* chlclient;
	extern IClientEntityList* entitylist;
	extern IPanel* vguipanel;
	extern ISurface* isurface;
	extern IEngineTrace* enginetrace;
	extern IVDebugOverlay* debugoverlay;
	extern IVModelInfoClient* modelinfo;
	extern ICvar* cvar;
	extern IPhysicsSurfaceProps* physsurf;
	extern IMoveHelper* movehepler;
	extern IPrediction* prediction;
	extern CGameMovement* gamemovement; 
	extern CClientState* clientstate;
	extern IMaterialSystem* matsys;
	extern IVModelRender* mdlrender;
	extern IVRenderView* renderview;
	extern glowobjectmanager *glowobjmnj;
	void initialize();
	bool createfonts();
}
