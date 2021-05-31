#pragma once
#include "../menu.h"

class listbox : public element {
public:
	listbox( int x, int y, const char** elements, int size, int *option );

	virtual void update( ) override;
	virtual void render( ) override;

private:
	void draw_scroll( int x, int y, int h, float min, float max, void* value );
	
	std::vector<std::string> m_elements;
	int* m_option = nullptr;
};