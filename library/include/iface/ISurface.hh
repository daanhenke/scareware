#pragma once

#include "iface/IAppSystem.hh"

namespace sw::iface
{
    enum FontDrawType_t
	{
		// Use the "additive" value from the scheme file
		FONT_DRAW_DEFAULT = 0,

		// Overrides
		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,

		FONT_DRAW_TYPE_COUNT = 2,
	};

    class ISurface
    {
    public:
        void DrawSetColor(int r, int g, int b, int a)
        {
            DEFINE_MEMBER(__thiscall* DrawSetColor_t, void, int, int, int, int);
            CALL_MEMBER(DrawSetColor_t, this, 15, r, g, b, a);
        }

        void DrawFilledRect(int x1, int y1, int x2, int y2)
        {
            DEFINE_MEMBER(__thiscall* DrawFilledRect_t, void, int, int, int, int);
            CALL_MEMBER(DrawFilledRect_t, this, 16, x1, y1, x2, y2);
        }

        void DrawSetTextPos(int x, int y)
        {
            DEFINE_MEMBER(__thiscall* DrawSetTextPos_t, void, int, int);
            CALL_MEMBER(DrawSetTextPos_t, this, 23, x, y);
        }

        void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
        {
            DEFINE_MEMBER(__thiscall* DrawPrintText_t, void, const wchar_t*, int, FontDrawType_t);
            CALL_MEMBER(DrawPrintText_t, this, 25, text, textLen, drawType);
        }
    };
}