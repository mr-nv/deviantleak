#include "menu.h"

// element includes
#include "controls\tab.h"
#include "controls\group.h"
#include "controls\button.h"
#include "controls\kbind.h"
#include "controls\dropdown.h"
#include "controls\slider.h"
#include "controls\function.h"
#include "controls\text.h"
#include "controls\colourpicker.h"
#include "controls\listbox.h"
#include "controls\multi_dropdown.h"
#include "../interfaces.h"
#include "../main.h"
#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)
 /*tabs*/
tab * rage = new tab("rage");
tab * antiaim = new tab("anti-aim");
tab * visuals = new tab("visuals");
tab * misc = new tab("misc");
tab * skins = new tab("skins");
tab * players = new tab("players");

 /*groups*/
group * miscgroup = new group(100, 20, 220, 320, "miscellaneous");
group * espgroup = new group(100, 20, 220, 320, "player esp");
group * coloredmodelsgroup = new group(100, 364, 220, 115, "colored models");
 /*esp*/
button * esp_enb = new button(20, 20, "enabled", "espenabled", false);
button * esp_teammates = new button(20, 40, "teammates", "espteam", false);
std::vector< std::string > box_opt = { "none","corner","full" };
dropdown * esp_boxtype = new dropdown(25, 65, "box", "espboxtype", box_opt);
colourpicker * esp_boxclr = new colourpicker(190, 75, "boxclr", D3DCOLOR_ARGB(255, 255, 0, 0));
button * esp_name = new button(20, 95, "name", "espname", false);
button * esp_healthbar = new button(20, 115, "health bar", "esphealthbar", false);
button * esp_armorbar = new button(20, 135, "armor bar", "esparmorbar", false);
colourpicker * esp_armorclr = new colourpicker(155, 135, "armorclr", D3DCOLOR_ARGB(255, 0, 150, 0));
button * esp_weapon = new button(20, 155, "weapon", "espweapon", false);
button * esp_distance = new button(20, 175, "distance", "espdistance", false);
 /*colored models*/
std::vector< std::string > coloredmdls_opt = {"none","colored","flat","metalic"};
dropdown* clrdmdls_type = new dropdown(20, 10, " ", "clrdmdlstype", coloredmdls_opt);
button * clrdmdls_enemy = new button(20, 40, "enemy", "clrdmdlsenemy", false);
colourpicker * clrdmdls_enemyclr = new colourpicker(155, 40, "clrdmdlsenemyclr", D3DCOLOR_ARGB(255, 150, 150, 0));
button * clrdmdls_enemyw = new button(20, 60, "enemy (behind wall)", "clrdmdlsenemywall", false);
colourpicker * clrdmdls_enemywclr = new colourpicker(155, 60, "clrdmdlsenemywclr", D3DCOLOR_ARGB(255, 0, 150, 150));
button * clrdmdls_team = new button(20, 80, "team", "clrdmdlsteam", false);
colourpicker * clrdmdls_teamclr = new colourpicker(155, 80, "clrdmdlsteamclr", D3DCOLOR_ARGB(255, 0, 0, 150));
button * clrdmdls_teamw = new button(20, 100, "team (behind wall)", "clrdmdlsteamwall", false);
colourpicker * clrdmdls_teamwclr = new colourpicker(155, 100, "clrdmdlsteamwclr", D3DCOLOR_ARGB(255, 200, 75, 75));
 /*misc*/
button * misc_bunnyhop = new button(20, 20, "bhop", "miscbhop", false);
button * misc_strafe = new button(20, 40, "strafe", "miscstrafe", false);
button * misc_circlestrafe = new button(20, 60, "circle strafe", "misccirclestrafe", false);
kbind * misc_circlestrafebind = new kbind(180, 60, "misckbindstrafe");
button * misc_duckinair = new button(20, 80, "duck in air", "miscduckinair", false);
void menu::initialize( ) {
	// already initialized
	if ( m_initialized ) {
		return;
	}
	// initialize menu controls here
	add_tab(rage);
	add_tab(antiaim);
	add_tab(visuals);
	add_tab(misc);
	add_tab(skins);
	add_tab(players);
	visuals->add_child(espgroup);
	espgroup->add_child(esp_enb);
	espgroup->add_child(esp_teammates); 
	espgroup->add_child(esp_boxtype);
	espgroup->add_child(esp_boxclr);
	espgroup->add_child(esp_name);
	espgroup->add_child(esp_healthbar);
	espgroup->add_child(esp_armorbar);
	espgroup->add_child(esp_armorclr);
	espgroup->add_child(esp_weapon);
	espgroup->add_child(esp_distance);
	visuals->add_child(coloredmodelsgroup);
	coloredmodelsgroup->add_child(clrdmdls_type);
	coloredmodelsgroup->add_child(clrdmdls_enemy);
	coloredmodelsgroup->add_child(clrdmdls_enemyclr);
	coloredmodelsgroup->add_child(clrdmdls_enemyw);
	coloredmodelsgroup->add_child(clrdmdls_enemywclr);
	coloredmodelsgroup->add_child(clrdmdls_team);
	coloredmodelsgroup->add_child(clrdmdls_teamclr);
	coloredmodelsgroup->add_child(clrdmdls_teamw);
	coloredmodelsgroup->add_child(clrdmdls_teamwclr);
	misc->add_child(miscgroup);
	miscgroup->add_child(misc_bunnyhop);
	miscgroup->add_child(misc_strafe);
	miscgroup->add_child(misc_circlestrafe);
	miscgroup->add_child(misc_circlestrafebind);
	miscgroup->add_child(misc_duckinair);
}

void menu::render( ) {
	// handle initialization
	if ( !m_initialized ) {
		// free the tab region of memory
		m_tabs.clear( );
		m_tabs.shrink_to_fit( );

		// initialize the menu elements
		initialize( );

		// mark initialization as done
		m_initialized = true;
	}

	// forgot this earlier, reset input block
	m_block = false;
	m_blocking = nullptr;

	// update inputs
	poll_keyboard( );

	// update menu state if needed
	if ( key_updated(VK_INSERT) ) {
		toggle( );
	}

	// the menu will reach full fade in/out in 150 ms
	constexpr auto fade_factor = 1.0f / 0.15f;

	// calculate the fade increment according to frametime
	// this is so the animation doesnt result in the gay slow fade on low fps machines

	auto fade_increment = ( fade_factor * ctx::globals->frametime ) * 255.0f;

	// handle alpha modulation
	m_active ? ( m_alpha += fade_increment ) : ( m_alpha -= fade_increment );
	m_alpha = clamp( m_alpha, 0, 255 );

	// if we're clicking the menu, let's update it, too
	if ( clicked( ) ) {
		// now let's update the mouse data
		poll_mouse( );

		// and the menu pos
		m_pos = { m_cursor.x - m_drag.x, m_cursor.y - m_drag.y };
	}
	else {
		// ugly, but we have to update the mouse pos anyway
		poll_mouse( );
	}

	// lambda to handle outlines
	auto outline = [ & ] ( int offset, uintptr_t colour ) {
		auto box = [ ] ( int x, int y, int w, int h, uintptr_t c ) {
			render::d3d::get().line( x, y, x, y + h, c );
			render::d3d::get().line( x, y + h, x + w, y + h, c );
			render::d3d::get().line( x + w, y, x + w, y + h, c );
			render::d3d::get().line( x, y, x + w, y, c );
		};

		box( m_pos.x - offset, m_pos.y - offset, m_size.x + offset * 2, m_size.y + offset * 2, colour );
	};

	// gay, but looks nice as hell
	outline( 0, COLOUR( 31, 31, 31 ) );
	outline( 1, COLOUR( 60, 60, 60 ) );
	outline( 2, COLOUR( 40, 40, 40 ) );
	outline( 3, COLOUR( 40, 40, 40 ) );
	outline( 4, COLOUR( 40, 40, 40 ) );
	outline( 5, COLOUR( 60, 60, 60 ) );
	outline( 6, COLOUR( 31, 31, 31 ) );

	// let's draw the menu 'base'
	render::d3d::get().filled_box( m_pos.x, m_pos.y, m_size.x, m_size.y, COLOUR( 25, 25, 25 ) );
		
	// calc the sidebar
	constexpr auto sidebar = 75;

	// draw the sidebar 'base'
	//render::d3d::get().line( m_pos.x + sidebar + 1, m_pos.y, m_pos.x + sidebar + 1, m_pos.y + m_size.y, COLOUR( 75, 75, 75 ) );
//	render::d3d::get().filled_box( m_pos.x, m_pos.y, sidebar, m_size.y, COLOUR( 22, 22, 22 ) );
	render::d3d::get().bordered_box_outlined(m_pos.x+ 7, m_pos.y +10, sidebar -10, m_size.y - 15, COLOUR(22, 22, 22), COLOUR(255, 255, 255), 1);

	// can't forget the coloured bar at the tob
	render::d3d::get().gradient( m_pos.x, m_pos.y, m_size.x / 2, 1, COLOUR( 59, 175, 222 ), COLOUR( 202, 70, 205 ) );
	render::d3d::get().gradient( m_pos.x + ( m_size.x / 2 ), m_pos.y, m_size.x / 2, 1, COLOUR( 202, 70, 205 ), COLOUR( 201, 227, 58 ) );
	render::d3d::get().line( m_pos.x, m_pos.y + 2, m_pos.x + m_size.x, m_pos.y + 2, COLOUR( 5, 5, 5 ) );

	// do the tabs 
	for ( auto &e : m_tabs ) {
		// only non-retarded thing i took from d3dc0m lol
		if ( e == m_blocking ) {
			continue;
		}

		// only if we are allowed to give sane input
		if ( m_active ) {
			e->update( );
		}

		// render them always for the fade effect
		e->render( );
	}

	// if an element is blockign something 
	if ( m_blocking ) {
		// update the blocking element last
		if ( m_active ) {
			m_blocking->update( );
		}

		// yea this too
		m_blocking->render( );
	}

	// let's draw this little guy last
	render_cursor( );
}

/* pasted from stackoverflow */

uintptr_t menu::modulate_alpha( uintptr_t original, int alpha ) {
	int a = ( original >> 24 ) & 0xff;
	int r = ( original >> 16 ) & 0xff;
	int g = ( original >> 8 ) & 0xff;
	int b = original & 0xff;

	int newAlpha = ceil( alpha * a );

	UINT newColor = r << 16;
	newColor += g << 8;
	newColor += b;
	newColor += ( newAlpha << 24 );


	return ( DWORD ) newColor;
}

/* paste from stackoverflow ends */

void menu::poll_mouse( ) {
	// update cursor position
	GetCursorPos( &m_cursor );
	ScreenToClient(deviant.m_hwnd, &m_cursor );

	// update mouse state
	if ( key_updated( VK_LBUTTON ) ) {
		m_clicked = true;
	}
	else if ( m_clicked && m_keystate [ VK_LBUTTON ] ) {
		m_clicked = false;
		m_holding = true;
	}
	else if ( m_holding && !( m_keystate [ VK_LBUTTON ] ) ) {
		m_holding = false;
	}
}

void menu::poll_keyboard( ) {
	// save old states
	std::copy( m_keystate, m_keystate + 255, m_oldstate );

	// simple keyboard iteration
	for ( auto n = 0; n < 255; ++n ) {
		// update current state
		m_keystate [ n ] = GetAsyncKeyState( n );
	}
}

void menu::render_cursor( ) {

	for ( int i = 0; i < 4; i++ )
		render::d3d::get().filled_box( m_cursor.x + 2 + i, m_cursor.y + 2 + i, 1, 14 - ( i * 2 ), COLOUR( 108, 195, 18 ) );

	render::d3d::get().filled_box( m_cursor.x + 6, m_cursor.y + 6, 1, 8, COLOUR( 108, 195, 18 ) );
	render::d3d::get().filled_box( m_cursor.x + 7, m_cursor.y + 7, 1, 9, COLOUR( 108, 195, 18 ) );

	for ( int i = 0; i < 4; i++ )
		render::d3d::get().filled_box( m_cursor.x + 8 + i, m_cursor.y + 8 + i, 1, 4 - i, COLOUR( 108, 195, 18 ) );

	render::d3d::get().filled_box( m_cursor.x + 8, m_cursor.y + 14, 1, 4, COLOUR( 108, 195, 18 ) );
	render::d3d::get().filled_box( m_cursor.x + 9, m_cursor.y + 16, 1, 2, COLOUR( 108, 195, 18 ) );
}

bool menu::clicked( ) {
	// we aren't clicking anything
	if ( !key_held( VK_LBUTTON ) ) {
		return false;
	}

	// or the menu is not active
	if ( !m_active ) {
		return false;
	}

	// see if we're clicking the top of the menu
	if ( in_bounds( m_pos, { m_size.x, ( long ) ( m_size.y * 0.05f ) } ) ) {
		// get the delta between the cursor and the menu pos and store it
		m_drag = { m_cursor.x - m_pos.x, m_cursor.y - m_pos.y };

		// we touched the menu, act accordingly
		return true;
	}

	// we did nothing menu related
	return false;
}