#pragma once

#include "iface/common.hh"
#include "iface/IClientEntity.hh"
#include "iface/CMoveData.hh"

namespace sw::iface
{
	class IGameMovement
	{
	public:
		LAZY_MEMBER(ProcessMovement, void, (IClientEntity* localPlayer, CMoveData* moveData), 1, (this, localPlayer, moveData))
	};
}