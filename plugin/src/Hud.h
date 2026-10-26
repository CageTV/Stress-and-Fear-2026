/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// The stress bar: one thin horizontal bar, centred above the compass, that fills with your stress and changes colour with its band (calm, minor,
// moderate, high, severe). It has the same adjustable controls as the HUD bars of Frostfall 2026 (display mode, position, scale, length, thickness,
// opacity) and is drawn by SKSE Menu Framework. Its settings are kept in Data/SKSE/Plugins/StressAndFear/hud.ini.
namespace Hud
{
	struct Config
	{
		bool  enabled = true;
		int   displayMode = 1;           // 0 = always, 1 = contextual (fades out while you are calm and nothing changed)
		float contextualSeconds = 6.0f;  // how long it stays after your stress changes
		float posX = 0.50f;              // horizontal centre of the bar, fraction of screen width
		float posY = 0.925f;             // vertical centre of the bar, fraction of screen height (just above the vanilla compass)
		float scale = 1.0f;
		float opacity = 0.90f;
		float barLength = 300.0f;        // pixels at 1080p, before scale
		float barThickness = 5.0f;
		bool  showLabel = true;          // the word "Stress" above the left end
		bool  showValue = true;          // the number above the right end
		bool  showTicks = true;          // thin marks at the band edges (25, 45, 65, 85)
		bool  pulse = true;              // a slow pulse in the severe band
	};

	Config& Cfg();
	void    Load();
	void    Save();
	void    ResetLayout();   // position, scale, length, thickness and opacity back to the defaults
	void    MarkDirty();     // the file is written a moment after the last change
	void    MarkPreview();   // keep the bar visible while its settings page is open
	void    Render();        // every frame, from the menu framework's HUD callback
	const char* FilePath();
}
