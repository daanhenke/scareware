#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"

namespace sw::iface
{
	class ICollideable
	{
	public:
		LAZY_MEMBER(OBBMins, Vector&, (), 1, (this));
		LAZY_MEMBER(OBBMaxs, Vector&, (), 2, (this));
	};
}