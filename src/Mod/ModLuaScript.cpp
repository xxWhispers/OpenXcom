//
// Created by ken on 17/09/18.
//

#include "ModLuaScript.h"
#include "../Engine/Logger.h"
#include "../Engine/FileMap.h"
#include "Lua/LuaApi.h"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {

ModLuaScript::ModLuaScript(const FileMap::FileModInfo &modInfo, const std::string &file_relative_path)
        :
        _modInfo(modInfo),
        _file_relative_path(file_relative_path),
        _luaState(nullptr)
{

}

ModLuaScript::~ModLuaScript()
{

}

// Prepare the Lua context and load the script
int ModLuaScript::Load(Game* game)
{
    Log(LOG_INFO) << "Loading script '" << getFilename() << "'";
    Log(LOG_INFO) << "   mod '" << getModIfo().getId() << "'";
    Log(LOG_INFO) << "   path '" << getModIfo().getPath() << "'";

    if(_luaState != nullptr)
    {
        Log(LOG_ERROR) << "Can not initialise a lua state that is already initialised";
        return -1;
    }

    _luaState = luaL_newstate();

    // get the default lua libraries
    luaL_openlibs(_luaState);

    Lua::loadXcomLuaLib(_luaState, game);

    // load the file
    std::string full_filename = getModIfo().getPath() + "/" + getFilename();
    int error = luaL_loadfile(_luaState, full_filename.c_str());

    if(error)
    {
        Log(LOG_ERROR) << lua_tostring(_luaState, -1);
        lua_pop(_luaState, 1);  /* pop error message from the stack */
        Unload();
    }


    return 0;
}

// Run the script
int ModLuaScript::Run()
{
    if(_luaState == nullptr) { return -1; }
    Log(LOG_INFO) << "Running script '" << getFilename() << "'";
    Log(LOG_INFO) << "   mod '" << getModIfo().getId() << "'";
    Log(LOG_INFO) << "   path '" << getModIfo().getPath() << "'";

    lua_pcall(_luaState, 0, 0, 0);

    return 0;
}

// Cleanup and unload the script and safely shut down the Lua context.
int ModLuaScript::Unload()
{
    Log(LOG_INFO) << "Unloading script '" << getFilename() << "'";
    Log(LOG_INFO) << "   mod '" << getModIfo().getId() << "'";
    Log(LOG_INFO) << "   path '" << getModIfo().getPath() << "'";

    if(_luaState == nullptr)
    {
        Log(LOG_ERROR) << "Can not release a lua state that has never been initialised";
        return 0;   //it's not REALLY an error, but a performance issue if called too frequently
    }

    lua_close(_luaState);
    _luaState = nullptr;

    return 0;
}

}