/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Hud.h"
#include "Settings.h"

#include "SKSEMenuFramework.h"

namespace Hud
{
	namespace
	{
		using namespace ImGuiMCP;

		constexpr const char* kFile = "Data/SKSE/Plugins/StressAndFear/hud.ini";

		Config cfg;
		float  saveTimer = -1.0f;
		int    previewFrames = 0;

		float shown = -1.0f;        // animated fill (0..1)
		float lastRaw = -1.0f;      // last stress value, to notice a change
		float sinceChange = 1e6f;   // seconds since the stress last changed
		float groupAlpha = 1.0f;

		struct Rgb
		{
			float r, g, b;
		};
		Rgb Lerp(Rgb a, Rgb b, float t) { return { a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t }; }

		ImU32 Col(float r, float g, float b, float a)
		{
			return IM_COL32(static_cast<int>(std::clamp(r, 0.0f, 255.0f)), static_cast<int>(std::clamp(g, 0.0f, 255.0f)),
				static_cast<int>(std::clamp(b, 0.0f, 255.0f)), static_cast<int>(std::clamp(a, 0.0f, 1.0f) * 255.0f));
		}

		// The colour along the bar: calm green, then yellow-green, amber, orange and red, blending between the band centres.
		Rgb StressColor(float a_stress)
		{
			constexpr Rgb calm{ 105, 200, 130 }, minor{ 190, 205, 105 }, moderate{ 235, 185, 85 }, high{ 235, 130, 80 }, severe{ 225, 75, 70 };
			if (a_stress <= 15.0f) {
				return calm;
			}
			if (a_stress <= 35.0f) {
				return Lerp(calm, minor, (a_stress - 15.0f) / 20.0f);
			}
			if (a_stress <= 55.0f) {
				return Lerp(minor, moderate, (a_stress - 35.0f) / 20.0f);
			}
			if (a_stress <= 75.0f) {
				return Lerp(moderate, high, (a_stress - 55.0f) / 20.0f);
			}
			if (a_stress <= 92.0f) {
				return Lerp(high, severe, (a_stress - 75.0f) / 17.0f);
			}
			return severe;
		}

		std::string Trim(const std::string& a_s)
		{
			const auto b = a_s.find_first_not_of(" \t\r\n");
			const auto e = a_s.find_last_not_of(" \t\r\n");
			return b == std::string::npos ? std::string() : a_s.substr(b, e - b + 1);
		}

		bool HudHiddenByGame()
		{
			auto* ui = RE::UI::GetSingleton();
			if (!ui) {
				return true;
			}
			if (ui->GameIsPaused() || ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) || ui->IsMenuOpen(RE::MainMenu::MENU_NAME)) {
				return true;
			}
			return !ui->IsShowingMenus();  // the HUD was toggled off (tm)
		}

		bool GameRunning()
		{
			auto* player = RE::PlayerCharacter::GetSingleton();
			return player && player->Is3DLoaded() && Settings::Available();
		}

		void DrawLabel(ImDrawList* a_dl, const char* a_text, float a_x, float a_y, float a_size, float a_alpha, bool a_alignRight)
		{
			const float factor = a_size / std::max(GetFontSize(), 1.0f);
			const float width = CalcTextSize(a_text).x * factor;
			const float x = a_alignRight ? a_x - width : a_x;
			ImDrawListManager::AddText(a_dl, GetFont(), a_size, ImVec2(x + 1.0f, a_y + 1.0f), Col(0, 0, 0, 0.7f * a_alpha), a_text);
			ImDrawListManager::AddText(a_dl, GetFont(), a_size, ImVec2(x, a_y), Col(228, 232, 238, 0.95f * a_alpha), a_text);
		}

		void DrawBar(ImDrawList* a_dl, float a_dt, bool a_preview)
		{
			const auto* io = GetIO();
			const float W = io->DisplaySize.x;
			const float H = io->DisplaySize.y;
			const float k = (H / 1080.0f) * cfg.scale;

			const float stress = std::clamp(Settings::Stress(), 0.0f, 100.0f);
			const float frac = stress / 100.0f;
			if (shown < 0.0f) {
				shown = frac;
				lastRaw = stress;
			}
			if (std::abs(stress - lastRaw) > 0.05f) {
				sinceChange = 0.0f;
				lastRaw = stress;
			} else {
				sinceChange += a_dt;
			}
			shown += (frac - shown) * std::min(1.0f, a_dt * 6.0f);

			// Contextual: stay visible while your stress just changed or you are not calm.
			float target = 1.0f;
			if (cfg.displayMode == 1 && !a_preview) {
				target = (sinceChange < cfg.contextualSeconds || stress > 25.0f) ? 1.0f : 0.0f;
			}
			groupAlpha += (target - groupAlpha) * std::min(1.0f, a_dt * 3.0f);
			float alpha = groupAlpha * cfg.opacity;
			if (cfg.pulse && stress > 85.0f) {
				alpha *= 0.78f + 0.22f * std::sin(static_cast<float>(GetTime()) * 4.0f);
			}
			if (alpha < 0.01f) {
				return;
			}

			const float L = cfg.barLength * k;
			const float T = std::max(2.0f, cfg.barThickness * k);
			const float x = cfg.posX * W - L * 0.5f;
			const float y = cfg.posY * H - T * 0.5f;
			const float rounding = std::min(T * 0.5f, 3.0f);

			// track
			ImDrawListManager::AddRectFilled(a_dl, ImVec2(x, y), ImVec2(x + L, y + T), Col(10, 12, 16, 0.55f * alpha), rounding, 0);

			// fill: lighter on top, deeper underneath
			const float fw = L * std::clamp(shown, 0.0f, 1.0f);
			if (fw >= 1.0f) {
				const Rgb   c = StressColor(shown * 100.0f);
				const ImU32 cTop = Col(c.r + 35, c.g + 35, c.b + 35, 0.95f * alpha);
				const ImU32 cBot = Col(c.r * 0.8f, c.g * 0.8f, c.b * 0.8f, 0.95f * alpha);
				ImDrawListManager::AddRectFilledMultiColor(a_dl, ImVec2(x, y), ImVec2(x + fw, y + T), cTop, cTop, cBot, cBot);
			}

			// band edges
			if (cfg.showTicks) {
				for (const float edge : { 25.0f, 45.0f, 65.0f, 85.0f }) {
					const float tx = x + L * (edge / 100.0f);
					ImDrawListManager::AddLine(a_dl, ImVec2(tx, y), ImVec2(tx, y + T), Col(0, 0, 0, 0.45f * alpha), std::max(1.0f, k));
				}
			}

			// label and number above the ends of the bar
			const float textSize = std::max(9.0f, 12.0f * k);
			const float textY = y - textSize - 2.0f * k;
			if (cfg.showLabel) {
				DrawLabel(a_dl, "Stress", x, textY, textSize, alpha, false);
			}
			if (cfg.showValue) {
				char value[16];
				std::snprintf(value, sizeof(value), "%d", static_cast<int>(stress + 0.5f));
				DrawLabel(a_dl, value, x + L, textY, textSize, alpha, true);
			}
		}
	}

	Config& Cfg() { return cfg; }

	const char* FilePath() { return kFile; }

	void Load()
	{
		std::ifstream in(kFile);
		if (!in) {
			return;
		}
		std::string line;
		while (std::getline(in, line)) {
			if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') {
				continue;
			}
			const auto eq = line.find('=');
			if (eq == std::string::npos) {
				continue;
			}
			const std::string key = Trim(line.substr(0, eq));
			float             f = 0.0f;
			try {
				f = std::stof(Trim(line.substr(eq + 1)));
			} catch (...) {
				continue;
			}
			if (key == "enabled") {
				cfg.enabled = f > 0.5f;
			} else if (key == "display_mode") {
				cfg.displayMode = f > 0.5f ? 1 : 0;
			} else if (key == "contextual_seconds") {
				cfg.contextualSeconds = std::clamp(f, 1.0f, 30.0f);
			} else if (key == "pos_x") {
				cfg.posX = std::clamp(f, 0.0f, 1.0f);
			} else if (key == "pos_y") {
				cfg.posY = std::clamp(f, 0.0f, 1.0f);
			} else if (key == "scale") {
				cfg.scale = std::clamp(f, 0.5f, 3.0f);
			} else if (key == "opacity") {
				cfg.opacity = std::clamp(f, 0.1f, 1.0f);
			} else if (key == "bar_length") {
				cfg.barLength = std::clamp(f, 60.0f, 800.0f);
			} else if (key == "bar_thickness") {
				cfg.barThickness = std::clamp(f, 2.0f, 30.0f);
			} else if (key == "show_label") {
				cfg.showLabel = f > 0.5f;
			} else if (key == "show_value") {
				cfg.showValue = f > 0.5f;
			} else if (key == "show_ticks") {
				cfg.showTicks = f > 0.5f;
			} else if (key == "pulse") {
				cfg.pulse = f > 0.5f;
			}
		}
	}

	void Save()
	{
		std::error_code ec;
		std::filesystem::create_directories(std::filesystem::path(kFile).parent_path(), ec);
		std::ofstream out(kFile, std::ios::trunc);
		if (!out) {
			SKSE::log::warn("Cannot write {}", kFile);
			return;
		}
		out << "; Stress and Fear 2026 stress bar. Written by the in-game HUD page; safe to edit by hand while the game is closed.\n"
			<< "enabled=" << (cfg.enabled ? 1 : 0) << "\n"
			<< "display_mode=" << cfg.displayMode << "\n"
			<< "contextual_seconds=" << cfg.contextualSeconds << "\n"
			<< "pos_x=" << cfg.posX << "\n"
			<< "pos_y=" << cfg.posY << "\n"
			<< "scale=" << cfg.scale << "\n"
			<< "opacity=" << cfg.opacity << "\n"
			<< "bar_length=" << cfg.barLength << "\n"
			<< "bar_thickness=" << cfg.barThickness << "\n"
			<< "show_label=" << (cfg.showLabel ? 1 : 0) << "\n"
			<< "show_value=" << (cfg.showValue ? 1 : 0) << "\n"
			<< "show_ticks=" << (cfg.showTicks ? 1 : 0) << "\n"
			<< "pulse=" << (cfg.pulse ? 1 : 0) << "\n";
	}

	void ResetLayout()
	{
		const Config d;
		cfg.posX = d.posX;
		cfg.posY = d.posY;
		cfg.scale = d.scale;
		cfg.opacity = d.opacity;
		cfg.barLength = d.barLength;
		cfg.barThickness = d.barThickness;
	}

	void MarkDirty() { saveTimer = 0.6f; }

	void MarkPreview() { previewFrames = 2; }

	void Render()
	{
		const float dt = std::clamp(GetIO()->DeltaTime, 0.0f, 0.1f);

		if (saveTimer >= 0.0f) {
			saveTimer -= dt;
			if (saveTimer < 0.0f) {
				Save();
			}
		}

		const bool preview = previewFrames > 0;
		if (previewFrames > 0) {
			--previewFrames;
		}
		if (!cfg.enabled || !GameRunning() || !Settings::StressEnabled()) {
			return;
		}
		if (!preview && (SKSEMenuFramework::IsAnyBlockingWindowOpened() || HudHiddenByGame())) {
			return;
		}
		DrawBar(GetForegroundDrawList(), dt, preview);
	}
}
