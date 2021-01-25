#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"

namespace sw::iface
{
	class IVDebugOverlay
	{
	public:
		LAZY_MEMBER(ScreenPosition, int, (const Vector& origin, Vector& screen), 13, (this, origin, screen));
		LAZY_MEMBER(ScreenPosition, int, (float x, float y, Vector& screen), 14, (this, x, y, screen));
	};
}