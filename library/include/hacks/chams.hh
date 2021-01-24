#pragma once
#include "iface/ModelRender.hh"
#include "interfaces.hh"

namespace sw::hacks::chams
{
	void Initialize();
	bool Render(void* ctx, void* state, iface::ModelRenderInfo& info, iface::matrix3x4* customBoneToWorld);
}