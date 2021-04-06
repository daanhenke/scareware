#pragma once
#include "iface/Color.hh"

#include <string>
#include <vector>

namespace sw::ui
{
	class Rectangle
	{
	public:
		Rectangle(int x1, int y1, int x2, int y2);

		int x1;
		int y1;
		int x2;
		int y2;
	};

	class IUIElement
	{
	public:
		virtual void Render();
		virtual Rectangle GetSize();
	};

	class Window : public IUIElement
	{
		void Render();
		Rectangle GetSize();
	};

	void DrawUI();
}