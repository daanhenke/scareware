#pragma once

#include <type_traits>
#include "ClassId.hh"

namespace sw::iface
{
	class Entity;
	struct RecvTable;

	struct ClientClass {
		std::add_pointer_t<Entity* __cdecl(int, int)> createFunction;
		void* createEventFunction;
		char* networkName;
		RecvTable* recvTable;
		ClientClass* next;
		ClassId* classId;
	};
}