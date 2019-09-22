/*
 *  vec2.h
 */
#pragma once
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-value"

#define fast_sqrt(x) (sqrt)(x)
#define VALVE_RAND_MAX  0x7fff

/*
 *  is finite
 */

static inline unsigned long& _vec_float_bits(float& f)
{
    return *reinterpret_cast<unsigned long*>(&f);
}

static inline bool _vec_is_finite(float f)
{
    return ((_vec_float_bits(f) & 0x7F800000) != 0x7F800000);
}

/*
 *  vec2 class
 */

class vec2_t
{
public:
    
    vec_t x, y;
    
    vec2_t(void);
    vec2_t(vec_t x, vec_t y);
    vec2_t(const float* fvec);
    
    void init(float ix = 0.0f, float iy = 0.0f);
    void random(float min, float max);
    
    bool is_valid() const;
    
    float   operator[](int i) const;
    float&  operator[](int i);
    
    float*       base();
    float const* base() const;
    
    void negate();
    
    float length() const;
    float length_sqr(void) const;
    
    bool is_zero(float tolerance = 0.01f) const
    {
        return (x > -tolerance && x < tolerance && y > -tolerance && y < tolerance);
    }
    
    float normalise();
    float normalise_in_place();
    
    float dist_to(const vec2_t& other) const;
    float dist_to_sqr(const vec2_t& other) const;
    
    void copy_to_array(float* rgfl) const;
    
    float dot(const vec2_t& other) const;
    float dot(const float* other) const;
    
    vec2_t cross(const vec2_t& other) const;
    
    vec2_t min(const vec2_t& other) const;
    vec2_t max(const vec2_t& other) const;
    
    vec2_t& operator=(const vec2_t& other);
    
    vec2_t(const vec2_t& other);
    
    bool operator==(const vec2_t& v) const;
    bool operator!=(const vec2_t& v) const;
    
    vec2_t& operator+=(const vec2_t& v);
    vec2_t& operator-=(const vec2_t& v);
    vec2_t& operator*=(const vec2_t& v);
    vec2_t& operator*=(float s);
    vec2_t& operator/=(const vec2_t& v);
    vec2_t& operator/=(float s);
    
    vec2_t operator-(void) const;
    
    vec2_t operator+(const vec2_t& v) const;
    vec2_t operator-(const vec2_t& v) const;
    vec2_t operator*(const vec2_t& v) const;
    vec2_t operator/(const vec2_t& v) const;
    vec2_t operator*(float fl) const;
    vec2_t operator/(float fl) const;
};

/*
 *  constructor
 */

inline vec2_t::vec2_t(void)
{
    x = y = 0.f;
}

inline vec2_t::vec2_t(float X, float Y)
{
    x = X; y = Y;
}

inline vec2_t::vec2_t(const float* pFloat)
{
    x = pFloat[0]; y = pFloat[1];
}

inline vec2_t::vec2_t(const vec2_t& other)
{
    x = other.x; y = other.y;
}

/*
 *  initialization
 */

inline void vec2_t::init(float ix, float iy)
{
    x = ix; y = iy;
}

inline void vec2_t::random(float minVal, float maxVal)
{
    x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
    y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

/*
 *  array
 */

inline float& vec2_t::operator[](int i)
{
    return ((float*)this)[i];
}

inline float vec2_t::operator[](int i) const
{
    return ((float*)this)[i];
}

/*
 *  copy
 */

inline void vec2_t::copy_to_array(float* rgfl) const
{
    rgfl[0] = x; rgfl[1] = y;
}

/*
 *  base address
 */

inline float* vec2_t::base()
{
    return (float*)this;
}

inline const float* vec2_t::base() const
{
    return (float const*)this;
}

/*
 *  validity
 */

inline bool vec2_t::is_valid() const
{
    return _vec_is_finite(x) && _vec_is_finite(y);
}

/*
 *  dot product
 */

inline float vec2_t::dot(const vec2_t& other) const
{
    return (x * other.x + y * other.y);
}

inline float vec2_t::dot(const float* other) const
{
    return (x * other[0] + y * other[1]);
}

/*
 *  length
 */

inline float vec2_t::length(void) const
{
    return (float)fast_sqrt(x * x + y * y);
}

inline float vec2_t::length_sqr(void) const
{
    return (x * x + y * y);
}

/*
 *  min max
 */

inline vec2_t vec2_t::min(const vec2_t& other) const
{
    return vec2_t(x < other.x ? x : other.x, y < other.y ? y : other.y);
}

inline vec2_t vec2_t::max(const vec2_t& other) const
{
    return vec2_t(x > other.x ? x : other.x, y > other.y ? y : other.y);
}

/*
 *  noramlisation
 */

static inline float vec2_normalize(vec2_t& v)
{
    float l = v.length();
    
    if (l != 0.0f)
        v /= l;
    else
        v.x = v.y = 0.0f;
    
    return l;
}

inline float vec2_t::normalise_in_place()
{
    return vec2_normalize(*this);
}

/*
 *  assignment
 */

inline vec2_t& vec2_t::operator=(const vec2_t& other)
{
    x = other.x; y = other.y;
    return *this;
}

/*
 *  comparison
 */

inline bool vec2_t::operator==(const vec2_t& src) const
{
    return (src.x == x) && (src.y == y);
}

inline bool vec2_t::operator!=(const vec2_t& src) const
{
    return (src.x != x) || (src.y != y);
}

/*
 *  standard math operations
 */

inline void vec2_t::negate()
{
    x = -x;
    y = -y;
}

inline vec2_t& vec2_t::operator+=(const vec2_t& v)
{
    x += v.x; y += v.y;
    return *this;
}

inline vec2_t& vec2_t::operator-=(const vec2_t& v)
{
    x -= v.x; y -= v.y;
    return *this;
}

inline vec2_t& vec2_t::operator*=(float fl)
{
    x *= fl;
    y *= fl;
    return *this;
}

inline vec2_t& vec2_t::operator*=(const vec2_t& v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

inline vec2_t& vec2_t::operator/=(float fl)
{
    float oofl = 1.0f / fl;
    x *= oofl;
    y *= oofl;
    return *this;
}

inline vec2_t& vec2_t::operator/=(const vec2_t& v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

/*
 *  arithmetic operations
 */

static inline void vec2_add(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
    c.x = a.x + b.x;
    c.y = a.y + b.y;
}

static inline void vec2_subtract(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
    c.x = a.x - b.x;
    c.y = a.y - b.y;
}

static inline void vec2_multiply(const vec2_t& a, float b, vec2_t& c)
{
    c.x = a.x * b;
    c.y = a.y * b;
}

static inline void vec2_multiply(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
    c.x = a.x * b.x;
    c.y = a.y * b.y;
}

static inline void vec2_divide(const vec2_t& a, float b, vec2_t& c)
{
    float oob = 1.0f / b;
    c.x = a.x * oob;
    c.y = a.y * oob;
}

static inline void vec2_divide(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
    c.x = a.x / b.x;
    c.y = a.y / b.y;
}

static inline void vec2_ma(const vec2_t& start, float s, const vec2_t& dir, vec2_t& result)
{
    result.x = start.x + s*dir.x;
    result.y = start.y + s*dir.y;
}

inline vec2_t vec2_t::operator+(const vec2_t& v) const
{
    vec2_t res;
    vec2_add(*this, v, res);
    return res;
}

inline vec2_t vec2_t::operator-(void) const
{
    return vec2_t(-x, -y);
}

inline vec2_t vec2_t::operator-(const vec2_t& v) const
{
    vec2_t res;
    vec2_subtract(*this, v, res);
    return res;
}

inline vec2_t operator*(float fl, const vec2_t& v)
{
    return v * fl;
}

inline vec2_t vec2_t::operator*(float fl) const
{
    vec2_t res;
    vec2_multiply(*this, fl, res);
    return res;
}

inline vec2_t vec2_t::operator*(const vec2_t& v) const
{
    vec2_t res;
    vec2_multiply(*this, v, res);
    return res;
}

inline vec2_t vec2_t::operator/(float fl) const
{
    vec2_t res;
    vec2_divide(*this, fl, res);
    return res;
}

inline vec2_t vec2_t::operator/(const vec2_t& v) const
{
    vec2_t res;
    vec2_divide(*this, v, res);
    return res;
}

/*
 *  distance between
 */

inline float vec2_t::dist_to(const vec2_t& other) const
{
    vec2_t delta;
    vec2_subtract(*this, other, delta);
    return delta.length();
}

inline float vec2_t::dist_to_sqr(const vec2_t& other) const
{
    vec2_t delta;
    vec2_subtract(*this, other, delta);
    return delta.length_sqr();
}
