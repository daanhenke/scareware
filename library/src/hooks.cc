#include "hooks.hh"
#include "interfaces.hh"
#include "console.hh"
#include "logic.hh"
#include "util.hh"
#include "hacks/skin.hh"
#include "hacks/misc.hh"
#include "hacks/chams.hh"
#include "hacks/grief.hh"
#include "hacks/visuals.hh"
#include "hacks/predict.hh"
#include "hacks/chat.hh"
#include "memory.hh"
#include "ui/render.hh"

#include <cstring>

sw::vtable::VTableHook* sw::hooks::IBaseClientDLL = nullptr;
sw::vtable::VTableHook* sw::hooks::IPanel = nullptr;
sw::vtable::VTableHook* sw::hooks::ClientModeShared = nullptr;
sw::vtable::VTableHook* sw::hooks::IVModelRender = nullptr;
sw::vtable::VTableHook* sw::hooks::SvCheats = nullptr;
sw::vtable::VTableHook* sw::hooks::IGameEventManager2 = nullptr;
sw::vtable::VTableHook* sw::hooks::IDirect3DDevice9 = nullptr;

sw::hooks::DrawModelExecuteFn sw::hooks::oDrawModelExecute = nullptr;

sw::hooks::PaintTraverseFn oPaintTraverse;
void __fastcall pt_hook(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    // Call the original function
    oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

    // We want to cache the panel ID of the panel we want to render to
    static unsigned int uiPanelId = 0;
    static bool shouldDrawUi = false;
    static sw::iface::HFont font = 0;
    static int fontoff = 0;
    static bool fontoffChanged = false;

    // If the panel ID isn't found already try to find it
    if (uiPanelId == 0)
    {
        if (strstr(sw::interfaces::IPanel->GetName(vguiPanel), "MatSystemTop"))
        {
            uiPanelId = vguiPanel;
        }
    }

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

    if (uiPanelId != 0 && vguiPanel == uiPanelId)
    {
        sw::hacks::misc::NoscopeCrosshair();
        sw::hacks::visuals::Render();
    }
}

sw::hooks::CreateMoveFn oCreateMove;
bool __stdcall cm_hook(float frametime, sw::iface::CUserCmd* pCmd)
{
    bool res = oCreateMove(sw::interfaces::ClientModeShared, frametime, pCmd);

    if (!pCmd->command_number) return res;

    sw::hacks::misc::Bunnyhop(pCmd);
    sw::hacks::misc::RecoilControl(pCmd);
    //sw::hacks::grief::Blockbot(pCmd);

    //sw::hacks::predict::RunPrediction(pCmd);
    //sw::hacks::misc::JumpBug(pCmd);
    //sw::hacks::misc::Edgebug(pCmd);
    //sw::hacks::misc::LedgeJump(pCmd);
    //sw::hacks::misc::AutoStrafe(pCmd);
    //sw::hacks::misc::TriggerBot(pCmd);

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
            sw::hacks::misc::DisablePostProcessing();
            sw::hacks::misc::NadePreview();
            sw::hacks::misc::ChangeFOV();
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

    else if (!std::strcmp(eventName, "player_hurt"))
    {
        sw::hacks::misc::HitSound(event);
    }

    else if (!std::strcmp(eventName, "start_vote") || !std::strcmp(eventName, "vote_started"))
    {
        sw::hacks::chat::LogVoteStart(event);
    }

    else if (!std::strcmp(eventName, "vote_cast"))
    {
        sw::hacks::chat::LogVoteCast(event);
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

sw::hooks::EndSceneFn oEndScene;
long __stdcall es_hook(IDirect3DDevice9* device)
{
    static uintptr_t gameoverlay_return = 0;
    static bool nuklear_initialized = false;
    void* current_return = _ReturnAddress();
    static RECT window_rect;

    // We only want to run if we got called from gameoverlay, get it's return address and compare it with ours
    if (gameoverlay_return == 0)
    {
        MEMORY_BASIC_INFORMATION32 info;
        VirtualQuery(current_return, (MEMORY_BASIC_INFORMATION*)&info, sizeof(info));

        char mod_name[MAX_PATH];
        GetModuleFileNameA((HMODULE)info.AllocationBase, mod_name, sizeof(mod_name));

        if (strstr(mod_name, "gameoverlay"))
        {
            gameoverlay_return = (uintptr_t)current_return;
            sw::console::WriteFormat("Found gameoverlay ret addr: %x\n", gameoverlay_return);
        }
    }

    if (gameoverlay_return != (uintptr_t)current_return)
    {
        return oEndScene(device);
    }

    // We are now certain we're rendering to the screen, render ui & shit
    
    // Set up nuklear & wndproc hook if they haven't been set yet
    if (!nuklear_initialized)
    {
        sw::ui::render::Initialize(device);
        nuklear_initialized = true;
    }
    else
    {
        sw::ui::render::Render();
    }

    return oEndScene(device);
}

sw::hooks::ResetFn oReset;
long __stdcall rs_hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
    auto result = oReset(device, params);

    sw::ui::render::Reset(device, result);

    return result;
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
    IDirect3DDevice9 = new vtable::VTableHook((DWORD*)memory::D3DDevice);

    oPaintTraverse = (PaintTraverseFn) IPanel->HookMethod((DWORD) &pt_hook, 41);

    oCreateMove = (CreateMoveFn) ClientModeShared->HookMethod((DWORD) &cm_hook, 24);

    oFrameStageNotify = (FrameStageNotifyFn)IBaseClientDLL->HookMethod((DWORD)&fsn_hook, 37);

    oSvCheatsGet = (SvCheatsGetFn) SvCheats->HookMethod((DWORD)&svcheats_get_hook, 13);

    oFireEventsClientSide = (FireEventsClientSideFn)IGameEventManager2->HookMethod((DWORD)&fecs_hook, 9);

    oDrawModelExecute = (DrawModelExecuteFn)IVModelRender->HookMethod((DWORD)&dme_hook, 21);

    oEndScene = (EndSceneFn)IDirect3DDevice9->HookMethod((DWORD)&es_hook, 42);
    oReset = (ResetFn)IDirect3DDevice9->HookMethod((DWORD)&rs_hook, 16);

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
    IDirect3DDevice9->RestoreOld();

    interfaces::IGameEventManager2->RemoveListener(m_dummy_event_listener);

    if (ui::render::oWndProc != 0)
    {
        console::WriteFormat("wndproc: %x\n", ui::render::oWndProc);
        D3DDEVICE_CREATION_PARAMETERS params;
        memory::D3DDevice->GetCreationParameters(&params);
        SetWindowLongPtrA(params.hFocusWindow, GWL_WNDPROC, (LONG) ui::render::oWndProc);

        ui::render::Reset(memory::D3DDevice, 0);
    }
}