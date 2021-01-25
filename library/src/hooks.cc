#include "hooks.hh"
#include "interfaces.hh"
#include "console.hh"
#include "logic.hh"
#include "util.hh"
#include "hacks/skin.hh"
#include "hacks/misc.hh"
#include "hacks/chams.hh"
#include "memory.hh"

sw::vtable::VTableHook* sw::hooks::IBaseClientDLL = nullptr;
sw::vtable::VTableHook* sw::hooks::IPanel = nullptr;
sw::vtable::VTableHook* sw::hooks::ClientModeShared = nullptr;
sw::vtable::VTableHook* sw::hooks::IVModelRender = nullptr;
sw::vtable::VTableHook* sw::hooks::SvCheats = nullptr;
sw::vtable::VTableHook* sw::hooks::IGameEventManager2 = nullptr;

sw::hooks::DrawModelExecuteFn sw::hooks::oDrawModelExecute = nullptr;

sw::hooks::PaintTraverseFn oPaintTraverse;
void __fastcall pt_hook(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    // We want to cache the panel ID of the panel we want to render to
    static unsigned int uiPanelId = 0;
    static bool shouldDrawUi = false;
    static sw::iface::HFont font = 0;
    static int fontoff = 0;
    static bool fontoffChanged = false;

    // If the panel ID isn't found already try to find it
    if (uiPanelId == 0)
    {
        if (strstr(sw::interfaces::IPanel->GetName(vguiPanel), "MatSystemTopPanel"))
        {
            uiPanelId = vguiPanel;
            sw::interfaces::ICvar->ConsoleDPrintf("Found MatSystemTopPanel: 0x%x", vguiPanel);
            font = sw::interfaces::ISurface->CreateFont();
            sw::interfaces::ISurface->SetFontGlyphSet(font, "Arial", 40, 650, 0, 0, sw::iface::EFontFlags::FONTFLAG_ANTIALIAS);
        }
    }
    
    // Call the original function
    oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

    if (uiPanelId == 0 || uiPanelId != vguiPanel)
    {
        return;
    }

    // Handling input in the render func, bad...
    shouldDrawUi = sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::KEY_INSERT);

    // Uninject method
    if (sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::KEY_END))
    {
        sw::logic::UnloadSelf();
    }

    if (sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::MOUSE_4))
    {
        sw::interfaces::ISurface->PlaySound("buttons/arena_switch_press_02.wav");
    }

    if (sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::MOUSE_5))
    {
        if (! fontoffChanged) {
            fontoff += 10;
            sw::interfaces::ICvar->ConsoleDPrintf("Fontoff: %d\n", fontoff);
            fontoffChanged = true;
        }
    }
    else {
        fontoffChanged = false;
    }

    //sw::interfaces::ICvar->ConsoleDPrintf("Client Number: %x, Pointer to Entity: %x\n", localid, localent);
    //sw::interfaces::ICvar->ConsoleDPrintf("Eye pos: X %.4f, Y %.4f, Z %.4f\n", eyePos.x, eyePos.y, eyePos.z);
    //sw::interfaces::ICvar->ConsoleDPrintf("Currently in game: %s\n", sw::interfaces::IVEngineClient->IsInGame() ? "Yes" : "No :(");
    //sw::interfaces::ICvar->ConsoleDPrintf("Entity count xDDD: %x\n", sw::interfaces::IClientEntityList->NumberOfEntities());

    int x, y;
    sw::interfaces::ISurface->SurfaceGetCursorPos(x, y);
    //sw::interfaces::ICvar->ConsoleDPrintf("X %d, Y: %d\n", x, y);

    if (uiPanelId != 0 && vguiPanel == uiPanelId)
    {
        sw::hacks::misc::NoscopeCrosshair();
    }

    // Draw some random shit
    //if (uiPanelId != 0 && vguiPanel == uiPanelId && shouldDrawUi)
    //{
    //    const wchar_t* str = L"Ewa Gibba";
    //    sw::interfaces::ISurface->DrawSetTextFont(font);
    //    sw::interfaces::ISurface->DrawSetTextColor(0, 0, 0, 100);
    //    sw::interfaces::ISurface->DrawSetTextPos(x + 1, y + 1);
    //    sw::interfaces::ISurface->DrawPrintText(str, wcslen(str));
    //    sw::interfaces::ISurface->DrawSetTextColor(255, 0, 255, 255);
    //    sw::interfaces::ISurface->DrawSetTextPos(x, y);
    //    sw::interfaces::ISurface->DrawPrintText(str, wcslen(str));

    //    sw::interfaces::ISurface->DrawSetTextFont(0xa1);
    //    sw::interfaces::ISurface->DrawSetTextPos(x + 50, y);
    //    const wchar_t* meme = L"\x52";
    //    sw::interfaces::ISurface->DrawPrintText(meme, wcslen(meme));

    //    // Draw all entities
    //    int maxIndex = sw::interfaces::IClientEntityList->GetHighestEntityIndex();
    //    for (int entIndex = 0; entIndex < maxIndex; entIndex++)
    //    {
    //        sw::iface::IClientEntity* entity = sw::interfaces::IClientEntityList->GetClientEntity(entIndex);

    //        if (entity == nullptr) continue;
    //        //sw::iface::IClientNetworkable* networkable = entity->GetClientNetworkable();
    //        //if (networkable == nullptr) continue;

    //        //sw::interfaces::ICvar->ConsoleDPrintf("Entity '%x' dormant: %s\n", entIndex, networkable->IsDormant() ? "ye" : "na");

    //        //sw::iface::C_BaseEntity* base = entity->GetBaseEntity();
    //        //if (base == nullptr) continue;

    //        sw::iface::Vector pos3d; //= entity->GetEyePosition();
    //        pos3d.x = 0;
    //        pos3d.y = 0;
    //        pos3d.z = 0;
    //        sw::iface::Vector2D pos2d = sw::util::WorldToScreen(pos3d);

    //        sw::interfaces::ISurface->DrawSetColor(0, 0, 255, 255);
    //        sw::interfaces::ISurface->DrawFilledRect(pos2d.x - 2, pos2d.y - 2, pos2d.x + 2, pos2d.y + 2);
    //    } 

    //    // for (wchar_t i = 0; i < 10; i++)
    //    // {
    //    //     wchar_t val = i + fontoff;
    //    //     sw::interfaces::ISurface->DrawSetTextPos(50 * i, y + 50);
    //    //     sw::interfaces::ISurface->DrawPrintText(&val, wcslen(&val));
    //    // }
    //}
}

sw::hooks::CreateMoveFn oCreateMove;
bool __stdcall cm_hook(float frametime, sw::iface::CUserCmd* pCmd)
{
    bool res = oCreateMove(sw::interfaces::ClientModeShared, frametime, pCmd);

    if (!pCmd->command_number) return res;

    sw::hacks::misc::Bunnyhop(pCmd);
    sw::hacks::misc::RecoilControl(pCmd);

    return false;
}

sw::hooks::FrameStageNotifyFn oFrameStageNotify;
void __fastcall fsn_hook(void* _this, int edx, sw::iface::FrameStage stage)
{
    if (sw::interfaces::IVEngineClient->IsInGame())
    {
        if (stage == sw::iface::FrameStage::NET_UPDATE_POSTDATAUPDATE_START)
        {
            sw::hacks::skin::FrameStageNotify();
            sw::hacks::misc::ThirdPerson();
            //sw::hacks::misc::GlowPlayers();
            sw::hacks::misc::NoFlash();
            sw::hacks::misc::MemeRagdolls();
            sw::hacks::misc::Remove3dSky();
        }
        else if (stage == sw::iface::FrameStage::RENDER_START)
        {
            sw::hacks::misc::DarkWorld();
        }

        sw::hacks::misc::Skybox(stage);
        sw::hacks::misc::NoSmoke(stage);
        sw::hacks::misc::RemoveRecoil(stage);
    }

    oFrameStageNotify(sw::interfaces::IBaseClientDLL, edx, stage);
}

sw::hooks::SvCheatsGetFn oSvCheatsGet;
bool __fastcall svcheats_get_hook(void* _this)
{
    if (sw::memory::CameraThinkPtr == uintptr_t(_ReturnAddress()))
    {
        return true;
    }

    return oSvCheatsGet(_this);
}

sw::hooks::FireEventsClientSideFn oFireEventsClientSide;
bool __fastcall fecs_hook(void* _this, int edx, sw::iface::IGameEvent* event)
{
    auto eventName = event->GetName();

    if (! std::strcmp(eventName, "bullet_impact"))
    {
        sw::hacks::misc::BulletTracers(event);
    }

    return oFireEventsClientSide(_this, edx, event);
}

void __fastcall dme_hook(void* _this, int edx, void* ctx, void* state, sw::iface::ModelRenderInfo& info, sw::iface::matrix3x4* customBoneToWorld)
{
    auto localPlayer = sw::interfaces::GetLocalPlayer();
    if (localPlayer)
    {
        bool success = sw::hacks::chams::Render(ctx, state, info, customBoneToWorld);

        if (!success)
        {
            sw::hooks::oDrawModelExecute(_this, ctx, state, info, customBoneToWorld);
        }
    }

    sw::interfaces::IStudioRender->ForcedMaterialOverride(nullptr);
}

class DummyEventListener : public sw::iface::IGameEventListener2
{
    ~DummyEventListener() {}
    void FireGameEvent(sw::iface::IGameEvent* event)
    {
    }
};

DummyEventListener* m_dummy_event_listener = nullptr;
void sw::hooks::HookAll()
{
    IBaseClientDLL = new vtable::VTableHook((DWORD*) interfaces::IBaseClientDLL);
    IPanel = new vtable::VTableHook((DWORD*) interfaces::IPanel);
    ClientModeShared = new vtable::VTableHook((DWORD*) interfaces::ClientModeShared);
    SvCheats = new vtable::VTableHook((DWORD*)interfaces::ICvar->FindVar("sv_cheats"));
    IGameEventManager2 = new vtable::VTableHook((DWORD*) interfaces::IGameEventManager2);
    IVModelRender = new vtable::VTableHook((DWORD*)interfaces::IVModelRender);

    oPaintTraverse = (PaintTraverseFn) IPanel->HookMethod((DWORD) &pt_hook, 41);

    oCreateMove = (CreateMoveFn) ClientModeShared->HookMethod((DWORD) &cm_hook, 24);

    oFrameStageNotify = (FrameStageNotifyFn)IBaseClientDLL->HookMethod((DWORD)&fsn_hook, 37);

    oSvCheatsGet = (SvCheatsGetFn) SvCheats->HookMethod((DWORD)&svcheats_get_hook, 13);

    oFireEventsClientSide = (FireEventsClientSideFn)IGameEventManager2->HookMethod((DWORD)&fecs_hook, 9);

    oDrawModelExecute = (DrawModelExecuteFn)IVModelRender->HookMethod((DWORD)&dme_hook, 21);

    m_dummy_event_listener = new DummyEventListener();
    interfaces::IGameEventManager2->AddListener(m_dummy_event_listener, "bullet_impact", false);
}

void sw::hooks::UnhookAll()
{
    IBaseClientDLL->RestoreOld();
    IPanel->RestoreOld();
    ClientModeShared->RestoreOld();
    SvCheats->RestoreOld();
    IGameEventManager2->RestoreOld();
    IVModelRender->RestoreOld();

    interfaces::IGameEventManager2->RemoveListener(m_dummy_event_listener);
}