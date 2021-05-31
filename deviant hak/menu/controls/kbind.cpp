#include "kbind.h"

const char* keys[ ] = { "[_]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
"[_]", "[BSPC]", "[TAB]", "[_]", "[_]", "[_]", "[ENTER]", "[_]", "[_]", "[SHFT]",
"[CTRL]", "[ALT]","[PAUSE]","[CAPS]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[ESC]", "[_]", "[_]", "[_]", "[_]", "[SPACE]","[PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[Left]",
"[UP]", "[RIGHT]", "[DOWN]", "[_]", "[PRNT]", "[_]", "[PRTSCR]", "[INS]","[DEL]", "[_]", "[0]", "[1]",
"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
"[V]", "[W]", "[X]","[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[_]", "[_]", "[_]", "[NUM0]", "[NUM1]",
"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]","[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]","[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]", "[F21]",
"[F22]", "[F23]", "[F24]", "[_]", "[_]", "[_]", "[_]", "[_]","[_]", "[_]", "[_]",
"[NUMLCK]", "[SCRLLCK]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]","[_]", "[_]", "[_]", "[_]", "[_]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
"[RCTRL]", "[LMENU]", "[RMENU]", "[_]","[_]", "[_]","[_]", "[_]", "[_]", "[_]",
"[_]", "[_]", "[_]", "[NTRK]", "[PTRK]", "[STOP]", "[PLAY]", "[_]", "[_]",
"[_]", "[_]", "[_]", "[_]", "[;]", "[+]", "[,]", "[-]", "[.]", "[?]", "[~]", "[_]", "[_]",
"[_]", "[_]","[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]","[_]",
"[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]", "[_]","[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]", "[_]",
"[_]", "[_]" };

kbind::kbind( int x, int y, std::string var ) {
	m_opos = m_pos = { x - 30, y };
	m_var = var;
	m_size = { 60, 20 };
	
	// invalidate bind
	g_cfg [ var ].set( -1 );
}

void kbind::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x, m_parent->m_pos.y + m_opos.y };

	// we're listening
	if ( m_listening ) {
		// iterate all the keys
		for ( auto i = 0; i < 255; ++i ) {
			// a key is updated
			if ( menu::get( ).key_updated( i ) ) {
				// menu or escape key kills the listener
				if ( i == VK_DELETE || i == VK_ESCAPE ) {
					g_cfg [ m_var ].set( -1 );
					m_listening = false;
					break;
				}
				
				// we have a valid key
				else {
					g_cfg [ m_var ].set( i );
					m_listening = false;
					break;
				}
			}
		}
	}

	// let's see if we can listen
	else if ( menu::get( ).clicked( *this ) ) {
		m_listening = true;
	}
}

void kbind::render( ) {
	// sanity checks
	if ( !m_parent ) {
		return;
	}

	// listening or invalid key
	if ( m_listening || g_cfg [ m_var ].get<int>( ) == -1 ) {
		render::d3d::get().text( "[_]", m_pos.x + 60, m_pos.y - 2, right, fonts::menu_small, true, COLOUR( 110, 110, 110 ) );
	}

	// valid key
	else {
		render::d3d::get().text( keys[ g_cfg [ m_var ].get<int>( ) ], m_pos.x + 60, m_pos.y - 2, right, fonts::menu_small, true, COLOUR( 90, 90, 90 ) );
	}
}