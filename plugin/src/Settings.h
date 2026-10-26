/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// The settings of Stress and Fear 2026. Each one is a global variable of Stress and Fear itself (the same ones its MCM writes), so the pages
// and the original mod always agree. The values are also kept in Data/SKSE/Plugins/StressAndFear/settings.ini so they follow you to a new
// character. Your current stress level is progress, not a setting: it is shown and can be changed, but never saved.
namespace Settings
{
	inline constexpr float kRates[] = { 0.25f, 0.5f, 1.0f, 1.5f, 2.0f };  // the five steps of the original "Stress Rate" menu
	inline constexpr const char* kRateNames[] = { "Very Slow", "Slow", "Default", "Fast", "Very Fast" };

	bool Loaded();     // Stress and Fear.esp is in the load order
	bool Available();  // and its globals were found

	bool  StressEnabled();
	void  SetStressEnabled(bool a_on);  // switching it off also resets your stress to 0, as the original menu does
	bool  FearEnabled();
	void  SetFearEnabled(bool a_on);
	bool  BoxMessages();
	void  SetBoxMessages(bool a_on);
	float Rate();
	void  SetRate(float a_rate);

	float Stress();  // 0 to 100
	void  SetStress(float a_value);

	bool ApplyOnLoad();
	void SetApplyOnLoad(bool a_on);

	void OnDataLoaded();  // reads settings.ini
	void OnGameLoaded();  // applies settings.ini to the globals (or writes the file from the globals the first time)
	void SaveNow();
	void LoadNow();
	void ResetDefaults();
	void Tick(float a_dt);  // writes the file a moment after the last change
	const char* FilePath();
}
