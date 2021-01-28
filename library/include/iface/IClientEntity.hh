#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"
#include "iface/IClientUnknown.hh"
#include "iface/IClientRenderable.hh"
#include "iface/IClientNetworkable.hh"
#include "iface/IClientThinkable.hh"
#include "iface/WeaponId.hh"
#include "iface/ClientClass.hh"
#include "iface/ICollideable.hh"

namespace sw::iface
{
    enum class MoveType {
        NOCLIP = 8,
        LADDER = 9
    };

    enum class ObsMode {
        OBS_MODE_NONE = 0,	// not in spectator mode
        OBS_MODE_DEATHCAM,	// special mode for death cam animation
        OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
        OBS_MODE_FIXED,		// view from a fixed camera position
        OBS_MODE_IN_EYE,	// follow a player in first person view
        OBS_MODE_CHASE,		// follow a player in third person view
        OBS_MODE_ROAMING,	// free roaming

        NUM_OBSERVER_MODES,
    };

    enum class WeaponType {
        Knife = 0,
        Pistol,
        SubMachinegun,
        Rifle,
        Shotgun,
        SniperRifle,
        Machinegun,
        C4,
        Placeholder,
        Grenade,
        Unknown,
        StackableItem,
        Fists,
        BreachCharge,
        BumpMine,
        Tablet,
        Melee
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

        LAZY_MEMBER(GetCollideable, ICollideable*, (), 3, (this));

        LAZY_MEMBER(ShouldDraw, bool, (), 3, (this + sizeof(uintptr_t)));
        LAZY_MEMBER(IsDormant, bool, (), 9, (this + sizeof(uintptr_t) * 2));
        LAZY_MEMBER(Index, int, (), 10, (this + sizeof(uintptr_t) * 2));
        LAZY_MEMBER(PostDataUpdate, void, (int updateType), 7, (this + sizeof(uintptr_t) * 2, updateType));

        LAZY_MEMBER(GetAttachment, bool, (int index, Vector& origin), 83, (this, index, std::ref(origin)));

        LAZY_MEMBER(IsPlayer, bool, (), 157, (this));
        LAZY_MEMBER(IsWeapon, bool, (), 165, (this));

        LAZY_MEMBER(GetObserverMode, ObsMode, (), 293, (this));
        LAZY_MEMBER(GetObserverTarget, IClientEntity*, (), 294, (this));

        LAZY_MEMBER(GetActiveWeapon, IClientEntity*, (), 267, (this));
        LAZY_MEMBER(GetWeaponType, WeaponType, (), 454, (this));
        LAZY_MEMBER(GetMuzzleAttachment1stPerson, int, (IClientEntity* viewModel), 467, (this, viewModel));
        LAZY_MEMBER(GetMuzzleAttachment3rdPerson, int, (), 468, (this));

        NETVAR_OFFSET(movetype, "CBaseEntity", "m_nRenderMode", 1, MoveType);
        NETVAR(vecOrigin, "CBaseEntity", "m_vecOrigin", Vector);
        NETVAR(bSpotted, "CBaseEntity", "m_bSpotted", bool);
        NETVAR(fFlags, "CBasePlayer", "m_fFlags", int);
        NETVAR(vecVelocity, "CBasePlayer", "m_vecVelocity[0]", Vector);
        NETVAR(hMyWeapons, "CBaseCombatCharacter", "m_hMyWeapons", int[64]);
        NETVAR(iTeamNum, "CBaseEntity", "m_iTeamNum", int);
        NETVAR(aimPunchAngle, "CBasePlayer", "m_aimPunchAngle", Vector);
        NETVAR(viewPunchAngle, "CBasePlayer", "m_viewPunchAngle", Vector);
        NETVAR(hViewModel, "CBasePlayer", "m_hViewModel[0]", int);
        NETVAR(nTickBase, "CBasePlayer", "m_nTickBase", int);

        NETVAR(flFlashMaxAlpha, "CCSPlayer", "m_flFlashMaxAlpha", float);
        NETVAR(bIsScoped, "CCSPlayer", "m_bIsScoped", bool);
        NETVAR(angEyeAngles, "CCSPlayer", "m_angEyeAngles", Vector);

        NETVAR(iItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
        NETVAR(iItemDefinitionIndex2, "CBaseAttributableItem", "m_iItemDefinitionIndex", WeaponId);
        NETVAR(iItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
        NETVAR(iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
        NETVAR(szCustomName, "CBaseAttributableItem", "m_szCustomName", char[32]);
        NETVAR(nFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
        NETVAR(nFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
        NETVAR(flFallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
        NETVAR(nFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);
        NETVAR(iAccountID, "CBaseAttributableItem", "m_iAccountID", int);

        NETVAR(hWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel", int);

        PNETVAR(flFrozen, "CBaseAnimating", "m_flFrozen", float);
    };
}