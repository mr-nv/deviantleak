#pragma once
#include "../menu.h"

class multi_dropdown : public element {
public:
	multi_dropdown( int x, int y, std::string name, std::string var, std::vector< std::string > options );

	virtual void update( ) override;
	virtual void render( ) override;
private:
	bool m_selecting = false;

	int part_clicked( );

	std::string m_name = "Error";
	std::vector< uintptr_t > m_selects = { };
	std::vector< std::string > m_elements = { "Error" };
}; // pvrhraechp