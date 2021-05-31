#pragma once

#include "../math/qangle.h"
#include "studio.h"
#include "ienginetrace.h" //Has some structs we need here
#include "../hook.h"
class CPhysCollide;
class CUtlBuffer;
class IClientRenderable;
class CStudioHdr;
struct virtualmodel_t;

enum RenderableTranslucencyType_t
{
    RENDERABLE_IS_OPAQUE = 0,
    RENDERABLE_IS_TRANSLUCENT,
    RENDERABLE_IS_TWO_PASS,    // has both translucent and opaque sub-partsa
};

class IVModelInfo
{
public:
	inline void* GetModel(int Index) {
		return vmt::get().call_vfunc<void*(__thiscall *)(void*, int)>(this, 1)(this, Index);
	}
	int	GetModelIndex(const char *name)
	{
		typedef int(__thiscall* oGetModelName)(PVOID, const char *);
		return vmt::get().call_vfunc< oGetModelName >(this, 2)(this, name);
	}
	const char *GetModelName(const model_t *mod)
	{
		typedef const char *(__thiscall* oGetModelName)(PVOID, const model_t*);
		return vmt::get().call_vfunc< oGetModelName >(this, 3)(this, mod);
	}
	inline const char* GetModelName(const void* Model) {
		return vmt::get().call_vfunc<const char*(__thiscall *)(void*, const void*)>(this, 3)(this, Model);
	}
	studiohdr_t	*GetStudiomodel(const model_t *mod)
	{
		typedef studiohdr_t *(__stdcall* oGetStudiomodel)(const model_t*);
		return vmt::get().call_vfunc< oGetStudiomodel >(this, 32)(mod);
	}
};

class IVModelInfoClient : public IVModelInfo
{
public:
};