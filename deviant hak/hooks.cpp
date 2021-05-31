#include "hooks.h"
#include "menu/menu.h"
#include "interfaces.h"
#include "main.h"
#include "features\features.h"
#include <string>
#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)
auto init = false;

void hooked::initialize()
{
	//idk why it intialise in main.cpp
}

long __stdcall hooked::present(IDirect3DDevice9* device, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion) {
	if (!init) {
		render::d3d::get().initialize(device);

		init = true;
	}

	auto colour = g_cfg["menu_colour"].get<uintptr_t>();

	long o_r = get_r(colour);
	long o_g = get_g(colour);
	long o_b = get_b(colour);

	render::d3d::get().begin();
	render::d3d::get().reset();
	render::d3d::get().text("deviant beta", 0, 0, left, fonts::menu_large, true, D3DCOLOR_ARGB(255, 108, 195, 18));

	menu::get().m_device = device;

	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	menu::get().render();

	render::d3d::get().end();

	return deviant.m_device->get_original<fn_present>(17)(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

long __stdcall hooked::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp) {
	if (!init) {
		return deviant.m_device->get_original<fn_reset>(16)(device, pp);
	}

	render::d3d::get().release_objects();

	auto hr = deviant.m_device->get_original<fn_reset>(16)(device, pp);

	render::d3d::get().create_objects();

	return hr;
}

long __stdcall hooked::wndproc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (init && menu::get().active()) {
		return true;
	}

	return CallWindowProcA(deviant.m_wndproc, hWnd, uMsg, wParam, lParam);
}

void __stdcall hooked::painttraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	static auto oPaintTraverse = deviant.m_vguipanel_hook->get_original<fn_painttraverse>(41); // vguipanel_hook.get_original<PaintTraverse>(41)

	oPaintTraverse(ctx::vguipanel, panel, forceRepaint, allowForce);

	if (!panelId) {
		const auto panelName = ctx::vguipanel->GetName(panel);
		if (!strcmp(panelName, xorstr("FocusOverlayPanel"))) {
			panelId = panel;
		}
	}
	else if (panelId == panel) {
		esp::get().draw();
	}
}
bool __stdcall hooked::create_move(float input_sample_time, c_usercmd* cmd)
{
	if (!cmd || !cmd->command_number)
	{
		return deviant.m_clientmode->get_original<fn_create_move>(24)(input_sample_time, cmd);
	}
	bool sendpacket = true;
	deviant.usercmd = *cmd;
	ragebot::get().start_prediction(cmd);
	{
		ragebot::get().fix_recoil(cmd);
	}

	movement::get().bunnyhop(cmd);
	movement::get().strafe(cmd);
	movement::get().duckinair(cmd);
	cmd->move = movement::get().fix_movement(cmd, deviant.usercmd);
	uintptr_t* framePointer;
	__asm mov framePointer, ebp;
	*reinterpret_cast< bool* > (*framePointer - 0x1C) = sendpacket;
	return false;
}

void  __stdcall hooked::framestagenotify(ClientFrameStage_t curStage) {
	if (!ctx::engine->IsConnected() || !ctx::engine->IsInGame() || !deviant.localplayer)
		return;
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
	//	fakelatency->updateincomingsequences();
	}
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		aacorrection::get().corectangles();
	}
		deviant.m_clientmode->get_original<fn_framestagenotify>(36)(curStage);
}
void __fastcall hooked::scene_end(void* thisptr, void* edx) {
	int chamstype = g_cfg["clrdmdlstype"].get<int>();
	auto local = deviant.localplayer;
	if (local) {
		for (int i = 1; i <= ctx::globals->maxClients; i++) {
			auto entity = static_cast<C_BaseEntity*>(ctx::entitylist->GetClientEntity(i));
			if (entity && entity != local && entity->GetClientClass()->m_ClassID == (int)35 && entity->alive() && !entity->dormant() && chamstype != 0) {
				int teamnum = entity->team();
				bool textured = chamstype == 1;
				bool flat = chamstype == 2;
				bool metalic = chamstype == 3;

				if (g_cfg["clrdmdlsenemywall"].get<bool>() && teamnum != local->team()) {
					auto colour = g_cfg["clrdmdlsenemywclr"].get<uintptr_t>();
					auto r = get_r(colour);
					auto g = get_g(colour);
					auto b = get_b(colour);
					Color color = Color(r, g, b);
					chams->override_material(true, textured, flat, metalic, color);
					entity->DrawModel(0x1, entity->GetModelInstance());
					ctx::mdlrender->ForcedMaterialOverride(nullptr);
				}
				else if (g_cfg["clrdmdlsteamwall"].get<bool>() && teamnum == local->team()) {
					auto colour = g_cfg["clrdmdlsteamwclr"].get<uintptr_t>();
					auto r = get_r(colour);
					auto g = get_g(colour);
					auto b = get_b(colour);
					Color color = Color(r, g, b);
					chams->override_material(true, textured, flat, metalic, color);
					entity->DrawModel(0x1, entity->GetModelInstance());
					ctx::mdlrender->ForcedMaterialOverride(nullptr);
				}

				if (g_cfg["clrdmdlsenemy"].get<bool>() && teamnum != local->team()) {
					auto colour = g_cfg["clrdmdlsenemyclr"].get<uintptr_t>();
					auto r = get_r(colour);
					auto g = get_g(colour);
					auto b = get_b(colour);
					Color color = Color(r, g, b);
					chams->override_material(false, textured, flat, metalic, color);
					entity->DrawModel(0x1, entity->GetModelInstance());
					ctx::mdlrender->ForcedMaterialOverride(nullptr);
				}
				else if (g_cfg["clrdmdlsteam"].get<bool>() && teamnum == local->team()) {
					auto colour = g_cfg["clrdmdlsteamclr"].get<uintptr_t>();
					auto r = get_r(colour);
					auto g = get_g(colour);
					auto b = get_b(colour);
					Color color = Color(r, g, b);
					chams->override_material(false, textured, flat, metalic, color);
					entity->DrawModel(0x1, entity->GetModelInstance());
					ctx::mdlrender->ForcedMaterialOverride(nullptr);
				}
			}
			glow::get().work();
		}
	}
	deviant.m_renderview->get_original<fn_sceneend>(9)(thisptr);
}

int __fastcall hooked::senddatagram(INetChannel* netchan, void* edx, void* datagram) {
	if (!ctx::engine->IsInGame() || !ctx::engine->IsConnected() || datagram)
		return deviant.m_netchannel->get_original<fn_sendatagram>(48)(netchan, datagram);

	//fakelatency->m_netchan = netchan;

	auto instate = netchan->m_nInReliableState;
	auto in_sequencenr = netchan->m_nInSequenceNr;

	auto lag_s = 300.f / 1000.f;
	auto lag_delta = lag_s - ctx::engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

	//fakelatency->addlatencytonetchan(netchan, lag_delta);

	int ret = deviant.m_netchannel->get_original<fn_sendatagram>(48)(netchan, datagram);

	netchan->m_nInReliableState = instate;
	netchan->m_nInSequenceNr = in_sequencenr;
	return ret;
}
void __stdcall  hooked::onscreensizechanged(int oldWidth, int oldHeight)
{
	deviant.m_isurface->get_original<fn_onscreensizechanged>(116)(ctx::isurface, oldWidth, oldHeight);
	ctx::createfonts();
}