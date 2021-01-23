#pragma once

#include "iface/common.hh"

namespace sw::iface
{
	class IMaterial;

	class IMaterialSystem
	{
	public:
		LAZY_MEMBER(FindMaterial, IMaterial*, (const char* materialName, const char* textureGroupName = nullptr, bool complain = true, const char* complainPrefix = nullptr), 84, (this, materialName, textureGroupName, complain, complainPrefix));
		LAZY_MEMBER(FirstMaterial, short, (), 86, (this));
		LAZY_MEMBER(NextMaterial, short, (short handle), 87, (this, handle));
		LAZY_MEMBER(InvalidMaterial, short, (), 88, (this));
		LAZY_MEMBER(GetMaterial, IMaterial*, (short handle), 89, (this, handle));
	};
}