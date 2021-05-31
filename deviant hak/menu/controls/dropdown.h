#pragma once
#include "../menu.h"

class dropdown : public element {
public:
	dropdown( int x, int y, std::string name, std::string var, std::vector< std::string > options );

	virtual void update( ) override;
	virtual void render( ) override;
private:
	bool m_selecting = false;

	int part_clicked( );
	
	std::string m_name = "Error";
	std::vector< std::string > m_elements = { "Error" };
}; // pvrhraechp