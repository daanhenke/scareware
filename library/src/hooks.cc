#include "hooks.hh"
#include "interfaces.hh"
#include "console.hh"

sw::vtable::VTableHook* sw::hooks::IBaseClientDLL = nullptr;
sw::vtable::VTableHook* sw::hooks::IPanel = nullptr;

sw::hooks::PaintTraverseFn oPaintTraverse;
void __fastcall pt_hook(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    // We want to cache the panel ID of the panel we want to render to
    static unsigned int uiPanelId = 0;
    static bool shouldDrawUi = false;

    // If the panel ID isn't found already try to find it
    if (uiPanelId == 0)
    {
        if (strstr(sw::interfaces::IPanel->GetName(vguiPanel), "MatSystemTopPanel"))
        {
            uiPanelId = vguiPanel;
            sw::interfaces::ICvar->ConsoleDPrintf("Found MatSystemTopPanel: 0x%x", vguiPanel);
        }
    }
    
    // Call the original function
    oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

    // Handling input in the render func, bad...
    shouldDrawUi = sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::KEY_INSERT);

    // Draw some random shit
    if (uiPanelId != 0 && vguiPanel == uiPanelId && shouldDrawUi)
    {
        //sw::interfaces::ISurface->DrawSetTextPos(50, 50);
        const wchar_t* str = L"Ewa Gibba";
        //sw::interfaces::ISurface->DrawPrintText(str, wcslen(str));
        sw::interfaces::ISurface->DrawSetColor(255, 255, 255, 128);
        sw::interfaces::ISurface->DrawFilledRect(0, 0, 200, 150);
    }
}

void sw::hooks::HookAll()
{
    IBaseClientDLL = new vtable::VTableHook((DWORD*) interfaces::IBaseClientDLL);
    IPanel = new vtable::VTableHook((DWORD*) interfaces::IPanel);

    oPaintTraverse = (PaintTraverseFn) IPanel->HookMethod((DWORD) &pt_hook, 41);
    sw::interfaces::ICvar->ConsoleDPrintf("oPaintTraverse: %x\n", oPaintTraverse);
}

void sw::hooks::UnhookAll()
{
    IBaseClientDLL->RestoreOld();
    IPanel->RestoreOld();
}