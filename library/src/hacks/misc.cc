#include "hacks/misc.hh"
#include "interfaces.hh"
#include <cstring>
#include "console.hh"

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