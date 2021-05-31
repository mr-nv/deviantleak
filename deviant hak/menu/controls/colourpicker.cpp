#include "colourpicker.h"

/* pasted from stackoverflow */

LPDIRECT3DTEXTURE9 ColorPickerTexture;
LPD3DXSPRITE ColorPickerSprite;
int ColorPickerWidth;
int ColorPickerHeight;

D3DCOLOR HSLToRGB( float H, float S, float L ) {
	float Q;

	if ( L < 0.5f )
		Q = L * ( S + 1.0f );
	else
		Q = L + S - ( L * S );

	float P = 2 * L - Q;

	float RGBs [ 3 ];

	RGBs [ 0 ] = H + ( 1.0f / 3.0f );
	RGBs [ 1 ] = H;
	RGBs [ 2 ] = H - ( 1.0f / 3.0f );

	for ( int i = 0; i < 3; ++i ) {
		if ( RGBs [ i ] < 0 )
			RGBs [ i ] += 1.0f;

		if ( RGBs [ i ] > 1 )
			RGBs [ i ] -= 1.0f;

		if ( RGBs [ i ] < ( 1.0f / 6.0f ) )
			RGBs [ i ] = P + ( ( Q - P ) * 6 * RGBs [ i ] );
		else if ( RGBs [ i ] < 0.5f )
			RGBs [ i ] = Q;
		else if ( RGBs [ i ] < ( 2.0f / 3.0f ) )
			RGBs [ i ] = P + ( ( Q - P ) * 6 * ( ( 2.0f / 3.0f ) - RGBs [ i ] ) );
		else
			RGBs [ i ] = P;
	}

	return D3DCOLOR_XRGB( int( RGBs [ 0 ] * 255.0f ), int( RGBs [ 1 ] * 255.0f ), int( RGBs [ 2 ] * 255.0f ) );
}

void ColorPicker_Destroy( ) {
	if ( ColorPickerSprite != NULL ) {
		ColorPickerSprite->Release( );
		ColorPickerSprite = NULL;
	}

	if ( ColorPickerTexture != NULL ) {
		ColorPickerTexture->Release( );
		ColorPickerTexture = NULL;
	}
}

HRESULT ColorPicker_Init( LPDIRECT3DDEVICE9 Device, int Width, int Height ) {
	ColorPickerWidth = Width;
	ColorPickerHeight = Height;

	bool Bits32 = true;
	ColorPicker_Destroy( );

	HRESULT CreateTextureReturn;

	CreateTextureReturn = Device->CreateTexture( Width, Height, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &ColorPickerTexture, 0 );

	if ( FAILED( CreateTextureReturn ) ) {
		Bits32 = false;

		CreateTextureReturn = Device->CreateTexture( Width, Height, 1, 0, D3DFMT_X4R4G4B4, D3DPOOL_MANAGED, &ColorPickerTexture, 0 );

		if ( FAILED( CreateTextureReturn ) )
			return CreateTextureReturn;
	}

	D3DLOCKED_RECT Palette;

	CreateTextureReturn = ColorPickerTexture->LockRect( 0, &Palette, 0, 0 );

	if ( FAILED( CreateTextureReturn ) ) {
		ColorPicker_Destroy( );
		return CreateTextureReturn;
	}

	float H = 0;
	float S = 0.99f;
	float L = 1.0f;

	D3DCOLOR Color;

	BYTE R;
	BYTE G;
	BYTE B;

	DWORD* Colors32 = ( ( DWORD* ) Palette.pBits ) - 1;
	WORD* Colors = ( ( WORD* ) Palette.pBits ) - 1;

	for ( int i = 0; i < Width; ++i ) {
		for ( int j = 0; j < Height; ++j ) {
			Color = HSLToRGB( H, S, L );

			if ( Bits32 ) {
				Colors32++;
				*Colors32 = Color;
			}
			else {
				R = ( ( Color >> 16 ) & 0xFF ) / 0x10;
				G = ( ( Color >> 8 ) & 0xFF ) / 0x10;
				B = ( ( Color >> 0 ) & 0xFF ) / 0x10;

				Colors++;

				*Colors = ( 0xFF << 12 ) | ( R << 8 ) | ( G << 4 ) | ( B << 0 );
			}

			H += ( 1.0f / Width );
		}

		L -= ( 1.0f / Height );
		H = 0.0f;
	}

	ColorPickerTexture->UnlockRect( 0 );

	if ( ColorPickerSprite == NULL )
		D3DXCreateSprite( Device, &ColorPickerSprite );

	return S_OK;
}

LPDIRECT3DTEXTURE9 ColorPicker_GetTexture( ) {
	return ColorPickerTexture;
}

void DrawColorPicker( float X, float Y, LPDIRECT3DDEVICE9 Device ) {
	if ( !ColorPickerSprite )
		ColorPicker_Init( Device, 128, 128 );

	if ( ColorPickerSprite != NULL ) {
		ColorPickerSprite->Begin( D3DXSPRITE_ALPHABLEND );
		ColorPickerSprite->Draw( ColorPicker_GetTexture( ), NULL, NULL, &D3DXVECTOR3( X, Y, 0.0f ), 0xFFFFFFFF );
		ColorPickerSprite->End( );
	}
}

D3DCOLOR GetPickedColor( int X, int Y ) {
	float H = X * ( 1.0f / ColorPickerWidth );
	float S = 0.99f;
	float L = 1.0f - Y * ( 1.0f / ColorPickerHeight );

	return HSLToRGB( H, S, L );
}

/* paste from stackoverflow ends */

colourpicker::colourpicker( int x, int y, std::string var, uintptr_t default_colour ) {
	m_opos = m_pos = { x, y };
	m_size = { 15, 8 };
	m_var = var;

	// set default value l0l
	g_cfg [ m_var ].set( default_colour );
}

void colourpicker::update( ) {
	// sanity checks
	if ( !m_parent || menu::get( ).blocked( ) ) {
		return;
	}

	// setup new position
	m_pos = { m_parent->m_pos.x + m_opos.x + 3, m_parent->m_pos.y + m_opos.y };

	// toggle if it's clicked
	if ( menu::get( ).clicked( *this ) ) {
		m_picking = true;
	}

	// we should draw the picker too
	if ( m_picking ) {
		// don't mess anything else up, render this last.
		menu::get( ).block( true, this );
	}
}

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)

void colourpicker::render( ) {
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


	// let's draw the picker
	if ( m_picking ) {
		// render the picker next to the box
		DrawColorPicker( ( m_pos.x + m_size.x + 1 ) + 5, m_pos.y + 1, menu::get( ).m_device );
		box( ( m_pos.x + m_size.x + 1 ) + 5, m_pos.y + 1, 128, 128, COLOUR( 5, 5, 5 ) );

		// mouse is held, let's see what happened
		if ( menu::get( ).key_held( VK_LBUTTON ) ) {
			// we can pick a colour
			if ( menu::get( ).in_bounds( { ( m_pos.x + m_size.x + 1 ), m_pos.y - 4 }, { 133, 133 } ) ) {
				auto colour = GetPickedColor( menu::get( ).cursor( ).x - ( ( m_pos.x + m_size.x + 1 ) + 5 ), menu::get( ).cursor( ).y - ( m_pos.y + 1 ) );
				g_cfg [ m_var ].set( colour );
			}
			// probably can't, don't bother.
			else if ( !menu::get( ).in_bounds( *this ) ) {
				m_picking = false;
			}
		}

		if ( !menu::get( ).active( ) ) {
			m_picking = false;
		}
	}

	// get gradient shit
	auto colour = g_cfg [ m_var ].get<uintptr_t>( );

	// modulate for the gradient
	auto o_r = get_r( colour );
	auto o_g = get_g( colour );
	auto o_b = get_b( colour );
	auto new_alpha = clamp( menu::get( ).alpha( ), 0, ( int ) ( 255 * 0.46 ) );
	auto grad2 = D3DCOLOR_ARGB( menu::get( ).alpha( ), o_r, o_g, o_b );
	auto grad = D3DCOLOR_ARGB( new_alpha, o_r, o_g, o_b );

	// render in box
	render::d3d::get( ).gradient( m_pos.x + 1, m_pos.y + 1, m_size.x - 2, m_size.y - 2, grad2, grad, true );
}