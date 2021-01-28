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
}