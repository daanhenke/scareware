#pragma once

#include "iface/common.hh"

namespace sw::iface
{
	enum class MaterialVarFlag
	{
		NO_DRAW = 1 << 2,
		IGNOREZ = 1 << 15,
		WIREFRAME = 1 << 28
	};

	class IMaterial
	{
	public:
		LAZY_MEMBER(GetTextureGroupName, const char*, (), 1, (this));
		LAZY_MEMBER(ColorModulate, void, (float r, float g, float b), 28, (this, r, g, b));
		LAZY_MEMBER(IsPrecached, bool, (), 70, (this));
		LAZY_MEMBER(SetMaterialVarFlag, void, (MaterialVarFlag flag, bool enabled), 29, (this, flag, enabled));
	};
}