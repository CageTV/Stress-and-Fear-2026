/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Hud.h"
#include "LightTheWay.h"
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

		constexpr const char* kLogoPath = "Data\\Interface\\StressandFear\\stressandfear_logo.png";
		constexpr float       kLogoAspect = 0.427f;

		void Logo()
		{
			static ImTextureID tex = SKSEMenuFramework::LoadTexture(kLogoPath);
			if (!tex) {
				return;
			}
			const float avail = GetContentRegionAvail().x;
			const float w = std::min(avail, 460.0f);
			SetCursorPosX(GetCursorPosX() + (avail - w) * 0.5f);
			Image(tex, ImVec2(w, w * kLogoAspect));
			Spacing();
		}

		void __stdcall RenderOverview()
		{
			Logo();
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

		// ---- Light the Way ------------------------------------------------------------------------------------------------------------
		bool LtwGuard()
		{
			if (!Settings::Loaded() || !LightTheWay::Present()) {
				TextWrapped("Light the Way is not installed (Light the Way.esp is not loaded), so there is nothing to show here. Light the Way is an add-on of Stress and "
							"Fear that adds stress while you explore dangerous places.");
				return false;
			}
			return true;
		}

		void DrawLtwDef(const LightTheWay::Def& a_d, int a_id)
		{
			PushID(a_id);
			if (a_d.kind == LightTheWay::Kind::Toggle) {
				bool on = LightTheWay::Get(a_d) > 0.5f;
				if (Checkbox(a_d.label, &on)) {
					LightTheWay::Set(a_d, on ? 1.0f : 0.0f);
				}
			} else {
				float v = LightTheWay::Get(a_d);
				if (SliderFloat(a_d.label, &v, a_d.min, a_d.max, a_d.fmt)) {
					LightTheWay::Set(a_d, std::round(v / a_d.step) * a_d.step);
				}
			}
			Tip(a_d.tip);
			PopID();
		}

		void DrawLtwGroup(int a_group, const char* a_title)
		{
			SeparatorText(a_title);
			for (int i = 0; i < LightTheWay::kDefCount; ++i) {
				if (LightTheWay::kDefs[i].group == a_group) {
					DrawLtwDef(LightTheWay::kDefs[i], i);
				}
			}
		}

		void __stdcall RenderLtw()
		{
			if (!LtwGuard()) {
				return;
			}
			SeparatorText("Light the Way");
			TextWrapped("While you explore stressful places, Light the Way adds a little stress every interval unless a protection stops it (a torch, a lit fire, "
						"followers, sneaking and more; see the Protections page). These pages replace its MCM and its script.");
			bool master = LightTheWay::Master();
			if (Checkbox("Enable Light the Way", &master)) {
				LightTheWay::SetMaster(master);
			}
			Tip("The master switch of Light the Way.");
			bool takeover = LightTheWay::Takeover();
			if (Checkbox("Run Light the Way from this plugin", &takeover)) {
				LightTheWay::SetTakeover(takeover);
			}
			Tip("On: this plugin does the work and the original script is silenced (its master switch is held at 0). Off: the original script resumes at your next location change "
				"and its MCM works again. Your settings stay the same either way.");
			TextDisabled("Where you are: %s", LightTheWay::CurrentLocationText().c_str());
			DrawLtwGroup(LightTheWay::kGroupGeneral, "Stress");
			DrawLtwGroup(LightTheWay::kGroupLocations, "Where stress builds");
			DrawLtwGroup(LightTheWay::kGroupPowers, "Powers");
			SeparatorText("Defaults");
			static float confirm = 0.0f;
			confirm -= GetIO()->DeltaTime;
			if (confirm > 0.0f) {
				TextWrapped("Reset every Light the Way setting to what the mod ships with and turn all location types back on? Your marked locations are kept.");
				if (Button("Yes, reset")) {
					LightTheWay::ResetDefaults();
					confirm = 0.0f;
				}
				SameLine();
				if (Button("Cancel")) {
					confirm = 0.0f;
				}
			} else if (Button("Reset Light the Way to default")) {
				confirm = 10.0f;
			}
		}

		void __stdcall RenderLtwProtections()
		{
			if (!LtwGuard()) {
				return;
			}
			TextWrapped("Each protection rolls on its own every interval. At 100 % it always stops the stress while its condition holds. In combat every chance is halved "
						"(unless Combat Stress is off).");
			DrawLtwGroup(LightTheWay::kGroupProtections, "Protections");
			DrawLtwGroup(LightTheWay::kGroupFollowers, "Followers");
		}

		void __stdcall RenderLtwLocationTypes()
		{
			if (!LtwGuard()) {
				return;
			}
			TextWrapped("The kinds of place that count as stressful. A place needs at least one of these to build stress, unless you mark it yourself on the Custom Locations page.");
			Spacing();
			if (BeginTable("loctypes", 3)) {
				for (int i = 0; i < LightTheWay::KeywordCount(); ++i) {
					TableNextColumn();
					PushID(i);
					bool on = LightTheWay::KeywordEnabled(i);
					if (Checkbox(LightTheWay::KeywordName(i), &on)) {
						LightTheWay::SetKeywordEnabled(i, on);
					}
					PopID();
				}
				EndTable();
			}
		}

		void DrawLtwList(LightTheWay::ListKind a_kind, const char* a_title, const char* a_empty)
		{
			SeparatorText(a_title);
			const auto entries = LightTheWay::Entries(a_kind);
			if (entries.empty()) {
				TextDisabled("%s", a_empty);
				return;
			}
			for (const auto& e : entries) {
				PushID(static_cast<int>(e.formID));
				bool on = e.active;
				if (Checkbox(e.name.c_str(), &on)) {
					LightTheWay::SetEntryActive(a_kind, e.formID, on);
				}
				PopID();
			}
		}

		void __stdcall RenderLtwCustom()
		{
			if (!LtwGuard()) {
				return;
			}
			TextDisabled("Where you are: %s", LightTheWay::CurrentLocationText().c_str());
			if (Button("Mark or unmark this location")) {
				LightTheWay::MarkCurrent();
			}
			Tip("In a place that is not stressful, marks it as stressful. In a place that is, excludes it. In a place you marked before, undoes that.");
			SameLine();
			if (Button("Force this location as an interior")) {
				LightTheWay::ForceCurrentInterior();
			}
			Tip("Makes an outdoor place count as an interior (a ruin under the open sky, a walled fort). Press it again there to undo.");
			TextWrapped("Ticked places are in effect. Untick one to switch it off without forgetting it. Each list holds up to 128 places.");
			DrawLtwList(LightTheWay::ListKind::InteriorInclude, "Interiors marked as stressful", "None yet.");
			DrawLtwList(LightTheWay::ListKind::InteriorExclude, "Interiors excluded from stress", "None yet.");
			DrawLtwList(LightTheWay::ListKind::ExteriorInclude, "Outdoor places marked as stressful", "None yet.");
			DrawLtwList(LightTheWay::ListKind::ExteriorExclude, "Outdoor places excluded from stress", "None yet.");
			DrawLtwList(LightTheWay::ListKind::ForcedInterior, "Outdoor places forced to count as interiors", "None yet.");
		}

		// ---- the stress bar -----------------------------------------------------------------------------------------------------------
		bool Changed(bool a_changed)
		{
			if (a_changed) {
				Hud::MarkDirty();
			}
			return a_changed;
		}

		void __stdcall RenderHud()
		{
			auto& c = Hud::Cfg();
			Hud::MarkPreview();  // keep the bar visible while this page is open
			TextWrapped("A thin bar centred above the compass that fills with your stress and changes colour with its band. It stays on screen while this page is open so "
						"you can place it.");
			Spacing();
			Changed(Checkbox("Show the stress bar", &c.enabled));
			Tip("The bar is only drawn while the stress system is switched on in Settings.");
			BeginDisabled(!c.enabled);
			static const char* modes[] = { "Always", "Contextual (fade out while calm)" };
			Changed(Combo("Display", &c.displayMode, modes, 2));
			Tip("Contextual: the bar shows while your stress changes or is above the calm band (over 25), and fades out when you are calm and nothing has changed.");
			if (c.displayMode == 1) {
				Changed(SliderFloat("Stay visible after a change (s)", &c.contextualSeconds, 1.0f, 30.0f, "%.0f"));
			}

			SeparatorText("Position and size");
			static const char* orientations[] = { "Lying flat", "Standing" };
			Changed(Combo("Direction", &c.orientation, orientations, 2));
			Changed(Checkbox("Flip: fill from the other end", &c.flip));
			Tip("A flat bar fills from the left; flipped, from the right. A standing bar fills from the bottom; flipped, from the top.");
			if (Hud::EditMode()) {
				if (Button("Lock the bar")) {
					Hud::EditMode() = false;
				}
				SameLine();
				TextDisabled("Unlocked: drag the bar on the screen.");
			} else if (Button("Unlock to move the bar with the mouse")) {
				Hud::EditMode() = true;
			}
			Tip("While it is unlocked, click the bar on the screen and drag it where you want it. The sliders below do the same, to the pixel.");
			Changed(SliderFloat("Horizontal (centre)", &c.posX, 0.0f, 1.0f, "%.3f"));
			Changed(SliderFloat("Vertical (centre)", &c.posY, 0.0f, 1.0f, "%.3f"));
			Tip("0.925 sits just above the vanilla compass. Lower the number to raise the bar.");
			Changed(SliderFloat("Scale", &c.scale, 0.5f, 3.0f, "%.2f"));
			Changed(SliderFloat("Bar length", &c.barLength, 60.0f, 800.0f, "%.0f"));
			Changed(SliderFloat("Bar thickness", &c.barThickness, 2.0f, 30.0f, "%.0f"));
			Changed(SliderFloat("Opacity", &c.opacity, 0.1f, 1.0f, "%.2f"));

			SeparatorText("Details");
			Changed(Checkbox("Show the word Stress", &c.showLabel));
			Changed(Checkbox("Show the number", &c.showValue));
			Changed(Checkbox("Show the band marks (25, 45, 65, 85)", &c.showTicks));
			Changed(Checkbox("Pulse in the severe band", &c.pulse));

			Spacing();
			if (Button("Reset position and size")) {
				Hud::ResetLayout();
				Hud::MarkDirty();
			}
			EndDisabled();
			TextDisabled("%s", Hud::FilePath());
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
				LightTheWay::SaveNow();
			}
			SameLine();
			if (Button("Load the file now")) {
				Settings::LoadNow();
				LightTheWay::LoadNow();
			}
			if (LightTheWay::Present()) {
				TextDisabled("Light the Way: %s", LightTheWay::FilePath());
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
		void __stdcall Frame()
		{
			Settings::Tick(GetIO()->DeltaTime);
			LightTheWay::Tick(GetIO()->DeltaTime);
			Hud::Render();
		}
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
		SKSEMenuFramework::AddSectionItem("Light the Way", RenderLtw);
		SKSEMenuFramework::AddSectionItem("Protections", RenderLtwProtections);
		SKSEMenuFramework::AddSectionItem("Location Types", RenderLtwLocationTypes);
		SKSEMenuFramework::AddSectionItem("Custom Locations", RenderLtwCustom);
		SKSEMenuFramework::AddSectionItem("HUD", RenderHud);
		SKSEMenuFramework::AddSectionItem("Profile", RenderProfile);
		SKSEMenuFramework::AddHudElement(Frame);
		registered = true;
		SKSE::log::info("Registered with SKSE Menu Framework {}", SKSEMenuFramework::GetMenuFrameworkVersion());
	}
}
