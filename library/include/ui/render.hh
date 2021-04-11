#pragma once

#include <Windows.h>
#include <d3d9.h>

#ifndef NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "ui/nuklear.h"
#endif

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