#pragma once
#include "../menu.h"

class slider : public element {
public:
	slider( int x, int y, std::string name, std::string var, float min, float max, bool round, std::string format = "" );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	std::string m_name = "", m_add = "";

	bool m_sliding = false;
	bool m_round = false;

	float m_min = -1.0f, m_max = -1.0f;
};