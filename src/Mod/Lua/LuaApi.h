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

#include <vector>
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
class LuaScript;

class LuaApi
{
protected:
    std::vector<LuaScript*> _luaScripts;
    LuaScript* _currentScript;

    LuaGeoscapeBindings _geoscapeBindings;

public:
    LuaApi();
    ~LuaApi();

    /// Adds a script to be run
    void registerScript(const FileMap::FileModInfo &info, const std::string& file_relative_path);

    /// Load all the registered scripts
    int loadScripts(Game* game);
    /// Run all the loaded scripts
    int runScripts();
    /// Unload all the loaded scripts
    int unloadScripts();

    /// Get the geoscape bindings
    LuaGeoscapeBindings& getGeoscapeBindings() { return _geoscapeBindings; }

    /// get the current executing script(TODO: maybe a restrict protected friends classes because not sure everyone needs to know)
    inline LuaScript* getCurrentScript() const { return _currentScript; }
    inline void setCurrentScript(LuaScript* script) { _currentScript = script; }
};

}
}

