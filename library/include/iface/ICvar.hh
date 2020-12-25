#pragma once

#include "iface/IAppSystem.hh"

namespace sw::iface
{
    class ICvar
    {
    public:
        template <typename... Data>
        void ConsoleDPrintf(const char* szMsgFormat, Data... params)
        {
            // typedef void (* oConsoleDPrintf)(void*, const char*, ...);
            // return GetVirtualFunction<oConsoleDPrintf>(this, 27)(this, szMsgFormat, params...);
            DEFINE_MEMBER(*ConsoleDPrintf_t, void, const char*, ...);
            CALL_MEMBER(ConsoleDPrintf_t, this, 27, szMsgFormat, params...);
        }
    };
}