#include "ragebot.h"
#include "../sdk/iconvar.h"
#include "awall.h"
#include "../gamefunc.h"
void ragebot::fix_recoil(c_usercmd* cmd)
{
	auto local_player = deviant.localplayer;

	if (!local_player)
		return;

	float recoil_scale = ctx::cvar->FindVar("weapon_recoil_scale")->GetFloat();
	if (cmd->buttons & IN_ATTACK)
	{
		if (recoil_scale)
		{
			QAngle PunchAngles = local_player->aim_punch();
			QAngle AimAngles = cmd->viewangles;
			AimAngles -= PunchAngles * recoil_scale;
			cmd->viewangles = AimAngles;
		}
	}
}
CMoveData bMoveData[200];

void ragebot::start_prediction(c_usercmd* pCmd)
{
	auto local = deviant.localplayer;
	if (ctx::movehepler && local->alive())
	{
		float curtime = ctx::globals->curtime;
		float frametime = ctx::globals->frametime;
		int iFlags = local->flags();

		ctx::globals->curtime = (float)local->tick_base() * ctx::globals->interval_per_tick;
		ctx::globals->frametime = ctx::globals->interval_per_tick;

		ctx::movehepler->SetHost(local);

		ctx::prediction->SetupMove(local, pCmd, nullptr, bMoveData);
		ctx::gamemovement->ProcessMovement(local, bMoveData);
		ctx::prediction->FinishMove(local, pCmd, bMoveData);

		ctx::movehepler->SetHost(0);

		ctx::globals->curtime = curtime;
		ctx::globals->frametime = frametime;
		local->flags() = iFlags;
	}
}