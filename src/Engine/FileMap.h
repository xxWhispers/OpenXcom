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
#include <set>
#include <string>
#include <vector>

namespace OpenXcom
{

/**
 * Maps canonical names to file paths and maintains the virtual file system
 * for resource files.
 */
namespace FileMap
{

	/// Helper class to map mods to file locations
	class FileModInfo
	{
	protected:
		std::string _id;
		std::string _path;

		std::vector<std::string> _ruleFiles;
	public:
		FileModInfo(std::string id, std::string path) : _id(id), _path(path) { }
		~FileModInfo() { }

		const std::string& getId() const { return _id; }
		const std::string& getPath() const { return _path; }

		void addRuleFile(std::string filename) { _ruleFiles.push_back(filename); }

		const std::vector<std::string>& getRuleFiles() const { return _ruleFiles; }
	};


	/// Gets the real filesystem path for a data file when given a relative (case insensitive) path,
	/// like "units/zombie.pck".  If the file has not been mapped via the load() function, the input
	/// path is returned verbatim (for use in error messages when the file is ultimately not found).
	const std::string &getFilePath(const std::string &relativeFilePath);

	/// Returns the set of files in a virtual folder.  The virtual folder contains files from all active mods
	/// that are in similarly-named subdirectories.  The returned file names can then be translated to real
	/// filesystem paths via getFilePath()
	const std::set<std::string> &getVFolderContents(const std::string &relativePath);

	/// Returns the subset of the given files that matches the given extension
	std::set<std::string> filterFiles(const std::vector<std::string> &files, const std::string &ext);
	std::set<std::string> filterFiles(const std::set<std::string>    &files, const std::string &ext);

	/// Returns the ruleset files found, grouped by mod, while mapping resources.  The highest-priority mod
	/// will be last in the returned vector.
	const std::vector<FileModInfo> &getRulesets();

	/// clears FileMap state
	void clear();

	/// Scans a directory tree rooted at the specified filesystem path.  Any files it encounters that have already
	/// been mapped will be ignored.  Therefore, load files from mods with the highest priority first.  If
	/// ignoreMods is false, it will add any rulesets it finds to the front of the vector
	/// returned by getMods().
	void load(const std::string &modId, const std::string &path, bool ignoreMods);

	/// Determines if _resources set is empty
	bool isResourcesEmpty(void);
}

}
