#include "hacks/misc.hh"
#include "interfaces.hh"
#include <cstring>
#include "console.hh"
#include "memory.hh"

void sw::hacks::misc::Bunnyhop(iface::CUserCmd* cmd)
{
    auto local_player = sw::interfaces::GetLocalPlayer();

    if (!local_player) return;
    if (!local_player->IsAlive()) return;
    if (!(cmd->buttons & sw::iface::IN_JUMP)) return;
    if (!(local_player->fFlags() & 1)) cmd->buttons &= ~sw::iface::IN_JUMP;
}

void sw::hacks::misc::ThirdPerson()
{
    static bool isThirdPerson = false;
    if (sw::interfaces::IInputSystem->IsButtonDown(sw::iface::ButtonCode_t::KEY_HOME))
    {
        isThirdPerson = !isThirdPerson;
    }

    auto local_player = sw::interfaces::GetLocalPlayer();
    
    if (!local_player) return;
    if (!local_player->IsAlive()) return;

    static sw::iface::Vector viewAngles;
    sw::interfaces::IVEngineClient->GetViewAngles(viewAngles);

    if (isThirdPerson)
    {
        if (! sw::interfaces::CInput->m_fCameraInThirdPerson)
        {
            sw::interfaces::CInput->m_fCameraInThirdPerson = true;
            sw::interfaces::CInput->m_vecCameraOffset = sw::iface::Vector(viewAngles.x, viewAngles.y, 150.f);
        }
    }
    else
    {
        sw::interfaces::CInput->m_fCameraInThirdPerson = false;
        sw::interfaces::CInput->m_vecCameraOffset = sw::iface::Vector(viewAngles.x, viewAngles.y, 0);
    }
}

void sw::hacks::misc::GlowPlayers()
{
    auto local_player = interfaces::GetLocalPlayer();

    for (int i = 0; i < interfaces::CGlowObjectManager->size; i++)
    {
        iface::GlowObjectDefinition_t* glowDef = &interfaces::CGlowObjectManager->m_GlowObjectDefinitions[i];
        auto entity = glowDef->m_hEntity;

        if (glowDef->IsUnused() || !entity) continue;

        auto classInstance = entity->GetClientClass();
        if (classInstance == nullptr)
        {
            continue;
        }
        auto classId = classInstance->classId;

        iface::Vector color(0, 1.f, 0);

        if (classId == iface::ClassId::CSPlayer)
        {
            if (entity == interfaces::GetLocalPlayer()) continue;

            if (entity->iTeamNum() == local_player->iTeamNum())
            {
                color = iface::Vector(0, 0, 1.f);
            }
            else
            {
                color = iface::Vector(1.f, 0, 0);
            }
        }

        glowDef->m_bRenderWhenOccluded = true;
        glowDef->m_bRenderWhenUnoccluded = false;
        glowDef->m_flGlowAlpha = 0.8f;
        glowDef->m_flBloomAmount = 1.f;
        glowDef->m_vGlowColor = color;
    }
}

#include <vector>
std::vector<std::string> m_mats;
void sw::hacks::misc::DarkWorld()
{
    for (short handle = interfaces::IMaterialSystem->FirstMaterial(); handle != interfaces::IMaterialSystem->InvalidMaterial(); handle = interfaces::IMaterialSystem->NextMaterial(handle))
    {
        const auto material = interfaces::IMaterialSystem->GetMaterial(handle);

        if (!material || ! material->IsPrecached()) continue;

        const std::string_view textureGroup = material->GetTextureGroupName();
        if (textureGroup.starts_with("World") || textureGroup.starts_with("StaticProp"))
        {
            material->ColorModulate(.1f, .1f, .1f);
        }
        else
        {
            if (! std::count(m_mats.begin(), m_mats.end(), textureGroup))
            {
                console::WriteColorFormat(FOREGROUND_RED, "New texture group: %s\n", textureGroup);
                m_mats.emplace_back(textureGroup);
            }
        }
    }
}

void sw::hacks::misc::Skybox(iface::FrameStage stage)
{
    if (stage != iface::FrameStage::RENDER_END && stage != iface::FrameStage::RENDER_START) return;

    if (stage == iface::FrameStage::RENDER_START)
    {
        auto skybox = interfaces::ICvar->FindVar("sv_skyname");
        if (!skybox) return;
        skybox->SetValue("sky_csgo_night02");
    }
}

void sw::hacks::misc::NoFlash()
{
    auto localPlayer = interfaces::GetLocalPlayer();

    if (!localPlayer) return;

    if (localPlayer->flFlashMaxAlpha() > 0.f) localPlayer->flFlashMaxAlpha() = 0.f;
}

void sw::hacks::misc::NoSmoke(iface::FrameStage stage)
{
    if (stage != iface::FrameStage::RENDER_END) return;

    std::vector<const char*> m_smoke_materials = {
    "particle/vistasmokev1/vistasmokev1_emods",
    "particle/vistasmokev1/vistasmokev1_emods_impactdust",
    "particle/vistasmokev1/vistasmokev1_fire",
    "particle/vistasmokev1/vistasmokev1_smokegrenade"
    };

    for (auto entry : m_smoke_materials)
    {
        auto material = interfaces::IMaterialSystem->FindMaterial(entry);

        if (!material) continue;
        material->SetMaterialVarFlag(iface::MaterialVarFlag::WIREFRAME, true);
    }
}

void sw::hacks::misc::RemoveRecoil(iface::FrameStage stage)
{
    static iface::Vector aimPunch;
    static iface::Vector viewPunch;

    auto localPlayer = interfaces::GetLocalPlayer();

    if (stage == iface::FrameStage::RENDER_START)
    {
        aimPunch = localPlayer->aimPunchAngle();
        viewPunch = localPlayer->viewPunchAngle();

        localPlayer->aimPunchAngle() = iface::Vector{};
        localPlayer->viewPunchAngle() = iface::Vector{};
    }
    else if (stage == iface::FrameStage::RENDER_END)
    {
        localPlayer->aimPunchAngle() = aimPunch;
        localPlayer->viewPunchAngle() = viewPunch;
    }
}

void sw::hacks::misc::MemeRagdolls()
{
    auto clRagdollGravity = interfaces::ICvar->FindVar("cl_ragdoll_gravity");
    if (!clRagdollGravity) return;
    clRagdollGravity->SetValue(-600);
}

void sw::hacks::misc::Remove3dSky()
{
    auto r3dSky = interfaces::ICvar->FindVar("r_3dsky");
    if (!r3dSky) return;
    r3dSky->SetValue(false);
}

int GetUserId(sw::iface::IClientEntity* entity)
{
    sw::iface::PlayerInfo info;
    sw::interfaces::IVEngineClient->GetPlayerInfo(entity->Index(), info);
    return info.userId;
}

#include "memory.hh"
void sw::hacks::misc::BulletTracers(iface::IGameEvent* event)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    console::WriteFormat("got player: %x %x\n", GetUserId(localPlayer), event->GetInt("userid"));

    if (event->GetInt("userid") != GetUserId(localPlayer)) return;

    console::WriteFormat("local shot\n");

    auto activeWeapon = localPlayer->GetActiveWeapon();
    if (!activeWeapon) return;

    console::WriteFormat("got weapon\n");

    iface::BeamInfo_t beamInfo;
    if (!localPlayer->ShouldDraw())
    {
        auto viewModel = interfaces::IClientEntityList->GetClientEntityFromHandle(localPlayer->hViewModel());
        if (!viewModel) return;

        if (!viewModel->GetAttachment(activeWeapon->GetMuzzleAttachment1stPerson(viewModel), beamInfo.m_vecStart)) return;
    }
    else
    {
        auto worldModel = interfaces::IClientEntityList->GetClientEntityFromHandle(activeWeapon->hWeaponWorldModel());
        if (!worldModel) return;

        if (!worldModel->GetAttachment(activeWeapon->GetMuzzleAttachment3rdPerson(), beamInfo.m_vecStart)) return;
    }

    console::WriteFormat("setting up beam\n");

    beamInfo.m_nType = 0;
    beamInfo.m_flLife = 0.f;
    beamInfo.m_flAmplitude = 0.f;
    beamInfo.m_nSegments = -1;
    beamInfo.m_bRenderable = true;
    beamInfo.m_flSpeed = .2f;
    beamInfo.m_flWidth = beamInfo.m_flEndWidth = 2.f;
    beamInfo.m_nFlags = 0x40;
    beamInfo.m_flFadeLength = 20.f;
    beamInfo.m_nStartFrame = 0;
    beamInfo.m_flFrameRate = .0f;

    beamInfo.m_nHaloIndex = -1;
    beamInfo.m_pszHaloName = nullptr;

    beamInfo.m_nModelIndex = -1;
    beamInfo.m_pszModelName = "sprites/physbeam.vmt";

    beamInfo.m_flRed = 255.f;
    beamInfo.m_flGreen = 0.f;
    beamInfo.m_flBlue = 255.f;
    beamInfo.m_flBrightness = 255.f;

    beamInfo.m_vecEnd.x = event->GetFloat("x");
    beamInfo.m_vecEnd.y = event->GetFloat("y");
    beamInfo.m_vecEnd.z = event->GetFloat("z");

    auto beam = sw::memory::IViewRenderBeams->CreateBeamPoints(beamInfo);
    if (beam)
    {
        beam->m_nFlags &= ~0x4000;
        beam->m_flDie = interfaces::CGlobalVars->currenttime + 2.f;
    }
}

void sw::hacks::misc::NoscopeCrosshair()
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;
    if (localPlayer->bIsScoped()) return;
    auto weapon = localPlayer->GetActiveWeapon();
    if (!weapon) return;
    if (weapon->GetWeaponType() != iface::WeaponType::SniperRifle) return;

    int screen_width, screen_height;
    int crosshair_length = 16;
    int crosshair_width = 3;
    interfaces::IVEngineClient->GetScreenSize(screen_width, screen_height);

    interfaces::ISurface->DrawSetColor(255, 0, 0, 255);
    interfaces::ISurface->DrawFilledRect(screen_width / 2 - crosshair_length / 2, screen_height / 2 - crosshair_width / 2, screen_width / 2 + crosshair_length / 2, screen_height / 2 + crosshair_width / 2);
    interfaces::ISurface->DrawFilledRect(screen_width / 2 - crosshair_width / 2, screen_height / 2 - crosshair_length / 2, screen_width / 2 + crosshair_width / 2, screen_height / 2 + crosshair_length / 2);
}

sw::iface::Vector ClampAngle(sw::iface::Vector angle)
{
    if (angle.x > 89.f && angle.x <= 180.f) angle.x = 89.f;

    while (angle.x > 180.f) angle.x -= 360.f;
    while (angle.x < -89.f) angle.x = -89.f;
    while (angle.y > 180.f) angle.y -= 360.f;
    while (angle.y < -180.f) angle.y += 360.f;

    return angle;
}

sw::iface::Vector oldAngle;
void sw::hacks::misc::RecoilControl(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    if (cmd->buttons & iface::IN_ATTACK)
    {
        iface::Vector newAngle = localPlayer->aimPunchAngle();
        const float angleFix = 2.5;
        cmd->viewangles.x += (oldAngle.x - (newAngle.x * angleFix));
        cmd->viewangles.y += (oldAngle.y - (newAngle.y * angleFix));
        cmd->viewangles.z = 0;

        oldAngle = ClampAngle(newAngle);
    }
    else
    {
        oldAngle.x = oldAngle.y = oldAngle.z = 0;
    }
}