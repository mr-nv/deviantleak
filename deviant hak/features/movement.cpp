#include "movement.h"
#include "../common.h"
#include "../main.h"
#include "../interfaces.h"
#include "../math/math.h"
#include "lagcomp.h"
void movement::bunnyhop(c_usercmd * cmd)
{
	if (!deviant.localplayer || !cmd)
		return;

	if (!(deviant.localplayer->flags() & FL_ONGROUND) && cmd->buttons & IN_JUMP && g_cfg["miscbhop"].get<bool>())
		cmd->buttons &= ~IN_JUMP;

}
float movement::get_move_angle(float speed) 
{
	auto move_angle = RAD2DEG(asin(15.f / speed));

	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;

	return move_angle;
}

bool movement::get_closest_plane(Vector* plane) 
{
	auto local = deviant.localplayer;
	if (!local) return false;
	trace_t trace;
	CTraceFilterWorldOnly filter;
	Ray_t ray;
	auto collideable = local->GetCollideable();
	if (!collideable)
		return false;	

	auto start = local->origin(), mins = collideable->OBBMins(), maxs = collideable->OBBMaxs();

	Vector planes;
	auto count = 0;

	for (auto step = 0.f; step <= PI * 2.f; step += PI / 10.f)
	{
		auto end = start;

		end.x += cos(step) * 64.f;
		end.y += sin(step) * 64.f;
		ray.Init(start, end, mins, maxs);

		ctx::enginetrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		

		if (trace.fraction < 1.f)
		{
			planes += trace.plane.normal;
			count++;
		}
	}

	planes /= count;

	if (planes.z < 0.1f)
	{
		*plane = planes;
		return true;
	}

	return false;
}

bool movement::will_hit_obstacle_in_future(float predict_time, float step) 
{
	auto local = deviant.localplayer;
	if (!local) return false;
	static auto sv_gravity = ctx::cvar->FindVar("sv_gravity");
	static auto sv_jump_impulse = ctx::cvar->FindVar("sv_jump_impulse");

	bool ground = local->flags() & FL_ONGROUND;
	auto gravity_per_tick = sv_gravity->GetFloat() * ctx::globals->interval_per_tick;

	auto start = local->origin(), end = start;
	auto velocity = local->velocity();

	trace_t trace;
	CTraceFilterWorldOnly filter;
	Ray_t ray;
	auto collideable = local->GetCollideable();
	if (!collideable)
		return false;
	auto predicted_ticks_needed = time_to_ticks(predict_time);
	auto velocity_rotation_angle = RAD2DEG(atan2(velocity.y, velocity.x));
	auto ticks_done = 0;

	if (predicted_ticks_needed <= 0)
		return false;

	while (true)
	{
		auto rotation_angle = velocity_rotation_angle + step;

		velocity.x = cos(RAD2DEG(rotation_angle)) * local->velocity().Length2D();
		velocity.y = sin(RAD2DEG(rotation_angle)) * local->velocity().Length2D();
		velocity.z = ground ? sv_jump_impulse->GetFloat() : velocity.z - gravity_per_tick;

		end += velocity * ctx::globals->interval_per_tick;
		ray.Init(start, end, collideable->OBBMins(), collideable->OBBMaxs());

		ctx::enginetrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		if (trace.fraction != 1.f && trace.plane.normal.z <= 0.9f || trace.startsolid || trace.allsolid)
			break;

		end = trace.endpos;
		end.z -= 2.f;
		ray.Init(trace.endpos, end, collideable->OBBMins(), collideable->OBBMaxs());

		ctx::enginetrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);
		ground = (trace.fraction < 1.f || trace.allsolid || trace.startsolid) && trace.plane.normal.z >= 0.7f;

		if (++ticks_done >= predicted_ticks_needed)
			return false;

		velocity_rotation_angle = rotation_angle;
	}

	return true;
}

void movement::circle_strafe(c_usercmd* cmd, float* circle_yaw)
{
	auto local = deviant.localplayer;
	if (!local) return;
	const auto min_step = 2.25f;
	const auto max_step = 5.f;
	auto velocity_2d = local->velocity().Length2D();

	auto ideal_strafe = std::clamp(get_move_angle(velocity_2d) * 2.f, min_step, max_step);
	auto predict_time = std::clamp(320.f / velocity_2d, 0.35f, 1.f);

	auto step = ideal_strafe;

	while (true)
	{
		if (!will_hit_obstacle_in_future(predict_time, step) || step > max_step)
			break;

		step += 0.2f;
	}

	if (step > max_step)
	{
		step = ideal_strafe;

		while (true)
		{
			if (!will_hit_obstacle_in_future(predict_time, step) || step < -min_step)
				break;

			step -= 0.2f;
		}

		if (step < -min_step)
		{
			Vector plane;
			if (get_closest_plane(&plane))
				step = -math::get().normalizefloat(*circle_yaw -RAD2DEG(atan2(plane.y, plane.x))) * 0.1f;
		}
		else
			step -= 0.2f;
	}
	else
		step += 0.2f;

	deviant.usercmd.viewangles.yaw = *circle_yaw = math::get().normalizefloat(*circle_yaw + step);
	deviant.usercmd.move.y = copysign(450.f, -step);
}

void movement::strafe(c_usercmd* cmd)
{
	if (!g_cfg["miscstrafe"].get<bool>())
	return;
	auto local = deviant.localplayer;
	if (!local) return;
	static auto switch_key = 1.f;
	static auto circle_yaw = 0.f;
	static auto old_yaw = 0.f;

	auto velocity = local->velocity();
	velocity.z = 0.f;

	auto speed = velocity.Length2D();
	auto ideal_strafe = get_move_angle(speed);

	switch_key *= -1.f;

	if (local->movetype() != MOVETYPE_NOCLIP && local->movetype() != MOVETYPE_LADDER && !(local->flags() & FL_ONGROUND) && (deviant.usercmd.buttons & IN_JUMP) && local->alive())
	{
		if (g_cfg["misccirclestrafe"].get<bool>() && GetAsyncKeyState(g_cfg["misckbindstrafe"].get<int>()))
		{
			circle_strafe(cmd, &circle_yaw);
			return;
		}

		if (deviant.usercmd.move.x > 0.f)
			deviant.usercmd.move.x = 0.f;

		auto yaw_delta = math::get().normalizefloat(deviant.usercmd.viewangles.yaw - old_yaw);
		auto absolute_yaw_delta = abs(yaw_delta);

		circle_yaw = old_yaw = deviant.usercmd.viewangles.yaw;

		if (yaw_delta > 0.f)
			deviant.usercmd.move.y = -450.f;
		else if (yaw_delta < 0.f)
			deviant.usercmd.move.y = 450.f;

		if (absolute_yaw_delta <= ideal_strafe || absolute_yaw_delta >= 30.f)
		{
			QAngle velocity_angles;
			math::get().vector_angles(velocity, velocity_angles);

			auto velocity_delta = math::get().normalizefloat(deviant.usercmd.viewangles.yaw - velocity_angles.yaw);
			auto retrack = 2.f * (ideal_strafe * 2.f); 

			if (velocity_delta <= retrack || speed <= 15.f)
			{
				if (-retrack <= velocity_delta || speed <= 15.f)
				{
					deviant.usercmd.viewangles.yaw += ideal_strafe * switch_key;
					deviant.usercmd.move.y = switch_key * 450.f;
				}
				else
				{
					deviant.usercmd.viewangles.yaw = velocity_angles.yaw - retrack;
					deviant.usercmd.move.y = 450.f;
				}
			}
			else
			{
				deviant.usercmd.viewangles.yaw = velocity_angles.yaw + retrack;
				deviant.usercmd.move.y = -450.f;
			}
		}
	}
}
void movement::duckinair(c_usercmd* cmd)
{
	if (!g_cfg["miscduckinair"].get<bool>()) return;
	auto local = deviant.localplayer;
	if (!local || !local->alive()) return;
	if (!(local->flags() & FL_ONGROUND))
		cmd->buttons |= IN_DUCK;
}
Vector movement::fix_movement(c_usercmd* cmd, c_usercmd original_command) const
{
	Vector wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

	auto viewangles = cmd->viewangles;
	auto movedata = original_command.move;
	viewangles.Clamp();

	if (!(deviant.localplayer->flags() & FL_ONGROUND) && viewangles.roll != 0.f)
		movedata.y = 0.f;

	math::get().angle_vectors(original_command.viewangles, &wish_forward, &wish_right, &wish_up);
	math::get().angle_vectors(viewangles, &cmd_forward, &cmd_right, &cmd_up);

	auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

	Vector wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
		wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
		wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

	auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

	Vector cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
		cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
		cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

	auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

	Vector correct_movement;
	correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25
		+ (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28)
		+ (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
	correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25
		+ (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28)
		+ (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
	correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25
		+ (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28)
		+ (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

	correct_movement.x = std::clamp(correct_movement.x, -450.f, 450.f);
	correct_movement.y = std::clamp(correct_movement.y, -450.f, 450.f);
	correct_movement.z = std::clamp(correct_movement.z, -320.f, 320.f);

	return correct_movement;
}
