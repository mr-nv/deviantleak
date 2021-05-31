#pragma once
#include "../menu.h"

class group : public element {
public:
	group( int x, int y, int w, int h, std::string name );

	virtual void render( ) override;
	virtual void update( ) override;

private:
	std::string m_group = "Error";
};