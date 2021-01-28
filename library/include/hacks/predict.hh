#pragma once
#include "iface/CUserCmd.hh"

namespace sw::hacks::predict
{
	extern int OldFlags;

	void RunPrediction(iface::CUserCmd* cmd);
}