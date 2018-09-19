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
#include "../../Engine/FileMap.h"
#include "../../Engine/Logger.h"
#include <string>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace OpenXcom {
namespace Lua {

LuaApi::LuaApi()
{

}

LuaApi::~LuaApi()
{
    for(std::vector<LuaScript*>::const_iterator i = _luaScripts.begin(); i != _luaScripts.end(); ++i)
    {
        delete (*i);
    }
}

/// Adds a script to be run
void LuaApi::registerScript(const FileMap::FileModInfo &info, const std::string& file_relative_path)
{
    Lua::LuaScript* newScript;
    newScript = new Lua::LuaScript(info, file_relative_path);
    _luaScripts.push_back(newScript);
}


/// Load all the registered scripts
int LuaApi::loadScripts(Game* game)
{
    for(std::vector<LuaScript*>::const_iterator i = _luaScripts.begin(); i != _luaScripts.end(); ++i)
    {
        LuaScript* script = (*i);

        int ret = script->Load(game);

        if(ret != 0)
        {
            Log(LOG_ERROR) << "Unable to load script '" << script->getFilename() << "'";
            Log(LOG_ERROR) << "   mod '" << script->getModIfo().getId() << "'";
            Log(LOG_ERROR) << "   path '" << script->getModIfo().getPath() << "'";
            continue;
        }
    }
    return 0;
}

/// Run all the loaded scripts
int LuaApi::runScripts()
{
    for(std::vector<LuaScript*>::const_iterator i = _luaScripts.begin(); i != _luaScripts.end(); ++i)
    {
        _currentScript = (*i);

        int ret = _currentScript->Run();

        if(ret != 0)
        {
            Log(LOG_ERROR) << "Unable to run script '" << _currentScript->getFilename() << "'";
            Log(LOG_ERROR) << "   mod '" << _currentScript->getModIfo().getId() << "'";
            Log(LOG_ERROR) << "   path '" << _currentScript->getModIfo().getPath() << "'";
            continue;
        }

        _currentScript = nullptr;
    }
    return 0;
}

/// Unload all the loaded scripts
int LuaApi::unloadScripts()
{
    for(std::vector<LuaScript*>::const_iterator i = _luaScripts.begin(); i != _luaScripts.end(); ++i)
    {
        LuaScript *script = (*i);

        int ret = script->Unload();
        if (ret != 0)
        {
            Log(LOG_ERROR) << "Problem unloading script '" << script->getFilename() << "'";
            Log(LOG_ERROR) << "   mod '" << script->getModIfo().getId() << "'";
            Log(LOG_ERROR) << "   path '" << script->getModIfo().getPath() << "'";
            continue;
        }
    }
    return 0;
}


} //namespace Lua
} //namespace OpenXcom