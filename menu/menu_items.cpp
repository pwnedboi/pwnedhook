/*
 *  menu_items.cpp
 */
#include "menu.h"

namespace
{
    ImColor col_item(50, 50, 50, 255), col_item_active(157, 209, 70, 255), col_text(130, 130, 130, 255);
}

static bool mouse_clicked(int mouse = 0)
{
    return ImGui::GetIO().MouseClicked[mouse];
}

static bool mouse_down(int mouse = 0)
{
    return ImGui::GetIO().MouseDown[mouse];
}

void menu_t::set_offset(int x, int y)
{
    m_offsets.at(m_side) = {x, y};
}

ImVec2 menu_t::get_offset()
{
    return m_offsets.at(m_side);
}

void menu_t::reset_offset()
{
    m_offsets.at(side_left) = ImVec2(10 + 125 + 10 + 15 + 17,        10 + 15);
    m_offsets.at(side_right)= ImVec2(10 + 125 + 10 + 15 + 17 + 170,  10 + 15);
}

void menu_t::set_side(menu_side_t new_side)
{
    m_side = new_side;
}

ImVec2 menu_t::add_menu_item(menu_item_type_t item_type)
{
    ImVec2 offset = get_offset();
    ImVec2 new_offset = offset;
    static menu_item_type_t last_type = item_none;
    
    new_offset.y += item_type;
    
    set_offset(new_offset.x, new_offset.y);
    
    m_last_item_type = last_type;
    last_type = item_type;
    
    return offset;
}

/*
 *
 *  Acts like a button, flips value on click
 *  todo : change color on hover maybe
 */
void menu_t::label(std::string label, bool* value, bool cancel, std::vector<ImColor> colors)
{
    ImVec2 offset = add_menu_item(item_label);
    
    int x = this->x + offset.x;
    int y = this->x + offset.y;
    auto ts = draw->get_text_size(label.c_str());
    auto color = colors.at(0);
    
    // todo : more of a note but
    // use this logic for a key picker maybe, click label, press key
    if(value != nullptr)
    {
        static bool last_val  = false;
        static bool down_last = false;
        
        if(!down_last)
            last_val = *value;
        
        if(mouse_down())
        {
            if(draw->in_area(x, y, 100, ts.y) && !cancel)
                *value = !last_val;
            
            down_last = true;
        }
        else
        {
            if(down_last)
            {
                *value = last_val;
                down_last = false;
            }
        }
        
        if(*value && colors.size() > 1)
            color = colors.at(1);
    }
    
    draw->draw_string(x, y, label.c_str(), Fonts::small, color);
}

void menu_t::checkbox(std::string label, bool* val, bool cancel)
{
    ImVec2 offset = add_menu_item(item_checkbox);
    
    const int size = 7;
    int x = this->x + offset.x;
    int y = this->y + offset.y;
    
    ImColor col = col_item;
    
    if(*val)
        col = col_item_active;
    
    draw->draw_box_filled(x - 10 - size, y + 1, size, size, col);
    draw->draw_string(x, y, label.c_str(), Fonts::small, col_text);
    
    if(draw->in_area(x - 1 - 10 - size, y - 1, 100, size + 2) && mouse_clicked() && !cancel)
        *val = !(*val);
}

void menu_t::slider_i(std::string label, ImVec2 bounds, int* value, bool cancel, std::string suffix)
{
    ImVec2 offset = add_menu_item(item_slider);
    
    const int w = 120, h = 7;
    int x = this->x + offset.x;
    int y = this->y + offset.y;
    
    int fill_value = (*value) * ((float)w / (float)(bounds.y));
    
    ImVec2 mouse = ImGui::GetIO().MousePos;
    
    if (mouse.x > (x - 3) && mouse.y > (y + 15 - 3) && mouse.x < (x + w + 6) && mouse.y < (y + h + 15 + 6) && mouse_down() && !cancel)
        *value = ((mouse.x - x) / ((float)w / (float)(bounds.y)));
    
    // clamp
    if (*value > bounds.y)
        *value = bounds.y;
    
    if (*value < bounds.x)
        *value = bounds.x;
    
    // label and slider
    draw->draw_string(x, y, label.c_str(), Fonts::small, col_text);
    draw->draw_box_filled(x, y + 15, w, h, col_item);
    draw->draw_box_filled(x, y + 15, fill_value, h, col_item_active);
    
    std::string val_str = std::to_string(*value) + suffix;
    
    // value string
    draw->draw_string(x + fill_value + 5, y + 15 - 1, val_str.c_str(), Fonts::small, col_text);
    
    int increment_val = (bounds.y / w);
    
    if(increment_val < 1)
        increment_val = 1;
    
    // -
    draw->draw_line(x - 10, y + 15 + 3, x - 5, y + 15 + 3, col_text);
    
    if(draw->in_area(x - 10 - 1, y + 15 , 7, 7) && mouse_clicked())
        *value -= increment_val;
    
    // +
    draw->draw_line(x + w + 5, y + 15 + 3, x + w + 10, y + 15 + 3, col_text);
    draw->draw_line(x + w + 5 + 2, y + 15 + 1, x + w + 5 + 2, y + 15 + 6, col_text);
    
    if(draw->in_area(x + w + 5 - 1, y + 15, 7, 7) && mouse_clicked())
        *value += increment_val;
    
    // draw bounds
    // draw->draw_box(x - 3, y + 15 - 3, w + 6, h + 6, ImColor(255, 0, 0, 255));
}

void menu_t::slider_f(std::string label, ImVec2 bounds, float* value, bool cancel, std::string suffix, int prec)
{
    ImVec2 offset = add_menu_item(item_slider);
    
    const int w = 120, h = 7;
    int x = this->x + offset.x;
    int y = this->y + offset.y;
    
    float fill_value = (*value) * ((float)w / (float)(bounds.y));
    
    ImVec2 mouse = ImGui::GetIO().MousePos;
    
    if (mouse.x > (x - 3) && mouse.y > (y + 15 - 3) && mouse.x < (x + w + 6) && mouse.y < (y + h + 15 + 6) && mouse_down() && !cancel)
        *value = ((mouse.x - x) / ((float)w / (float)(bounds.y)));
    
    // clamp
    if (*value > bounds.y)
        *value = bounds.y;
    
    if (*value < bounds.x)
        *value = bounds.x;
    
    draw->draw_string(x, y, label.c_str(), Fonts::small, col_text);
    draw->draw_box_filled(x, y + 15, w, h, col_item);
    draw->draw_box_filled(x, y + 15, fill_value, h, col_item_active);
    
    // set precision
    std::string val_str = std::to_string(*value);
    std::string precision = "%." + std::to_string(prec) + "f";
    auto written = std::snprintf(&val_str[0], val_str.size(), precision.c_str(), *value);
    val_str.resize(written);
    val_str += suffix;
    
    draw->draw_string(x + fill_value + 5, y + 15 - 1, val_str.c_str(), Fonts::small, col_text);
    
    float increment_val = (bounds.y / w);
    
    // -
    draw->draw_line(x - 10, y + 15 + 3, x - 5, y + 15 + 3, col_text);
    
    if(draw->in_area(x - 10 - 1, y + 15 , 7, 7) && mouse_clicked())
        *value -= increment_val;
    
    // +
    draw->draw_line(x + w + 5, y + 15 + 3, x + w + 10, y + 15 + 3, col_text);
    draw->draw_line(x + w + 5 + 2, y + 15 + 1, x + w + 5 + 2, y + 15 + 6, col_text);
    
    if(draw->in_area(x + w + 5 - 1, y + 15, 7, 7) && mouse_clicked())
        *value += increment_val;
    
    // draw bounds
    // draw->draw_box(x - 3, y + 15 - 3, w + 6, h + 6, ImColor(255, 0, 0, 255));
}

bool menu_t::button(std::string label, bool* value)
{
    ImVec2 offset = add_menu_item(item_button);
    
    int w = 120, h = 20;
    int x = this->x + offset.x;
    int y = this->y + offset.y;
    
    ImColor box_col = col_item;
    
    if(draw->in_area(x, y, w, h))
    {
        box_col = ImColor(60, 60, 60);
        
        if(mouse_down())
            box_col = ImColor(70, 70, 70);
    }
    
    draw->draw_box_outlined(x, y, w, h, 1, box_col, ImColor(40, 40, 40));
    draw->draw_string(x + (w / 2), y + 2 + (h / 2), label.c_str(), Fonts::small, col_text, true);
    
    // return value = clicked the button?
    bool ret = draw->in_area(x, y, w, h) && mouse_down();
    
    if(value)
        *value = ret;
    return ret;
}

/*
 *  color_picker_slider
 *
 */
static void color_picker_slider(int x, int y, float* value, ImColor col, bool cancel, im_renderer_t* draw)
{
    const ImVec2 bounds = {0, 255};
    
    const int w = 120, h = 7;
    
    int fill_value = ((*value) * 255.f) * ((float)w / (float)(bounds.y));
    
    ImVec2 mouse = ImGui::GetIO().MousePos;
    
    if (mouse.x > (x - 3) && mouse.y > (y - 3) && mouse.x < (x + w + 6) && mouse.y < (y + h + 6) && mouse_down() && !cancel)
        *value = ((mouse.x - x) / ((float)w / (float)(bounds.y))) / 255.f;
    
    if (*value > bounds.y)
        *value = bounds.y;
    
    if (*value < bounds.x)
        *value = bounds.x;
    
    // slider
    draw->draw_box_filled(x, y, w, h, col_item);
    draw->draw_box_filled(x, y, fill_value, h, col);
    
    std::string val_str = std::to_string((int)(*value * 255.f));
    
    // value string
    draw->draw_string(x + fill_value + 5, y - 1, val_str.c_str(), Fonts::small, col_text);
    
    float increment_val = (1.f / 255.f);
    
    // -
    draw->draw_line(x - 10, y + 3, x - 5, y + 3, col_text);
    
    if(draw->in_area(x - 10 - 1, y , 7, 7) && mouse_clicked())
        *value -= increment_val;
    
    // +
    draw->draw_line(x + w + 5, y + 3, x + w + 10, y + 3, col_text);
    draw->draw_line(x + w + 5 + 2, y + 1, x + w + 5 + 2, y + 6, col_text);
    
    if(draw->in_area(x + w + 5 - 1, y, 7, 7) && mouse_clicked())
        *value += increment_val;
}

/*
 *  color picker
 */

struct color_picker_item_t
{
    ImVec2 offset;
    ImColor* value;
    bool* open;
    bool cancel;
    bool alpha;
};

static std::vector<color_picker_item_t> g_picker_list = {};

/*
 *
 *
 */
void menu_t::color_picker(ImColor* value, bool* open, bool cancel, bool alpha)
{
    auto offset = add_menu_item(item_color_picker);
    
    offset.y -= m_last_item_type;
    
    const int h = 8, w = 20;
    int x = this->x + offset.x;
    int y = this->y + offset.y + 1;
    
    // color box with outline
    draw->draw_box_outlined(x + 120 - w, y, w, h, 1, *value, ImColor(50, 50, 50, 255));
    
    // open / close
    if(mouse_clicked() && !cancel)
    {
        if(draw->in_area(x + 120 - w, y, w, h))
            *open = !(*open);
        else if(!draw->in_area(x + 125, y, 150, 42 + (alpha ? (42/3) : 0)))
            *open = false;
    }
    
    // only the opened bit needs to be rendered later
    
    g_picker_list.push_back({offset, value, open, cancel, alpha});
}

/*
 *
 *
 */
void menu_t::render_color_pickers()
{
    int size = (int)g_picker_list.size();
    
    for(int i = size - 1; i > 0; i--)
    {
        auto picker = g_picker_list.at(i);
        
        ImVec2   offset = picker.offset;
        ImColor* value  = picker.value;
        bool     cancel = picker.cancel;
        bool     alpha  = picker.alpha;
        
        const int w = 150, h = 42 + 5 + (alpha ? (42 / 3) : 0);
        int x = this->x + offset.x + 125;
        int y = this->y + offset.y + 1;
        
        if(*picker.open)
        {
            
            draw->draw_box_outlined(x, y, w, h, 1, ImColor(14, 14, 14, 255), ImColor(70, 70, 70, 255));
            
            x += 15;
            y += 5;
            
            color_picker_slider(x, y, &value->Value.x, *value, cancel, draw);
            
            y += 15;
            
            color_picker_slider(x, y, &value->Value.y, *value, cancel, draw);
            
            y += 15;
            
            color_picker_slider(x, y, &value->Value.z, *value, cancel, draw);
            
            // draw alpha bar?
            if(alpha)
            {
                y += 15;
                color_picker_slider(x, y, &value->Value.w, *value, cancel, draw);
            }
        }
    }
}

/*
 *  combo boxes
 */

struct combo_item_t
{
    ImVec2 offset;
    std::string label;
    std::vector<std::string> items;
    int* value;
    bool* open;
    bool cancel;
};

struct multi_combo_item_t
{
    ImVec2 offset;
    std::string label;
    std::vector<std::string> items;
    std::vector<bool>* value;
    bool* open;
    bool cancel;
};

// quick workaround to make them render in 1 func kinda
struct combo_vec_t
{
    bool                _multi = false;
    combo_item_t        combo;
    multi_combo_item_t  multi;
};

static std::vector<combo_vec_t> g_combo_list = {};

/*
 *
 *
 */
static void draw_combo(int _x, int _y, combo_item_t combo, im_renderer_t* draw)
{
    static int ticks_since_open = 0;
    static bool should_wait = false;
    
    if(ticks_since_open < 2 && should_wait)
    {
        should_wait = true;
        ticks_since_open++;
    }
    else if(ticks_since_open >= 2)
    {
        should_wait = false;
        ticks_since_open = 0;
    }
    
    ImVec2 offset = combo.offset;
    
    const int w = 120, h = 13;
    int x = _x + offset.x;
    int y = _y + offset.y;
    
    // label
    draw->draw_string(x, y, combo.label.c_str(), Fonts::small, col_text);
    
    y += 15;
    
    draw->draw_box_filled(x, y, w, h, col_item);
    
    std::string combo_label = "none";
    
    // get selected label
    if(combo.items.size())
    {
        if((*combo.value) < combo.items.size())
            combo_label = combo.items.at(*combo.value);
    }
    
    // draw the label of the selected item
    draw->draw_string(x + 5, y + 2, combo_label.c_str(), Fonts::small, col_text);
    
    // draw items
    if(*combo.open)
    {
        for(int i = 0; i < combo.items.size(); i++)
        {
            ImColor _col = col_item;
            
            if(draw->in_area(x, y + (h * i), w, h))
            {
                _col = ImColor(70, 70, 70);
                
                if(ImGui::GetIO().MouseClicked[0] && !should_wait)
                {
                    *combo.value = i;
                    *combo.open  = false;
                    should_wait  = true;
                }
            }
            
            draw->draw_box_filled(x, y + (h * i), w, h, _col);
            draw->draw_string(x + 5, y + 2 + (h * i), combo.items.at(i).c_str(), Fonts::small, col_text);
        }
    }
    
    // open it
    if(draw->in_area(x, y, w, h) && ImGui::GetIO().MouseClicked[0] && !should_wait)
    {
        *combo.open = !(*combo.open);
        should_wait = true;
    }
}

/*
 *
 *
 */
static void draw_multi_combo(int _x, int _y, multi_combo_item_t combo, im_renderer_t* draw)
{
    static int ticks_since_open = 0;
    static bool should_wait = false;
    
    if(ticks_since_open < 2 && should_wait)
    {
        should_wait = true;
        ticks_since_open++;
    }
    else if(ticks_since_open >= 2)
    {
        should_wait = false;
        ticks_since_open = 0;
    }
    
    // multi_combo_item_t combo = g_multi_combo_positions.at(_i);
    
    ImVec2 offset = combo.offset;
    
    const int w = 120, h = 13;
    int x = _x + offset.x;
    int y = _y + offset.y;
    
    // label
    draw->draw_string(x, y, combo.label.c_str(), Fonts::small, col_text);
    
    y += 15;
    
    draw->draw_box_filled(x, y, w, h, col_item);
    
    std::string combo_label = "";
    
    // get selected label
    if(combo.items.size() && combo.value->size())
    {
        combo_label.clear();
        
        for(int i = 0; i < combo.items.size() && i < combo.value->size(); i++)
        {
            if(combo.value->at(i))
                combo_label += combo.items.at(i) + ", ";
        }
        
        // erase the last ", "
        if(combo_label.size())
            combo_label.resize(combo_label.size() - 2);
        
        if(combo_label.size() > 14)
        {
            combo_label.resize(14);
            combo_label += "...";
        }
    }
    
    if(!combo_label.size())
        combo_label = "none";
    
    // draw the label of the selected item
    draw->draw_string(x + 5, y + 2, combo_label.c_str(), Fonts::small, col_text);
    
    // draw items
    if(*combo.open)
    {
        for(int i = 0; i < combo.items.size(); i++)
        {
            ImColor _col = col_item;
            
            if(draw->in_area(x, y + (h * i), w, h))
            {
                _col = ImColor(70, 70, 70);
                
                if(mouse_clicked() && !should_wait)
                    combo.value->at(i) = !(combo.value->at(i));
            }
            else if(combo.value->at(i))
            {
                _col = ImColor(60, 60, 60);
            }
            
            draw->draw_box_filled(x, y + (h * i), w, h, _col);
            draw->draw_string(x + 5, y + 2 + (h * i), combo.items.at(i).c_str(), Fonts::small, col_text);
        }
    }
    
    // open it if not open, dont wanna close it tho
    if(!(*combo.open) && draw->in_area(x, y, w, h) && mouse_clicked() && !should_wait)
    {
        *combo.open = true;
        should_wait = true;
    }
    
    // if clicked outside of combo then close
    if(!draw->in_area(x, y, w, h * (int)combo.items.size()) && mouse_clicked())
        *combo.open = false;
}

/*
 *
 *
 */
void menu_t::combo(std::string label, std::vector<std::string> items, int* value, bool* open, bool cancel)
{
    ImVec2 offset = add_menu_item(item_combo);
    
    combo_item_t combo = {offset, label, items, value, open, cancel};
    combo_vec_t  vec = {false, combo, {}};
    
    g_combo_list.push_back(vec);
}

/*
 *
 *
 */
void menu_t::combo_multi(std::string label, std::vector<std::string> items, std::vector<bool>* value, bool* open, bool cancel)
{
    ImVec2 offset = add_menu_item(item_combo_multi);
    
    multi_combo_item_t combo = {offset, label, items, value, open, cancel};
    combo_vec_t  vec = {true, {}, combo};
    
    g_combo_list.push_back(vec);
}

/*
 *
 *
 */
void menu_t::render_combos()
{
    int size = (int)g_combo_list.size() - 1;
    
    // start at from the last one so it renders on the bottom
    for(int _i = size; _i >= 0; _i--)
    {
        combo_vec_t vec = g_combo_list.at(_i);
        
        // have to pass the x, y and renderer bc the structs are only defined in this file
        if(vec._multi)
            draw_multi_combo(this->x, this->y, vec.multi, this->draw);
        else
            draw_combo(this->x, this->y, vec.combo, this->draw);
    }
    
    // finished rendering so clear the vec for the next tick
    g_combo_list.clear();
}
