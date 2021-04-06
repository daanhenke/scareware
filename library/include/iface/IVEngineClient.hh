#pragma once

#include "iface/common.hh"
#include "iface/VMatrix.hh"
#include "iface/PlayerInfo.hh"

#include <functional>

namespace sw::iface
{
    class IVEngineClient
    {
    public:
        void GetScreenSize(int& width, int& height)
        {
            DEFINE_MEMBER(__thiscall* GetScreenSize_t, void, int&, int&);
            CALL_MEMBER(GetScreenSize_t, this, 5, width, height);
        }

        LAZY_MEMBER(GetPlayerInfo, bool, (int entityIndex, PlayerInfo& info), 8, (this, entityIndex, std::ref(info)));

        int GetLocalPlayer()
        {
            DEFINE_MEMBER_GETTER(__thiscall* GetLocalPlayer_t, int);
            return CALL_MEMBER_GETTER(GetLocalPlayer_t, this, 12);
        }

        void GetViewAngles(Vector& angles)
        {
            DEFINE_MEMBER(__thiscall * GetViewAngles_t, void, Vector&);
            return CALL_MEMBER(GetViewAngles_t, this, 18, std::ref(angles));
        }

        LAZY_MEMBER(GetMaxClients, int, (), 20, (this));

        bool IsInGame()
        {
            DEFINE_MEMBER_GETTER(__thiscall * IsInGame_t, bool);
            return CALL_MEMBER_GETTER(IsInGame_t, this, 26);
        }

        LAZY_MEMBER(CullBox, bool, (const Vector& mins, const Vector& maxs), 33, (this, mins, maxs));

        VMatrix& WorldToScreenMatrix()
        {
            DEFINE_MEMBER_GETTER(__thiscall* WorldToScreenMatrix_t, VMatrix&);
            return CALL_MEMBER_GETTER(WorldToScreenMatrix_t, this, 37);
        }
    };
}