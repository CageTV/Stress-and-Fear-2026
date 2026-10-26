/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Phobias.h"

namespace Phobias
{
	namespace
	{
		constexpr const char* kPlugin = "Stress and Fear.esp";

		struct Entry
		{
			const char* name;
			unsigned    spell;  // Stress_PhobiaSpell_X (local id in the light plugin)
			unsigned    perk;   // Stress_Champion_X
		};

		constexpr Entry kEntries[] = {
			{ "Wolves", 0x817, 0x819 },       { "Dragons", 0x824, 0x821 },   { "Vampires", 0x82F, 0x82D },  { "Mages", 0x833, 0x831 },
			{ "Undead", 0x839, 0x835 },       { "Bandits", 0x83E, 0x83A },   { "Falmer", 0x843, 0x83F },    { "Forsworn", 0x848, 0x844 },
			{ "Hagravens", 0x84D, 0x849 },    { "Ghosts", 0x852, 0x84E },    { "Thalmor", 0x857, 0x853 },   { "Bears", 0x85C, 0x858 },
			{ "Spiders", 0x861, 0x85D },      { "Chaurus", 0x866, 0x862 },   { "Sabre Cats", 0x86B, 0x867 }, { "Ice Wraiths", 0x870, 0x86C },
			{ "Werewolves", 0x898, 0x871 },   { "Trolls", 0x878, 0x875 },    { "Dremora", 0x87E, 0x879 },   { "Atronachs", 0x883, 0x87F },
			{ "Dwarven Automatons", 0x888, 0x884 }, { "Giants", 0x88D, 0x889 }, { "Spriggans", 0x892, 0x88E }, { "Wispmothers", 0x897, 0x893 },
		};

		RE::SpellItem* Spell(int a_i)
		{
			auto* dh = RE::TESDataHandler::GetSingleton();
			return dh ? dh->LookupForm<RE::SpellItem>(kEntries[a_i].spell, kPlugin) : nullptr;
		}

		RE::BGSPerk* Perk(int a_i)
		{
			auto* dh = RE::TESDataHandler::GetSingleton();
			return dh ? dh->LookupForm<RE::BGSPerk>(kEntries[a_i].perk, kPlugin) : nullptr;
		}

		bool Valid(int a_i) { return a_i >= 0 && a_i < static_cast<int>(std::size(kEntries)); }

		// Changes to the player happen on the game thread.
		template <class F>
		void OnGameThread(F a_f)
		{
			SKSE::GetTaskInterface()->AddTask([a_f]() {
				if (auto* player = RE::PlayerCharacter::GetSingleton()) {
					a_f(player);
				}
			});
		}
	}

	int Count() { return static_cast<int>(std::size(kEntries)); }

	const char* Name(int a_index) { return Valid(a_index) ? kEntries[a_index].name : ""; }

	State StateOf(int a_index)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player || !Valid(a_index)) {
			return State::None;
		}
		if (auto* perk = Perk(a_index); perk && player->HasPerk(perk)) {
			return State::Overcome;
		}
		if (auto* spell = Spell(a_index); spell && player->HasSpell(spell)) {
			return State::Afraid;
		}
		return State::None;
	}

	void Give(int a_index)
	{
		if (!Valid(a_index)) {
			return;
		}
		OnGameThread([a_index](RE::PlayerCharacter* a_p) {
			auto* spell = Spell(a_index);
			auto* perk = Perk(a_index);
			if (spell && !(perk && a_p->HasPerk(perk)) && !a_p->HasSpell(spell)) {
				a_p->AddSpell(spell);
			}
		});
	}

	void Cure(int a_index)
	{
		if (!Valid(a_index)) {
			return;
		}
		OnGameThread([a_index](RE::PlayerCharacter* a_p) {
			if (auto* spell = Spell(a_index); spell && a_p->HasSpell(spell)) {
				a_p->RemoveSpell(spell);
			}
		});
	}

	void Overcome(int a_index)
	{
		if (!Valid(a_index)) {
			return;
		}
		OnGameThread([a_index](RE::PlayerCharacter* a_p) {
			if (auto* spell = Spell(a_index); spell && a_p->HasSpell(spell)) {
				a_p->RemoveSpell(spell);
			}
			if (auto* perk = Perk(a_index); perk && !a_p->HasPerk(perk)) {
				a_p->AddPerk(perk);
			}
		});
	}

	void Reset(int a_index)
	{
		if (!Valid(a_index)) {
			return;
		}
		OnGameThread([a_index](RE::PlayerCharacter* a_p) {
			if (auto* spell = Spell(a_index); spell && a_p->HasSpell(spell)) {
				a_p->RemoveSpell(spell);
			}
			if (auto* perk = Perk(a_index); perk && a_p->HasPerk(perk)) {
				a_p->RemovePerk(perk);
			}
		});
	}

	int AfraidCount()
	{
		int n = 0;
		for (int i = 0; i < Count(); ++i) {
			n += StateOf(i) == State::Afraid ? 1 : 0;
		}
		return n;
	}

	int OvercomeCount()
	{
		int n = 0;
		for (int i = 0; i < Count(); ++i) {
			n += StateOf(i) == State::Overcome ? 1 : 0;
		}
		return n;
	}
}
