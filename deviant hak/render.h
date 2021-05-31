#pragma once
#include "common.h"
#include "misc\color.h"
#include "math\vector.h"
#include "sdk\isurface.h"
#include <array>
enum circle_type {
	full, half, quarter
};

enum text_alignment {
	left, centered, right
};

struct vertex {
	float x, y, z, rhw;
	unsigned long color;
};

namespace fonts {
	extern LPD3DXFONT menu;
	extern LPD3DXFONT menu_large;
	extern LPD3DXFONT menu_small;
};

class render : public singleton<render> {
public:
	class d3d : public singleton<d3d> {
	public:
		void initialize(IDirect3DDevice9* device);
		void reset(void);

		void create_objects();
		void release_objects();

		void begin();
		void end();

		int string_width(ID3DXFont* font, std::string in);

		void line(int x, int y, int x2, int y2, D3DCOLOR color);

		void filled_box(int x, int y, int w, int h, D3DCOLOR color);
		void filled_box_outlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness = 1);

		void bordered_box(int x, int y, int w, int h, D3DCOLOR color, int thickness = 1);
		void bordered_box_outlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness = 1);

		void gradient(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, bool vertical = false);
		void gradient_outlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, D3DCOLOR outline_color, bool vertical = false);

		void circle(int x, int y, int r, int p, D3DCOLOR color);
		void filled_circle(int x, int y, int r, int p, D3DCOLOR color);

		void text(std::string text, int x, int y, int o, LPD3DXFONT font, bool bordered, D3DCOLOR color);
	private:
		struct screen_t {
			float w;
			float h;
			float xc;
			float yc;
		} m_screen;

		LPDIRECT3DDEVICE9 m_device;
		LPDIRECT3DVERTEXBUFFER9 m_vb;
		LPDIRECT3DINDEXBUFFER9 m_ib;
		IDirect3DStateBlock9*				m_stateblock;

		LPD3DXLINE m_line;
		LPD3DXSPRITE m_sprite;

		bool m_candraw = false;

	};
	class isurface : public singleton<isurface> {
	public:

	//	void drawstring(unsigned long font, int x, int y, Color color, unsigned long alignment, const char * msg, ...);

		void rect(int x, int y, int w, int h, Color color);

		void outline(int x, int y, int w, int h, Color color);

		void emptyrect(int x1, int y1, int x2, int y2, Color color, unsigned char = 0);

		void text(int x, int y, const char * _Input, int font, Color color);



		RECT gettextsize(const char * _Input, int font);

		void line(int x, int y, int x2, int y2, Color color);

		void texturedpolygon(int n, Vertex_t * vertice, Color color);

		

		void add_triangle_filled(std::array<Vector2D, 3> points, Color color);

	};
	bool worldtoscreen(Vector world, Vector & screen);
};