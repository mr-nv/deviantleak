#include "glow.h"
#include "../c_baseentity.h"
#include "../interfaces.h"
#include "../main.h"
#include "../gamefunc.h"
void glow::work()
{
	for (auto i = 0; i < ctx::glowobjmnj->m_GlowObjectDefinitions.Count(); i++)
	{
		auto &glowObject = ctx::glowobjmnj->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BaseEntity*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity)
			continue;
		bool is_enemy = entity->team() != deviant.localplayer->team();
		bool playerTeam = entity->team() == 2;

		if (!entity->alive())
			continue;

		if (!is_enemy)
			continue;
		auto class_id = entity->GetClientClass()->m_ClassID;
		Color color = Color(0, 0, 150, 255);
		switch (class_id)
		{
		case ClassId_CCSPlayer:
		{
			glowObject.m_nGlowStyle = /*glowstyle*/0;
			glowObject.m_flAlpha = color.a() / 255.0f;
			//bool vis = gamefunc::get().visible(deviant.localplayer,entity,0);

			break;
		}
		default:
		{
			glowObject.m_flAlpha = 0.f;
			break;
		}
		}

		glowObject.m_flRed = color.r() / 255.0f;
		glowObject.m_flGreen = color.g() / 255.0f;
		glowObject.m_flBlue = color.b() / 255.0f;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_bRenderWhenUnoccluded = false;
	}
}

void glow::clear()
{
	for (auto i = 0; i < ctx::glowobjmnj->m_GlowObjectDefinitions.Count(); i++)
	{
		auto &glowObject = ctx::glowobjmnj->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BaseEntity*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		glowObject.m_flAlpha = 0.0f;
	}
}