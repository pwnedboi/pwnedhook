/*
 *  color.h
 */
#pragma once

class color_t
{
private:
    
    unsigned char _color[4];
    
public:
    
    color_t()
    {
        *((int*)this) = 0;
    }
    
    color_t(int _r,int _g,int _b)
    {
        set_color(_r, _g, _b, 255);
    }
    
    color_t(int _r,int _g,int _b,int _a)
    {
        set_color(_r, _g, _b, _a);
    }
    
    inline void set_color(int _r, int _g, int _b, int _a = 0)
    {
        _color[0] = (unsigned char)_r;
        _color[1] = (unsigned char)_g;
        _color[2] = (unsigned char)_b;
        _color[3] = (unsigned char)_a;
    }
    
    inline void get_color(int &_r, int &_g, int &_b, int &_a) const
    {
        _r = _color[0];
        _g = _color[1];
        _b = _color[2];
        _a = _color[3];
    }
    
    inline void set_raw_color(int color32)
    {
        *((int*)this) = color32;
    }
    
    inline int get_raw_color() const
    {
        return *((int*)this);
    }
    
    inline int r() const { return _color[0]; }
    inline int g() const { return _color[1]; }
    inline int b() const { return _color[2]; }
    inline int a() const { return _color[3]; }
    
    inline void set_r(uint8_t _i) { _color[0] = _i; }
    inline void set_g(uint8_t _i) { _color[1] = _i; }
    inline void set_b(uint8_t _i) { _color[2] = _i; }
    inline void set_a(uint8_t _i) { _color[3] = _i; }
    
    unsigned char& operator[](int index)
    {
        return _color[index];
    }
    
    const unsigned char& operator[](int index) const
    {
        return _color[index];
    }
    
    bool operator==(const color_t& rhs) const
    {
        return (*((int*)this) == *((int*)&rhs));
    }
    
    bool operator!=(const color_t &rhs) const
    {
        return !(operator==(rhs));
    }
    
    color_t& operator=(const color_t& rhs)
    {
        set_raw_color(rhs.get_raw_color());
        return *this;
    }
    
    static color_t get_from_hsb(float hue, float saturation, float brightness)
    {
        float h = hue == 1.0f ? 0 : hue * 6.0f;
        float f = h - (int)h;
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        unsigned char r = 0, g = 0, b = 0;
        
        if (h < 1)
        {
            r = (brightness * 255);
            g = (t * 255);
            b = (p * 255);
        }
        else if (h < 2)
        {
            r = (q * 255);
            g = (brightness * 255);
            b = (p * 255);
        }
        else if (h < 3)
        {
            r = (p * 255);
            g = (brightness * 255);
            b = (t * 255);
        }
        else if (h < 4)
        {
            r = (p * 255);
            g = (q * 255);
            b = (brightness * 255);
        }
        else if (h < 5)
        {
            r = (t * 255);
            g = (p * 255);
            b = (brightness * 255);
        }
        else
        {
            r = (brightness * 255);
            g = (p * 255);
            b = (q * 255);
        }
        
        return color_t(r, g, b);
    }
    
public:
    
    static color_t white, black, green, blue, magenta, pwned;
    
};

typedef color_t colour_t;
