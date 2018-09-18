//
// Created by ken on 18/09/18.
//

#include "LuaApi.h"
#include "../../Engine/Game.h"
#include "../../Engine/State.h"
#include "../../Engine/Logger.h"
#include "../../Savegame/SavedGame.h"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {

Game* getGame(lua_State* luaState)
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


static int lua_gameGetFunds(lua_State* luaState)
{
    Game* game = getGame(luaState);

    int funds = 0;
    if(game != nullptr)
    {
        funds = game->getSavedGame()->getFunds();
    }

    lua_pushnumber(luaState, funds);
    return 1;
}

static int lua_gameSetFunds(lua_State* luaState)
{
    //sanity checks
    if(lua_gettop(luaState) != 1) {
        return luaL_error(luaState, "setFunds(number funds) takes 1 argument.");
    }

    Game* game = getGame(luaState);

    int funds = luaL_checknumber(luaState, 1);
    if(game != nullptr)
    {
        game->getSavedGame()->setFunds(funds);
    }

    return 0;
}

/// Given a lua context, insert the "xcom.game" lua API bindings
void loadXcomGameLuaLib(lua_State* luaState, Game* game)
{
    // __self is located in the root xcom table because it's used frequently and this makes it easier to access
    lua_pushstring(luaState, "__self");
    Game** ppGame = static_cast<Game**>(lua_newuserdata(luaState, sizeof(Game*)));
    *ppGame = game;
    luaL_setmetatable(luaState, "Game");
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "game");
    lua_newtable(luaState); //begin "game" table

    lua_pushstring(luaState, "getFunds");
    lua_pushcfunction(luaState, lua_gameGetFunds);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "setFunds");
    lua_pushcfunction(luaState, lua_gameSetFunds);
    lua_settable(luaState, -3);

    lua_settable(luaState, -3); //end "game" table
}

} //namespace Lua
} //namespace OpenXcom