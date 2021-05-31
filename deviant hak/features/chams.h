#pragma once

#include "../common.h"

class Chams {
public:
	void override_material(bool ignoreZ, bool textured, bool flat, bool metalic, const Color& rgba);
	void override_material(bool ignoreZ, bool flat, const float alpha);

	Chams();
	~Chams();

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialMetalic = nullptr;
	IMaterial* materialMetalicIgnoreZ = nullptr;
};

extern Chams* chams;