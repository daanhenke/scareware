#pragma once

#include "iface/common.hh"
#include "iface/IClientEntity.hh"
#include "iface/CUserCmd.hh"
#include "iface/IMoveHelper.hh"
#include "iface/CMoveData.hh"

namespace sw::iface
{
	class CPrediction
	{
	public:
		LAZY_MEMBER(SetupMove, void, (IClientEntity* localPlayer, CUserCmd* cmd, IMoveHelper* moveHelper, CMoveData* data), 20, (this, localPlayer, cmd, moveHelper, data));
		LAZY_MEMBER(FinishMove, void, (IClientEntity* localPlayer, CUserCmd* cmd, CMoveData* data), 21, (this, localPlayer, cmd, data));
	};
}