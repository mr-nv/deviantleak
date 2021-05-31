#pragma once
#include "common.h"
#include "netvar.h"
#include "c_baseentity.h"
#define netvar_fn( t, f, a, b )\
t& f( void )\
{\
static uintptr_t o = netvar->get_offset( a, b );\
return *( t* ) ( ( uintptr_t ) this + o );\
}

// creates a fn for the netvar, and then adds the desired offset
// note to whoever didnt do 6th grade math, adding negative numbers = subtracting
#define anetvar_fn( t, f, a, b, d )\
t& f( void )\
{\
static uintptr_t o = netvar->get_offset( a, b );\
return *( t* ) ( ( uintptr_t ) ( this + o ) + d );\
}

// creates a fn based on a static offset
// note from fluc: ur REALLY gay if u use this.
#define offset_fn( t, f, o )\
t& f( void )\
{\
return *( t* )( ( uintptr_t ) this + o );\
}
// ================================================
//  system imports
// ================================================
