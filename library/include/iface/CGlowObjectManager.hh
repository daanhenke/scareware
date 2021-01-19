#pragma once
#include "iface/IClientEntity.hh"
#include "iface/Vector.hh"

namespace sw::iface
{
	struct GlowObjectDefinition_t
	{
		constexpr bool IsUnused() const
		{
			return m_nNextFreeSlot != -2;
		}

		IClientEntity* m_hEntity;
		Vector m_vGlowColor;
		float m_flGlowAlpha;

		char _pad0[4];
		float _pad1;

		float m_flBloomAmount;
		float m_somethingsomethinglocalplayer;

		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;

		char _pad2;

		int m_nFullBloomStencilTestValue;
		int _pad3;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
	};

	struct CGlowObjectManager
	{
		GlowObjectDefinition_t* m_GlowObjectDefinitions;
		int max_size;
		int _pad0;
		int size;
		GlowObjectDefinition_t* m_GlowObjectDefinitions2;
		int currentObjects;
	};
}