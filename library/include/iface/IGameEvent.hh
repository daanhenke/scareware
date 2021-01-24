#pragma once

#include "iface/common.hh"

namespace sw::iface
{
	class IGameEvent
	{
	public:
		LAZY_MEMBER(GetName, const char*, (), 1, (this));
		LAZY_MEMBER(GetInt, int, (const char* keyName, int defaultValue = 0), 6, (this, keyName, defaultValue));
		LAZY_MEMBER(GetFloat, float, (const char* keyName, float defaultValue = 0.f), 8, (this, keyName, defaultValue));
	};
}