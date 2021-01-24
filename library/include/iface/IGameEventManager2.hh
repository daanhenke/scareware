#pragma once

#include "iface/common.hh"
#include "iface/IGameEvent.hh"
#include "iface/IGameEventListener2.hh"

namespace sw::iface
{
	class IGameEventManager2
	{
	public:
		LAZY_MEMBER(AddListener, bool, (IGameEventListener2* listener, const char* name, bool serverSide = false), 3, (this, listener, name, serverSide));
		LAZY_MEMBER(RemoveListener, void, (IGameEventListener2* listener), 5, (this, listener));
	};
}