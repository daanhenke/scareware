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