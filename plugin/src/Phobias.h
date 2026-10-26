/*
 * Stress and Fear 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// The 24 phobias of Stress and Fear. A phobia you have is a spell on the player (Stress_PhobiaSpell_X); one you have overcome is a perk
// (Stress_Champion_X) and the spell is gone. This reads that state and can change it, the same way the mod's own scripts do.
namespace Phobias
{
	enum class State { None, Afraid, Overcome };

	int         Count();
	const char* Name(int a_index);
	State       StateOf(int a_index);

	void Give(int a_index);     // the fear: adds the phobia spell (not if the fear is overcome)
	void Cure(int a_index);     // removes the phobia spell and leaves the fear able to come back
	void Overcome(int a_index); // removes the phobia spell and gives the permanent perk, as overcoming it in game does
	void Reset(int a_index);    // back to never having had it: no spell and no perk

	int AfraidCount();
	int OvercomeCount();
}
