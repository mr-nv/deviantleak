#include "resolver.h"
bool aacorrection::playermove(C_BaseEntity* entity)
{
	if (entity->velocity().Length2D() > 0.1f)
		return true;
	else return false;
}
void aacorrection::corectangles()
{
	for (int i = 1; i < ctx::entitylist->GetHighestEntityIndex(); i++)
	{
		auto entity = static_cast<C_BaseEntity*>(ctx::entitylist->GetClientEntity(i));

		if (!entity)
			continue;

		if (entity == deviant.localplayer)
			continue;

		if (!entity->alive())
			continue;

		if (entity->team() == deviant.localplayer->team())
			continue;

		if (entity->GetClientClass()->m_ClassID == ClassId_CCSPlayer) {
			if (playermove(entity))
				entity->eye_angles().y = entity->lower_body_yaw();
		}
	}
}
		