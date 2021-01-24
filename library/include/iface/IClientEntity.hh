#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"
#include "iface/IClientUnknown.hh"
#include "iface/IClientRenderable.hh"
#include "iface/IClientNetworkable.hh"
#include "iface/IClientThinkable.hh"
#include "iface/WeaponId.hh"
#include "iface/ClientClass.hh"

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
            return CALL_MEMBER_GETTER(IsAlive_t, this, 155);
        }

        ClientClass* GetClientClass()
        {
            DEFINE_MEMBER_GETTER(__thiscall* IsAlive_t, ClientClass*);
            return CALL_MEMBER_GETTER(IsAlive_t, this + sizeof(uintptr_t) * 2, 2);
        }

        LAZY_MEMBER(ShouldDraw, bool, (), 3, (this + sizeof(uintptr_t)));
        LAZY_MEMBER(Index, int, (), 10, (this + sizeof(uintptr_t) * 2));

        LAZY_MEMBER(GetAttachment, bool, (int index, Vector& origin), 83, (this, index, std::ref(origin)));
        LAZY_MEMBER(GetActiveWeapon, IClientEntity*, (), 267, (this));
        LAZY_MEMBER(GetMuzzleAttachment1stPerson, int, (IClientEntity* viewModel), 467, (this, viewModel));
        LAZY_MEMBER(GetMuzzleAttachment3rdPerson, int, (), 468, (this));

        NETVAR_OFFSET(movetype, "CBaseEntity", "m_nRenderMode", 1, MoveType)
        NETVAR(fFlags, "CBasePlayer", "m_fFlags", int);
        NETVAR(hMyWeapons, "CBaseCombatCharacter", "m_hMyWeapons", int[64]);
        NETVAR(iTeamNum, "CBaseEntity", "m_iTeamNum", int);
        NETVAR(aimPunchAngle, "CBasePlayer", "m_aimPunchAngle", Vector);
        NETVAR(viewPunchAngle, "CBasePlayer", "m_viewPunchAngle", Vector);
        NETVAR(hViewModel, "CBasePlayer", "m_hViewModel[0]", int);

        NETVAR(flFlashMaxAlpha, "CCSPlayer", "m_flFlashMaxAlpha", float);

        NETVAR(iItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
        NETVAR(iItemDefinitionIndex2, "CBaseAttributableItem", "m_iItemDefinitionIndex", WeaponId);
        NETVAR(iItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
        NETVAR(iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
        NETVAR(szCustomName, "CBaseAttributableItem", "m_szCustomName", char[32]);
        NETVAR(nFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
        NETVAR(nFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
        NETVAR(flFallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
        NETVAR(nFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);

        NETVAR(hWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel", int);
    };
}