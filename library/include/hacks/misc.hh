#pragma once
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"

namespace sw::hacks::misc
{
	void Bunnyhop(iface::CUserCmd* cmd);
	void ThirdPerson();

	void GlowPlayers();
	void DarkWorld();
	void Skybox(iface::FrameStage stage);

	void NoFlash();
	void NoSmoke(iface::FrameStage stage);
}