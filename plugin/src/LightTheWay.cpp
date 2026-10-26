/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "LightTheWay.h"
#include "Settings.h"

#include <random>
#include <set>
#include <type_traits>

namespace LightTheWay
{
	namespace
	{
		constexpr const char* kPlugin = "Light the Way.esp";
		constexpr const char* kSF = "Stress and Fear.esp";
		constexpr const char* kFile = "Data/SKSE/Plugins/StressAndFear/lighttheway.ini";
		constexpr float       kRadiusCampfire = 600.0f;
		constexpr float       kRadiusFollowers = 4096.0f;

		// Local ids (light plugin) of the master switch and of the forms the logic reads.
		constexpr unsigned kEnabledGlobal = 0xD73;      // LTW_Enabled
		constexpr unsigned kIntervalGlobal = 0xDB9;     // LTW_IntervalSeconds
		constexpr unsigned kAmountGlobal = 0xD74;       // LTW_StressAmount
		constexpr unsigned kForcedInteriors = 0xB7D, kAllForcedInteriors = 0xB7C;
		constexpr unsigned kIntInclude = 0x8A7, kAllIntInclude = 0x8A6, kIntExclude = 0x8A9, kAllIntExclude = 0x8A8;
		constexpr unsigned kExtInclude = 0x8AB, kAllExtInclude = 0x8AA, kExtExclude = 0x8AD, kAllExtExclude = 0x8AC;
		constexpr unsigned kEnabledKeywords = 0xDE3, kFollowerBlock = 0xDE2, kCampfireForms = 0x928;
		constexpr unsigned kKwMonstrosity = 0xD6A, kKwIlluminate = 0xDEC, kKwInvisible = 0xDEB, kKwLanterns = 0x805, kKwNightVision = 0xDED;
		constexpr unsigned kSpellMark = 0x802, kSpellCheck = 0xDE9, kSpellForce = 0xB7F;

#define TOG(id, l, tip, loc, d, grp) { id, l, tip, Kind::Toggle, loc, d, 0, 1, 1, "", grp }
#define SLD(id, l, tip, loc, d, mn, mx, st, f, grp) { id, l, tip, Kind::Slider, loc, d, mn, mx, st, f, grp }

		constexpr const char* kProbTip = "The chance, each interval, that this protection stops the stress from being added. Several protections each roll on their own.";
	}

	const Def kDefs[] = {
		// General
		SLD("stress_amount", "Stress Per Interval", "How much stress you gain each interval in a stressful place.", kAmountGlobal, 1.0f, 0.1f, 10.0f, 0.1f, "%.1f", kGroupGeneral),
		SLD("interval", "Interval", "How many seconds pass between stress gains (game time does not matter, menus pause it).", kIntervalGlobal, 60.0f, 5.0f, 600.0f, 1.0f, "%.0f s", kGroupGeneral),
		TOG("notify", "Show Stress After Each Gain", "A small notification with your stress level every time stress is added.", 0xD78, 1, kGroupGeneral),
		TOG("combat", "Combat Stress", "While you are in combat, every protection works at half strength.", 0xE06, 1, kGroupGeneral),
		TOG("level_cap", "Level Cap", "Light the Way stops adding stress at a ceiling that falls as you level: 100 under level 30, then 90, 80, 70, 60 and 50 from level 70.", 0xD80, 1, kGroupGeneral),
		// Where
		TOG("interior", "Interior Stress", "Stress builds in interiors that have one of the location types on the Location Types page.", 0xDE4, 1, kGroupLocations),
		TOG("wilderness", "Wilderness Stress", "Stress builds outdoors in places that have one of the location types on the Location Types page (bandit camps, hagraven nests, and so on).", 0xDE5, 1, kGroupLocations),
		TOG("night_only", "Wilderness At Night Only", "Outdoor stress only builds between 8 in the evening and 5 in the morning.", 0xDE6, 1, kGroupLocations),
		// Protections (toggle then probability)
		TOG("campfires", "Campfires", "Being within 600 units of a lit fire (campfire, fireplace, brazier) can stop stress. Campfire's own fires count while they burn.", 0xDEE, 1, kGroupProtections),
		SLD("campfires_p", "  Chance", kProbTip, 0xDEF, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("sneaking", "Sneaking", "Hiding in the shadows can stop stress.", 0xDF0, 1, kGroupProtections),
		SLD("sneaking_p", "  Chance", kProbTip, 0xDF1, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("torches", "Torches", "Only a torch in your hand counts.", 0xDF2, 1, kGroupProtections),
		SLD("torches_p", "  Chance", kProbTip, 0xDF3, 50, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("lanterns", "Lanterns", "A wearable lantern from a supported lantern mod.", 0xDF4, 1, kGroupProtections),
		SLD("lanterns_p", "  Chance", kProbTip, 0xDF5, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("invisibility", "Invisibility", "When they cannot see you, you feel less stressed.", 0xDF6, 1, kGroupProtections),
		SLD("invisibility_p", "  Chance", kProbTip, 0xDF7, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("candlelight", "Candlelight", "A hovering ball of light from a Candlelight-type spell.", 0xDF8, 1, kGroupProtections),
		SLD("candlelight_p", "  Chance", kProbTip, 0xDF9, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("night_vision", "Night Eye", "Seeing in the dark puts you at ease.", 0xDFA, 1, kGroupProtections),
		SLD("night_vision_p", "  Chance", kProbTip, 0xDFB, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("vampire_undead", "Vampires And Undead", "Creatures of the night have a natural resistance to fear.", 0xDFC, 1, kGroupProtections),
		SLD("vampire_undead_p", "  Chance", kProbTip, 0xDFD, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		TOG("werewolf", "Werewolves And Vampire Lords", "In beast form there is nothing to be afraid of.", 0xDFE, 1, kGroupProtections),
		SLD("werewolf_p", "  Chance", kProbTip, 0xDFF, 100, 0, 100, 1, "%.0f %%", kGroupProtections),
		// Followers
		TOG("followers_ext", "Followers Protect Outdoors", "Followers near you can stop stress in the wilderness. Fewer than the required number gives a scaled-down chance.", 0xE00, 0, kGroupFollowers),
		SLD("followers_ext_n", "  Followers For Full Chance", "How many followers it takes to reach the full chance outdoors.", 0xE01, 1, 1, 10, 1, "%.0f", kGroupFollowers),
		SLD("followers_ext_p", "  Chance", kProbTip, 0xE02, 100, 0, 100, 1, "%.0f %%", kGroupFollowers),
		TOG("followers_int", "Followers Protect Indoors", "Followers near you can stop stress in interiors.", 0xE03, 0, kGroupFollowers),
		SLD("followers_int_n", "  Followers For Full Chance", "How many followers it takes to reach the full chance indoors.", 0xE04, 1, 1, 10, 1, "%.0f", kGroupFollowers),
		SLD("followers_int_p", "  Chance", kProbTip, 0xE05, 100, 0, 100, 1, "%.0f %%", kGroupFollowers),
		// Powers
		TOG("power_mark", "Power: Mark Location", "Gives you a power that marks the place you stand in as stressful (or, where it already is, as stress free). You can do the same from the Custom Locations page.", 0xD7E, 0, kGroupPowers),
		TOG("power_check", "Power: Check Stress", "Gives you a power that shows your stress level.", 0xDEA, 0, kGroupPowers),
		TOG("power_force", "Power: Force Interior", "Gives you a power that makes the outdoor place you stand in count as an interior.", 0xB7B, 0, kGroupPowers),
	};
	const int kDefCount = static_cast<int>(std::size(kDefs));

#undef TOG
#undef SLD

	namespace
	{
		struct KeywordDef
		{
			const char* name;
			unsigned    id;
			const char* plugin;
		};
		// The 21 location types of the original menu, in alphabetical order of their names.
		constexpr KeywordDef kKeywords[] = {
			{ "Animal Den", 0x130DE, "Skyrim.esm" }, { "Ash Spawn Lair", 0x2C969, "Dragonborn.esm" }, { "Bandit Camp", 0x130DF, "Skyrim.esm" },
			{ "Cave", 0x130EF, "Skyrim.esm" }, { "Dragon Lair", 0x130E0, "Skyrim.esm" }, { "Dragon Priest Lair", 0x130E1, "Skyrim.esm" },
			{ "Draugr Crypt", 0x130E2, "Skyrim.esm" }, { "Dungeon", 0x130DB, "Skyrim.esm" }, { "Dwarven Automaton Lair", 0x130E3, "Skyrim.esm" },
			{ "Dwarven Ruin", 0x130F0, "Skyrim.esm" }, { "Falmer Hive", 0x130E4, "Skyrim.esm" }, { "Forsworn Camp", 0x130EE, "Skyrim.esm" },
			{ "Giant Camp", 0x130E5, "Skyrim.esm" }, { "Hagraven Nest", 0x130E6, "Skyrim.esm" }, { "Ice Cave", 0x100819, "Skyrim.esm" },
			{ "Nordic Ruin", 0x130F2, "Skyrim.esm" }, { "Shipwreck", 0x1929F, "Skyrim.esm" }, { "Spriggan Grove", 0x130EA, "Skyrim.esm" },
			{ "Vampire Lair", 0x130EB, "Skyrim.esm" }, { "Warlock Lair", 0x130EC, "Skyrim.esm" }, { "Werewolf Lair", 0x130ED, "Skyrim.esm" },
		};

		// Campfire.esm's lit fires (local ids), the same ones Light the Way's script looks for.
		constexpr unsigned kCampfireMod[] = { 0x0328A8, 0x0328A6, 0x05C8D8, 0x040013, 0x0328B9, 0x033E67, 0x033E69, 0x06ABB2 };

		// ---- state --------------------------------------------------------------------------------------------------------------------
		std::recursive_mutex mu;
		bool                 takeover = true;
		bool                 master = true;
		bool                 fileExisted = false;
		bool                 dirty = false;
		float                writeTimer = 0.0f;
		std::map<std::string, float> iniVals;
		std::set<std::string>        iniKeywords;
		bool                         iniHasKeywords = false;

		std::atomic<float> clockS{ 0.0f };   // real seconds since the last interval (or location change)
		std::atomic<float> pollS{ 0.0f };
		std::atomic<float> reevalIn{ -1.0f };  // seconds until the location is evaluated again after a list change (-1 = none)
		std::atomic<bool>  needEval{ true };
		std::atomic<bool>  qualifying{ false };
		std::atomic<bool>  busy{ false };
		float              keywordFillIn = -1.0f;  // seconds of play until the location types are filled in if the list is still empty (-1 = none)
		unsigned int       lastKey = 0xFFFFFFFFu;
		std::string        locText = "unknown";

		template <class T>
		T* Ptr(T* a_p) { return a_p; }
		template <class T>
		T* Ptr(T& a_r) { return &a_r; }

		RE::TESDataHandler* DH() { return RE::TESDataHandler::GetSingleton(); }

		template <class T>
		T* Form(unsigned a_local, const char* a_plugin = kPlugin)
		{
			auto* dh = DH();
			return dh ? dh->LookupForm<T>(a_local, a_plugin) : nullptr;
		}

		RE::TESGlobal* G(unsigned a_local)
		{
			static std::map<unsigned, RE::TESGlobal*> cache;
			auto it = cache.find(a_local);
			if (it != cache.end() && it->second) {
				return it->second;
			}
			auto* g = Form<RE::TESGlobal>(a_local);
			if (g) {
				cache[a_local] = g;
			}
			return g;
		}

		float GV(unsigned a_local, float a_default = 0.0f)
		{
			auto* g = G(a_local);
			return g ? g->value : a_default;
		}

		bool On(unsigned a_local) { return GV(a_local) > 0.5f; }

		RE::BGSListForm* List(unsigned a_local) { return Form<RE::BGSListForm>(a_local); }

		RE::BGSKeyword* Kw(int a_i)
		{
			if (a_i < 0 || a_i >= static_cast<int>(std::size(kKeywords))) {
				return nullptr;
			}
			return Form<RE::BGSKeyword>(kKeywords[a_i].id, kKeywords[a_i].plugin);
		}

		void Notify(const std::string& a_text)
		{
#if __has_include(<RE/S/SendHUDMessage.h>)
			RE::SendHUDMessage::ShowHUDMessage(a_text.c_str());
#else
			RE::DebugNotification(a_text.c_str());
#endif
		}

		float Clamp(float a_v, float a_lo, float a_hi) { return std::min(std::max(a_v, a_lo), a_hi); }

		// Form-list edits go through the game's own Papyrus functions, exactly as the original scripts do, so the lists stay valid in the save.
		void ListOp(RE::BGSListForm* a_list, const char* a_method, RE::TESForm* a_form)
		{
			if (!a_list || !a_form) {
				return;
			}
			SKSE::GetTaskInterface()->AddTask([a_list, a_method, a_form]() {
				auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
				if (!vm) {
					return;
				}
				const auto handle = vm->GetObjectHandlePolicy()->GetHandleForObject(RE::BGSListForm::FORMTYPE, a_list);
				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> none;
				vm->DispatchMethodCall2(handle, "FormList", a_method, RE::MakeFunctionArguments(static_cast<RE::TESForm*>(a_form)), none);
			});
		}

		void Touch()
		{
			dirty = true;
			writeTimer = 0.6f;
		}

		void AskReevaluate()
		{
			needEval = true;
			reevalIn = 1.0f;  // the list edit above runs on the next game tick; look again a moment later
		}

		bool Active() { return Present() && takeover && master; }

		// ---- the file -----------------------------------------------------------------------------------------------------------------
		std::string Trim(const std::string& a_s)
		{
			const auto b = a_s.find_first_not_of(" \t\r\n");
			const auto e = a_s.find_last_not_of(" \t\r\n");
			return b == std::string::npos ? std::string() : a_s.substr(b, e - b + 1);
		}

		bool ReadFile()
		{
			std::ifstream in(kFile);
			if (!in) {
				return false;
			}
			iniVals.clear();
			iniKeywords.clear();
			iniHasKeywords = false;
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
				const std::string val = Trim(line.substr(eq + 1));
				if (key == "keywords") {
					iniHasKeywords = true;
					std::stringstream ss(val);
					std::string       item;
					while (std::getline(ss, item, ',')) {
						if (!Trim(item).empty()) {
							iniKeywords.insert(Trim(item));
						}
					}
					continue;
				}
				try {
					const float f = std::stof(val);
					if (key == "takeover") {
						takeover = f > 0.5f;
					} else if (key == "master") {
						master = f > 0.5f;
					} else {
						iniVals[key] = f;
					}
				} catch (...) {
				}
			}
			if (iniKeywords.empty()) {
				iniHasKeywords = false;
			}
			return true;
		}

		void WriteFile()
		{
			std::error_code ec;
			std::filesystem::create_directories(std::filesystem::path(kFile).parent_path(), ec);
			std::ofstream out(kFile, std::ios::trunc);
			if (!out) {
				SKSE::log::warn("Cannot write {}", kFile);
				return;
			}
			out << "; Light the Way settings of Stress and Fear 2026. Written by the in-game pages; safe to edit by hand while the game is closed.\n"
				<< "takeover=" << (takeover ? 1 : 0) << "\n"
				<< "master=" << (master ? 1 : 0) << "\n";
			for (const auto& d : kDefs) {
				out << d.id << "=" << Get(d) << "\n";
			}
			// An empty list is never written: right after a load the list may still be filling (list edits go through the game's script engine, which is busy then),
			// and an empty line would switch every location type off at the next load.
			std::string enabled;
			for (int i = 0; i < KeywordCount(); ++i) {
				if (KeywordEnabled(i)) {
					enabled += (enabled.empty() ? "" : ",");
					enabled += kKeywords[i].name;
				}
			}
			if (!enabled.empty()) {
				out << "keywords=" << enabled << "\n";
			}
			dirty = false;
			fileExisted = true;
		}

		void PushIniToGame()
		{
			for (const auto& d : kDefs) {
				if (auto it = iniVals.find(d.id); it != iniVals.end()) {
					if (auto* g = G(d.local)) {
						g->value = Clamp(it->second, d.min, d.max);
					}
				}
			}
			if (iniHasKeywords) {
				for (int i = 0; i < KeywordCount(); ++i) {
					const bool want = iniKeywords.count(kKeywords[i].name) > 0;
					if (want != KeywordEnabled(i)) {
						SetKeywordEnabled(i, want);
					}
				}
			}
		}

		void ApplyTakeover()
		{
			if (!Present()) {
				return;
			}
			if (auto* g = G(kEnabledGlobal)) {
				g->value = takeover ? 0.0f : (master ? 1.0f : 0.0f);  // 0 silences the original script
			}
		}

		void SyncSpells()
		{
			if (!Active()) {
				return;
			}
			SKSE::GetTaskInterface()->AddTask([]() {
				auto* player = RE::PlayerCharacter::GetSingleton();
				if (!player) {
					return;
				}
				const struct
				{
					unsigned spell;
					unsigned toggle;
				} powers[] = { { kSpellMark, 0xD7E }, { kSpellCheck, 0xDEA }, { kSpellForce, 0xB7B } };
				for (const auto& p : powers) {
					if (auto* spell = Form<RE::SpellItem>(p.spell)) {
						const bool has = player->HasSpell(spell);
						if (On(p.toggle) && !has) {
							player->AddSpell(spell);
						} else if (!On(p.toggle) && has) {
							player->RemoveSpell(spell);
						}
					}
				}
			});
		}

		// ---- the logic ----------------------------------------------------------------------------------------------------------------
		bool InInterior(RE::PlayerCharacter* a_p)
		{
			auto* cell = a_p->GetParentCell();
			return cell && cell->IsInteriorCell();
		}

		bool ListHas(RE::BGSListForm* a_list, const RE::TESForm* a_form) { return a_list && a_form && a_list->HasForm(a_form); }

		bool LocationHasEnabledKeyword(RE::BGSLocation* a_loc)
		{
			auto* list = List(kEnabledKeywords);
			if (!a_loc || !list) {
				return false;
			}
			bool found = false;
			list->ForEachForm([&](auto&& a_f) {
				auto* kw = Ptr(a_f) ? Ptr(a_f)->template As<RE::BGSKeyword>() : nullptr;
				if (kw && a_loc->HasKeyword(kw)) {
					found = true;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
			return found;
		}

		struct LocationState
		{
			bool effectiveInterior = false;
			bool qualifies = false;
		};

		LocationState EvaluateLocation(RE::PlayerCharacter* a_p)
		{
			LocationState s;
			auto*         loc = a_p->GetCurrentLocation();
			const bool    interior = InInterior(a_p);
			const bool    forced = ListHas(List(kForcedInteriors), loc);
			s.effectiveInterior = interior || forced;

			const bool hasKw = LocationHasEnabledKeyword(loc);
			const bool baseStress = hasKw && ((s.effectiveInterior && On(0xDE4)) || (!s.effectiveInterior && On(0xDE5)));
			auto*      inc = List(s.effectiveInterior ? kIntInclude : kExtInclude);
			auto*      exc = List(s.effectiveInterior ? kIntExclude : kExtExclude);
			s.qualifies = (baseStress || ListHas(inc, loc)) && !ListHas(exc, loc);
			return s;
		}

		bool IsNight()
		{
			auto* cal = RE::Calendar::GetSingleton();
			if (!cal) {
				return false;
			}
			const float hour = cal->GetHour();
			return hour >= 20.0f || hour < 5.0f;
		}

		bool HasEffectWithKeyword(RE::Actor* a_actor, RE::BGSKeyword* a_kw)
		{
			if (!a_kw) {
				return false;
			}
			auto* target = a_actor->AsMagicTarget();
			auto* list = target ? target->GetActiveEffectList() : nullptr;
			if (!list) {
				return false;
			}
			for (auto* effect : *list) {
				if (!effect || effect->flags.any(RE::ActiveEffect::Flag::kInactive, RE::ActiveEffect::Flag::kDispelled)) {
					continue;
				}
				auto* base = effect->GetBaseObject();
				if (base && base->HasKeyword(a_kw)) {
					return true;
				}
			}
			return false;
		}

		bool NearLitFire(RE::PlayerCharacter* a_p)
		{
			std::set<RE::FormID> bases;
			if (auto* list = List(kCampfireForms)) {
				list->ForEachForm([&](auto&& a_f) {
					if (auto* f = Ptr(a_f)) {
						bases.insert(f->GetFormID());
					}
					return RE::BSContainer::ForEachResult::kContinue;
				});
			}
			if (auto* dh = DH(); dh && dh->LookupModByName("Campfire.esm")) {
				for (const unsigned local : kCampfireMod) {
					if (auto* f = dh->LookupForm(local, "Campfire.esm")) {
						bases.insert(f->GetFormID());
					}
				}
			}
			bool litFire = false;
			if (auto* tes = RE::TES::GetSingleton(); tes && !bases.empty()) {
				tes->ForEachReferenceInRange(a_p, kRadiusCampfire, [&](auto&& a_r) {
					auto* ref = Ptr(a_r);
					if (ref && !ref->IsDisabled()) {
						if (auto* base = ref->GetBaseObject(); base && bases.count(base->GetFormID())) {
							litFire = true;
							return RE::BSContainer::ForEachResult::kStop;
						}
					}
					return RE::BSContainer::ForEachResult::kContinue;
				});
			}
			return litFire;
		}

		int CountFollowers(RE::PlayerCharacter* a_p)
		{
			int  n = 0;
			auto* pl = RE::ProcessLists::GetSingleton();
			auto* block = List(kFollowerBlock);
			auto* followerFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x05C84E);  // CurrentFollowerFaction
			if (!pl) {
				return 0;
			}
			pl->ForEachHighActor([&](auto&& a_a) {
				RE::Actor* act = Ptr(a_a);
				if (!act || act == a_p || act->IsDead() || act->IsDisabled() || act->IsDeleted()) {
					return RE::BSContainer::ForEachResult::kContinue;
				}
				if (block && (block->HasForm(act->GetActorBase()) || block->HasForm(act))) {
					return RE::BSContainer::ForEachResult::kContinue;
				}
				if (act->GetPosition().GetDistance(a_p->GetPosition()) > kRadiusFollowers) {
					return RE::BSContainer::ForEachResult::kContinue;
				}
				if (act->IsPlayerTeammate() || (followerFaction && act->IsInFaction(followerFaction))) {
					++n;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
			return n;
		}

		bool Roll(float a_probability, bool a_halve)
		{
			static thread_local std::mt19937 rng{ std::random_device{}() };
			const float                      p = a_halve ? a_probability * 0.5f : a_probability;
			return std::uniform_int_distribution<int>(1, 100)(rng) <= static_cast<int>(p);
		}

		// Does any protection stop the stress this interval? The order, the half strength in combat and the follower maths are the original script's.
		bool PreventedThisInterval(RE::PlayerCharacter* a_p, bool a_effectiveInterior)
		{
			const bool halve = On(0xE06) && a_p->IsInCombat();
			auto       check = [&](unsigned a_toggle, unsigned a_prob, bool a_condition) {
                return On(a_toggle) && a_condition && Roll(GV(a_prob), halve);
			};

			if (On(0xDEE) && NearLitFire(a_p) && Roll(GV(0xDEF), halve)) {
				return true;
			}
			if (check(0xDF0, 0xDF1, a_p->IsSneaking())) {
				return true;
			}
			const auto isTorch = [](RE::TESForm* a_f) { return a_f && a_f->Is(RE::FormType::Light); };
			if (check(0xDF2, 0xDF3, isTorch(a_p->GetEquippedObject(false)) || isTorch(a_p->GetEquippedObject(true)))) {
				return true;
			}
			if (check(0xDF4, 0xDF5, HasEffectWithKeyword(a_p, Form<RE::BGSKeyword>(kKwLanterns)))) {
				return true;
			}
			if (check(0xDF6, 0xDF7, HasEffectWithKeyword(a_p, Form<RE::BGSKeyword>(kKwInvisible)))) {
				return true;
			}
			if (check(0xDF8, 0xDF9, HasEffectWithKeyword(a_p, Form<RE::BGSKeyword>(kKwIlluminate)))) {
				return true;
			}
			if (check(0xDFA, 0xDFB, HasEffectWithKeyword(a_p, Form<RE::BGSKeyword>(kKwNightVision)))) {
				return true;
			}
			auto*      race = a_p->GetRace();
			auto*      vampire = RE::TESForm::LookupByID<RE::BGSKeyword>(0x0A82BB);
			auto*      undead = RE::TESForm::LookupByID<RE::BGSKeyword>(0x013796);
			const bool nightCreature = race && ((vampire && race->HasKeyword(vampire)) || (undead && race->HasKeyword(undead)));
			if (check(0xDFC, 0xDFD, nightCreature)) {
				return true;
			}
			auto* monster = Form<RE::BGSKeyword>(kKwMonstrosity);
			if (check(0xDFE, 0xDFF, race && monster && race->HasKeyword(monster))) {
				return true;
			}

			// Followers: the chance scales with how many of the required number you have, rounded to the nearest whole percent.
			const bool  followersOn = a_effectiveInterior ? On(0xE03) : On(0xE00);
			const float baseProb = a_effectiveInterior ? GV(0xE05) : GV(0xE02);
			const int   required = std::max(1, static_cast<int>(a_effectiveInterior ? GV(0xE04, 1.0f) : GV(0xE01, 1.0f)));
			if (followersOn) {
				const int count = CountFollowers(a_p);
				float     effective = 0.0f;
				if (count > 0) {
					const float ratio = std::min(1.0f, static_cast<float>(count) / static_cast<float>(required));
					const float calc = ratio * baseProb;
					const float whole = std::floor(calc);
					effective = (calc - whole >= 0.5f) ? std::ceil(calc) : whole;
					effective = std::min(effective, 100.0f);
				}
				if (Roll(effective, halve)) {
					return true;
				}
			}
			return false;
		}

		float LevelCap(std::uint16_t a_level)
		{
			if (a_level < 30) {
				return 100.0f;
			}
			if (a_level < 40) {
				return 90.0f;
			}
			if (a_level < 50) {
				return 80.0f;
			}
			if (a_level < 60) {
				return 70.0f;
			}
			if (a_level < 70) {
				return 60.0f;
			}
			return 50.0f;
		}

		// On the game thread: has the place changed (or a list)? Then work out again whether it is stressful, and restart the interval.
		void CheckLocation()
		{
			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player || !player->Is3DLoaded()) {
				return;
			}
			auto*      loc = player->GetCurrentLocation();
			const bool eff = InInterior(player) || ListHas(List(kForcedInteriors), loc);
			const auto key = (static_cast<unsigned>(loc ? loc->GetFormID() : 0u) << 1) | (eff ? 1u : 0u);
			if (key == lastKey && !needEval.exchange(false)) {
				return;
			}
			needEval = false;
			lastKey = key;
			const auto s = EvaluateLocation(player);
			qualifying = s.qualifies;
			{
				std::scoped_lock lk(mu);
				std::string      name = loc && loc->GetName() && loc->GetName()[0] ? loc->GetName() : "(no named location)";
				locText = name + (s.effectiveInterior ? " - interior" : " - outdoors") + (s.qualifies ? ", stressful" : ", not stressful");
			}
			clockS = 0.0f;
		}

		// On the game thread, once per interval: add the stress unless a protection stops it.
		void OnInterval()
		{
			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player || !Active() || !player->Is3DLoaded() || !qualifying) {
				return;
			}
			const bool interior = InInterior(player) || ListHas(List(kForcedInteriors), player->GetCurrentLocation());
			if (!interior && On(0xDE6) && !IsNight()) {
				return;
			}
			if (PreventedThisInterval(player, interior)) {
				return;
			}
			float stress = Settings::Stress() + GV(kAmountGlobal, 1.0f);
			if (On(0xD80)) {
				stress = std::min(stress, LevelCap(player->GetLevel()));
			}
			stress = std::min(stress, 100.0f);
			Settings::SetStress(stress);
			if (On(0xD78)) {
				Notify("Current Stress: " + std::to_string(static_cast<int>(Settings::Stress())) + "/100");
			}
		}
	}

	// ---- public ---------------------------------------------------------------------------------------------------------------------
	bool Present()
	{
		auto* dh = DH();
		return dh && dh->LookupModByName(kPlugin) && dh->LookupModByName(kSF) && G(kEnabledGlobal);
	}

	bool Takeover() { return takeover; }

	void SetTakeover(bool a_on)
	{
		std::scoped_lock lk(mu);
		takeover = a_on;
		ApplyTakeover();
		if (a_on) {
			SyncSpells();
		}
		needEval = true;
		Touch();
	}

	bool Master() { return master; }

	void SetMaster(bool a_on)
	{
		std::scoped_lock lk(mu);
		master = a_on;
		ApplyTakeover();
		needEval = true;
		Touch();
	}

	float Get(const Def& a_d) { return GV(a_d.local, a_d.def); }

	void Set(const Def& a_d, float a_value)
	{
		std::scoped_lock lk(mu);
		if (auto* g = G(a_d.local)) {
			g->value = Clamp(a_value, a_d.min, a_d.max);
		}
		if (a_d.group == kGroupPowers) {
			SyncSpells();
		}
		if (a_d.group == kGroupLocations) {
			needEval = true;
		}
		Touch();
	}

	int         KeywordCount() { return static_cast<int>(std::size(kKeywords)); }
	const char* KeywordName(int a_i) { return (a_i >= 0 && a_i < KeywordCount()) ? kKeywords[a_i].name : ""; }

	bool KeywordEnabled(int a_i)
	{
		auto* list = List(kEnabledKeywords);
		auto* kw = Kw(a_i);
		return list && kw && list->HasForm(kw);
	}

	void SetKeywordEnabled(int a_i, bool a_on)
	{
		auto* list = List(kEnabledKeywords);
		auto* kw = Kw(a_i);
		if (!list || !kw || list->HasForm(kw) == a_on) {
			return;
		}
		ListOp(list, a_on ? "AddForm" : "RemoveAddedForm", kw);
		AskReevaluate();
		std::scoped_lock lk(mu);
		Touch();
	}

	namespace
	{
		struct ListPair
		{
			unsigned all;
			unsigned active;
		};
		ListPair PairOf(ListKind a_k)
		{
			switch (a_k) {
			case ListKind::InteriorInclude:
				return { kAllIntInclude, kIntInclude };
			case ListKind::InteriorExclude:
				return { kAllIntExclude, kIntExclude };
			case ListKind::ExteriorInclude:
				return { kAllExtInclude, kExtInclude };
			case ListKind::ExteriorExclude:
				return { kAllExtExclude, kExtExclude };
			default:
				return { kAllForcedInteriors, kForcedInteriors };
			}
		}
	}

	std::vector<Entry> Entries(ListKind a_kind)
	{
		std::vector<Entry> out;
		const auto         pair = PairOf(a_kind);
		auto*              all = List(pair.all);
		auto*              active = List(pair.active);
		if (!all) {
			return out;
		}
		all->ForEachForm([&](auto&& a_f) {
			auto* form = Ptr(a_f);
			if (auto* loc = form ? form->template As<RE::BGSLocation>() : nullptr) {
				const char* n = loc->GetName();
				out.push_back({ (n && n[0]) ? std::string(n) : std::format("{:08X}", loc->GetFormID()), loc->GetFormID(), active && active->HasForm(loc) });
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});
		std::sort(out.begin(), out.end(), [](const Entry& a, const Entry& b) { return a.name < b.name; });
		return out;
	}

	void SetEntryActive(ListKind a_kind, unsigned int a_formID, bool a_on)
	{
		auto* active = List(PairOf(a_kind).active);
		auto* form = RE::TESForm::LookupByID(a_formID);
		if (!active || !form || active->HasForm(form) == a_on) {
			return;
		}
		ListOp(active, a_on ? "AddForm" : "RemoveAddedForm", form);
		AskReevaluate();
	}

	void MarkCurrent()
	{
		SKSE::GetTaskInterface()->AddTask([]() {
			auto* player = RE::PlayerCharacter::GetSingleton();
			auto* loc = player ? player->GetCurrentLocation() : nullptr;
			if (!loc || !loc->GetName() || !loc->GetName()[0]) {
				Notify("Invalid location.");
				return;
			}
			const bool interior = InInterior(player);
			const bool baseQual = LocationHasEnabledKeyword(loc) && ((interior && On(0xDE4)) || (!interior && On(0xDE5)));
			auto*      allInc = List(interior ? kAllIntInclude : kAllExtInclude);
			auto*      inc = List(interior ? kIntInclude : kExtInclude);
			auto*      allExc = List(interior ? kAllIntExclude : kAllExtExclude);
			auto*      exc = List(interior ? kIntExclude : kExtExclude);
			if (!allInc || !inc || !allExc || !exc) {
				return;
			}
			if (allInc->HasForm(loc)) {
				ListOp(allInc, "RemoveAddedForm", loc);
				ListOp(inc, "RemoveAddedForm", loc);
				Notify("Location marking removed.");
			} else if (allExc->HasForm(loc)) {
				ListOp(allExc, "RemoveAddedForm", loc);
				ListOp(exc, "RemoveAddedForm", loc);
				Notify("Location marking removed.");
			} else {
				auto* targetAll = baseQual ? allExc : allInc;
				auto* targetActive = baseQual ? exc : inc;
				if (targetAll->scriptAddedTempForms && targetAll->scriptAddedTempForms->size() >= 128) {
					Notify("Custom location limit reached.");
					return;
				}
				ListOp(targetAll, "AddForm", loc);
				ListOp(targetActive, "AddForm", loc);
				Notify(baseQual ? "Location excluded from stress." : "Location marked as stressful.");
			}
			AskReevaluate();
		});
	}

	void ForceCurrentInterior()
	{
		SKSE::GetTaskInterface()->AddTask([]() {
			auto* player = RE::PlayerCharacter::GetSingleton();
			auto* loc = player ? player->GetCurrentLocation() : nullptr;
			if (!loc || !loc->GetName() || !loc->GetName()[0]) {
				Notify("Invalid location.");
				return;
			}
			if (InInterior(player)) {
				Notify("Location is already an interior.");
				return;
			}
			auto* all = List(kAllForcedInteriors);
			auto* active = List(kForcedInteriors);
			if (!all || !active) {
				return;
			}
			if (all->HasForm(loc)) {
				ListOp(all, "RemoveAddedForm", loc);
				ListOp(active, "RemoveAddedForm", loc);
				Notify("Location no longer forced as interior.");
			} else {
				if (all->scriptAddedTempForms && all->scriptAddedTempForms->size() >= 128) {
					Notify("Custom location limit reached.");
					return;
				}
				ListOp(all, "AddForm", loc);
				ListOp(active, "AddForm", loc);
				Notify("Location marked as interior.");
			}
			AskReevaluate();
		});
	}

	std::string CurrentLocationText()
	{
		std::scoped_lock lk(mu);
		return locText;
	}

	bool Qualifying() { return qualifying; }

	void OnDataLoaded()
	{
		std::scoped_lock lk(mu);
		fileExisted = ReadFile();
		SKSE::log::info("Light the Way: {} (Light the Way.esp {}, takeover {})", fileExisted ? "lighttheway.ini read" : "no lighttheway.ini yet",
			Present() ? "loaded" : "not loaded", takeover ? "on" : "off");
	}

	void OnGameLoaded()
	{
		std::scoped_lock lk(mu);
		if (!Present()) {
			return;
		}
		if (!fileExisted) {
			// First time: adopt the settings the game already has, and keep the original master switch value.
			master = GV(kEnabledGlobal, 1.0f) > 0.5f || !takeover;
			keywordFillIn = 25.0f;  // the original menu fills the list itself when it starts; only if it has not by then is it done here (see Tick)
			Touch();
			SKSE::log::info("Light the Way: settings adopted from the game, takeover {}", takeover ? "on" : "off");
		} else if (Settings::ApplyOnLoad()) {
			PushIniToGame();
		}
		ApplyTakeover();
		SyncSpells();
		lastKey = 0xFFFFFFFFu;
		needEval = true;
		clockS = 0.0f;
	}

	void Tick(float a_dt)
	{
		if (writeTimer > 0.0f && dirty) {
			std::scoped_lock lk(mu);
			writeTimer -= a_dt;
			if (writeTimer <= 0.0f) {
				WriteFile();
			}
		}
		if (keywordFillIn > 0.0f && Present()) {
			keywordFillIn -= a_dt;
			if (keywordFillIn <= 0.0f) {
				keywordFillIn = -1.0f;
				SKSE::GetTaskInterface()->AddTask([]() {
					auto* list = List(kEnabledKeywords);
					if (list && list->forms.empty() && (!list->scriptAddedTempForms || list->scriptAddedTempForms->empty())) {
						for (int i = 0; i < KeywordCount(); ++i) {
							SetKeywordEnabled(i, true);  // the original menu starts with every location type on
						}
						SKSE::log::info("Light the Way: the location type list was empty, all 21 types switched on");
					}
				});
			}
		}
		if (!Active()) {
			return;
		}
		auto* ui = RE::UI::GetSingleton();
		if (!ui || ui->GameIsPaused()) {
			return;  // a menu is open: nothing counts
		}
		if (reevalIn > 0.0f) {
			reevalIn = reevalIn - a_dt;
			if (reevalIn <= 0.0f) {
				needEval = true;
				reevalIn = -1.0f;
			}
		}
		clockS = clockS + a_dt;
		pollS = pollS + a_dt;
		if (pollS >= 1.0f || needEval) {
			pollS = 0.0f;
			if (!busy.exchange(true)) {
				SKSE::GetTaskInterface()->AddTask([]() {
					CheckLocation();
					busy = false;
				});
			}
		}
		if (clockS >= std::max(1.0f, GV(kIntervalGlobal, 60.0f))) {
			clockS = 0.0f;
			SKSE::GetTaskInterface()->AddTask([]() { OnInterval(); });
		}
	}

	void SaveNow()
	{
		std::scoped_lock lk(mu);
		WriteFile();
	}

	void LoadNow()
	{
		std::scoped_lock lk(mu);
		if (ReadFile()) {
			PushIniToGame();
			ApplyTakeover();
			SyncSpells();
			needEval = true;
		}
	}

	void ResetDefaults()
	{
		std::scoped_lock lk(mu);
		for (const auto& d : kDefs) {
			if (auto* g = G(d.local)) {
				g->value = d.def;
			}
		}
		for (int i = 0; i < KeywordCount(); ++i) {
			SetKeywordEnabled(i, true);
		}
		SyncSpells();
		needEval = true;
		Touch();
	}

	const char* FilePath() { return kFile; }
}
