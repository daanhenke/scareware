#pragma once

namespace sw::iface
{
	struct CGlobalVars
	{
		float realtime;
		int framecount;
		float absoluteFrameTime;
		char _pad0[4];
		float currenttime;
		float frametime;
		int maxClients;
		int tickCount;
		float intervalPerTick;
	};
}