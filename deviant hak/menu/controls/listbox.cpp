#include "listbox.h"

listbox::listbox( int x, int y, const char** elements, int size, int *option ) {
	m_opos = m_pos = { x, y };
	m_size = { 185, 240 };
	m_option = option;

	// copy array contents
	for ( auto i = 0; i < size; ++i ) {
		m_elements.emplace_back( elements [ i ] );
	}
}

void listbox::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };
}

void listbox::render( ) {
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

	float g = 0;
	static float curg = 0;
	int maxelem = 11;

	if ( m_elements.size( ) > maxelem ) {
		g = ( float ) ( ( m_elements.size( ) / maxelem ) );
	}

	render::d3d::get().filled_box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 36, 36, 36 ) );

	int offy = 1;
	for ( int i = 0; i < m_elements.size( ); i++ ) {
		int gg = ( ( curg + 1 ) * maxelem );
		if ( i >= ( gg - maxelem ) && i <= gg ) {
			if ( i == *m_option ) {
				render::d3d::get().filled_box( m_pos.x, m_pos.y + offy, m_size.x, 20, COLOUR( 26, 26, 26 ) );
			}
			if ( menu::get( ).in_bounds( { m_pos.x, m_pos.y + offy }, { m_size.x, 19 } ) ) {
				render::d3d::get().filled_box( m_pos.x, m_pos.y + offy, m_size.x, 20, COLOUR( 56, 56, 56 ) );
				if ( menu::get( ).key_updated( VK_LBUTTON ) ) {
					*m_option = i;
				}
			}
			if ( i == *m_option ) {
				render::d3d::get().text( m_elements.at( i ), m_pos.x + 9, m_pos.y + offy + 4, left, fonts::menu, false, COLOUR( 110, 194, 21 ) );
			}
			else {
				render::d3d::get().text( m_elements.at( i ), m_pos.x + 9, m_pos.y + offy + 4, left, fonts::menu, false, COLOUR( 153, 153, 153 ) );
			}
			offy += 20;
		}
	}

	draw_scroll( m_pos.x + m_size.x, m_pos.y + 1, ( m_size.y - 21 ), 0, g, &curg );

	box( m_pos.x, m_pos.y, m_size.x + 11, m_size.y + 1, COLOUR( 5, 5, 5 ) );
}

void listbox::draw_scroll( int x, int y, int h, float min, float max, void* value ) {
	constexpr auto width = 6;

	if ( *( float* ) value < min ) {
		*( float* ) value = min;
	}
	else if ( *( float* ) value > max ) {
		*( float* ) value = max;
	}

	auto new_y = y + ( ( *( float* ) value - min ) * h / ( max - min ) );

	if ( menu::get( ).in_bounds( { x, y }, { width + 20, h } ) && menu::get( ).key_held( VK_LBUTTON ) ) {
		*( float* ) value = ( menu::get( ).cursor( ).y - y ) * ( max - min ) / h;
	}

	render::d3d::get().filled_box( x, y - 1, width + 5, h + 22, COLOUR( 50, 50, 50 ) );

	// lambda for a nicer box
	auto box = [ ] ( int x, int y, int w, int h, uintptr_t c ) {
		render::d3d::get().line( x, y, x, y + h, c );
		render::d3d::get().line( x, y + h, x + w, y + h, c );
		render::d3d::get().line( x + w, y, x + w, y + h, c );
		render::d3d::get().line( x, y, x + w, y, c );
	};

	render::d3d::get().filled_box( x + 2, new_y, width, 20, COLOUR( 75, 75, 75 ) );
}