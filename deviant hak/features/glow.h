#pragma once
#include "../singleton.h"

class glow : public singleton<glow> {

public:
	void work();
	void clear();
};
