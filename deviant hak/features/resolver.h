#pragma once
#include "../common.h"
#include "../interfaces.h"
#include "../c_baseentity.h"
#include "../main.h"
#include "../singleton.h"
#include <array>


class aacorrection : public singleton<aacorrection> {

public:

	bool playermove(C_BaseEntity * entity);

	void corectangles();

};
