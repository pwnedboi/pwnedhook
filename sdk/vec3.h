/*
 *  vec3.h
 */
#pragma once
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-value"

#include <math.h>
#include <stdlib.h>
#include <string>

#define M_RADPI         57.295779513082
#define M_PI_F          ((float)(M_PI))         // Shouldn't collide with anything.
#define M_PHI           1.61803398874989484820  // golden ratio
#ifndef RAD2DEG
#	define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif
#ifndef DEG2RAD
#	define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif
#ifndef rad
#   define rad(a) a * 0.01745329251
#endif

/*
 *  vec type
 */

typedef float vec_t;

/*
 *  vec3 class
 */

class vec3_t
{
public:
    
    vec_t x, y, z;
    
    vec3_t(void);
    vec3_t(vec_t X, vec_t Y, vec_t Z);
    explicit vec3_t(vec_t XYZ);
    
    void init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
    
    vec_t   operator[](int i) const;
    vec_t&  operator[](int i);
    
    vec_t*       base();
    const vec_t* base() const;
    
    inline bool is_valid() const;
    inline void invalidate();
    
    inline void negate();
    
    inline void zero();
    inline bool is_zero(float tolerance = 0.01f) const;
    
    inline vec3_t get_normalised(void);
    inline vec3_t normalise(void);
    inline vec3_t normalised();
    inline vec_t  normalise_in_place();
    
    inline vec_t dist_to(const vec3_t& other) const;
    
    void copy_to_array(float* arr) const;
    
    vec_t dot(const vec3_t& other) const;
    float dot(float* other) const;
    
    vec_t length(void) const;
    vec_t length_sqr(void) const;
    vec_t length_2d(void) const;
    vec_t length_2d_sqr(void) const;
    
    vec3_t cross(const vec3_t& other)  const;
    vec3_t lerp();
    void clamp();
    
    vec3_t& operator=(const vec3_t& other);
    
    bool operator==(const vec3_t& v) const;
    bool operator!=(const vec3_t& v) const;
    
    inline vec3_t& operator+=(const vec3_t &v);
    inline vec3_t& operator-=(const vec3_t &v);
    inline vec3_t& operator*=(const vec3_t &v);
    inline vec3_t& operator*=(float s);
    inline vec3_t& operator/=(const vec3_t &v);
    inline vec3_t& operator/=(float s);
    inline vec3_t& operator+=(float fl);
    inline vec3_t& operator-=(float fl);
    
    vec3_t operator-(void)              const;
    vec3_t operator+(const vec3_t& v)   const;
    vec3_t operator-(const vec3_t& v)   const;
    vec3_t operator*(const vec3_t& v)   const;
    vec3_t operator/(const vec3_t& v)   const;
    vec3_t operator*(float fl)          const;
    vec3_t operator/(float fl)          const;
    
    inline std::string to_string();
};

/*
 *  initialization
 */

inline vec3_t::vec3_t(void)
{
    x = y = z = 0.f;
}

inline vec3_t::vec3_t(vec_t X, vec_t Y, vec_t Z)
{
    x = X; y = Y; z = Z;
}

inline vec3_t::vec3_t(vec_t XYZ)
{
    x = y = z = XYZ;
}

/*
 *  initialisation
 */

inline void vec3_t::init(vec_t ix, vec_t iy, vec_t iz)
{
    x = ix; y = iy; z = iz;
}

/*
 *  array access
 */

inline vec_t& vec3_t::operator[](int i)
{
    return ((vec_t*)this)[i];
}

inline vec_t vec3_t::operator[](int i) const
{
    return ((vec_t*)this)[i];
}

/*
 *  base address
 */

inline vec_t* vec3_t::base()
{
    return (vec_t*)this;
}

inline vec_t const* vec3_t::base() const
{
    return (vec_t const*)this;
}

/*
 *  (in)validation
 */

inline bool vec3_t::is_valid() const
{
    return isfinite(x) && isfinite(y) && isfinite(z);
}

inline void vec3_t::invalidate()
{
    x = y = z = std::numeric_limits<float>::infinity();
}

/*
 *  zero
 */

inline void vec3_t::zero()
{
    x = y = z = 0.0f;
}

inline bool vec3_t::is_zero(float tolerance) const
{
    return (x > -tolerance && x < tolerance && y > -tolerance && y < tolerance && z > -tolerance && z < tolerance);
}

/*
 *  normalise
 */

inline vec3_t vec3_t::get_normalised(void)
{
    float length = this->length();
    
    if (length != 0)
        return vec3_t(*this / length);
    
    return vec3_t(0, 0, 1);
}

inline vec3_t vec3_t::normalise(void)
{
    *this = get_normalised();
    return *this;
}

inline vec3_t vec3_t::normalised()
{
    if (this->x != this->x)
        this->x = 0;
    
    if (this->y != this->y)
        this->y = 0;
    
    if (this->z != this->z)
        this->z = 0;
    
    if (this->x > 89.f)
        this->x = 89.f;
    
    if (this->x < -89.f)
        this->x = -89.f;
    
    while (this->y > 180)
        this->y -= 360;
    
    while (this->y <= -180)
        this->y += 360;
    
    if (this->y > 180.f)
        this->y = 180.f;
    
    if (this->y < -180.f)
        this->y = -180.f;
    
    this->z = 0;
    
    return *this;
}

inline vec_t vec3_t::normalise_in_place()
{
    vec3_t& v = *this;
    
    float iradius = 1.f / (this->length() + 1.192092896e-07F); // FLT_EPSILON
    
    v.x *= iradius;
    v.y *= iradius;
    v.z *= iradius;
}

/*
 *  distance between
 */

inline vec_t vec3_t::dist_to(const vec3_t& other) const
{
    vec3_t delta;
    
    delta.x = x - other.x;
    delta.y = y - other.y;
    delta.z = z - other.z;
    
    return delta.length();
}

/*
 *  copy
 */

inline void vec3_t::copy_to_array(float* rgfl) const
{
    rgfl[0] = x;
    rgfl[1] = y;
    rgfl[2] = z;
}

/*
 *  dot product
 */

static inline vec_t _vec_dot_product(const vec3_t& a, const vec3_t& b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline vec_t vec3_t::dot(const vec3_t& other) const
{
    return _vec_dot_product(*this, other);
}

inline float vec3_t::dot(float* other) const
{
    return (this->x * other[0] + this->y * other[1] + this->z * other[2]);
}

/*
 *  length
 */

inline vec_t vec3_t::length(void) const
{
    float root = 0.0f;
    float sqsr = x * x + y * y + z * z;
    
    __asm
    {
        sqrtss xmm0, sqsr
        movss root, xmm0
    }
    
    return root;
}

inline vec_t vec3_t::length_sqr(void) const
{
    return (x * x + y * y + z * z);
}

inline vec_t vec3_t::length_2d(void) const
{
    float root = 0.0f;
    float sqst = x * x + y * y;
    
    __asm
    {
        sqrtss xmm0, sqst
        movss root, xmm0
    }
    
    return root;
}

/*
 *  clamp
 */

inline void vec3_t::clamp()
{
    if(this->x > 89)
        this->x = 89;
    
    if(this->x < -89)
        this->x = -89;
    
    while(this->y > 180)
        this->y -= 360;
    
    while(this->y < -180)
        this->y += 360;
    
    this->z = 0;
}

/*
 *  assignment
 */

inline vec3_t& vec3_t::operator=(const vec3_t &other)
{
    x = other.x; y = other.y; z = other.z;
    return *this;
}

/*
 *  comparison
 */

inline bool vec3_t::operator==(const vec3_t& src) const
{
    return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool vec3_t::operator!=(const vec3_t& src) const
{
    return (src.x != x) || (src.y != y) || (src.z != z);
}

/*
 *  standard math operations
 */

inline void vec3_t::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

inline vec3_t& vec3_t::operator+=(const vec3_t& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline vec3_t& vec3_t::operator+=(float fl)
{
    x += fl;
    y += fl;
    z += fl;
    return *this;
}

inline vec3_t& vec3_t::operator-=(const vec3_t& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

inline vec3_t& vec3_t::operator-=(float fl)
{
    x -= fl;
    y -= fl;
    z -= fl;
    return *this;
}

inline vec3_t& vec3_t::operator*=(const vec3_t& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

inline vec3_t& vec3_t::operator*=(float fl)
{
    x *= fl;
    y *= fl;
    z *= fl;
    return *this;
}

inline vec3_t& vec3_t::operator/=(const vec3_t& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

inline vec3_t& vec3_t::operator/=(float fl)
{
    float oofl = 1.0f / fl;
    x *= oofl;
    y *= oofl;
    z *= oofl;
    return *this;
}

/*
 *  arithmetic operations
 */

static inline void vec3_subtract(const vec3_t& a, const vec3_t& b, vec3_t& c)
{
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
}

static inline void vec3_add(const vec3_t& a, const vec3_t& b, vec3_t& result)
{
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
}

static inline void vec3_multiply(const vec3_t& a, const vec3_t& b, vec3_t& c)
{
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
}

static inline void vec3_multiply(const vec3_t& a, vec_t b, vec3_t& c)
{
    c.x = a.x * b;
    c.y = a.y * b;
    c.z = a.z * b;
}

static inline void vec3_divide(const vec3_t& a, vec_t b, vec3_t& c)
{
    vec_t oob = 1.0f / b;
    c.x = a.x * oob;
    c.y = a.y * oob;
    c.z = a.z * oob;
}

static inline void vec3_divide(const vec3_t& a, const vec3_t& b, vec3_t& c)
{
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
}

inline vec3_t vec3_t::operator-(void) const
{
    return vec3_t(-x, -y, -z);
}

inline vec3_t vec3_t::operator-(const vec3_t& v) const
{
    vec3_t res;
    vec3_subtract(*this, v, res);
    return res;
}

inline vec3_t vec3_t::operator+(const vec3_t& v) const
{
    vec3_t res;
    vec3_add(*this, v, res);
    return res;
}

inline vec3_t operator*(float fl, const vec3_t& v)
{
    return v * fl;
}

inline vec3_t vec3_t::operator*(const vec3_t& v) const
{
    vec3_t res;
    vec3_multiply(*this, v, res);
    return res;
}

inline vec3_t vec3_t::operator*(float fl) const
{
    vec3_t res;
    vec3_multiply(*this, fl, res);
    return res;
}

inline vec3_t vec3_t::operator/(float fl) const
{
    vec3_t res;
    vec3_divide(*this, fl, res);
    return res;
}

inline vec3_t vec3_t::operator/(const vec3_t& v) const
{
    vec3_t res;
    vec3_divide(*this, v, res);
    return res;
}

/*
 *  to string
 */

inline std::string vec3_t::to_string()
{
    return std::string("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}

/*
 *  Aligned vector
 */

class __attribute__((aligned(16))) vec3_aligned_t : public vec3_t
{
public:
    
    inline vec3_aligned_t(void) {};
    
    inline vec3_aligned_t(float X, float Y, float Z)
    {
        init(X, Y, Z);
    }
    
public:
    
    explicit vec3_aligned_t(const vec3_t &other)
    {
        init(other.x, other.y, other.z);
    }
    
    vec3_aligned_t& operator=(const vec3_t &other)
    {
        init(other.x, other.y, other.z);
        return *this;
    }
    
    float w; // this space is used anyway
};
