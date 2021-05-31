#pragma once
#include "../interfaces.h"
#include "cclientstate.h"

class incomingsequence {
public:
	int in_reliable_state,
		out_reliable_state,
		in_sequence_nr;
	float realtime;

	incomingsequence() {
		in_reliable_state = (ctx::clientstate)->m_NetChannel->m_nInReliableState;
		out_reliable_state = (ctx::clientstate)->m_NetChannel->m_nOutReliableState;
		in_sequence_nr = (ctx::clientstate)->m_NetChannel->m_nInSequenceNr;
		realtime = ctx::globals->realtime;
	}

	incomingsequence(int irs, int ors, int isn, float _realtime) {
		in_reliable_state = irs;
		out_reliable_state = ors;
		in_sequence_nr = isn;
		realtime = _realtime;
	}
};
