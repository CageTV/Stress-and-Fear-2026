/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Menu.h"
#include "Phobias.h"
#include "Settings.h"

#include "SKSEMenuFramework.h"

namespace Menu
{
	namespace
	{
		using namespace ImGuiMCP;

		bool registered = false;

		void Tip(const char* a_text)
		{
			if (IsItemHovered()) {
				BeginTooltip();
				PushTextWrapPos(420.0f);
				TextUnformatted(a_text);
				PopTextWrapPos();
				EndTooltip();
			}
		}

		bool Guard()
		{
			if (!Settings::Loaded() || !Settings::Available()) {
				TextColored(ImVec4(1.0f, 0.45f, 0.4f, 1.0f), "Stress and Fear.esp is not loaded.");
				return false;
			}
			return true;
		}

		// The same bands the original mod uses for its messages.
		const char* Band(float a_stress)
		{
			if (a_stress > 85.0f) {
				return "Severe";
			}
			if (a_stress > 65.0f) {
				return "High";
			}
			if (a_stress > 45.0f) {
				return "Moderate";
			}
			if (a_stress > 25.0f) {
				return "Minor";
			}
			return "Calm";
		}

		void __stdcall RenderOverview()
		{
			if (!Guard()) {
				return;
			}
			TextColored(ImVec4(0.65f, 0.9f, 0.5f, 1.0f), "Stress and Fear 2026 is active.");
			Spacing();
			TextWrapped("Getting hit raises your stress, rest and comfort lower it, and a close call against a kind of enemy can leave you afraid of it until you win against "
						"it. These pages replace the mod's own menu and add a live view of your stress and fears.");
			Spacing();
			SeparatorText("Stress");
			const float stress = Settings::Stress();
			Text("%.0f / 100  (%s)", stress, Band(stress));
			ProgressBar(stress / 100.0f, ImVec2(-1.0f, 0.0f));
			if (!Settings::StressEnabled()) {
				TextDisabled("The stress system is switched off.");
			}
			Spacing();
			SeparatorText("Fears");
			if (!Settings::FearEnabled()) {
				TextDisabled("The fear system is switched off.");
			}
			Text("%d afraid, %d overcome, out of %d kinds of enemy", Phobias::AfraidCount(), Phobias::OvercomeCount(), Phobias::Count());
			Spacing();
			TextDisabled("Stress and Fear by JaySerpa. Settings pages by CageTV.");
		}

		void __stdcall RenderSettings()
		{
			if (!Guard()) {
				return;
			}
			SeparatorText("Stress");
			bool stress = Settings::StressEnabled();
			if (Checkbox("Enable Stress System", &stress)) {
				Settings::SetStressEnabled(stress);
			}
			Tip("Turns the stress system and its effects off. Turning it off also sets your stress back to 0.");
			float rate = Settings::Rate();
			if (SliderFloat("Stress Rate", &rate, 0.1f, 4.0f, "%.2f")) {
				Settings::SetRate(rate);
			}
			Tip("How much stress each hit adds. The original menu offers 0.25, 0.5, 1 (default), 1.5 and 2.");
			Text("Presets:");
			for (int i = 0; i < static_cast<int>(std::size(Settings::kRates)); ++i) {
				SameLine();
				if (Button(Settings::kRateNames[i])) {
					Settings::SetRate(Settings::kRates[i]);
				}
			}

			SeparatorText("Fear");
			bool fear = Settings::FearEnabled();
			if (Checkbox("Enable Fear System", &fear)) {
				Settings::SetFearEnabled(fear);
			}
			Tip("Turns the fears (phobias) off. A fear you already have stays on your character until you cure it on the Fears page.");

			SeparatorText("Messages");
			bool box = Settings::BoxMessages();
			if (Checkbox("Show Message Box", &box)) {
				Settings::SetBoxMessages(box);
			}
			Tip("On: important notices appear as a message box in the middle of the screen. Off: a small notification in the corner instead.");

			SeparatorText("Your stress now (a cheat, not saved)");
			float now = Settings::Stress();
			if (SliderFloat("Current Stress", &now, 0.0f, 100.0f, "%.0f / 100")) {
				Settings::SetStress(now);
			}
			Tip("Changes your stress level directly. It is progress, not a setting, so it is never written to the settings file.");
		}

		const char* StateText(Phobias::State a_s)
		{
			switch (a_s) {
			case Phobias::State::Afraid:
				return "Afraid";
			case Phobias::State::Overcome:
				return "Overcome";
			default:
				return "-";
			}
		}

		void __stdcall RenderFears()
		{
			if (!Guard()) {
				return;
			}
			TextWrapped("Your fears and the ones you have overcome. Fears come and go in play; the buttons are for fixing a save or testing. Cure removes a fear (it can come back "
						"later), Overcome gives the permanent result of beating it, Reset clears everything about that enemy.");
			Spacing();
			if (BeginTable("fears", 3)) {
				for (int i = 0; i < Phobias::Count(); ++i) {
					const auto state = Phobias::StateOf(i);
					PushID(i);
					TableNextColumn();
					Text("%s", Phobias::Name(i));
					TableNextColumn();
					if (state == Phobias::State::Afraid) {
						TextColored(ImVec4(1.0f, 0.55f, 0.4f, 1.0f), "%s", StateText(state));
					} else if (state == Phobias::State::Overcome) {
						TextColored(ImVec4(0.65f, 0.9f, 0.5f, 1.0f), "%s", StateText(state));
					} else {
						TextDisabled("%s", StateText(state));
					}
					TableNextColumn();
					if (state == Phobias::State::None) {
						if (Button("Give fear")) {
							Phobias::Give(i);
						}
					} else if (state == Phobias::State::Afraid) {
						if (Button("Cure")) {
							Phobias::Cure(i);
						}
						SameLine();
						if (Button("Overcome")) {
							Phobias::Overcome(i);
						}
					} else {
						if (Button("Reset")) {
							Phobias::Reset(i);
						}
					}
					PopID();
				}
				EndTable();
			}
		}

		void __stdcall RenderProfile()
		{
			static float confirmTimer = 0.0f;
			confirmTimer -= GetIO()->DeltaTime;
			SeparatorText("Settings file");
			bool apply = Settings::ApplyOnLoad();
			if (Checkbox("Apply the saved settings every time a game loads", &apply)) {
				Settings::SetApplyOnLoad(apply);
			}
			Tip("On: your settings follow you to new characters and old saves. Off: each save keeps the values it has, and the file is only written.");
			TextDisabled("%s", Settings::FilePath());
			Spacing();
			if (Button("Save the current values now")) {
				Settings::SaveNow();
			}
			SameLine();
			if (Button("Load the file now")) {
				Settings::LoadNow();
			}
			Spacing();
			SeparatorText("Defaults");
			if (confirmTimer > 0.0f) {
				TextWrapped("Reset every setting to what the mod ships with? Your stress and your fears are left alone.");
				if (Button("Yes, reset")) {
					Settings::ResetDefaults();
					confirmTimer = 0.0f;
				}
				SameLine();
				if (Button("Cancel")) {
					confirmTimer = 0.0f;
				}
			} else if (Button("Reset all settings to default")) {
				confirmTimer = 10.0f;
			}
		}

		// Drawn every frame whether or not the menu is open: writes the settings file a moment after the last change.
		void __stdcall Frame() { Settings::Tick(GetIO()->DeltaTime); }
	}

	bool Registered() { return registered; }

	void Register()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			SKSE::log::warn("SKSE Menu Framework is not installed: the Stress and Fear settings pages are not available");
			return;
		}
		SKSEMenuFramework::SetSection("Stress and Fear");
		SKSEMenuFramework::AddSectionItem("Overview", RenderOverview);
		SKSEMenuFramework::AddSectionItem("Settings", RenderSettings);
		SKSEMenuFramework::AddSectionItem("Fears", RenderFears);
		SKSEMenuFramework::AddSectionItem("Profile", RenderProfile);
		SKSEMenuFramework::AddHudElement(Frame);
		registered = true;
		SKSE::log::info("Registered with SKSE Menu Framework {}", SKSEMenuFramework::GetMenuFrameworkVersion());
	}
}
