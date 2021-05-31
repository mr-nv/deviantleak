#include "function.h"

function::function( int x, int y, std::string name, uintptr_t fn ) {
	m_opos = m_pos = { x, y };
	m_size = { 155, 20 };

	m_name = name;
	m_fn = fn;
}

void function::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };

	// toggle if it's clicked
	if ( menu::get( ).in_bounds( *this ) && menu::get( ).key_updated( VK_LBUTTON ) ) {
		// pasted lol
		auto func_to_call = m_fn;
		__asm {
			CALL func_to_call;
		}
	}
}

void function::render( ) {
	// sanity checks
	if ( !m_parent ) {
		return;
	}

	// lambda for a nicer box
	auto box = [ ] ( int x, int y, int w, int h, uintptr_t c ) {
		render::d3d::get().line( x, y, x, y + h, c );
		render::d3d::get().line( x, y + h, x + w, y + h, c );
		render::d3d::get().line( x + w, y, x + w, y + h, c );
		render::d3d::get().line( x, y, x + w, y, c );
	};

	// render box outline
	box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 5, 5, 5 ) );
	render::d3d::get().gradient( m_pos.x + 1, m_pos.y + 1, m_size.x - 2, m_size.y - 2, COLOUR( 35, 35, 35 ), COLOUR( 25, 25, 25 ), true );

	// also the name
	render::d3d::get().text( m_name, m_pos.x + 77.5, m_pos.y + 4, centered, fonts::menu_large, true, COLOUR( 255, 255, 255 ) );
}