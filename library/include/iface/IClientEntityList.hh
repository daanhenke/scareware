#pragma once

#include "iface/common.hh"
#include "iface/IClientEntity.hh"

namespace sw::iface
{
    class IClientEntityList
    {
    public:
        IClientEntity* GetClientEntity(int entnum)
        {
            DEFINE_MEMBER(__thiscall * GetClientEntity_t, IClientEntity*, int);
            return CALL_MEMBER(GetClientEntity_t, this, 3, entnum);
        }

        IClientEntity* GetClientEntityFromHandle(CBaseHandle hEntity)
        {
            DEFINE_MEMBER(__thiscall * GetClientEntityFromHandle_t, IClientEntity*, CBaseHandle);
            return CALL_MEMBER(GetClientEntityFromHandle_t, this, 4, hEntity);
        }

        IClientEntity* GetClientEntityFromHandle(unsigned long hEntity)
        {
            DEFINE_MEMBER(__thiscall * GetClientEntityFromHandle_t, IClientEntity*, unsigned long);
            return CALL_MEMBER(GetClientEntityFromHandle_t, this, 4, hEntity);
        }

        int NumberOfEntities(bool bIncludeNonNetworkable = true)
        {
            DEFINE_MEMBER(__thiscall* NumberOfEntities_t, int, bool);
            return CALL_MEMBER(NumberOfEntities_t, this, 5, bIncludeNonNetworkable);
        }

        int GetHighestEntityIndex()
        {
            DEFINE_MEMBER_GETTER(__thiscall* GetHighestEntityIndex_t, int);
            return CALL_MEMBER_GETTER(GetHighestEntityIndex_t, this, 6);
        }
    };
}