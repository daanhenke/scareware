#include "ui/settings.hh"
#include "config.hh"

bool sw::ui::settings::is_open = false;
sw::ui::settings::currenttab_t sw::ui::settings::current_tab = sw::ui::settings::currenttab_t::TAB_MISC;

void sw::ui::settings::RenderColorPickerRow(nk_colorf* color, bool* enabled, std::string label_text)
{
    using namespace render;

    // Create layout of 3 columns and 18px high
    nk_layout_row_dynamic(nuklear_context, 18, 3);

    // Label as first column
    nk_label_wrap(nuklear_context, label_text.c_str());

    // Spacer as second
    nk_spacing(nuklear_context, 1);

    // Button styling
    nk_style_button btn_style = nuklear_context->style.button;
    btn_style.border_color = nk_rgb_cf(*color);

    // Button as third
    if (nk_button_label_styled(nuklear_context, &btn_style, "Edit"))
    {
        *enabled = ! *enabled;
    }

    if (*enabled)
    {
        auto pos = nk_widget_position(nuklear_context);
        auto rect = nk_rect(200, 0, 200, 200);
        if (nk_popup_begin(nuklear_context, NK_POPUP_DYNAMIC, ("Color Picker " + label_text).c_str(), NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE, rect))
        {
            // New row for color picker
            nk_layout_row_dynamic(nuklear_context, 100, 1);
            *color = nk_color_picker(nuklear_context, *color, NK_RGB);;
            nk_popup_end(nuklear_context);
        }
        else
        {
            *enabled = false;
        }
        
    }
}

#include <map>
using sw::ui::settings::currenttab_t;
struct tab_t
{
    currenttab_t type;
    const char* name;
};

const tab_t tab_list[] =
{
    {.type = currenttab_t::TAB_MISC, .name = "Misc"},
    {.type = currenttab_t::TAB_VISUALS, .name = "Visuals"},
    {.type = currenttab_t::TAB_SKINS, .name = "Skins"},
    {.type = currenttab_t::TAB_MOVEMENT, .name = "Movement"},
};

void sw::ui::settings::Render()
{
    using namespace render;

    if (!is_open) return;

    if (nk_begin(nuklear_context, "Settings", nk_rect(200, 200, 800, 800), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE))
    {
        const int tab_count = 4;
        nk_layout_row_dynamic(nuklear_context, 32, tab_count);
        for (auto i = 0; i < tab_count; i++)
        {
            if (nk_button_label(nuklear_context, tab_list[i].name))
            {
                current_tab = tab_list[i].type;
            }
        }

        if (current_tab == currenttab_t::TAB_MISC)
        {
            nk_layout_row_dynamic(nuklear_context, 300, 2);

            if (nk_group_begin(nuklear_context, "g1", 0))
            {
                nk_layout_row_dynamic(nuklear_context, 80, 1);
                if (nk_group_begin(nuklear_context, "Bunnyhop", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
                {
                    nk_layout_row_dynamic(nuklear_context, 30, 1);
                    nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.bhop.enabled);

                    nk_layout_row_end(nuklear_context);
                    nk_group_end(nuklear_context);
                }

                nk_group_end(nuklear_context);
            }

            if (nk_group_begin(nuklear_context, "g2", 0))
            {
                nk_layout_row_dynamic(nuklear_context, 80, 1);

                if (nk_group_begin(nuklear_context, "Noflash", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
                {
                    nk_layout_row_dynamic(nuklear_context, 30, 1);
                    nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.bhop.enabled);

                    nk_layout_row_end(nuklear_context);
                    nk_group_end(nuklear_context);
                }

                nk_group_end(nuklear_context);
            }
        }
        else if (current_tab == currenttab_t::TAB_VISUALS)
        {
            nk_layout_row_dynamic(nuklear_context, 300, 2);

            if (nk_group_begin(nuklear_context, "g1", 0))
            {
                nk_layout_row_dynamic(nuklear_context, 180, 1);

                if (nk_group_begin(nuklear_context, "Chams", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
                {
                    nk_layout_row_dynamic(nuklear_context, 30, 1);
                    nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.chams.enabled);

                    static bool t_colorpicker_active = false;
                    static bool t_hidden_colorpicker_active = false;
                    static bool ct_colorpicker_active = false;
                    static bool ct_hidden_colorpicker_active = false;
                    RenderColorPickerRow(&config::CurrentConfig.chams.terrorist_color, &t_colorpicker_active, "T:");
                    RenderColorPickerRow(&config::CurrentConfig.chams.terrorist_color_hidden, &t_hidden_colorpicker_active, "T (Obscured):");
                    RenderColorPickerRow(&config::CurrentConfig.chams.counterterrorist_color, &ct_colorpicker_active, "CT:");
                    RenderColorPickerRow(&config::CurrentConfig.chams.counterterrorist_color_hidden, &ct_hidden_colorpicker_active, "CT (Obscured):");

                    nk_group_end(nuklear_context);
                }

                nk_group_end(nuklear_context);
            }

            if (nk_group_begin(nuklear_context, "g2", 0))
            {
                nk_layout_row_dynamic(nuklear_context, 80, 1);
                nk_group_end(nuklear_context);
            }
        }
        else if (current_tab == currenttab_t::TAB_SKINS)
        {
            nk_layout_row_dynamic(nuklear_context, 300, 1);

            if (nk_group_begin(nuklear_context, "Skins", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
            {
                nk_layout_row_dynamic(nuklear_context, 30, 2);

                nk_checkbox_label(nuklear_context, "Enabled", (nk_bool*)&config::CurrentConfig.skins.enabled);
                nk_spacing(nuklear_context, 1);

                const char* weapons[] =
                {
                    "AWP",
                    "AK-47"
                };
                const int weapon_count = 2;

                nk_label(nuklear_context, "Weapon:", NK_TEXT_ALIGN_LEFT);
                static const char* selected_weapon = weapons[0];
                if (nk_combo_begin_label(nuklear_context, selected_weapon, nk_vec2(nk_widget_width(nuklear_context), 24 * 3)))
                {
                    nk_layout_row_dynamic(nuklear_context, 18, 1);

                    for (int i = 0; i < weapon_count; i++)
                    {
                        if (nk_combo_item_label(nuklear_context, weapons[i], NK_TEXT_ALIGN_LEFT))
                        {
                            selected_weapon = weapons[i];
                        }
                    }

                    nk_combo_end(nuklear_context);
                }

                struct skin_t
                {
                    int id;
                    const char* name;
                };
                const skin_t skins[] =
                {
                    {344, "Dragon Lore"},
                    {174, "BOOM"},
                    {803, "Neo-Noir"},
                    {475, "Hyper Beast"}
                };
                const int skin_count = 4;
                static const char* selected_skin = skins[0].name;

                nk_label(nuklear_context, "Skin:", NK_TEXT_ALIGN_LEFT);
                if (nk_combo_begin_label(nuklear_context, selected_skin, nk_vec2(nk_widget_width(nuklear_context), 24 * 3)))
                {
                    nk_layout_row_dynamic(nuklear_context, 18, 1);

                    for (int i = 0; i < skin_count; i++)
                    {
                        if (nk_combo_item_label(nuklear_context, skins[i].name, NK_TEXT_ALIGN_LEFT))
                        {
                            selected_skin = skins[i].name;
                            config::CurrentConfig.skins.awp_skin = skins[i].id;
                        }
                    }

                    nk_combo_end(nuklear_context);
                }

                nk_group_end(nuklear_context);
            }
        }
    }
    else
    {
        is_open = false;
    }

    nk_end(nuklear_context);
}