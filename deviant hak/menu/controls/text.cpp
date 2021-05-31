#include "text.h"

text::text( int x, int y, std::string name ) {
	m_opos = m_pos = { x, y };
	m_name = name;
}

void text::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };
}

void text::render( ) {
	render::d3d::get().text( m_name, m_pos.x + 20, m_pos.y - 2, left, fonts::menu, true, COLOUR( 255, 255, 255 ) );
}