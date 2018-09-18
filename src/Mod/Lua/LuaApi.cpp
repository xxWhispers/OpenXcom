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
#include "../../Engine/Logger.h"
#include <string>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {


void loadXcomLuaLib(lua_State *luaState, Game* game)
{
    //get the global state
    lua_pushglobaltable(luaState);

    lua_newtable(luaState); //begin "xcom" table

    loadXcomGameLuaLib(luaState, game);
    loadXcomGeoscapeLuaLib(luaState);

    lua_setglobal(luaState, "xcom"); //end "xcom" table

    lua_pop(luaState, 1);
}

} //namespace Lua
} //namespace OpenXcom