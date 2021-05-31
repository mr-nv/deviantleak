#pragma once
#include "../menu.h"

class kbind : public element {
public:
	kbind( int x, int y, std::string var );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	bool m_listening = false;
};