#include "slider.h"
#include <string>

slider::slider( int x, int y, std::string name, std::string var, float min, float max, bool round, std::string format ) {
	m_opos = m_pos = { x, y };
	m_size = { 155, 9 };
	m_name = name;
	m_var = var;
	m_min = min;
	m_max = max;
	m_round = round;
	m_add = format;

	g_cfg [ m_var ].set( min );
}

void slider::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };
	m_size = { 155, 6 };

	// bandaid pasted fix idk idc
	static auto active = false;

	// toggle if it's in the bounds
	if ( menu::get( ).in_bounds( { m_pos.x - 2, m_pos.y - 2 }, { m_size.x + 4, m_size.y + 4 } ) ) {
		active = true;
	}

	// handle the +/- things lol
	auto orig = g_cfg [ m_var ].get<float>( );

	if ( menu::get( ).key_updated( VK_LBUTTON ) ) {
		if ( menu::get( ).in_bounds( { m_pos.x - 15, m_pos.y }, { 15, m_size.y } ) ) {
			if ( m_round ) {
				orig -= 1.00f;
			}
			else {
				orig -= 0.01;
			}
		}

		if ( menu::get( ).in_bounds( { m_pos.x + m_size.x, m_pos.y }, { 15, m_size.y } ) ) {
			if ( m_round ) {
				orig += 1.00f;
			}
			else {
				orig += 0.01;
			}
		}
	}

	auto clamped = clamp( orig, m_min, m_max );
	g_cfg [ m_var ].set<float>( clamped );

	// handle mouse movement
	if ( active ) {
		if ( menu::get( ).key_held( VK_LBUTTON ) ) {
			// block input to others
			menu::get( ).block( true, nullptr );

			// get cursor
			auto cursor = menu::get( ).cursor( );

			// ghetto fix?
			if ( abs( cursor.y - m_pos.y ) >= 15 ) {
				active = false;
				return;
			}

			// calculate new slider value based on relative position to slider
			auto relative_x = clamp( ( int ) ( cursor.x - m_pos.x ), 0, ( int ) ( m_size.x ) );

			// more ghetto
			if ( m_size.x - abs( cursor.x - m_pos.x ) <= -15 ) {
				active = false;
				return;
			}

			// let's convert this into a ratio
			auto ratio = ( relative_x / ( float ) m_size.x );

			// finally, calculate the value
			auto value = m_min + ( ( m_max - m_min ) * ratio );

			// and round if necessary
			if ( m_round ) {
				value = floor( value );
			}

			// store
			g_cfg [ m_var ].set( value );
		}
		else {
			active = false;
		}
	}
}

void slider::render( ) {
	// sanity checks
	if ( !m_parent ) {
		return;
	}

	// calculate visual slider
	auto value = g_cfg [ m_var ].get( );
	auto ratio = ( value - m_min ) / ( m_max - m_min );
	auto position = ( int ) ( m_size.x * ratio );

	// lambda for a nicer box
	auto box = [ ] ( int x, int y, int w, int h, uintptr_t c ) {
		render::d3d::get().line( x, y, x, y + h, c );
		render::d3d::get().line( x, y + h, x + w, y + h, c );
		render::d3d::get().line( x + w, y, x + w, y + h, c );
		render::d3d::get().line( x, y, x + w, y, c );
	};

	// render slider outline
	box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 5, 5, 5 ) );

	// render slider inlines
	render::d3d::get().gradient( m_pos.x + 1, m_pos.y + 1, m_size.x - 1, m_size.y - 1, COLOUR( 35, 35, 35 ), COLOUR( 25, 25, 25 ), true );

	// also fill it in if our value is positive
	if ( value > 0.0f ) {
		render::d3d::get().gradient( m_pos.x + 1, m_pos.y + 1, position - 1, m_size.y - 1, COLOUR( 149, 184, 6 ), COLOUR( 80, 99, 3 ), true );
	}

	// format the string
	char buffer [ 24 ];
	sprintf_s( buffer, m_round ? "%.0f" : "%.2f", value );

	// put the string in a string XD
	std::string format( buffer );
	format += m_add;

	// render the lil +/- thingos
	render::d3d::get().text( "-", m_pos.x - 7, m_pos.y - 4, left, fonts::menu, false, COLOUR( 204, 204, 204 ) );
	render::d3d::get().text( "+", m_pos.x + m_size.x + 3, m_pos.y - 4, left, fonts::menu, false, COLOUR( 204, 204, 204 ) );

	// render it
	render::d3d::get().text( format, m_pos.x + position + 1, m_pos.y + 1, centered, fonts::menu_large, true, COLOUR( 204, 204, 204 ) );

	// and the name too
	render::d3d::get().text( m_name, m_pos.x, m_pos.y - 15, left, fonts::menu, true, COLOUR( 204, 204, 204 ) );
}