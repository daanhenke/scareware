#include "hooks.hh"
#include "interfaces.hh"
#include "console.hh"

sw::vtable::VTableHook* sw::hooks::IBaseClientDLL = nullptr;
sw::vtable::VTableHook* sw::hooks::IPanel = nullptr;

sw::hooks::PaintTraverseFn oPaintTraverse;
void __fastcall pt_hook(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    static int meme = 0;
    meme++;
    sw::interfaces::ICvar->ConsoleDPrintf("Hooked render function, iteration: %x\n", meme);
    oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
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