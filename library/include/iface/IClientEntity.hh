#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"
#include "iface/IClientUnknown.hh"
#include "iface/IClientRenderable.hh"
#include "iface/IClientNetworkable.hh"
#include "iface/IClientThinkable.hh"
#include "iface/WeaponId.hh"

namespace sw::iface
{
    enum class MoveType {
        NOCLIP = 8,
        LADDER = 9
    };

    class IClientEntity
    {
    public:
        bool IsAlive()
        {
            DEFINE_MEMBER_GETTER(__thiscall * IsAlive_t, bool);
            return CALL_MEMBER_GETTER(IsAlive_t, this, 3);
        }

        ClientClass* GetClientClass()
        {
            DEFINE_MEMBER_GETTER(__thiscall * IsAlive_t, ClientClass*);
            return CALL_MEMBER_GETTER(IsAlive_t, this + sizeof(uintptr_t) * 2, 2);
        }

        NETVAR_OFFSET(movetype, "CBaseEntity", "m_nRenderMode", 1, MoveType)
        NETVAR(fFlags, "CBasePlayer", "m_fFlags", int);
        NETVAR(hMyWeapons, "CBaseCombatCharacter", "m_hMyWeapons", int[64]);
        NETVAR(iTeamNum, "CBaseEntity", "m_iTeamNum", int);

        NETVAR(iItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
        NETVAR(iItemDefinitionIndex2, "CBaseAttributableItem", "m_iItemDefinitionIndex", WeaponId);
        NETVAR(iItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
        NETVAR(iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
        NETVAR(szCustomName, "CBaseAttributableItem", "m_szCustomName", char[32]);
        NETVAR(nFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
        NETVAR(nFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
        NETVAR(flFallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
        NETVAR(nFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);
    };
}