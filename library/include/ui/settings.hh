#pragma once

#include <string>

struct nk_colorf;

namespace sw::ui::settings
{
	extern bool is_open;

	enum class currenttab_t
	{
		TAB_MISC,
		TAB_VISUALS,
		TAB_SKINS,
		TAB_MOVEMENT
	};

	extern currenttab_t current_tab;

	void RenderColorPickerRow(nk_colorf* color, bool* enabled, std::string label_text);
	void Render();
}