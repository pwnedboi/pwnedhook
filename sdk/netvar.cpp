/*  netvar.cpp
 *
 *
 */
#include "common.h"
#include "netvar.h"

netvar_manager_t g_netvar;

void netvar_manager_t::init()
{
    m_tables = {};
    m_tables = get_all_tables();
}

vector<recv_table_t*> netvar_manager_t::get_all_tables()
{
    vector<recv_table_t*> tables = {};
    
    if(!g_client)
        return tables;
    
    client_class_t* client_class = g_client->get_all_classes();
    
    if (!client_class)
        return tables;
    
    while (client_class)
    {
        tables.push_back(client_class->m_recv_table);
        client_class = client_class->m_next;
    }
    
    return tables;
}

recv_table_t* netvar_manager_t::get_table(string table_name)
{
    if (m_tables.empty())
        return nullptr;
    
    for (unsigned long i = 0; i < m_tables.size(); i++)
    {
        recv_table_t* table = m_tables.at(i);
        
        if (!table)
            continue;
        
        if (strcasecmp(table->m_net_table_name, table_name.c_str()) == 0)
            return table;
    }
    
    return nullptr;
}

int netvar_manager_t::get_offset(string table_name, string prop_name)
{
    int offset = get_prop(table_name, prop_name);
    
    if (!offset)
        return 0;
    
    return offset;
}

int netvar_manager_t::get_prop(string table_name, string prop_name, recv_prop_t** prop)
{
    recv_table_t* table = get_table(table_name);
    
    if (!table)
        return 0;
    
    int offset = get_prop(table, prop_name, prop);
    
    if (!offset)
        return 0;
    
    return offset;
}

int netvar_manager_t::get_prop(recv_table_t* table, string prop_name, recv_prop_t** in_prop)
{
    int extra_offset = 0;
    
    for (int i = 0; i < table->m_prop_count; ++i)
    {
        recv_prop_t* prop   = &table->m_props[i];
        recv_table_t*child  = prop->m_data_table;
        
        if (child && (child->m_prop_count > 0))
        {
            int tmp = get_prop(child, prop_name, in_prop);
            
            if (tmp)
                extra_offset += (prop->m_offset + tmp);
        }
        
        if (strcasecmp(prop->m_var_name, prop_name.c_str()))
            continue;
        
        if (in_prop)
            *in_prop = prop;
        
        return prop->m_offset + extra_offset;
    }
    
    return extra_offset;
}

string netvar_manager_t::dump_table(recv_table_t* table, int depth)
{
    string pre("");
    stringstream ss;
    
    for (int i = 0; i < depth; i++)
        pre.append("\t");
    
    ss << pre << table->m_net_table_name << "\n";
    
    for (int i = 0; i < table->m_prop_count; i++)
    {
        recv_prop_t* prop = &table->m_props[i];
        
        if (!prop)
            continue;
        
        string var_name = prop->m_var_name;
        
        if (var_name.find("baseclass") == 0 || var_name.find("0") == 0 || var_name.find("1") == 0 || var_name.find("2") == 0)
            continue;
        
        ss << pre << "\t" << var_name << " [0x" << hex << prop->m_offset << "]\n";
        
        if (prop->m_data_table)
            ss << dump_table(prop->m_data_table, depth + 1);
    }
    
    return ss.str();
}

/*
 *
 *  Replaces a game func with our func and returns the orignal func
 */
uintptr_t netvar_manager_t::hook(string table_name, string prop_name, recv_var_proxy_fn func)
{
    recv_prop_t* prop = nullptr;
    
    get_prop(table_name, prop_name, &prop);
    
    if (!prop)
        return 0;
    
    uintptr_t old_fn = (uintptr_t)prop->m_proxy_fn;
    
    prop->m_proxy_fn = func;
    
    return old_fn;
}

/*
 *
 *  Prints all netvars to the game console
 */
void netvar_manager_t::print_netvars()
{
    if(!g_client || !g_cvar)
        return;
    
    ofstream file;
    
    file.open("/Users/Finn/Desktop/netvars.txt");
    
    for (client_class_t* cclass = g_client->get_all_classes(); cclass != nullptr; cclass = cclass->m_next)
    {
        recv_table_t* table = cclass->m_recv_table;
        std::string dump = dump_table(table, 0);
        file << dump << std::endl;
        g_cvar->console_print_color(color_t::green, "%s", dump.c_str());
    }
    
    file.close();
}
