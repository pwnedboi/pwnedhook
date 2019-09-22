/*
 *  game_event.h
 */
#pragma once

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class game_event_t
{
public:
    
    virtual ~game_event_t() {}
    
    virtual const char* get_name() const = 0;
    
    virtual bool is_reliable() const = 0;
    virtual bool is_local() const = 0;
    virtual bool is_empty(const char* keyname = nullptr) = 0;
    
    virtual bool get_bool(const char* keyname = nullptr, bool default_value = false) = 0;
    virtual int  get_int(const char* keyname = nullptr, int default_value = 0) = 0;
    virtual uint64_t get_uint64(const char* keyname = nullptr, uint64_t default_value = 0) = 0;
    virtual float det_float(const char* keyname = nullptr, float default_value = 0.0f) = 0;
    virtual const char* get_string(const char* keyname = nullptr, const char* default_value = "") = 0;
    virtual const wchar_t* get_w_string(const char* keyname = nullptr, const wchar_t* default_value = L"") = 0;
    virtual const void* get_pointer(const char* keyname = nullptr, const void* default_values = nullptr) = 0;
    
    virtual void set_bool(const char* keyname, bool value) = 0;
    virtual void set_int(const char* keyname, int value) = 0;
    virtual void set_uint64(const char* keyname, uint64_t value) = 0;
    virtual void set_float(const char* keyname, float value) = 0;
    virtual void set_string(const char* keyname, const char* value) = 0;
    virtual void set_w_string(const char* keyname, const wchar_t* value) = 0;
    virtual void set_pointer(const char* keyname, const void* value) = 0;
};

class game_event_listener2_t
{
public:
    
    virtual ~game_event_listener2_t() {}
    
    virtual void fire_game_event(game_event_t* event) = 0;
    virtual int  get_event_debug_id() = 0;
};

struct bf_read;
struct bf_write;
class key_values_t;

class game_event_manager2_t
{
public:
    virtual ~game_event_manager2_t() {}
    virtual int  load_events_from_file(const char* filename) = 0;
    virtual void reset() = 0;
    virtual bool add_listener(game_event_listener2_t* listener, const char* name, bool serverside) = 0;
    virtual bool find_listener(game_event_listener2_t* listener, const char* name) = 0;
    virtual void remove_listener(game_event_listener2_t* listener) = 0;
    virtual void add_listener_global(game_event_listener2_t* listener, bool serverside) = 0;
    virtual game_event_t* create_event(const char* name, bool force = false, int* cookie = nullptr) = 0;
    virtual bool fire_event(game_event_t* event, bool dont_broadcast = false) = 0;
    virtual bool fire_event_client_Side(game_event_t* event) = 0;
    virtual game_event_t* duplicate_event(game_event_t* event) = 0;
    virtual void free_event(game_event_t* event) = 0;
    virtual bool serialize_event(game_event_t* event, bf_write* buffer) = 0;
    virtual game_event_t* unserialize_event(bf_read* buffer) = 0;
    virtual key_values_t* get_event_data_types(game_event_t* event) = 0;
};
