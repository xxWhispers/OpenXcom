//
// Created by ken on 18/09/18.
//

#include "LuaApi.h"
#include "../../Geoscape/GeoscapeState.h"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {

GeoscapeState* getGeoscape(lua_State* luaState)
{
    Game* game = getGame(luaState);
    if(game != nullptr)
    {
        //walk up the state stack to get the topmost state. That's where you'll find the goescape state
    }
}

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
    return 0;
}

/// Given a lua context, insert the "xcom.geoscape" lua API bindings
void loadXcomGeoscapeLuaLib(lua_State* luaState)
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

} //namespace Lua
} //namespace OpenXcom