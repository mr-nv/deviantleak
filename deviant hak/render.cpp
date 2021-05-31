#include "render.h"
#include "interfaces.h"
namespace fonts {
	LPD3DXFONT menu;
	LPD3DXFONT menu_large; 
	LPD3DXFONT menu_small;
};

void render::d3d::create_objects( ) {
	// if ( FAILED( m_device->CreateStateBlock( D3DSBT_ALL, &m_stateblock ) ) ) {
	//	m_candraw = false;
	// }
	//else {
	m_candraw = true;
	//}

	D3DXCreateLine( m_device, &m_line );
	D3DXCreateSprite( m_device, &m_sprite );

	D3DXCreateFontA( m_device, 12, 0, 400, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Verdana", &fonts::menu );
	D3DXCreateFontA( m_device, 12, 0, 700, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Verdana", &fonts::menu_large );
	D3DXCreateFontA( m_device, 13, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "Smallest Pixel-7", &fonts::menu_small );
}

void render::d3d::initialize( IDirect3DDevice9* device ) {
	m_device = device;
	create_objects( );
}

void render::d3d::release_objects( ) {
	if ( !m_candraw ) {
		return;
	}
	if ( m_line ) {
		m_line->Release( );
	}

	if ( m_sprite ) {
		m_sprite->Release( );
	}

	if ( fonts::menu ) {
		fonts::menu->Release( );
	}

	if ( fonts::menu_small ) {
		fonts::menu_small->Release( );
	}

	if ( fonts::menu_large ) {
		fonts::menu_large->Release( );
	}

}

void render::d3d::begin( ) {
	if ( !m_candraw ) {
		return;
	}

	// m_stateblock->Capture( );
	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_device->SetRenderState( D3DRS_ZENABLE, false );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );
	m_device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xFFFFFFFF );
	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
}

void render::d3d::end( ) {
	// if ( m_candraw )
	// 	m_stateblock->Apply( );
}

void render::d3d::reset( void ) {
	D3DVIEWPORT9 screen;
	m_device->GetViewport( &screen );

	m_screen.w = screen.Width;
	m_screen.h = screen.Height;
	m_screen.xc = screen.Width / 2.0f;
	m_screen.yc = screen.Height / 2.0f;
}

int render::d3d::string_width( ID3DXFont* font, std::string in ) {
	auto rect = RECT( );
	font->DrawTextA( 0, in.c_str( ), strlen( in.c_str( ) ), &rect, DT_CALCRECT, D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
	return rect.right - rect.left;
}

void render::d3d::line( int x, int y, int x2, int y2, D3DCOLOR color ) {
	vertex pVertex [ 2 ] = { { x, y, 0.0f, 1.0f, color }, { x2, y2, 0.0f, 1.0f, color } };

	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_device->SetTexture( 0, nullptr );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	m_device->DrawPrimitiveUP( D3DPT_LINELIST, 1, &pVertex, sizeof( vertex ) );
}

void render::d3d::filled_box( int x, int y, int w, int h, D3DCOLOR color ) {
	vertex pVertex [ 4 ] = { { x, y + h, 0.0f, 1.0f, color }, { x, y, 0.0f, 1.0f, color }, { x + w, y + h, 0.0f, 1.0f, color }, { x + w, y, 0.0f, 1.0f, color } };

	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_device->SetTexture( 0, nullptr );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	m_device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof( vertex ) );
}

void render::d3d::filled_box_outlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness ) {
	filled_box( x, y, w, h, color );
	bordered_box( x, y, w, h, outline );
}

void render::d3d::bordered_box( int x, int y, int w, int h, D3DCOLOR color, int thickness ) {
	filled_box( x, y, w, thickness, color );
	filled_box( x, y, thickness, h, color );
	filled_box( x + w - thickness, y, thickness, h, color );
	filled_box( x, y + h - thickness, w, thickness, color );
}

void render::d3d::bordered_box_outlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness ) {
	bordered_box( x, y, w, h, outline, thickness );
	bordered_box( x + thickness, y + thickness, w - ( thickness * 0.5f), h - ( thickness * 0.5f), color, thickness );
	bordered_box( x + ( thickness * 0.5f ), y + ( thickness * 0.5f), w - ( thickness * 2 ), h - ( thickness * 2 ), outline, thickness );
}

void render::d3d::gradient( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, bool vertical ) {
	vertex pVertex [ 4 ] = { { x, y, 0.0f, 1.0f, color }, { x + w, y, 0.0f, 1.0f, vertical ? color : other_color }, { x, y + h, 0.0f, 1.0f, vertical ? other_color : color }, { x + w, y + h, 0.0f, 1.0f, other_color } };

	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_device->SetTexture( 0, nullptr );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	m_device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof( vertex ) );
}

void render::d3d::gradient_outlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, D3DCOLOR outline_color, bool vertical ) {
	gradient( x, y, w, h, color, other_color, vertical );
	bordered_box( x, y, w, h, outline_color, 1 );
}

void render::d3d::circle( int x, int y, int r, int p, D3DCOLOR color ) {
	vertex* pVertex = new vertex [ p + 1 ];

	for ( int i = 0; i <= p; i++ ) {
		pVertex [ i ] = { x + r * cos( D3DX_PI * ( i / ( p / 2.0f ) ) ), y - r * sin( D3DX_PI * ( i / ( p / 2.0f ) ) ), 0.0f, 1.0f, color };
	}

	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	m_device->DrawPrimitiveUP( D3DPT_LINESTRIP, p, pVertex, sizeof( vertex ) );

	delete [ ] pVertex;
}

void render::d3d::filled_circle( int x, int y, int r, int p, D3DCOLOR color ) {
	vertex* pVertex = new vertex [ p + 1 ];

	for ( int i = 0; i <= p; i++ ) {
		pVertex [ i ] = { x + r * cos( D3DX_PI * ( i / ( p / 2.0f ) ) ), y + r * sin( D3DX_PI * ( i / ( p / 2.0f ) ) ), 0.0f, 1.0f, color };
	}

	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	m_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, p, pVertex, sizeof( vertex ) );

	delete [ ] pVertex;
}

#define GetA(col) (((col)&0xff000000)>>24)

void render::d3d::text( std::string text, int x, int y, int o, LPD3DXFONT font, bool bordered, D3DCOLOR color ) {
	RECT rect;

	auto alpha = GetA( color );

	switch ( o ) {
		case left:
			if ( bordered ) {
				SetRect( &rect, x - 1, y, x - 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x + 1, y, x + 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
			}
			SetRect( &rect, x, y, x, y );
			font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_LEFT | DT_NOCLIP, color );
			break;
		case centered:
			if ( bordered ) {
				SetRect( &rect, x - 1, y, x - 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x + 1, y, x + 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
			}
			SetRect( &rect, x, y, x, y );
			font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_CENTER | DT_NOCLIP, color );
			break;
		case right:
			if ( bordered ) {
				SetRect( &rect, x - 1, y, x - 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x + 1, y, x + 1, y );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
				SetRect( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB( alpha, 0, 0, 0 ) );
			}
			SetRect( &rect, x, y, x, y );
			font->DrawTextA( NULL, text.c_str( ), -1, &rect, DT_RIGHT | DT_NOCLIP, color );
			break;
	}
}
bool render::worldtoscreen(Vector world, Vector& screen)
{
	return (ctx::debugoverlay->ScreenPosition(world, screen) != 1);
}
void render::isurface::rect(int x, int y, int w, int h, Color color)
{
	ctx::isurface->DrawSetColor(color);
	ctx::isurface->DrawFilledRect(x, y, x + w, y + h);
}
void render::isurface::outline(int x, int y, int w, int h, Color color)
{
	ctx::isurface->DrawSetColor(color);
	ctx::isurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void render::isurface::emptyrect(int x1, int y1, int x2, int y2, Color color, unsigned char ignore_flags)
{
	ctx::isurface->DrawSetColor(color);
	if (!(ignore_flags & 0b1))
		ctx::isurface->DrawLine(x1, y1, x2, y1);
	if (!(ignore_flags & 0b10))
		ctx::isurface->DrawLine(x2, y1, x2, y2);
	if (!(ignore_flags & 0b100))
		ctx::isurface->DrawLine(x2, y2, x1, y2);
	if (!(ignore_flags & 0b1000))
		ctx::isurface->DrawLine(x1, y2, x1, y1);
}
void render::isurface::text(int x, int y, const char* _Input, int font, Color color)
{
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	ctx::isurface->DrawSetTextColor(color);
	ctx::isurface->DrawSetTextFont(font);
	ctx::isurface->DrawSetTextPos(x, y);
	ctx::isurface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}
RECT render::isurface::gettextsize(const char* _Input, int font)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;
	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);
	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
	int Width = 0, Height = 0;

	ctx::isurface->GetTextSize(font, WideBuffer, Width, Height);

	RECT outcome = { 0, 0, Width, Height };
	return outcome;
}
void render::isurface::line(int x, int y, int x2, int y2, Color color)
{
	ctx::isurface->DrawSetColor(color);
	ctx::isurface->DrawLine(x, y, x2, y2);
}
void render::isurface::texturedpolygon(int n, Vertex_t * vertice, Color color)
{
	static int texture_id = ctx::isurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	ctx::isurface->DrawSetTextureRGBA(texture_id, buf, 1, 1); 
	ctx::isurface->DrawSetColor(color);
	ctx::isurface->DrawSetTexture(texture_id); 
	ctx::isurface->DrawTexturedPolygon(n, vertice); 
}
void render::isurface::add_triangle_filled(std::array<Vector2D, 3>points, Color color)
{
	std::array<Vertex_t, 3>vertices{ Vertex_t(points.at(0)), Vertex_t(points.at(1)), Vertex_t(points.at(2)) };
	texturedpolygon(3, vertices.data(), color);
}