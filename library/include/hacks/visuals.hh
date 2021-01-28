#pragma once

#include "iface/IClientEntity.hh"

namespace sw::hacks::visuals
{
	void Render();
	void RenderPlayer(iface::IClientEntity* entity);
	void RenderVelocity();
	void RenderSpectatorList();

	extern int Velocity;
}