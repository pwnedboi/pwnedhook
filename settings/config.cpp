/*      config.cpp
 *
 *
 */
#include "common.h"
#include "config.h"
#include <nlohmann/json.hpp>
#include <dirent.h>

using json = nlohmann::json;

config_manager_t g_config;

const  std::string g_config_ext     = ".txt";
static std::string g_config_folder  = "";

/*
 *  does_file_exist
 *  Returns true if given file exists
 */
inline bool does_file_exist(const string& path)
{
    fstream f(path);
    return  f.good();
}

/*
 *  find_files
 *
 */
std::vector<std::string> find_files(const std::string& path, const std::string& extention = "")
{
    std::vector<std::string> found = {};
    
    DIR* dir = opendir(path.c_str());
    
    if(!dir)
        return {};
    
    dirent* ent;
    
    // iterate thru all files in dir
    while((ent = readdir(dir)))
    {
        if(ent->d_type == DT_DIR)
            continue;
        
        std::string name = ent->d_name;
        
        // no extention given so save all
        if(extention.empty())
        {
            found.push_back(name);
        }
        else
        {
            // only save extention
            if(name.find(g_config_ext) != std::string::npos)
                found.push_back(name);
        }
    }
    
    return found;
}

/*
 *  init
 *  Sets up the config manager
 */
void config_manager_t::init()
{
    // set the folder path, /Users/username/pwned/
    g_config_folder = std::string(getenv("HOME")) + "/pwned/";
    
    // create the config folder if not exists
    if(!does_file_exist(g_config_folder))
        mkdir(g_config_folder.c_str(), 0755);
    
    // check the config folder for configs
    m_configs = find_files(g_config_folder, g_config_ext);
    
    // no configs found
    if(m_configs.size() < 1)
    {
        // create the default config
        m_configs.push_back("default");
        
        // create the new config
        this->save();
    }
    
    // load the config
    load();
    
    // resave it to update it incase new settings added
    save();
}

/*
 *  set_color_value
 *  Saves an ImColor to the config
 */
void set_color_value(json& config, const string& key, ImColor color)
{
    config["colors"][key]["r"] = color.Value.x;
    config["colors"][key]["g"] = color.Value.y;
    config["colors"][key]["b"] = color.Value.z;
    config["colors"][key]["a"] = color.Value.w;
}

/*
 *  save
 *  Saves the selected config
 */
void config_manager_t::save()
{
    json config;
    string section;
    
    section = "menu";
    config[section]["key"]              = set.menu.key;
    
    section = "legit";
    config[section]["aimbot"]           = set.legit.aimbot;
    config[section]["fov"]              = set.legit.fov;
    
    section = "rage";
    
    section = "antiaim";
    
    section = "misc";
    
    section = "colors";
    set_color_value(config, "menu", set.colors.menu);
    set_color_value(config, "player_box", set.colors.players.box);
    
    std::string current = m_configs.at(set.config);
    
    ofstream f;
    f.open(g_config_folder + current + g_config_ext);
    f << config.dump(2, ' ');
    f.close();
}

/*
 *  get_value
 *  Gets and sets the given value from the config
 */
template<class T>
void get_value(json config, string section, string key, T* value)
{
    try
    {
        *value = config[section][key];
    }
    catch(exception& e)
    {
        
    }
}

/*
 *  get_color_value
 *  Gets and sets the given color value from the config
 */
void get_color_value(json config, string key, ImColor* value)
{
    try
    {
        value->Value.x = config["colors"][key]["r"];
        value->Value.y = config["colors"][key]["g"];
        value->Value.z = config["colors"][key]["b"];
        value->Value.w = config["colors"][key]["a"];
    }
    catch(exception& e)
    {
        
    }
}

/*
 *  load
 *  Loads the selected configs settings
 */
void config_manager_t::load()
{
    std::string current = m_configs.at(set.config);
    
    string buffer, line;
    
    if(!does_file_exist(g_config_folder + current + g_config_ext))
    {
        pwnedhook("Cant find config");
        return;
    }
    
    ifstream f(g_config_folder + current + g_config_ext);
    
    // read the config into "buffer"
    while(getline(f, line))
        buffer += line;
    
    f.close();
    
    json config = json::parse(buffer);
    
    string section;
    
    section = "menu";
    get_value(config, section, "key", &set.menu.key);
    
    section = "legit";
    get_value(config, section, "aimbot", &set.legit.aimbot);
    get_value(config, section, "fov", &set.legit.fov);
    
    section = "colors";
    get_color_value(config, "menu", &set.colors.menu);
    get_color_value(config, "player_box", &set.colors.players.box);
}
