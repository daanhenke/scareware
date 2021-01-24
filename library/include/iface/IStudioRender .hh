#pragma once

#include "iface/common.hh"
#include "iface/IMaterial.hh"

namespace sw::iface
{
	enum OverrideType_t
	{
		OVERRIDE_NORMAL = 0,
		OVERRIDE_BUILD_SHADOWS,
		OVERRIDE_DEPTH_WRITE,
		OVERRIDE_SSAO_DEPTH_WRITE,
	};

	class IStudioRender
	{
	public:
		LAZY_MEMBER(ForcedMaterialOverride, void, (IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int iIndex = -1), 33, (this, newMaterial, nOverrideType, iIndex))
	};
}