#pragma once

#include "iface/common.hh"

namespace sw::iface
{
	struct ConVar
	{
		LAZY_MEMBER(GetFloat, float, (), 12, (this));
		LAZY_MEMBER(GetInt, int, (), 13, (this));
		LAZY_MEMBER(SetValue, void, (const char* value), 14, (this, value));
		LAZY_MEMBER(SetValue, void, (float value), 15, (this, value));
		LAZY_MEMBER(SetValue, void, (int value), 16, (this, value));

		char pad0[24];
		std::add_pointer_t<void __cdecl> changeCallback;
		ConVar* parent;
		const char* defaultValue;
		char* stringValue;
	};
}