#include "hacks/misc.hh"
#include "interfaces.hh"
#include <cstring>
#include "console.hh"
#include "memory.hh"
#include "util.hh"
#include "hacks/predict.hh"
#include "config.hh"

void sw::hacks::misc::Bunnyhop(iface::CUserCmd* cmd)
{
    if (! config::CurrentConfig.bhop.enabled) return;

    auto localPlayer = sw::interfaces::GetLocalPlayer();

    if (!localPlayer) return;
    if (!localPlayer->IsAlive()) return;
    if (!(cmd->buttons & sw::iface::IN_JUMP)) return;
    if (localPlayer->movetype() == iface::MoveType::LADDER) return;
    if (!(localPlayer->fFlags() & 1)) cmd->buttons &= ~iface::IN_JUMP;
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

    if (localPlayer->flFlashMaxAlpha() > 0.f) localPlayer->flFlashMaxAlpha() = 50.f;
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

void sw::hacks::misc::ChangeFOV()
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    if (localPlayer->bIsScoped())
    {
        localPlayer->iFOV() = 10;
        localPlayer->iFOVStart() = 10;
    }
    else
    {
        localPlayer->iFOV() = 115;
        localPlayer->iFOVStart() = 115;
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

    if (event->GetInt("userid") != GetUserId(localPlayer)) return;

    auto activeWeapon = localPlayer->GetActiveWeapon();
    if (!activeWeapon) return;

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
        beam->m_flDie = interfaces::CGlobalVars->currenttime + 5.f;
    }
}

void sw::hacks::misc::NoscopeCrosshair()
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    auto weaponDebugSpreadShow = interfaces::ICvar->FindVar("weapon_debug_spread_show");
    if (!weaponDebugSpreadShow) return;

    auto weapon = localPlayer->GetActiveWeapon();
    if (!weapon) return;

    if (weapon->GetWeaponType() != iface::WeaponType::SniperRifle || localPlayer->bIsScoped())
    {
        weaponDebugSpreadShow->SetValue(0);
    }
    else
    {
        weaponDebugSpreadShow->SetValue(3);
    }

    /*int screen_width, screen_height;
    int crosshair_length = 16;
    int crosshair_width = 3;
    interfaces::IVEngineClient->GetScreenSize(screen_width, screen_height);

    interfaces::ISurface->DrawSetColor(255, 0, 0, 255);
    interfaces::ISurface->DrawFilledRect(screen_width / 2 - crosshair_length / 2, screen_height / 2 - crosshair_width / 2, screen_width / 2 + crosshair_length / 2, screen_height / 2 + crosshair_width / 2);
    interfaces::ISurface->DrawFilledRect(screen_width / 2 - crosshair_width / 2, screen_height / 2 - crosshair_length / 2, screen_width / 2 + crosshair_width / 2, screen_height / 2 + crosshair_length / 2);*/
}

sw::iface::Vector oldAngle;
void sw::hacks::misc::RecoilControl(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    if (cmd->buttons & iface::IN_ATTACK)
    {
        iface::Vector newAngle = localPlayer->aimPunchAngle();
        const float angleFix = 2;

        auto viewAnglesNew = iface::Vector(cmd->viewangles.x - newAngle.x * angleFix, cmd->viewangles.y - newAngle.y * angleFix);
        auto viewClamped = util::ClampAngle(viewAnglesNew);
        cmd->viewangles.x = viewClamped.x;
        cmd->viewangles.y = viewClamped.y;
        cmd->viewangles.z = viewClamped.z;

        oldAngle = util::ClampAngle(newAngle);
    }
    else
    {
        oldAngle.x = oldAngle.y = oldAngle.z = 0;
    }
}

void sw::hacks::misc::DisablePostProcessing()
{
    auto matPostprocessEnable = interfaces::ICvar->FindVar("mat_postprocess_enable");
    if (!matPostprocessEnable) return;

    matPostprocessEnable->SetValue(0);
}


void sw::hacks::misc::NadePreview()
{
    auto grenadepreview = interfaces::ICvar->FindVar("cl_grenadepreview");
    if (!grenadepreview) return;

    grenadepreview->SetValue(1);
}

void sw::hacks::misc::HitSound(iface::IGameEvent* event)
{
    auto localPlayer = sw::interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    auto attackerId = event->GetInt("attacker");
    
    iface::PlayerInfo info;
    interfaces::IVEngineClient->GetPlayerInfo(localPlayer->Index(), info);

    if (info.userId != attackerId) return;

    sw::interfaces::ISurface->PlaySound("buttons/arena_switch_press_02.wav");

    auto hudchat = sw::interfaces::ClientModeShared->GetHudChat();
}

void sw::hacks::misc::JumpBug(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;
    if (!interfaces::IInputSystem->IsButtonDown(iface::MOUSE_5)) return;

    if (!(predict::OldFlags & 1) && (localPlayer->fFlags() & 1))
    {
        cmd->buttons != iface::IN_DUCK;
        cmd->buttons &= iface::IN_JUMP;
    }
}
void sw::hacks::misc::Edgebug(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;
    if (!interfaces::IInputSystem->IsButtonDown(iface::MOUSE_MIDDLE)) return;

    if (!(predict::OldFlags & 1) && (localPlayer->fFlags() & 1))

        cmd->buttons != iface::IN_DUCK;
}

void sw::hacks::misc::LedgeJump(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;
    if (!interfaces::IInputSystem->IsButtonDown(iface::KEY_X)) return;

    if (localPlayer->movetype() == iface::MoveType::LADDER) return;

    if ((predict::OldFlags & 1) && !(localPlayer->fFlags() & 1))
    {
        cmd->buttons |= iface::IN_JUMP;
    }
}

void sw::hacks::misc::AutoStrafe(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!interfaces::IInputSystem->IsButtonDown(iface::MOUSE_5)) return;
    if (!(localPlayer->fFlags() & 1))
    {
        if (cmd->mousedx < 0) cmd->sidemove = -450.f;
        else if(cmd->mousedx > 0
            ) cmd->sidemove = 450.f;
    }

}

void sw::hacks::misc::TriggerBot(iface::CUserCmd* cmd)
{
    auto localPlayer = interfaces::GetLocalPlayer();
    if (!localPlayer) return;

    if (!interfaces::IInputSystem->IsButtonDown(iface::KEY_H)) return;


    console::WriteFormat("weapon getting!!!\n");

    auto weapon = localPlayer->GetActiveWeapon();
    if (!weapon) return;

    auto weaponData = weapon->GetWeaponData();
    if (!weaponData) return;

    console::WriteFormat("tracing!!!\n");


    auto startPos = localPlayer->GetEyePosition();
    auto endPos = startPos + iface::Vector::FromAngle(cmd->viewangles + (localPlayer->aimPunchAngle())) * weaponData->range;

    iface::Ray ray(startPos, endPos);

    iface::Trace trace;
    iface::TraceFilter filter(localPlayer);
    interfaces::IEngineTrace->TraceRay(ray, 0x46004009, filter, trace);/*

    if (!trace.entity) return;
    console::WriteFormat("triggerbot working!!!\n");
    cmd->buttons |= iface::IN_ATTACK;*/
}