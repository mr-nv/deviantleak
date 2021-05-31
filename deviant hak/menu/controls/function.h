#pragma once
#include "../menu.h"

class function : public element {
public:
	function( int x, int y, std::string name, uintptr_t fn );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	std::string m_name = "Error";
	uintptr_t m_fn = 0;
};