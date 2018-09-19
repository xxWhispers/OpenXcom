#pragma once
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

#include <string>
#include "LuaGeoscape.h"

// predeclaration
typedef struct lua_State lua_State;

namespace OpenXcom
{

class Game;
namespace FileMap {
    class FileModInfo;
}

namespace Lua
{

class LuaScript
{
protected:
    const FileMap::FileModInfo& _modInfo;
    const std::string _file_relative_path;

    lua_State* _luaState;

    /// Helpers
    /// Given a lua context, insert the "xcom" lua API bindings
    void loadXcomLuaLib(lua_State* luaState, Game* game);

public:
    LuaScript(const FileMap::FileModInfo& modInfo, const std::string& file_relative_path);
    ~LuaScript();

    // Prepare the Lua context and load the script.
    int Load(Game* game);
    // Run the script
    int Run();
    // Cleanup and unload the script and safely shut down the Lua context.
    int Unload();

    const std::string& getFilename() const { return _file_relative_path; }
    const FileMap::FileModInfo& getModIfo() const { return _modInfo; }

    inline lua_State* getLuaState() const { return _luaState; }
};

/// helper function to load the game object from the lua state
Game* getGameFromLuaState(lua_State* luaState);


}   // end namespace Lua
}   // end namespace OpenXcom
