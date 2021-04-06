#pragma once
#include "iface/IGameEvent.hh"

namespace sw::hacks::chat
{
	void LogVoteStart(iface::IGameEvent* ev);
	void LogVoteCast(iface::IGameEvent* ev);
}