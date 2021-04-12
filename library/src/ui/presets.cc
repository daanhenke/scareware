#include "ui/presets.hh"
#include "ui/settings.hh"
#include "config.hh"
#include "logic.hh"

void sw::ui::presets::Render()
{
	using namespace render;

    if (nk_begin(nuklear_context, "Booger Cheats", nk_rect(50, 50, 220, 220), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(nuklear_context, 18, 1);
        nk_label(nuklear_context, "Booger Cheats v0.0", NK_TEXT_ALIGN_CENTERED);

        nk_layout_row_dynamic(nuklear_context, 18, 1);
        
        if (nk_button_label(nuklear_context, "Unload"))
        {
            logic::UnloadSelf();
        }

        nk_layout_row_dynamic(nuklear_context, 18, 1);

        if (nk_button_label(nuklear_context, "Edit Settings"))
        {
            settings::is_open = true;
        }

        nk_spacing(nuklear_context, 1);
        nk_label(nuklear_context, "Preset:", NK_TEXT_ALIGN_LEFT);

        nk_layout_row_dynamic(nuklear_context, 24, 1);

        static const char* current_preset = "meme";
        if (nk_combo_begin_label(nuklear_context, current_preset, nk_vec2(nk_widget_width(nuklear_context), 24 * 3)))
        {
            nk_layout_row_dynamic(nuklear_context, 18, 1);
            nk_combo_item_label(nuklear_context, "meme", NK_TEXT_ALIGN_LEFT);
            for (int i = 0; i < 5; i++)
            {
                nk_combo_item_label(nuklear_context, ("Fakka " + std::to_string(i)).c_str(), NK_TEXT_ALIGN_LEFT);
            }

            nk_combo_end(nuklear_context);
        }

        nk_end(nuklear_context);
    }
}