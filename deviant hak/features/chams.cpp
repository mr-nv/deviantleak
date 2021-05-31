#include "Chams.h"
#include "../interfaces.h"
#include "../security/xorstr.h"
#include <fstream>

Chams* chams;
Chams::Chams()
{
	std::ofstream("csgo\\materials\\deviant_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[1 1 1]"
}
)#";
	std::ofstream("csgo\\materials\\deviant_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[1 1 1]"
}
)#";
	std::ofstream("csgo\\materials\\deviant_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\deviant_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
std::ofstream("csgo\\materials\\deviant_regular_reflective.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
std::ofstream("csgo\\materials\\deviant_regular_reflective_ignorez.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "1"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
	materialRegular = ctx::matsys->FindMaterial(xorstr("deviant_regular"), TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = ctx::matsys->FindMaterial(xorstr("deviant_ignorez"), TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = ctx::matsys->FindMaterial(xorstr("deviant_flat_ignorez"), TEXTURE_GROUP_MODEL);
	materialFlat = ctx::matsys->FindMaterial(xorstr("deviant_flat"), TEXTURE_GROUP_MODEL);
	materialMetalic= ctx::matsys->FindMaterial(xorstr("deviant_regular_reflective"), TEXTURE_GROUP_MODEL);
	materialMetalicIgnoreZ = ctx::matsys->FindMaterial(xorstr("deviant_regular_reflective_ignorez"), TEXTURE_GROUP_MODEL);
}

Chams::~Chams()
{
	std::remove(xorstr("csgo\\materials\\deviant_regular.vmt"));
	std::remove(xorstr("csgo\\materials\\deviant_ignorez.vmt"));
	std::remove(xorstr("csgo\\materials\\deviant_flat.vmt"));
	std::remove(xorstr("csgo\\materials\\deviant_flat_ignorez.vmt"));
}

void Chams::override_material(bool ignoreZ,bool textured, bool flat,bool metalic, const Color& rgba)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else if (textured) {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}
	else if (metalic)
	{
		if (ignoreZ)
			material = materialMetalicIgnoreZ;
		else
			material = materialMetalic;
	}
    material->AlphaModulate(rgba.a() / 255.0f);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	ctx::mdlrender->ForcedMaterialOverride(material);
}

void Chams::override_material(bool ignoreZ, bool flat, const float alpha)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	material->AlphaModulate(alpha / 255.0f);

	ctx::mdlrender->ForcedMaterialOverride(material);
}