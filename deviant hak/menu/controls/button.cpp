#include "button.h"

button::button( int x, int y, std::string display, std::string var, bool default_val ) {
	m_name = display;
	m_var = var;
	m_opos = m_pos = { x, y };
	m_size = { 8, 8 };

	// set the default value
	g_cfg [ var ].set( default_val );
}

void button::update( ) {
	// sanity checks
	if ( !m_parent ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };

	// update value if needed
	if ( menu::get( ).clicked( *this ) ) {
		auto old = g_cfg [ m_var ].get<bool>( );
		g_cfg [ m_var ].set( !old );
	}
}

void button::render( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// lambda for a nicer box
	auto box = [ ] ( int x, int y, int w, int h, uintptr_t c ) {
		render::d3d::get().line( x, y, x, y + h, c );
		render::d3d::get().line( x, y + h, x + w, y + h, c );
		render::d3d::get().line( x + w, y, x + w, y + h, c );
		render::d3d::get().line( x, y, x + w, y, c );
	};

	// render the basic checkbox
	box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 5, 5, 5 ) );

	// if it's enabled, fill it in with the right colour
	if ( g_cfg [ m_var ].get<bool>( ) ) {
		render::d3d::get().gradient( m_pos.x + 1, m_pos.y + 1, m_size.x - 2, m_size.y - 2, COLOUR( 149, 184, 6 ), COLOUR( 80, 99, 3 ), true );
	}
	else {
		render::d3d::get().gradient( m_pos.x + 1, m_pos.y + 1, m_size.x - 2, m_size.y - 2, COLOUR( 65, 65, 65 ), COLOUR( 45, 45, 45 ), true );
	}

	// render the name, too
	render::d3d::get().text( m_name, m_pos.x + 20, m_pos.y - 2, left, fonts::menu, true, COLOUR( 255, 255, 255 ) );
}