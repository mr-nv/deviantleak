#pragma once
#include "common.h"
#include "c_baseentity.h"
#include "math\math.h"
#include "interfaces.h"
#include "singleton.h"
class gamefunc : public singleton<gamefunc>
{
public:
	bool onscreen(Vector origin, Vector& screen)
	{
		if (!render::get().worldtoscreen(origin, screen)) return false;
		int iScreenWidth, iScreenHeight;
		ctx::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
		return xOk && yOk;
	}
	bool can_shoot()
	{
		auto m_local = deviant.localplayer;
		if (!m_local) return false;

		auto m_weapon = m_local->activeweapon().Get();
		if (!m_local)
			return false;

		if (!m_weapon)
			return false;

		float flServerTime = m_local->tick_base() * ctx::globals->interval_per_tick;

		return (m_weapon->next_attack() <= flServerTime);
	}
Vector get_hitbox_location(C_BaseEntity* obj, int hitbox_id) {
	matrix3x4_t bone_matrix[128];

	if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
		if (obj->GetModel()) {
			auto studio_model = ctx::modelinfo->GetStudiomodel(obj->GetModel());
			if (studio_model) {
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
				if (hitbox) {
					auto min = Vector{}, max = Vector{};

					math::get().vectortransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
					math::get().vectortransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
	}
	return Vector{};
}
void trace_line(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)memory::find_pattern("client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
bool visible(C_BaseEntity* local, C_BaseEntity* entity, int BoneID)
{
	static trace_t trace;
	static C_BaseEntity* ent_cpy;

	if (BoneID < 0) return false;

	ent_cpy = entity;
	Vector start = local->origin() + local->view_offset();
	Vector end = get_hitbox_location(entity, 0);

	trace_line(start, end, MASK_SOLID, local, 0, &trace);

	if (trace.hit_entity == ent_cpy)
	{
		return true;
	}

	if (trace.fraction == 1.0f)
	{
		return true;
	}

	return false;
}
};