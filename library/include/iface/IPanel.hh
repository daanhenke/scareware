#pragma once

#include "iface/common.hh"

namespace sw::iface
{
    typedef unsigned int VPANEL;

    class IPanel
    {
    public:
        void GetPos(VPANEL vguiPanel, int& x, int& y)
        {
            DEFINE_MEMBER(__thiscall* GetPos_t, void, VPANEL, int&, int&);
            CALL_MEMBER(GetPos_t, this, 3, vguiPanel, x, y);
        }
        
        void GetAbsPos(VPANEL vguiPanel, int& x, int& y)
        {
            DEFINE_MEMBER(__thiscall* GetClipRect_t, void, VPANEL, int&, int&);
            CALL_MEMBER(GetClipRect_t, this, 10, vguiPanel, x, y);
        }
        
        void GetClipRect(VPANEL vguiPanel, int& x1, int& y1, int& x2, int& y2)
        {
            DEFINE_MEMBER(__thiscall* GetClipRect_t, void, VPANEL, int&, int&, int&, int&);
            CALL_MEMBER(GetClipRect_t, this, 11, vguiPanel, x1, y1, x2, y2);
        }

        const char* GetName(VPANEL vguiPanel)
        {
            DEFINE_MEMBER(__thiscall* GetName_t, const char*, VPANEL);
            return CALL_MEMBER(GetName_t, this, 36, vguiPanel);
        }
    };
}