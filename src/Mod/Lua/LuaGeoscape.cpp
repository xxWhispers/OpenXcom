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

#include "LuaGeoscape.h"
#include "LuaApi.h"
#include "LuaScript.h"
#include "../Mod.h"
#include "../../Engine/Game.h"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {

static int lua_geoscapeRegister5SecCallback(lua_State* luaState)
{
    return 0;
}

static int lua_geoscapeRegister10MinCallback(lua_State* luaState)
{
    return 0;
}

static int lua_geoscapeRegister1HourCallback(lua_State* luaState)
{
    Game* game = getGameFromLuaState(luaState);
    LuaApi* api = game->getMod()->getLuaApi();

    LuaScript* currentScript = api->getCurrentScript();
    api->getGeoscapeBindings().register1HourCallback(currentScript, luaState);
    return 0;
}

/// Given a lua context, insert the "xcom.geoscape" lua API bindings
void LuaGeoscapeBindings::loadXcomGeoscapeLuaLib(lua_State* luaState)
{
    lua_pushstring(luaState, "geoscape");
    lua_newtable(luaState); //begin "geoscape" table

    //define the geoscape register hooks

    lua_pushstring(luaState, "registerOn5SecCallback");
    lua_pushcfunction(luaState, lua_geoscapeRegister5SecCallback);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "registerOn10MinCallback");
    lua_pushcfunction(luaState, lua_geoscapeRegister10MinCallback);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "registerOn1HourCallback");
    lua_pushcfunction(luaState, lua_geoscapeRegister1HourCallback);
    lua_settable(luaState, -3);

    lua_settable(luaState, -3); //end "geoscape" table
}

void LuaGeoscapeBindings::register1HourCallback(LuaScript* script, lua_State* luaState)
{
    int ref = luaL_ref(luaState, LUA_REGISTRYINDEX);
    std::pair<LuaScript*, int> scriptCallback(script, ref);
    _1hourCallbacks.push_back(scriptCallback);
}

void LuaGeoscapeBindings::execute1HourCallbacks()
{
    for(std::vector<std::pair<LuaScript*, int>>::iterator i = _1hourCallbacks.begin(); i!= _1hourCallbacks.end(); ++i)
    {
        LuaScript* script = (*i).first;
        int ref = (*i).second;

        lua_State* luaState = script->getLuaState();
        Game* game = getGameFromLuaState(luaState); //TODO: maybe pass Game as a parameter?
        LuaApi* api = game->getMod()->getLuaApi();

        api->setCurrentScript(script);

        lua_rawgeti(luaState, LUA_REGISTRYINDEX, ref);
        lua_pcall(luaState,0,0,0);

        api->setCurrentScript(0);

    }
}

} //namespace Lua
} //namespace OpenXcom