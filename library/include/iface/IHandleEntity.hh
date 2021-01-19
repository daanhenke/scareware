#pragma once

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF;

namespace sw::iface
{
    class CBaseHandle
    {
    public:
        CBaseHandle(int index) : m_idx(index)
        {}

        inline int GetIndex()
        {
            return m_idx;
        }
    protected:
        unsigned long m_idx;
    };

    class IHandleEntity
    {
    public:
        virtual ~IHandleEntity() {}
        virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
        virtual const CBaseHandle& GetRefEHandle() const = 0;
    };
}