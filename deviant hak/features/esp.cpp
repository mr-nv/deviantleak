#include "esp.h"
#include "../main.h"
#include "../interfaces.h"
#include "../math/math.h"
#include "../gamefunc.h"
#include <stdio.h>
#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)
float flPlayerAlpha[65];
bool get_box(C_BaseEntity* entity, boxes& box, bool dynamic) {


	auto vector_transform_z = [](const Vector& in1, const matrix3x4_t &in2, Vector &out) {
		auto vector_transform_a = [](const float *in1, const matrix3x4_t& in2, float *out) {
			auto dot_product_t = [](const float* a, const float* b) -> float {
				return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
			};
			out[0] = dot_product_t(in1, in2[0]) + in2[0][3];
			out[1] = dot_product_t(in1, in2[1]) + in2[1][3];
			out[2] = dot_product_t(in1, in2[2]) + in2[2][3];
		};

		vector_transform_a(&in1.x, in2, &out.x);
	};

	DWORD m_rgflCoordinateFrame = (DWORD)0x470 - 0x30;
	const matrix3x4_t& trnsf = *(matrix3x4_t*)((DWORD)entity + (DWORD)m_rgflCoordinateFrame);

	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;

	float left, boxe, right, bottom;
	auto collideable = entity->GetCollideable();
	if (!collideable)
		return false;
	vOrigin = entity->origin();
	min = collideable->OBBMins();
	max = collideable->OBBMaxs();

	if (!dynamic) {
		min += vOrigin;
		max += vOrigin;
	}

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	Vector vector_transformed[8];

	if (dynamic)
	{
		for (int i = 0; i < 8; i++)
		{
			vector_transform_z(points[i], trnsf, vector_transformed[i]);
			points[i] = vector_transformed[i];
		}
	}
	if (!render::get().worldtoscreen(points[3], flb) || !render::get().worldtoscreen(points[5], brt)
		|| !render::get().worldtoscreen(points[0], blb) || !render::get().worldtoscreen(points[4], frt)
		|| !render::get().worldtoscreen(points[2], frb) || !render::get().worldtoscreen(points[1], brb)
		|| !render::get().worldtoscreen(points[6], blt) || !render::get().worldtoscreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	boxe = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (boxe > arr[i].y)
			boxe = arr[i].y;
	}

	box.x = left;
	box.y = boxe;
	box.w = right - left;
	box.h = bottom - boxe;

	return true;
}
void esp::draw()
{
	auto local = deviant.localplayer;
	if (local && g_cfg["espenabled"].get<bool>()) {
		for (int i = 0; i < ctx::entitylist->GetHighestEntityIndex(); i++) {
			auto entity = static_cast<C_BaseEntity*>(ctx::entitylist->GetClientEntity(i));
			if (!entity) continue;

			if (entity->GetClientClass()->m_ClassID == ClassId_CCSPlayer) {
				if (entity->IsDormant() && flPlayerAlpha[i] > 0) flPlayerAlpha[i] -= 5;
				else if (!(entity->IsDormant()) && flPlayerAlpha[i] < 255) flPlayerAlpha[i] += 5;
				float alpha = flPlayerAlpha[i];
				math::get().clamp(alpha, 0, 255);

				if (entity->alive()) {
					auto colour = g_cfg["boxclr"].get<uintptr_t>();
					auto r = get_r(colour);
					auto g = get_g(colour);
					auto b = get_b(colour);
					Color plc = Color(r, g, b, alpha);

					boxes box; if (!get_box(entity, box, 1)) {

						continue;
					}

					if (!g_cfg["espteam"].get<bool>() && entity->team() == local->team() || entity == local) continue;

					if (false && !gamefunc::get().visible(local, entity, 0)) continue;

					drawplayer(entity, box, plc);
				}
			}
		}
	}
	povarrows();
}

void esp::box(C_BaseEntity * ent, Color col, boxes boxe)
{
	float alpha = flPlayerAlpha[ent->EntIndex()];
	auto boxtype = g_cfg["espboxtype"].get<int>();
	if (boxtype == 2) {
		render::isurface::get().outline(boxe.x, boxe.y, boxe.w, boxe.h, col);
		render::isurface::get().outline(boxe.x - 1, boxe.y - 1, boxe.w + 2, boxe.h + 2, Color(21, 21, 21, alpha));
		render::isurface::get().outline(boxe.x + 1, boxe.y + 1, boxe.w - 2, boxe.h - 2, Color(21, 21, 21, alpha));
	}
	else if (boxtype == 1)
	{
		float width_corner = boxe.w / 4;
		float height_corner = width_corner;

		render::isurface::get().rect(boxe.x - 1, boxe.y - 1, width_corner + 2, 3, Color(21, 21, 21, alpha));
		render::isurface::get().rect(boxe.x - 1, boxe.y - 1, 3, height_corner + 2, Color(21, 21, 21, alpha));

		render::isurface::get().rect((boxe.x + boxe.w) - width_corner - 1, boxe.y - 1, width_corner + 2, 3, Color(21, 21, 21, alpha));
		render::isurface::get().rect(boxe.x + boxe.w - 1, boxe.y - 1, 3, height_corner + 2, Color(21, 21, 21, alpha));

		render::isurface::get().rect(boxe.x - 1, boxe.y + boxe.h - 4, width_corner + 2, 3, Color(21, 21, 21, alpha));
		render::isurface::get().rect(boxe.x - 1, (boxe.y + boxe.h) - height_corner - 4, 3, height_corner + 2, Color(21, 21, 21, alpha));

		render::isurface::get().rect((boxe.x + boxe.w) - width_corner - 1, boxe.y + boxe.h - 4, width_corner + 2, 3, Color(21, 21, 21, alpha));
		render::isurface::get().rect(boxe.x + boxe.w - 1, (boxe.y + boxe.h) - height_corner - 4, 3, height_corner + 3, Color(21, 21, 21, alpha));


		render::isurface::get().rect(boxe.x, boxe.y, width_corner, 1, col);
		render::isurface::get().rect(boxe.x, boxe.y, 1, height_corner, col);

		render::isurface::get().rect((boxe.x + boxe.w) - width_corner, boxe.y, width_corner, 1, col);
		render::isurface::get().rect(boxe.x + boxe.w, boxe.y, 1, height_corner, col);

		render::isurface::get().rect(boxe.x, boxe.y + boxe.h - 3, width_corner, 1, col);
		render::isurface::get().rect(boxe.x, (boxe.y + boxe.h) - height_corner - 3, 1, height_corner, col);

		render::isurface::get().rect((boxe.x + boxe.w) - width_corner, boxe.y + boxe.h - 3, width_corner, 1, col);
		render::isurface::get().rect(boxe.x + boxe.w, (boxe.y + boxe.h) - height_corner - 3, 1, height_corner + 1, col);
	}
}
void esp::drawname(C_BaseEntity* ent, boxes boxe)
{
	if (g_cfg["espname"].get<bool>())
	{
		float alpha = flPlayerAlpha[ent->EntIndex()];
		player_info_t info = ent->playerinfo();
		RECT name_size = render::isurface::get().gettextsize(info.szName, deviant.visualsfont);
		render::isurface::get().text(boxe.x + (boxe.w / 2) - (name_size.right / 2), boxe.y - 14, info.szName, deviant.visualsfont, Color(225, 225, 225, alpha));
	}
}
void esp::drawhealth(C_BaseEntity* ent, boxes boxe)
{
	if (g_cfg["esphealthbar"].get<bool>())
	{
		float alpha = flPlayerAlpha[ent->EntIndex()];
		int hpent = 100;
		hpent = ent->health();
		char nameBuffer[512];
		sprintf_s(nameBuffer, "%d", hpent);
		float h = (boxe.h);
		float offset = (h / 4.f) + 5;
		float w = h / 64.f;
		float health = ent->health();
		UINT hp = h - (UINT)((h * health) / 100);

		int Red = 255 - (health*2.55);
		int Green = health * 2.55;

		render::isurface::get().rect((boxe.x - 6) - 1, boxe.y - 1, 3, h + 2, Color(0, 0, 0, alpha));

		render::isurface::get().line((boxe.x - 6), boxe.y + hp, (boxe.x - 6), boxe.y + h, Color(Red, Green, 0, alpha));
	}
}
void esp::drawweapon(C_BaseEntity* ent, boxes boxe)
{
	if (g_cfg["espweapon"].get<bool>())
	{
		float alpha = flPlayerAlpha[ent->EntIndex()];
		char* format = "";
		char buffer[128];
		auto weapon = ent->activeweapon().Get();
		bool armor = true;
		if (!weapon) return;
		format = ("%s | %1.0f");
		float ammo = weapon->ammoclip();
		sprintf_s(buffer, format, weapon->getgunname(), ammo);
		RECT size = render::isurface::get().gettextsize(buffer, deviant.weapfont);
		render::isurface::get().text(boxe.x + (boxe.w / 2) - (size.right / 2), boxe.y + boxe.h + (armor ? 5 : 2), buffer, deviant.weapfont, Color(225, 225, 225, alpha));
	}
}
void esp::drawarmor(C_BaseEntity* ent, boxes boxe)
{
	if (g_cfg["esparmorbar"].get<bool>())
	{
		auto colour = g_cfg["armorclr"].get<uintptr_t>();
		auto r = get_r(colour);
		auto g = get_g(colour);
		auto b = get_b(colour);

		int player_armor = ent->armor() > 100 ? 100 : ent->armor();
		float alpha = flPlayerAlpha[ent->EntIndex()];

		if (player_armor) {
			Color arc = Color(r, g, b, alpha);

			render::isurface::get().outline(boxe.x - 1, boxe.y + boxe.h + 2, boxe.w + 2, 4, Color(21, 21, 21, alpha));

			int armor_width = player_armor * boxe.w / 100;

			render::isurface::get().rect(boxe.x, boxe.y + boxe.h + 3, boxe.w, 2, Color(21, 21, 21, alpha));
			render::isurface::get().rect(boxe.x, boxe.y + boxe.h + 3, armor_width, 2, arc);
		}
	}
}

void esp::drawft(C_BaseEntity* ent, boxes boxe)
{
	if (g_cfg["espdistance"].get<bool>())
	{
		float alpha = flPlayerAlpha[ent->EntIndex()];
		auto resolve_distance = [](Vector src, Vector dest)
		{
			Vector delta = src - dest;

			float fl_dist = ::sqrtf((delta.Length()));

			if (fl_dist < 1.0f)
				return 1.0f;

			return fl_dist;
		};
		auto local = deviant.localplayer;
		if (!local)
			return;
		Vector vecOrigin = ent->origin();
		Vector vecOriginLocal = local->origin();
		char dist_to[32];
		sprintf_s(dist_to, "%.0f ft", resolve_distance(vecOriginLocal, vecOrigin));
		RECT size = render::isurface::get().gettextsize(dist_to, deviant.visualsfont);
		render::isurface::get().text(boxe.x + (boxe.w / 2) - (size.right / 2), boxe.y + boxe.h + (15), dist_to, deviant.visualsfont, Color(225, 225, 225, alpha));
	}
}
void esp::povarrows()
{

	auto local = deviant.localplayer;
	if (!local)
		return;

	static auto alpha = 0.f;
	static auto plus_or_minus = false;
	if (alpha <= 0.f || alpha >= 255.f)
		plus_or_minus = !plus_or_minus;

	alpha += plus_or_minus ? (255.f / 0.5f * ctx::globals->frametime) : -(255.f / 0.5f * ctx::globals->frametime);
	alpha = std::clamp<float>(alpha, 0.f, 255.f);

	int screen_width, screen_height;
	ctx::engine->GetScreenSize(screen_width, screen_height);

	const auto screen_center_x = screen_width / 2;
	const auto screen_center_y = screen_height / 2;

	for (auto i = 1; i <= ctx::globals->maxClients; i++)
	{
		auto entity = static_cast<C_BaseEntity*>(ctx::entitylist->GetClientEntity(i));
		if (!entity || !(entity->GetClientClass()->m_ClassID == ClassId_CCSPlayer) || entity->IsDormant() || !entity->alive()
			|| (entity->team() == local->team()))
			continue;

		Vector screen_point;
		ctx::debugoverlay->ScreenPosition(entity->origin(), screen_point);
		if (screen_point.x < 0 || screen_point.y < 0 || screen_point.x > screen_width || screen_point.y > screen_height)
		{
			auto angle = QAngle();
			math::get().vector_angles(Vector(screen_center_x - screen_point.x, screen_center_y - screen_point.y, 0), angle);

			const auto angle_yaw_rad = DEG2RAD(angle.yaw + 180.0f);

			const auto new_point_x = screen_center_x +
				200 * cosf(angle_yaw_rad);
			const auto new_point_y = screen_center_y +
				200 * sinf(angle_yaw_rad);

			std::array<Vector2D, 3>points{ Vector2D(new_point_x - 10, new_point_y - 10),
				Vector2D(new_point_x + 25, new_point_y),
				Vector2D(new_point_x - 10, new_point_y + 10) };

			math::get().rotate_triangle(points, angle.yaw + +180.0f);
			render::isurface::get().add_triangle_filled(points, Color(255, 255, 0, alpha));
		}
	}
}
void esp::drawskeleton(C_BaseEntity* ent)
{

}
