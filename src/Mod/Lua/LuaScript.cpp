/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "LuaApi.h"
#include "LuaScript.h"
#include "../../Engine/Logger.h"
#include "../../Engine/FileMap.h"
#include "LuaGame.h"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {
LuaScript::LuaScript(const FileMap::FileModInfo &modInfo, const std::string &file_relative_path)
        :
        _modInfo(modInfo),
        _file_relative_path(file_relative_path),
        _luaState(nullptr)
{

}

LuaScript::~LuaScript()
{

}

void LuaScript::loadXcomLuaLib(lua_State* luaState, Game* game)
{
    //get the global state
    lua_pushglobaltable(luaState);

    lua_newtable(luaState); //begin "xcom" table

    //
    LuaGameBindings::loadXcomGameLuaLib(luaState, game);
    LuaGeoscapeBindings::loadXcomGeoscapeLuaLib(luaState);

    lua_setglobal(luaState, "xcom"); //end "xcom" table

    lua_pop(luaState, 1);
}

// Prepare the Lua context and load the script
int LuaScript::Load(Game* game)
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

    loadXcomLuaLib(_luaState, game);

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
int LuaScript::Run()
{
    if(_luaState == nullptr) { return -1; }
    Log(LOG_INFO) << "Running script '" << getFilename() << "'";
    Log(LOG_INFO) << "   mod '" << getModIfo().getId() << "'";
    Log(LOG_INFO) << "   path '" << getModIfo().getPath() << "'";

    lua_pcall(_luaState, 0, 0, 0);

    return 0;
}

// Cleanup and unload the script and safely shut down the Lua context.
int LuaScript::Unload()
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


Game* getGameFromLuaState(lua_State* luaState)
{
    //get the game pointer
    lua_pushglobaltable(luaState);

    lua_pushstring(luaState, "xcom");
    lua_gettable(luaState, -2);

    Game* game = nullptr;
    lua_getfield(luaState, -1, "__self");
    if(lua_isuserdata(luaState, -1))
    {
        game = *static_cast<Game**>(lua_touserdata(luaState, -1));
    }

    lua_pop(luaState, 3);

    return game;
}


}   // end namespace Lua
}   // end namespace OpenXcom