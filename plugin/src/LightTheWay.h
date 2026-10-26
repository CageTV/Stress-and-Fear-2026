/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// Light the Way (by Blackspine18859) adds gradual stress while you adventure: every interval, in a dangerous place, you gain a little stress unless one
// of several protections succeeds. Its script and its MCM are replaced here by a native loop and settings pages. The original plugin stays loaded as the
// data file: every value below is one of its own global variables and every location list is one of its own form lists, so an existing save and its
// settings carry over unchanged. Its alias script is silenced by clearing its master switch (LTW_Enabled); "Hand control back" restores it.
namespace LightTheWay
{
	enum class Kind { Toggle, Slider };

	struct Def
	{
		const char* id;     // key in lighttheway.ini
		const char* label;
		const char* tip;
		Kind        kind;
		unsigned    local;  // local FormID of the global in Light the Way.esp (light plugin)
		float       def, min, max, step;
		const char* fmt;    // slider display format
		int         group;  // see the Group* constants
	};

	inline constexpr int kGroupGeneral = 0;
	inline constexpr int kGroupLocations = 1;
	inline constexpr int kGroupProtections = 2;
	inline constexpr int kGroupFollowers = 3;
	inline constexpr int kGroupPowers = 4;

	extern const Def kDefs[];
	extern const int kDefCount;

	enum class ListKind { InteriorInclude, InteriorExclude, ExteriorInclude, ExteriorExclude, ForcedInterior };

	struct Entry
	{
		std::string  name;
		unsigned int formID;
		bool         active;
	};

	bool Present();      // Light the Way.esp and Stress and Fear.esp are both loaded
	bool Takeover();     // this plugin runs Light the Way (its own script is silenced)
	void SetTakeover(bool a_on);
	bool Master();       // Light the Way is switched on
	void SetMaster(bool a_on);

	float Get(const Def& a_d);
	void  Set(const Def& a_d, float a_value);

	int         KeywordCount();
	const char* KeywordName(int a_i);
	bool        KeywordEnabled(int a_i);
	void        SetKeywordEnabled(int a_i, bool a_on);

	std::vector<Entry> Entries(ListKind a_kind);
	void               SetEntryActive(ListKind a_kind, unsigned int a_formID, bool a_on);
	void               MarkCurrent();           // the "mark location" power: mark the place you stand in as stressful, or exclude it, or undo that
	void               ForceCurrentInterior();  // the "force interior" power

	std::string CurrentLocationText();  // name of where you are and whether it qualifies, for the pages
	bool        Qualifying();

	void OnDataLoaded();
	void OnGameLoaded();
	void Tick(float a_dt);  // every frame, from the menu framework's HUD callback
	void SaveNow();
	void LoadNow();
	void ResetDefaults();
	const char* FilePath();
}
