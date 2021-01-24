#pragma once

#include "iface/common.hh"
#include "iface/IClientEntity.hh"
#include "iface/Vector.hh"

namespace sw::iface
{
	struct BeamInfo_t
	{
		int m_nType;
		IClientEntity* m_pStartEnt;
		int m_nStartAttachment;
		IClientEntity* m_pEndEnt;
		int m_nEndAttachment;

		Vector m_vecStart;
		Vector m_vecEnd;

		int m_nModelIndex;
		const char* m_pszModelName;

		int m_nHaloIndex;
		const char* m_pszHaloName;
		float m_flHaloScale;

		float m_flLife;
		float m_flWidth;
		float m_flEndWidth;
		float m_flFadeLength;
		float m_flAmplitude;

		float m_flBrightness;
		float m_flSpeed;

		int m_nStartFrame;
		float m_flFrameRate;

		float m_flRed;
		float m_flGreen;
		float m_flBlue;

		bool m_bRenderable;
		int m_nSegments;
		int m_nFlags;
		Vector m_vecRingCenter;
		float m_flRingStartRadius;
		float m_flRingEndRadius;
	};

	struct CBeam
	{
		char _pad0[52];
		int m_nFlags;
		char _pad1[144];
		float m_flDie;
	};

	class IViewRenderBeams
	{
	public:
		LAZY_MEMBER(CreateBeamPoints, CBeam*, (BeamInfo_t& beamInfo), 12, (this, std::ref(beamInfo)));
	};
}