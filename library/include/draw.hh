#pragma once

#include <string>
#include "iface/ISurface.hh"
#include "iface/Color.hh"

namespace sw::draw
{
	extern iface::HFont FontDefault;
	extern iface::HFont FontCSGOIcons;

	void InitializeFonts();
	void Initialize();

	iface::HFont MakeFont(const char* windowsFontName, int tall, int weight = 300, int blur = 0, int scanlines = 0, int flags = iface::EFontFlags::FONTFLAG_ANTIALIAS, int nRangeMin = 0, int nRangeMax = 0);

	#undef DrawText
	void DrawText(iface::HFont font, std::wstring text, int x, int y, iface::Color color = iface::Color(255, 255, 255, 255));
	void DrawShadedText(iface::HFont font, std::wstring text, int x, int y, iface::Color color = iface::Color(255, 255, 255, 255), int offset_x = 2, int offset_y = 2, iface::Color bg_color = iface::Color(0, 0, 0, 200));

	void DrawFillRect(int x, int y, int width, int height, iface::Color color);
}