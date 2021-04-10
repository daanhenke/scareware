#include "ui/render.hh"
#include "memory.hh"

#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "ui/nuklear.h"

#define NK_D3D9_IMPLEMENTATION
#include "ui/nuklear_d3d9.h"

RECT window_rect;

nk_context* sw::ui::render::nuklear_context = nullptr;
bool sw::ui::render::should_render = true;

WNDPROC sw::ui::render::oWndProc = 0;
LRESULT __stdcall WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    if (uMsg == WM_KEYUP && wParam == VK_INSERT)
    {
        sw::ui::render::should_render = !sw::ui::render::should_render;
    }

    if (sw::ui::render::should_render)
    {
        if (nk_d3d9_handle_event(hWnd, uMsg, wParam, lParam)) return 0;
    }

    return CallWindowProcW(sw::ui::render::oWndProc, hWnd, uMsg, wParam, lParam);
}

void sw::ui::render::Initialize(IDirect3DDevice9* device)
{
    D3DDEVICE_CREATION_PARAMETERS params;
    sw::memory::D3DDevice->GetCreationParameters(&params);

    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(params.hFocusWindow, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

    GetWindowRect(params.hFocusWindow, &window_rect);
    nuklear_context = nk_d3d9_init(device, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top);

    struct nk_font_atlas* atlas;
    nk_d3d9_font_stash_begin(&atlas);
    nk_d3d9_font_stash_end();
}

void sw::ui::render::Render()
{
    if (should_render)
    {
        if (nk_begin(nuklear_context, "Meme Window", nk_rect(50, 50, 220, 220), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
        {
            nk_layout_row_static(nuklear_context, 35, 80, 1);
            nk_button_label(nuklear_context, "Test!");
        }
        nk_end(nuklear_context);
    }

    nk_d3d9_render(NK_ANTI_ALIASING_ON);

    nk_input_begin(nuklear_context);
    nk_input_end(nuklear_context);
}

void sw::ui::render::Reset(IDirect3DDevice9* device, HRESULT result)
{
    if (nuklear_context)
    {
        nk_free(nuklear_context);
        nuklear_context = nullptr;
    }

    nk_d3d9_release();

    if (SUCCEEDED(result))
    {
        D3DVIEWPORT9 viewport;
        device->GetViewport(&viewport);

        nuklear_context = nk_d3d9_init(device, viewport.Width, viewport.Height);
        struct nk_font_atlas* atlas;
        nk_d3d9_font_stash_begin(&atlas);
        nk_d3d9_font_stash_end();
    }
}