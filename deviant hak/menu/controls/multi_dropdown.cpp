#include "multi_dropdown.h"

multi_dropdown::multi_dropdown( int x, int y, std::string name, std::string var, std::vector< std::string > options ) {
	m_opos = m_pos = { x, y };
	m_size = { 155, 20 };
	m_name = name;
	m_var = var;
	m_elements = options;
	g_cfg [ m_var ].set<int>( 0 );

	for ( auto n = 0; n < m_elements.size( ); ++n ) {
		auto value = ( 1 << ( n + 1 ) );
		m_selects.push_back( value );
	}
}

void multi_dropdown::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };

	// toggle if it's clicked
	if ( menu::get( ).clicked( *this ) ) {
		m_selecting = true;
	}

	// bandaid fix
	static bool was_selecting = false;

	// disable input, too
	if ( m_selecting ) {
		// block input
		menu::get( ).block( true, this );

		// get the clicked part
		auto index = part_clicked( );

		// if it's valid
		if ( index != -1 && menu::get( ).key_updated( VK_LBUTTON ) ) {
			auto old_value = g_cfg [ m_var ].get<uintptr_t>( );
			auto val = m_selects.at( index );

			if ( old_value & ( uintptr_t ) val ) {
				old_value &= ( uintptr_t ) ~val;
			}
			else {
				old_value |= ( uintptr_t ) val;
			}

			g_cfg [ m_var ].set( old_value );
		}

		else if ( !menu::get( ).in_bounds( *this ) ) {
			if ( menu::get( ).key_updated( VK_LBUTTON ) ) {
				m_selecting = false;
				was_selecting = false;
				return;
			}
		}

		was_selecting = true;
	}
	else {
		was_selecting = false;
	}
}

int multi_dropdown::part_clicked( ) {
	// store original pos
	auto original_pos = m_pos;

	// check all the elements
	for ( auto i = 0; i < m_elements.size( ); ++i ) {
		// make sure to move the bounding box
		m_pos.y += m_size.y;

		// see if the element is clicked
		if ( menu::get( ).in_bounds( *this ) ) {
			// restore original pos
			m_pos = original_pos;

			return i;
		}
	}

	// restore original pos
	m_pos = original_pos;

	// none available, sadly
	return -1;
}

void multi_dropdown::render( ) {
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

	// fill in base
	if ( menu::get( ).in_bounds( *this ) ) {
		render::d3d::get().filled_box( m_pos.x + 1, m_pos.y + 1, m_size.x - 1, m_size.y - 1, COLOUR( 26, 26, 26 ) );
	}
	else {
		render::d3d::get().filled_box( m_pos.x + 1, m_pos.y + 1, m_size.x - 1, m_size.y - 1, COLOUR( 36, 36, 36 ) );
	}

	// lambda for the arrow
	auto arrow = [ ] ( int x, int y ) {
		for ( auto i = 5; i >= 2; --i ) {
			auto offset = 5 - i;

			render::d3d::get().line( x + offset, y + offset, x + offset + clamp( i - offset, 0, 5 ), y + offset, COLOUR( 152, 152, 152 ) );
		}
	};

	// draw the actual thing
	arrow( m_pos.x + m_size.x - 10, ( m_pos.y + m_size.y / 2 ) );

	// also render the text
	render::d3d::get().text( m_name, m_pos.x, m_pos.y - 15, left, fonts::menu, true, COLOUR( 204, 204, 204 ) );

	// and the selected item
	auto index = g_cfg [ m_var ].get<uintptr_t>( );

	std::string format;

	for ( auto n = 0; n < m_elements.size( ); ++n ) {
		auto format_len = format.length( ) < 16;
		auto first = format.length( ) <= 0;
		if ( ( index & m_selects.at( n ) ) && format_len ) {
			if ( !first ) {
				format.append( ", " );
			}

			format.append( m_elements.at( n ) );
		}
		else if ( !format_len ) {
			format.append( "..." );
			break;
		}
	}

	if ( format.length( ) <= 0 ) {
		format += "Off";
	}

	render::d3d::get().text( format, m_pos.x + 11, m_pos.y + 4, left, fonts::menu, false, COLOUR( 204, 204, 204 ) );

	// handle dropdown
	if ( m_selecting ) {
		// nicer tbh
		auto i = 0;

		// iterate all the elements and render them
		for ( ; i < m_elements.size( ); ++i ) {
			// time savers
			auto bold = i == part_clicked( );
			auto opti = ( index & m_selects.at( i ) );

			// fill
			render::d3d::get().filled_box( m_pos.x, m_pos.y + ( m_size.y * ( i + 1 ) ), m_size.x, m_size.y + 1, bold ? COLOUR( 26, 26, 26 ) : COLOUR( 36, 36, 36 ) );

			// fix lines
			render::d3d::get().line( m_pos.x, m_pos.y, m_pos.x, m_pos.y + m_size.y, COLOUR( 5, 5, 5 ) );
			render::d3d::get().line( m_pos.x + m_size.x, m_pos.y, m_pos.x + m_size.x, m_pos.y + m_size.y, COLOUR( 5, 5, 5 ) );

			// render element names
			render::d3d::get().text( m_elements.at( i ), m_pos.x + 11, m_pos.y + 4 + ( m_size.y * ( i + 1 ) ), left, ( opti || bold ) ? fonts::menu_large : fonts::menu, false, opti ? COLOUR( 80, 99, 3 ) : bold ? COLOUR( 255, 255, 255 ) : COLOUR( 153, 153, 153 ) );
		}

		// outline
		box( m_pos.x, m_pos.y, m_size.x, m_size.y * ( i + 1 ), COLOUR( 5, 5, 5 ) );
	}

	// this at the end
	box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 5, 5, 5 ) );
}