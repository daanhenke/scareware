#pragma once

#include "iface/common.hh"
#include "iface/VMatrix.hh"

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

        int GetLocalPlayer()
        {
            DEFINE_MEMBER_GETTER(__thiscall* GetLocalPlayer_t, int);
            return CALL_MEMBER_GETTER(GetLocalPlayer_t, this, 12);
        }

        bool IsInGame()
        {
            DEFINE_MEMBER_GETTER(__thiscall* IsInGame_t, bool);
            return CALL_MEMBER_GETTER(IsInGame_t, this, 26);
        }

        VMatrix& WorldToScreenMatrix()
        {
            DEFINE_MEMBER_GETTER(__thiscall* WorldToScreenMatrix_t, VMatrix&);
            return CALL_MEMBER_GETTER(WorldToScreenMatrix_t, this, 36);
        }
    };
}