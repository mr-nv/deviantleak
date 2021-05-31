#pragma once
#include "../menu.h"

class text : public element {
public:
	text( int x, int y, std::string name );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	std::string m_name = "Error";
};