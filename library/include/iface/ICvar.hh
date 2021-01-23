#pragma once

#include "iface/IAppSystem.hh"

namespace sw::iface
{
    struct ConVar;

    class ICvar
    {
    public:
        template <typename... Data>
        void ConsoleDPrintf(const char* szMsgFormat, Data... params)
        {
            DEFINE_MEMBER(*ConsoleDPrintf_t, void, const char*, ...);
            CALL_MEMBER(ConsoleDPrintf_t, this, 27, szMsgFormat, params...);
        }

        ConVar* FindVar(const char* var_name)
        {
            DEFINE_MEMBER(__thiscall* FindVar_t, ConVar*, const char*);
            return CALL_MEMBER(FindVar_t, this, 15, var_name);
        }
    };
}