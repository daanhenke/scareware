#pragma once

#include "iface/common.hh"
#include "iface/IGameEvent.hh"

namespace sw::iface
{
	class IGameEventListener2
	{
	public:
		virtual ~IGameEventListener2() {};
		virtual void FireGameEvent(IGameEvent* event) = 0;
		virtual int GetEventDebugID(void)
		{
			return 42;
		}
	};
}