#pragma once

#include "iface/common.hh"
#include "iface/IHandleEntity.hh"
#include "iface/IClientUnknown.hh"
#include "iface/IClientRenderable.hh"
#include "iface/IClientNetworkable.hh"
#include "iface/IClientThinkable.hh"

namespace sw::iface
{
    class ICollideable;
    class C_BaseEntity;
    class IClientEntity;

    class IClientUnknown : public IHandleEntity
    {
    public:
        virtual ICollideable* GetCollideable() = 0;
        virtual IClientNetworkable* GetClientNetworkable() = 0;
        virtual IClientRenderable* GetClientRenderable() = 0;
        virtual IClientEntity* GetClientEntity() = 0;
        virtual C_BaseEntity* GetBaseEntity() = 0;
        virtual IClientThinkable* GetClientThinkable() = 0;
    };
}