#include "ui/settings.hh"
#include "ui/render.hh"
#include "config.hh"

void sw::ui::settings::Render()
{
    using namespace render;

    if (nk_begin(nuklear_context, "Settings", nk_rect(200, 200, 400, 400), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(nuklear_context, 300, 2);

        if (nk_group_begin(nuklear_context, "Bunnyhop", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(nuklear_context, 30, 1);
            nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.bhop.enabled);

            nk_layout_row_end(nuklear_context);
            nk_group_end(nuklear_context);
        }

        if (nk_group_begin(nuklear_context, "Chams", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(nuklear_context, 70, 1);
            nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.chams.enabled);

            static bool t_colorpicker_active = false;
            if (nk_button_label(nuklear_context, "Terrorist"))
            {
                t_colorpicker_active = !t_colorpicker_active;
            }
            if (t_colorpicker_active)
            {
                config::CurrentConfig.chams.terrorist_color = nk_color_picker(nuklear_context, config::CurrentConfig.chams.terrorist_color, NK_RGB);
            }

            nk_group_end(nuklear_context);
        }

        nk_end(nuklear_context);
    }
}