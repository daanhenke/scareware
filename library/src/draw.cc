#include "draw.hh"
#include "interfaces.hh"

sw::iface::HFont sw::draw::FontDefault = 0;
sw::iface::HFont sw::draw::FontCSGOIcons = 0xa1; // Hardcoded, not initialized by InitializeFonts

void sw::draw::InitializeFonts()
{
	FontDefault = MakeFont("Tahoma", 12, 300, 0, 0, iface::EFontFlags::FONTFLAG_OUTLINE);
}

void sw::draw::Initialize()
{
	InitializeFonts();
}

sw::iface::HFont sw::draw::MakeFont(const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
{
	iface::HFont handle = interfaces::ISurface->CreateFont();

	interfaces::ISurface->SetFontGlyphSet(handle, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);

	return handle;
}

#undef DrawText
void sw::draw::DrawText(iface::HFont font, std::wstring text, int x, int y, iface::Color color)
{
	interfaces::ISurface->DrawSetTextFont(font);
	interfaces::ISurface->DrawSetTextColor(color.r, color.g, color.b, color.a);
	interfaces::ISurface->DrawSetTextPos(x, y);
	interfaces::ISurface->DrawPrintText(text.c_str(), text.length());
}


void sw::draw::DrawShadedText(iface::HFont font, std::wstring text, int x, int y, iface::Color color, int offset_x, int offset_y, iface::Color bg_color)
{
	DrawText(font, text, x + offset_x, y + offset_y, bg_color);
	DrawText(font, text, x, y, color);
}

void sw::draw::DrawFillRect(int x, int y, int width, int height, iface::Color color)
{
	interfaces::ISurface->DrawSetColor(color.r, color.g, color.b, color.a);
	interfaces::ISurface->DrawFilledRect(x, y, x + width, y + height);
}