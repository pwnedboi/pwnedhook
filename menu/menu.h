/*
 *  menu.h
 */
#include <array>
#include <vector>
#include <string>
#include "imrenderer.h"

enum menu_side_t
{
    side_left  = 0,
    side_right = 1,
};

// the offset amount
enum menu_item_type_t
{
    item_none           = 0,
    item_label          = 15,
    item_checkbox       = item_label,
    item_slider         = item_label * 2,
    item_combo          = 35,
    item_combo_multi    = item_combo,
    item_color_picker   = item_none,
    item_button         = 20,
};

class menu_t
{
private:
    
    // general vars
    int x, y, w, h, tab;
    im_renderer_t* draw;
    
    // menu item vars
    // const int num_cols = 2
    std::array<ImVec2, 2>   m_offsets;
    menu_side_t             m_side;
    menu_item_type_t        m_last_item_type;
    
    // opened combos n pickers, todo : auto check if open
    // private struct of menu, only accessable to the menu
    struct
    {
        // combos
        bool config     = false;
        bool team_flags = false;
        bool bot_bar    = false;
        bool hitmarkers = false;
        bool backtrack  = false;
        bool chams_players = false;
        bool rage_hitboxes = false;
        
        // color pickers
        bool picker_box = false;
        bool picker_chams_players = false;
        bool picker_behind_walls  = false;
    }opened;
    
private:
    
    // menu window
    
    void draw_window();
    void draw_tabs();
    void tab_rage();
    void tab_antiaim();
    void tab_legit();
    void tab_visuals();
    void tab_movement();
    void tab_misc();
    void tab_skins();
    void tab_players();
    
    // controls
    
    void    set_offset(int x, int y);
    ImVec2  get_offset();
    void    reset_offset();
    
    void    set_side(menu_side_t new_side);
    
    ImVec2  add_menu_item(menu_item_type_t item_type);
    void    label(std::string label, bool* value = nullptr, bool cancel = false, std::vector<ImColor> colors = {ImColor(130, 130, 130, 255)});
    void    checkbox(std::string label, bool*   val, bool cancel = false);
    void    slider_i(std::string label, ImVec2 bounds, int*   val, bool cancel = false, std::string suffix = "");
    void    slider_f(std::string label, ImVec2 bounds, float* val, bool cancel = false, std::string suffix = "", int precision = 2);
    bool    button(std::string label, bool* val = nullptr);
    void    combo(std::string label, std::vector<std::string> items, int* value, bool* open, bool cancel = false);
    void    combo_multi(std::string label, std::vector<std::string> items, std::vector<bool>* value, bool* open, bool cancel = false);
    void    color_picker(ImColor* value, bool* open, bool cancel = false, bool alpha = false);
    
    void    render_combos();
    void    render_color_pickers();
    
public:
    
    ~menu_t();
    
    void init();
    void move();
    void render();
    
};
