#pragma once

#include <Windows.h>
#include <d3d9.h>

struct nk_context;

namespace sw::ui::render
{
	extern WNDPROC oWndProc;
	extern nk_context *nuklear_context;
	extern bool should_render;

	void Initialize(IDirect3DDevice9* device);
	void Render();
	void Reset(IDirect3DDevice9* device, HRESULT result);
}