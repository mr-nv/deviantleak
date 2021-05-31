#pragma once
#include "../singleton.h"
#include "../common.h"
#include "../c_baseentity.h"
#include "../interfaces.h"
#include "../math/math.h"
#include "../main.h"
#include "../gamefunc.h"


class ragebot : public singleton<ragebot>
{
public:
	void fix_recoil(c_usercmd * cmd);
	void start_prediction(c_usercmd * pCmd);
private:
};