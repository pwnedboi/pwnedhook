/*
 *  base_handle.h
 */
#pragma once

#define NUM_ENT_ENTRY_BITS          (11 + 2)
#define NUM_ENT_ENTRIES             (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX        0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS          16
#define NUM_SERIAL_NUM_SHIFT_BITS   (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK              ((1 << NUM_SERIAL_NUM_BITS) - 1)

class base_handle_t
{
private:
    
    friend class base_entity_list_t;
    
public:
    
    base_handle_t();
    base_handle_t(const base_handle_t& other);
    base_handle_t(unsigned long value);
    base_handle_t(int entry, int serial_number);
    
    void init(int entry, int serial_number);
    void term();
    
    bool is_valid() const;
    
    int get_entry_index() const;
    int get_serial_number() const;
    
    int to_int() const;
    bool operator !=(const base_handle_t& other) const;
    bool operator ==(const base_handle_t& other) const;
    bool operator ==(const handle_entity_t* ent) const;
    bool operator !=(const handle_entity_t* ent) const;
    bool operator <(const base_handle_t& other) const;
    bool operator <(const handle_entity_t* ent) const;
    
    const base_handle_t& operator=(const handle_entity_t* entity);
    const base_handle_t& set(const handle_entity_t* entity);
    
    handle_entity_t* get() const;
    
protected:
    
    unsigned long  m_index;
    
};

template<class T>
class handle_t : public base_handle_t
{
public:
    
    handle_t();
    handle_t(int entry, int serial_numer);
    handle_t(const base_handle_t& handle);
    handle_t(T* val);
    
    static handle_t<T> from_index(int index);
    
    T* get() const;
    void set(const T* val);
    
    operator T*();
    operator T*() const;
    
    bool operator !() const;
    bool operator==(T* val) const;
    bool operator!=(T* val) const;
    const base_handle_t& operator=(const T* val);
    
    T* operator->() const;
};

template<class T>
handle_t<T>::handle_t()
{
    
}

template<class T>
handle_t<T>::handle_t(int entry, int serial_numer)
{
    init(entry, serial_numer);
}

template<class T>
handle_t<T>::handle_t(const base_handle_t& handle) : base_handle_t(handle)
{
    
}

template<class T>
handle_t<T>::handle_t(T* obj)
{
    term();
    set(obj);
}

template<class T>
inline handle_t<T> handle_t<T>::from_index(int index)
{
    handle_t<T> ret;
    ret.m_index = index;
    return ret;
}

template<class T>
inline T* handle_t<T>::get() const
{
    return (T*)base_handle_t::get();
}

template<class T>
inline bool handle_t<T>::operator !() const
{
    return !get();
}

template<class T>
inline bool handle_t<T>::operator==(T* val) const
{
    return get() == val;
}

template<class T>
inline bool handle_t<T>::operator!=(T* val) const
{
    return get() != val;
}

template<class T>
void handle_t<T>::set(const T* val)
{
    base_handle_t::set(reinterpret_cast<const handle_entity_t*>(val));
}

template<class T>
inline const base_handle_t& handle_t<T>::operator=(const T* val)
{
    set(val);
    return *this;
}

template<class T>
T* handle_t<T>::operator -> () const
{
    return get();
}

inline base_handle_t::base_handle_t()
{
    m_index = INVALID_EHANDLE_INDEX;
}

inline base_handle_t::base_handle_t(const base_handle_t &other)
{
    m_index = other.m_index;
}

inline base_handle_t::base_handle_t(unsigned long value)
{
    m_index = value;
}

inline base_handle_t::base_handle_t(int entry, int serial_number)
{
    init(entry, serial_number);
}

inline void base_handle_t::init(int entry, int serial_number)
{
    m_index = (unsigned long)(entry | (serial_number << NUM_SERIAL_NUM_SHIFT_BITS));
}

inline void base_handle_t::term()
{
    m_index = INVALID_EHANDLE_INDEX;
}

inline bool base_handle_t::is_valid() const
{
    return m_index != INVALID_EHANDLE_INDEX;
}

inline int base_handle_t::get_entry_index() const
{
    // There is a hack here: due to a bug in the original implementation of the
    // entity handle system, an attempt to look up an invalid entity index in
    // certain cirumstances might fall through to the the mask operation below.
    // This would mask an invalid index to be in fact a lookup of entity number
    // NUM_ENT_ENTRIES, so invalid ent indexes end up actually looking up the
    // last slot in the entities array. Since this slot is always empty, the
    // lookup returns NULL and the expected behavior occurs through this unexpected
    // route.
    // A lot of code actually depends on this behavior, and the bug was only exposed
    // after a change to NUM_SERIAL_NUM_BITS increased the number of allowable
    // static props in the world. So the if-stanza below detects this case and
    // retains the prior (bug-submarining) behavior.
    
    if(!is_valid())
        return NUM_ENT_ENTRIES - 1;
    
    return m_index & ENT_ENTRY_MASK;
}

inline int base_handle_t::get_serial_number() const
{
    return (int)m_index >> NUM_SERIAL_NUM_SHIFT_BITS;
}

inline int base_handle_t::to_int() const
{
    return (int)m_index;
}

inline bool base_handle_t::operator !=(const base_handle_t& other) const
{
    return m_index != other.m_index;
}

inline bool base_handle_t::operator ==(const base_handle_t& other) const
{
    return m_index == other.m_index;
}

inline bool base_handle_t::operator ==(const handle_entity_t* ent) const
{
    return get() == ent;
}

inline bool base_handle_t::operator !=(const handle_entity_t* ent) const
{
    return get() != ent;
}

inline bool base_handle_t::operator <(const base_handle_t& other) const
{
    return m_index < other.m_index;
}

inline bool base_handle_t::operator <(const handle_entity_t* entity) const
{
    unsigned long index = (entity) ? entity->get_ref_ehandle().m_index : INVALID_EHANDLE_INDEX;
    return m_index < index;
}

inline const base_handle_t& base_handle_t::operator=(const handle_entity_t* entity)
{
    return set(entity);
}

inline const base_handle_t& base_handle_t::set(const handle_entity_t* entity)
{
    if(entity)
        *this = entity->get_ref_ehandle();
    else
        m_index = INVALID_EHANDLE_INDEX;
    
    return *this;
}
