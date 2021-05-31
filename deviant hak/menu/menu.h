#pragma once
#include "../common.h"
#include "../hooks.h"
#include "config.h"

class menu;
class element {
public:
	virtual void render( ) = 0;
	virtual void update( ) = 0;

	void add_child( element* e ) {
		e->m_parent = this;
		m_children.emplace_back( e );
	}

	POINT pos( ) {
		return m_pos;
	}

	POINT size( ) {
		return m_size;
	}

	std::string m_var = "var_unknown";

	POINT m_opos = { };
	POINT m_pos = { };
	POINT m_size = { };

	std::vector<element*> m_children;
	element* m_parent = nullptr;
};

class menu : public singleton<menu> {
public:
	// recursively free the elements
	~menu( ) {
		for ( auto &t : m_tabs ) {
			delete t;
		}
	}

	void render( );
	void initialize( );

	bool active( ) {
		return m_active;
	}

	void toggle( ) {
		m_active = !m_active;
	}

	bool key_updated( int key ) {
		return ( m_keystate [ key ] == true && m_oldstate [ key ] == false );
	}

	bool key_held( int key ) {
		return m_keystate [ key ];
	}

	int alpha( ) {
		return m_alpha;
	}

	uintptr_t modulate_alpha( uintptr_t original, int alpha );

	inline bool in_bounds( POINT pos, POINT size ) {
		// grab cursor
		auto cursor = m_cursor;

		// math for inbounds
		return ( cursor.x > pos.x &&
			cursor.y > pos.y &&
			cursor.x < pos.x + size.x &&
			cursor.y < pos.y + size.y );
	}

	inline bool clicked( POINT pos, POINT size ) {
		return ( in_bounds( pos, size ) && key_updated( VK_LBUTTON ) );
	}

	inline bool in_bounds( element& e ) {
		return ( in_bounds( e.pos( ), e.size( ) ) );
	}

	inline bool clicked( element& e ) {
		// basic click handler
		return ( in_bounds( e ) && key_updated( VK_LBUTTON ) );
	}

	inline bool blocked( ) {
		return m_block;
	}

	void block( bool enable, element* blocking ) {
		m_block = enable;
		m_blocking = blocking;
	}

	POINT cursor( ) {
		return m_cursor;
	}

	std::vector<element*> m_tabs;
	POINT m_pos = { 45, 45 };

	IDirect3DDevice9* m_device = nullptr;

private:
	bool m_initialized = false;
	bool m_active = false;
	int m_alpha = 0;

	bool m_clicked = false;
	bool m_holding = false;

	bool m_block = false;
	element* m_blocking = nullptr;

	bool m_keystate [ 256 ] = { };
	bool m_oldstate [ 256 ] = { };

	void poll_mouse( );
	void poll_keyboard( );

	POINT m_drag = { };
	POINT m_cursor = { };
	POINT m_size = { 605, 495 };

	void render_cursor( );

	bool clicked( );

	void add_tab( element* e ) {
		m_tabs.emplace_back( e );
	}
};

#define COLOUR( r, g, b ) D3DCOLOR_ARGB( menu::get( ).alpha( ), r, g, b )

template <typename t>
inline t clamp( t number, t min, t max ) {
	if ( number < min ) {
		return min;
	}
	else if ( number > max ) {
		return max;
	}
	else {
		return number;
	}
}