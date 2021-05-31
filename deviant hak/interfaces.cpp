#include "interfaces.h"
#include "main.h"
#include "security\xorstr.h"
namespace ctx
{
	CGlobalVarsBase* globals;
	IVEngineClient* engine;
	IBaseClientDLL* chlclient;
	IClientEntityList* entitylist;
	IEngineTrace* enginetrace;
	IPanel* vguipanel;
	ISurface* isurface;
	IVDebugOverlay* debugoverlay;
	IVModelInfoClient* modelinfo;
	ICvar* cvar;
	IPhysicsSurfaceProps* physsurf;
	IMoveHelper* movehepler;
	IPrediction* prediction;
	CGameMovement* gamemovement;
	CClientState* clientstate;
	IMaterialSystem* matsys;
	IVModelRender* mdlrender;
	IVRenderView* renderview;
	glowobjectmanager *glowobjmnj;
	fn_createinterface get_module_factory(HMODULE module)
	{
		return reinterpret_cast<fn_createinterface>(GetProcAddress(module, xorstr("CreateInterface")));
	}

	template<typename T>
	T* get_interface(fn_createinterface f, const char* szInterfaceVersion)
	{
		auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

		if (!result) {
			throw std::runtime_error(std::string(xorstr("error to get interface:")) + szInterfaceVersion);
		}

		return result;
	}
	bool createfonts()
	{
		deviant.visualsfont = isurface->CreateFont_();
		deviant.weapfont = isurface->CreateFont_();
		isurface->SetFontGlyphSet(deviant.visualsfont, xorstr("Verdana"), 12, 700, 0, 0, FONTFLAG_OUTLINE);
		isurface->SetFontGlyphSet(deviant.weapfont, xorstr("Courier New"), 13, 560, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		return true;
	}
	void initialize()
	{
		auto engineFactory = get_module_factory(GetModuleHandleW(xorstr(L"engine.dll")));
		auto clientFactory = get_module_factory(GetModuleHandleW(xorstr(L"client.dll")));
		auto matSysFactory = get_module_factory(GetModuleHandleW(xorstr(L"materialsystem.dll")));
		auto clientmdl = GetModuleHandleW(xorstr(L"client.dll"));
		auto enginemdl = GetModuleHandleW(xorstr(L"engine.dll"));
		auto vgui2Factory = get_module_factory(GetModuleHandleW(xorstr(L"vgui2.dll")));
		auto vguiFactory = get_module_factory(GetModuleHandleW(xorstr(L"vguimatsurface.dll")));
		auto valveStdFactory = get_module_factory(GetModuleHandleW(xorstr(L"vstdlib.dll")));
		auto vphysicsFactory = get_module_factory(GetModuleHandleW(xorstr(L"vphysics.dll")));
		chlclient = get_interface<IBaseClientDLL>(clientFactory, xorstr("VClient018"));
		engine = get_interface<IVEngineClient>(engineFactory, xorstr("VEngineClient014"));
		clientstate = **(CClientState***)(memory::find_pattern(enginemdl, xorstr("A1 ? ? ? ? 8B 80 ? ? ? ? C3")) + 1);
		entitylist = get_interface<IClientEntityList>(clientFactory, xorstr("VClientEntityList003"));
		globals = **(CGlobalVarsBase***)(memory::find_pattern(clientmdl, xorstr("A1 ? ? ? ? 5E 8B 40 10")) + 1);
		vguipanel = get_interface<IPanel>(vgui2Factory, xorstr("VGUI_Panel009"));
		isurface = get_interface<ISurface>(vguiFactory, xorstr("VGUI_Surface031"));
		enginetrace = get_interface<IEngineTrace>(engineFactory, xorstr("EngineTraceClient004"));
		debugoverlay = get_interface<IVDebugOverlay>(engineFactory, xorstr("VDebugOverlay004"));
		cvar = get_interface<ICvar>(valveStdFactory, xorstr("VEngineCvar007"));
		matsys = get_interface<IMaterialSystem>(matSysFactory, xorstr("VMaterialSystem080"));
		renderview = get_interface<IVRenderView>(engineFactory, xorstr("VEngineRenderView014"));
		physsurf = get_interface<IPhysicsSurfaceProps>(vphysicsFactory, xorstr("VPhysicsSurfaceProps001"));
		mdlrender = get_interface<IVModelRender>(engineFactory, xorstr("VEngineModel016"));
		modelinfo = get_interface<IVModelInfoClient>(engineFactory, xorstr("VModelInfoClient004"));
		prediction = get_interface<IPrediction>(clientFactory, xorstr("VClientPrediction001"));
		gamemovement = get_interface<CGameMovement>(clientFactory, xorstr("GameMovement001"));
		movehepler = **(IMoveHelper***)(memory::find_pattern(clientmdl, xorstr("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01")) + 2);
		glowobjmnj = *(glowobjectmanager**)(memory::find_pattern(clientmdl, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		deviant.localplayer = *(localplayer_t*)(memory::find_pattern(clientmdl, xorstr("8B 0D ? ? ? ? 83 FF FF 74 07")) + 2);

		createfonts();
	}
}