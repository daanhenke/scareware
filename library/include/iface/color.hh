#pragma once

namespace sw::iface
{
	struct Color
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) :r(r), g(g), b(b), a(a) {}
	};

	Color MixColors(Color a, Color b, float mix)
	{
		Color result;
		result.r = a.r + mix * (b.r - a.r);
		result.g = a.g + mix * (b.g - a.g);
		result.b = a.b + mix * (b.b - a.b);
		result.a = a.a + mix * (b.a - a.a);
		return result;
	}
}