#pragma once
#include "../menu.h"

class button : public element {
public:
	button( int x, int y, std::string display, std::string var, bool default_val = false );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	std::string m_name = "Error";
};