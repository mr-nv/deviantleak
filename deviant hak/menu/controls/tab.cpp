#include "tab.h"

int m_selected_tab = 0;
int m_oldselected = 0;
int m_amount = 0;

tab::tab( ) {
	m_count = m_amount++;
}

tab::tab( std::string icon ) {
	m_icon = icon;
	m_count = m_amount++;
	m_size = { 76, 0 };
}

void tab::render( ) {
	// bandaid fix for tab pos
	auto m_old = m_pos;
	m_pos = menu::get( ).m_pos;

	// update the children of the tab
	for ( auto &c : m_children ) {
		// update only when menu active
		if ( menu::get( ).active( ) ) {
			c->update( );
		}
	}

	// restore old pos
	m_pos = m_old;

	// calculate text position
	auto x_offset = m_size.x / 2;
	//auto y_offset = ( m_size.y / 2 ) - 30;

	// ghetto fix for the 1 tick flickering
	if ( m_selected_tab != m_oldselected ) {
		m_oldselected = m_selected_tab;
		return;
	}
	//if (m_selected_tab == m_oldselected && clicked())
	//	return;
	if ( m_count == m_selected_tab ) {
		// render tab outlines
	//	render::d3d::get().line( m_pos.x, m_pos.y, m_pos.x + m_size.x + 1, m_pos.y, COLOUR( 48, 48, 48 ) );
	//	render::d3d::get().line( m_pos.x, m_pos.y + m_size.y - 1, m_pos.x + m_size.x + 1, m_pos.y + m_size.y - 1, COLOUR( 48, 48, 48 ) );

		// render text
		render::d3d::get().text( m_icon, m_pos.x/* + x_offset*/ +14, m_pos.y /*+ 22*/ /*+ y_offset*/, left, fonts::menu_small, false, COLOUR( 210, 210, 210 ) );
		// render the children of the tab
		for ( auto &c : m_children ) {
			// always draw
			c->render( );
		}
	}
	else {
		// render
		render::d3d::get().text( m_icon, m_pos.x /*+ x_offset*/ + 14, m_pos.y/*  + 22y_offset*/, left, fonts::menu_small, false, COLOUR( 90, 90, 90 ) );
	}
}

void tab::update( ) { //tab_count * 22
	// get tab count and menu pos for calcs
	auto tab_count = menu::get( ).m_tabs.size( );
	auto menu_pos = menu::get( ).m_pos;

	// calculate new pos
	m_pos = { menu_pos.x, menu_pos.y + 15 + ( long ) ( ( 83 / tab_count ) * m_count ) };

	// calculate size if needed
	m_size.y = ( long ) ( 445 / tab_count );

	// if clicked, block input to other elements for that frame
	if ( menu::get( ).clicked( *this ) ) {
		menu::get( ).block( true, nullptr );
		m_selected_tab = m_count;
	}
}