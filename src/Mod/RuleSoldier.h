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
#include <yaml-cpp/yaml.h>
#include "Unit.h"
#include "../Engine/Script.h"

namespace OpenXcom
{

class Mod;
class ModScript;
class SoldierNamePool;
class StatString;

/**
 * Represents the creation data for an X-COM unit.
 * This info is copied to either Soldier for Geoscape or BattleUnit for Battlescape.
 * @sa Soldier BattleUnit
 */
class RuleSoldier
{
public:

	/// Name of class used in script.
	static constexpr const char *ScriptName = "RuleSoldier";
	/// Register all useful function used by script.
	static void ScriptRegister(ScriptParserBase* parser);

private:
	std::string _type;
	int _listOrder;
	std::vector<std::string> _requires;
	std::vector<std::string> _requiresBuyBaseFunc;
	UnitStats _minStats, _maxStats, _statCaps, _trainingStatCaps, _dogfightExperience;
	std::string _armor;
	int _costBuy, _costSalary, _costSalarySquaddie, _costSalarySergeant, _costSalaryCaptain, _costSalaryColonel, _costSalaryCommander;
	int _standHeight, _kneelHeight, _floatHeight;
	int _femaleFrequency, _value;
	std::vector<int> _deathSoundMale, _deathSoundFemale;
	std::vector<SoldierNamePool*> _names;
	std::string _armorForAvatar;
	int _avatarOffsetX, _avatarOffsetY, _flagOffset;
	bool _allowPromotion, _allowPiloting;
	std::vector<StatString*> _statStrings;
	std::vector<std::string> _rankStrings;
	int _rankSprite, _rankSpriteBattlescape, _rankSpriteTiny;
	ScriptValues<RuleSoldier> _scriptValues;

	void addSoldierNamePool(const std::string &namFile);
public:
	/// Creates a blank soldier ruleset.
	RuleSoldier(const std::string &type);
	/// Cleans up the soldier ruleset.
	~RuleSoldier();
	/// Loads the soldier data from YAML.
	void load(const YAML::Node& node, Mod *mod, int listOrder, const ModScript &parsers);
	/// Gets the soldier's type.
	std::string getType() const;
	/// Gets the list/sort order of the soldier's type.
	int getListOrder() const;
	/// Gets the soldier's requirements.
	const std::vector<std::string> &getRequirements() const;
	/// Gets the base functions required to buy solder.
	const std::vector<std::string> &getRequiresBuyBaseFunc() const;
	/// Gets the minimum stats for the random stats generator.
	UnitStats getMinStats() const;
	/// Gets the maximum stats for the random stats generator.
	UnitStats getMaxStats() const;
	/// Gets the stat caps.
	UnitStats getStatCaps() const;
	/// Gets the training stat caps.
	UnitStats getTrainingStatCaps() const;
	/// Gets the improvement chances for pilots (after dogfight).
	UnitStats getDogfightExperience() const;
	/// Gets the cost of the soldier.
	int getBuyCost() const;
	/// Does salary depend on rank?
	bool isSalaryDynamic() const;
	/// Gets the monthly salary of the soldier (for a given rank).
	int getSalaryCost(int rank) const;
	/// Gets the height of the soldier when it's standing.
	int getStandHeight() const;
	/// Gets the height of the soldier when it's kneeling.
	int getKneelHeight() const;
	/// Gets the elevation of the soldier when it's flying.
	int getFloatHeight() const;
	/// Gets the default-equipped armor.
	std::string getArmor() const;
	/// Gets the armor for avatar display.
	std::string getArmorForAvatar() const;
	/// Gets the X offset used for avatar.
	int getAvatarOffsetX() const;
	/// Gets the Y offset used for avatar.
	int getAvatarOffsetY() const;
	/// Gets the flag offset.
	int getFlagOffset() const;
	/// Gets the allow promotion flag.
	bool getAllowPromotion() const;
	/// Gets the allow piloting flag.
	bool getAllowPiloting() const;
	/// Gets the female appearance ratio.
	int getFemaleFrequency() const;
	/// Gets the soldier's male death sounds.
	const std::vector<int> &getMaleDeathSounds() const;
	/// Gets the soldier's female death sounds.
	const std::vector<int> &getFemaleDeathSounds() const;
	/// Gets the pool list for soldier names.
	const std::vector<SoldierNamePool*> &getNames() const;
	/// Gets the value - for score calculation.
	int getValue() const;
	/// Gets the list of StatStrings.
	const std::vector<StatString *> &getStatStrings() const;
	/// Gets the list of strings for ranks.
	const std::vector<std::string> &getRankStrings() const;
	/// Gets the offset of the rank sprite in BASEBITS.PCK.
	int getRankSprite() const;
	/// Gets the offset of the rank sprite in SMOKE.PCK.
	int getRankSpriteBattlescape() const;
	/// Gets the offset of the rank sprite in TinyRanks.
	int getRankSpriteTiny() const;
};

}
