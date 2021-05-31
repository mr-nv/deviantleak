#pragma once
#include "../singleton.h"
#include "../common.h"
#include "../c_baseentity.h"
struct boxes
{
	int x = -1, y = -1, w = -1, h = -1;
};
class esp : public singleton<esp>
{
public:

	void draw();
	void drawplayer(C_BaseEntity* ent, boxes boxe, Color color) {
		box(ent, color, boxe);
		drawname(ent,boxe);
		drawarmor(ent, boxe);
		drawft(ent, boxe);
		drawweapon(ent, boxe);
		drawhealth(ent, boxe);
		drawskeleton(ent);
	}
	void box(C_BaseEntity * ent, Color col, boxes boxe);
	void drawname(C_BaseEntity * ent, boxes boxe);
	void drawhealth(C_BaseEntity * ent, boxes boxe);
	void drawweapon(C_BaseEntity * ent, boxes boxe);
	void drawarmor(C_BaseEntity * ent, boxes boxe);
	void drawft(C_BaseEntity * ent, boxes boxe);
	void povarrows();
	void drawskeleton(C_BaseEntity * ent);
};