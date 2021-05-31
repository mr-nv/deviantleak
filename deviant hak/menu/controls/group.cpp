#include "group.h"

group::group( int x, int y, int w, int h, std::string name ) {
	m_opos = m_pos = { x, y };
	m_size = { w, h };
	m_group = name;
}

void group::render( ) {
	// sanity check
	if ( !m_parent ) {
		return;
	}

	// let's draw the group box
	auto width = render::d3d::get().string_width( fonts::menu_large, m_group );

	// first outlines
	render::d3d::get().line( m_pos.x, m_pos.y, m_pos.x, m_pos.y + m_size.y, COLOUR( 5, 5, 5 ) );
	render::d3d::get().line( m_pos.x, m_pos.y + m_size.y, m_pos.x + m_size.x, m_pos.y + m_size.y, COLOUR( 5, 5, 5 ) );
	render::d3d::get().line( m_pos.x + m_size.x, m_pos.y, m_pos.x + m_size.x, m_pos.y + m_size.y, COLOUR( 5, 5, 5 ) );
	render::d3d::get().line( m_pos.x, m_pos.y, m_pos.x + 8, m_pos.y, COLOUR( 5, 5, 5 ) );
	render::d3d::get().line( m_pos.x + width + 14, m_pos.y, m_pos.x + m_size.x, m_pos.y, COLOUR( 5, 5, 5 ) );

	// second outlines
	render::d3d::get().line( m_pos.x + 1, m_pos.y + 1, m_pos.x + 1, m_pos.y + m_size.y - 1, COLOUR( 65, 65, 65 ) );
	render::d3d::get().line( m_pos.x + 1, m_pos.y + m_size.y - 1, m_pos.x + m_size.x - 1, m_pos.y + m_size.y - 1, COLOUR( 65, 65, 65 ) );
	render::d3d::get().line( m_pos.x + m_size.x - 1, m_pos.y + 1, m_pos.x + m_size.x - 1, m_pos.y + m_size.y - 1, COLOUR( 65, 65, 65 ) );
	render::d3d::get().line( m_pos.x + 1, m_pos.y + 1, m_pos.x + 7, m_pos.y + 1, COLOUR( 65, 65, 65 ) );
	render::d3d::get().line( m_pos.x + width + 14, m_pos.y + 1, m_pos.x + m_size.x - 1, m_pos.y + 1, COLOUR( 65, 65, 65 ) );

	// also render the text
	render::d3d::get().text( m_group, m_pos.x + 11, m_pos.y - 5, left, fonts::menu_large, true, COLOUR( 181, 181, 181 ) );

	// now let's render the children
	for ( auto &c : m_children ) {
		// update only when menu active
		if ( menu::get( ).active( ) ) {
			c->update( );
		}

		// always draw
		c->render( );
	}
}

void group::update( ) {
	// sanity check
	if ( !m_parent ) {
		return;
	}

	// inherit parent's position
	m_pos = m_parent->m_pos;
	m_pos.x += m_opos.x;
	m_pos.y += m_opos.y;
}