#pragma once
#include "../menu.h"

class colourpicker : public element {
public:
	colourpicker( int x, int y, std::string var, uintptr_t default_colour );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	bool m_picking = false;
};