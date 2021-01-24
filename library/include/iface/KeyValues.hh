#pragma once

#include "memory.hh"
#include "iface/common.hh"

namespace sw::iface
{
	class KeyValues
	{
	public:
		static KeyValues* FromString(const char* name, const char* value)
		{
			const auto kvFromString = sw::memory::KeyValuesFromString;
			KeyValues* kv;

			__asm
			{
				push 0
				mov edx, value
				mov ecx, name
				call kvFromString
				add esp, 4
				mov kv, eax
			}

			return kv;
		}

		KeyValues* FindKey(const char* key, bool create)
		{
			return memory::KeyValuesFindKey(this, key, create);
		}

		void SetString(const char* key, const char* value)
		{
			if (const auto keyInst = FindKey(key, true))
			{
				memory::KeyValuesSetString(keyInst, value);
			}
		}
	};
}