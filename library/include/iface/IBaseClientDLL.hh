#pragma once

#include "iface/common.hh"
#include "iface/ClientClass.hh"

namespace sw::iface
{
    class IBaseClientDLL
    {
    public:
        ClientClass* GetAllClasses()
        {
            DEFINE_MEMBER_GETTER(__thiscall * GetAllClasses_t, ClientClass*);
            return CALL_MEMBER_GETTER(GetAllClasses_t, this, 8);
        }
    };
}
