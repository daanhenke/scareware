#pragma once

#include "iface/IAppSystem.hh"
#undef PlaySound
#undef CreateFont

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

    typedef unsigned long FontHandle_t;
    typedef FontHandle_t HFont;

    enum EFontFlags
	{
		FONTFLAG_NONE,
		FONTFLAG_ITALIC			= 0x001,
		FONTFLAG_UNDERLINE		= 0x002,
		FONTFLAG_STRIKEOUT		= 0x004,
		FONTFLAG_SYMBOL			= 0x008,
		FONTFLAG_ANTIALIAS		= 0x010,
		FONTFLAG_GAUSSIANBLUR	= 0x020,
		FONTFLAG_ROTARY			= 0x040,
		FONTFLAG_DROPSHADOW		= 0x080,
		FONTFLAG_ADDITIVE		= 0x100,
		FONTFLAG_OUTLINE		= 0x200,
		FONTFLAG_CUSTOM			= 0x400,		// custom generated font - never fall back to asian compatibility mode
		FONTFLAG_BITMAP			= 0x800,		// compiled bitmap font - no fallbacks
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

        void DrawSetTextFont(HFont font)
        {
            DEFINE_MEMBER(__thiscall* DrawSetTextFont_t, void, HFont);
            CALL_MEMBER(DrawSetTextFont_t, this, 23, font);
        }

        void DrawSetTextColor(int r, int g, int b, int a)
        {
            DEFINE_MEMBER(__thiscall* DrawSetTextColor_t, void, int, int, int, int);
            CALL_MEMBER(DrawSetTextColor_t, this, 25, r, g, b, a);
        }

        void DrawSetTextPos(int x, int y)
        {
            DEFINE_MEMBER(__thiscall* DrawSetTextPos_t, void, int, int);
            CALL_MEMBER(DrawSetTextPos_t, this, 26, x, y);
        }

        void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
        {
            DEFINE_MEMBER(__thiscall* DrawPrintText_t, void, const wchar_t*, int, FontDrawType_t);
            CALL_MEMBER(DrawPrintText_t, this, 28, text, textLen, drawType);
        }

        HFont CreateFont()
        {
            DEFINE_MEMBER_GETTER(__thiscall* CreateFont_t, HFont);
            return CALL_MEMBER_GETTER(CreateFont_t, this, 71);
        }

        void SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
        {
            DEFINE_MEMBER(__thiscall* SetFontGlyphSet_t, void, HFont, const char*, int, int, int, int, int, int, int);
            CALL_MEMBER(SetFontGlyphSet_t, this, 72, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
        }

        void PlaySound(const char* fileName)
        {
            DEFINE_MEMBER(__thiscall* PlaySound_t, void, const char*);
            CALL_MEMBER(PlaySound_t, this, 82, fileName);
        }

        void SurfaceGetCursorPos(int& x, int& y)
        {
            DEFINE_MEMBER(__thiscall* SurfaceGetCursorPos_t, void, int&, int&);
            CALL_MEMBER(SurfaceGetCursorPos_t, this, 100, x, y);
        }

        LAZY_MEMBER(GetTextSize, void, (HFont font, const wchar_t* text, int* wide, int* tall), 79, (this, font, text, wide, tall));
    };
}