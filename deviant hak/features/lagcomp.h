#pragma once
#include "../common.h"
#include "../interfaces.h"
#include "../c_baseentity.h"
#include "../singleton.h"
#include "../sdk/cincomingsequences.h"
#include "../sdk/animstate.h"
#define ticks_to_time( t )		( ctx::globals->interval_per_tick *( t ) )
#define time_to_ticks( dt )		( (int)( 0.5f + (float)(dt) / tick_interval ) )
#define tick_interval			( ctx::globals->interval_per_tick )
class lagcomp : public singleton<lagcomp>
{
public:
};