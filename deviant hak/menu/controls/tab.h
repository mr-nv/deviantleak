#pragma once
#include "../menu.h"

class tab : public element {
public:
	tab( );
	tab( std::string icon );

	virtual void render( ) override;
	virtual void update( ) override;

private:
	std::string m_icon = "#";
	int m_count = 0;
};