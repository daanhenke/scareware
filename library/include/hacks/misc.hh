#pragma once
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"
#include "iface/IGameEvent.hh"

namespace sw::hacks::misc
{
	void Bunnyhop(iface::CUserCmd* cmd);
	void ThirdPerson();

	void GlowPlayers();
	void DarkWorld();
	void Skybox(iface::FrameStage stage);
	void Remove3dSky();

	void NoFlash();
	void NoSmoke(iface::FrameStage stage);

	void RemoveRecoil(iface::FrameStage stage);
	void RecoilControl(iface::CUserCmd* cmd);

	void MemeRagdolls();
	void NoscopeCrosshair();

	void BulletTracers(iface::IGameEvent* event);
}