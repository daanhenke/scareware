#pragma once

#include "iface/common.hh"
#include "iface/IClientEntity.hh"

namespace sw::iface
{
	class IMoveHelper
	{
	public:
		LAZY_MEMBER(SetHost, void, (iface::IClientEntity* entity), 1, (this, entity));
	};
}