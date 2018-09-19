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

#include "LuaGame.h"
#include "LuaApi.h"
#include "LuaScript.h"
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

static int lua_gameGetFunds(lua_State* luaState)
{
    Game* game = getGameFromLuaState(luaState);

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

    Game* game = getGameFromLuaState(luaState);

    int funds = luaL_checknumber(luaState, 1);
    if(game != nullptr)
    {
        game->getSavedGame()->setFunds(funds);
    }

    return 0;
}

/// Given a lua context, insert the "xcom.game" lua API bindings
void LuaGameBindings::loadXcomGameLuaLib(lua_State* luaState, Game* game)
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