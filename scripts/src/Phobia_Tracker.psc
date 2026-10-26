Scriptname Phobia_Tracker extends ActiveMagicEffect  

Actor Property PlayerRef Auto
Formlist property Stress_LastAttacker Auto
GlobalVariable Property Stress_Total Auto
GlobalVariable Property Phobias_Enabled Auto
GlobalVariable Property Stress_BoxMessages Auto
Potion Property Stress_Potion_Reduce50 Auto
Topic Property Stress_Quest_Trauma Auto
ImageSpaceModifier Property Stress_PhobiaImageEffect Auto

Keyword Property ActorTypeDragon Auto
Keyword property Vampire Auto
Keyword property ActorTypeUndead auto
Keyword property ActorTypeGhost auto
Keyword property ActorTypeTroll auto
Keyword property ActorTypeDwarven auto
Keyword property ActorTypeGiant auto

Faction Property WarlockFaction Auto
Faction Property NecromancerFaction Auto
Faction Property BanditFaction Auto
Faction Property FalmerFaction Auto
Faction Property ForswornFaction Auto
Faction Property ThalmorFaction Auto

Race Property HagravenRace Auto
Race Property ChaurusRace Auto
Race Property ChaurusReaperRace Auto
Race Property IceWraithRace Auto
Race Property WerewolfRace Auto
Race Property DremoraRace Auto
Race Property AtronachFlameRace Auto
Race Property AtronachFrostRace Auto
Race Property AtronachStormRace Auto
Race Property WispRace Auto
Race Property DLC2AshSpawnRace Auto

VoiceType Property CRWolfVoice Auto
VoiceType Property CRBearVoice Auto
VoiceType Property CrFrostbiteSpiderVoice Auto
VoiceType Property CrFrostbiteSpiderGiantVoice Auto
VoiceType Property CrSabreCatVoice Auto
VoiceType Property CrSprigganVoice Auto

Spell Property Stress_PhobiaSpell_Wolf Auto
Spell Property Stress_PhobiaSpell_Dragons Auto
Spell Property Stress_PhobiaSpell_Vampires Auto
Spell Property Stress_PhobiaSpell_Mages Auto
Spell Property Stress_PhobiaSpell_Undead Auto
Spell Property Stress_PhobiaSpell_Bandits Auto
Spell Property Stress_PhobiaSpell_Falmer Auto
Spell Property Stress_PhobiaSpell_Forsworn Auto
Spell Property Stress_PhobiaSpell_Hagravens Auto
Spell Property Stress_PhobiaSpell_Ghosts Auto
Spell Property Stress_PhobiaSpell_Thalmor Auto
Spell Property Stress_PhobiaSpell_Bears Auto
Spell Property Stress_PhobiaSpell_Spiders Auto
Spell Property Stress_PhobiaSpell_Chaurus Auto
Spell Property Stress_PhobiaSpell_Sabrecats Auto
Spell Property Stress_PhobiaSpell_IceWraiths Auto
Spell Property Stress_PhobiaSpell_Werewolves Auto
Spell Property Stress_PhobiaSpell_Trolls Auto
Spell Property Stress_PhobiaSpell_Dremora Auto
Spell Property Stress_PhobiaSpell_Atronachs Auto
Spell Property Stress_PhobiaSpell_Dwarven Auto
Spell Property Stress_PhobiaSpell_Giants Auto
Spell Property Stress_PhobiaSpell_Spriggans Auto
Spell Property Stress_PhobiaSpell_Wispmothers Auto

Perk Property Stress_Champion_Wolf Auto
Perk Property Stress_Champion_Dragons Auto
Perk Property Stress_Champion_Vampires Auto
Perk Property Stress_Champion_Mages Auto
Perk Property Stress_Champion_Undead Auto
Perk Property Stress_Champion_Bandits Auto
Perk Property Stress_Champion_Falmer Auto
Perk Property Stress_Champion_Forsworn Auto
Perk Property Stress_Champion_Hagravens Auto
Perk Property Stress_Champion_Ghosts Auto
Perk Property Stress_Champion_Thalmor Auto
Perk Property Stress_Champion_Bears Auto
Perk Property Stress_Champion_Spiders Auto
Perk Property Stress_Champion_Chaurus Auto
Perk Property Stress_Champion_Sabrecats Auto
Perk Property Stress_Champion_IceWraiths Auto
Perk Property Stress_Champion_Werewolves Auto
Perk Property Stress_Champion_Trolls Auto
Perk Property Stress_Champion_Dremora Auto
Perk Property Stress_Champion_Atronachs Auto
Perk Property Stress_Champion_Dwarven Auto
Perk Property Stress_Champion_Giants Auto
Perk Property Stress_Champion_Spriggans Auto
Perk Property Stress_Champion_Wispmothers Auto

Message Property Stress_Message_Wolves1 Auto
Message Property Stress_Message_Wolves2 Auto
Message Property Stress_Message_Dragons1 Auto
Message Property Stress_Message_Dragons2 Auto
Message Property Stress_Message_Vampires1 Auto
Message Property Stress_Message_Vampires2 Auto
Message Property Stress_Message_Mages1 Auto
Message Property Stress_Message_Mages2 Auto
Message Property Stress_Message_Undead1 Auto
Message Property Stress_Message_Undead2 Auto
Message Property Stress_Message_Bandits1 Auto
Message Property Stress_Message_Bandits2 Auto
Message Property Stress_Message_Falmer1 Auto
Message Property Stress_Message_Falmer2 Auto
Message Property Stress_Message_Forsworn1 Auto
Message Property Stress_Message_Forsworn2 Auto
Message Property Stress_Message_Hagravens1 Auto
Message Property Stress_Message_Hagravens2 Auto
Message Property Stress_Message_Ghosts1 Auto
Message Property Stress_Message_Ghosts2 Auto
Message Property Stress_Message_Thalmor1 Auto
Message Property Stress_Message_Thalmor2 Auto
Message Property Stress_Message_Bears1 Auto
Message Property Stress_Message_Bears2 Auto
Message Property Stress_Message_Spiders1 Auto
Message Property Stress_Message_Spiders2 Auto
Message Property Stress_Message_Chaurus1 Auto
Message Property Stress_Message_Chaurus2 Auto
Message Property Stress_Message_Sabrecats1 Auto
Message Property Stress_Message_Sabrecats2 Auto
Message Property Stress_Message_IceWraiths1 Auto
Message Property Stress_Message_IceWraiths2 Auto
Message Property Stress_Message_Werewolves1 Auto
Message Property Stress_Message_Werewolves2 Auto
Message Property Stress_Message_Trolls1 Auto
Message Property Stress_Message_Trolls2 Auto
Message Property Stress_Message_Dremora1 Auto
Message Property Stress_Message_Dremora2 Auto
Message Property Stress_Message_Atronachs1 Auto
Message Property Stress_Message_Atronachs2 Auto
Message Property Stress_Message_Dwarven1 Auto
Message Property Stress_Message_Dwarven2 Auto
Message Property Stress_Message_Giants1 Auto
Message Property Stress_Message_Giants2 Auto
Message Property Stress_Message_Spriggans1 Auto
Message Property Stress_Message_Spriggans2 Auto
Message Property Stress_Message_Wispmothers1 Auto
Message Property Stress_Message_Wispmothers2 Auto

Message Property Stress_Message_Wolves1_ALT Auto
Message Property Stress_Message_Wolves2_ALT Auto
Message Property Stress_Message_Dragons1_ALT Auto
Message Property Stress_Message_Dragons2_ALT Auto
Message Property Stress_Message_Vampires1_ALT Auto
Message Property Stress_Message_Vampires2_ALT Auto
Message Property Stress_Message_Mages1_ALT Auto
Message Property Stress_Message_Mages2_ALT Auto
Message Property Stress_Message_Undead1_ALT Auto
Message Property Stress_Message_Undead2_ALT Auto
Message Property Stress_Message_Bandits1_ALT Auto
Message Property Stress_Message_Bandits2_ALT Auto
Message Property Stress_Message_Falmer1_ALT Auto
Message Property Stress_Message_Falmer2_ALT Auto
Message Property Stress_Message_Forsworn1_ALT Auto
Message Property Stress_Message_Forsworn2_ALT Auto
Message Property Stress_Message_Hagravens1_ALT Auto
Message Property Stress_Message_Hagravens2_ALT Auto
Message Property Stress_Message_Ghosts1_ALT Auto
Message Property Stress_Message_Ghosts2_ALT Auto
Message Property Stress_Message_Thalmor1_ALT Auto
Message Property Stress_Message_Thalmor2_ALT Auto
Message Property Stress_Message_Bears1_ALT Auto
Message Property Stress_Message_Bears2_ALT Auto
Message Property Stress_Message_Spiders1_ALT Auto
Message Property Stress_Message_Spiders2_ALT Auto
Message Property Stress_Message_Chaurus1_ALT Auto
Message Property Stress_Message_Chaurus2_ALT Auto
Message Property Stress_Message_Sabrecats1_ALT Auto
Message Property Stress_Message_Sabrecats2_ALT Auto
Message Property Stress_Message_IceWraiths1_ALT Auto
Message Property Stress_Message_IceWraiths2_ALT Auto
Message Property Stress_Message_Werewolves1_ALT Auto
Message Property Stress_Message_Werewolves2_ALT Auto
Message Property Stress_Message_Trolls1_ALT Auto
Message Property Stress_Message_Trolls2_ALT Auto
Message Property Stress_Message_Dremora1_ALT Auto
Message Property Stress_Message_Dremora2_ALT Auto
Message Property Stress_Message_Atronachs1_ALT Auto
Message Property Stress_Message_Atronachs2_ALT Auto
Message Property Stress_Message_Dwarven1_ALT Auto
Message Property Stress_Message_Dwarven2_ALT Auto
Message Property Stress_Message_Giants1_ALT Auto
Message Property Stress_Message_Giants2_ALT Auto
Message Property Stress_Message_Spriggans1_ALT Auto
Message Property Stress_Message_Spriggans2_ALT Auto
Message Property Stress_Message_Wispmothers1_ALT Auto
Message Property Stress_Message_Wispmothers2_ALT Auto

Sound Property UISkillIncrease Auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
Stress_LastAttacker.Revert()
EndEvent

; ---------------------------------------------------------------------------------------------------------------------------------------
; Stress and Fear 2026: the same behaviour as the original OnEffectFinish, written once instead of 24 times.
;
; At the end of a fight the original picks the kind of enemy that hurt the player last (first match of an ordered list of tests) and then runs
; one identical block of code per kind. Here the tests are one function, ClassifyAttacker, in the original order, and the block is two functions
; that read the spell, perk and messages of the kind from tables. Chances, health threshold, waits, sounds and messages are unchanged. The enemy's
; voice type and race are read once instead of up to three times each.
; ---------------------------------------------------------------------------------------------------------------------------------------

Spell[] fearSpells
Perk[] fearPerks
Message[] fearMessage1
Message[] fearMessage2
Message[] fearMessage1Alt
Message[] fearMessage2Alt
bool tablesBuilt = false

; Kind numbers, in the order the original tests them (the first matching test wins).
int Function ClassifyAttacker(Actor akAttacker)
	VoiceType attackerVoice = akAttacker.GetVoiceType()
	Race attackerRace = akAttacker.GetRace()

	If attackerVoice == CRWolfVoice
		Return 0 ; Wolves
	ElseIf akAttacker.HasKeyword(ActorTypeDragon)
		Return 1 ; Dragons
	ElseIf akAttacker.HasKeyword(Vampire)
		Return 2 ; Vampires
	ElseIf akAttacker.IsInFaction(WarlockFaction) || akAttacker.IsInFaction(NecromancerFaction)
		Return 3 ; Mages
	ElseIf akAttacker.HasKeyword(ActorTypeUndead)
		Return 4 ; Undead
	ElseIf akAttacker.IsInFaction(BanditFaction)
		Return 5 ; Bandits
	ElseIf akAttacker.IsInFaction(FalmerFaction)
		Return 6 ; Falmer
	ElseIf akAttacker.IsInFaction(ForswornFaction)
		Return 7 ; Forsworn
	ElseIf attackerRace == HagravenRace
		Return 8 ; Hagravens
	ElseIf akAttacker.HasKeyword(ActorTypeGhost) || akAttacker.IsGhost()
		Return 9 ; Ghosts
	ElseIf akAttacker.IsInFaction(ThalmorFaction)
		Return 10 ; Thalmor
	ElseIf attackerVoice == CRBearVoice
		Return 11 ; Bears
	ElseIf attackerVoice == CrFrostbiteSpiderVoice || attackerVoice == CrFrostbiteSpiderGiantVoice
		Return 12 ; Spiders
	ElseIf attackerRace == ChaurusRace || attackerRace == ChaurusReaperRace
		Return 13 ; Chaurus
	ElseIf attackerVoice == CrSabreCatVoice
		Return 14 ; Sabre cats
	ElseIf attackerRace == IceWraithRace
		Return 15 ; Ice wraiths
	ElseIf attackerRace == WerewolfRace
		Return 16 ; Werewolves
	ElseIf akAttacker.HasKeyword(ActorTypeTroll)
		Return 17 ; Trolls
	ElseIf attackerRace == DremoraRace
		Return 18 ; Dremora
	ElseIf attackerRace == AtronachStormRace || attackerRace == AtronachFrostRace || attackerRace == AtronachFlameRace
		Return 19 ; Atronachs
	ElseIf akAttacker.HasKeyword(ActorTypeDwarven) && attackerRace != DLC2AshSpawnRace
		Return 20 ; Dwarven automatons
	ElseIf akAttacker.HasKeyword(ActorTypeGiant)
		Return 21 ; Giants
	ElseIf attackerVoice == CrSprigganVoice
		Return 22 ; Spriggans
	ElseIf attackerRace == WispRace
		Return 23 ; Wispmothers
	EndIf
	Return -1
EndFunction

Function BuildTables()
	If tablesBuilt
		Return
	EndIf

	fearSpells = new Spell[24]
	fearSpells[0] = Stress_PhobiaSpell_Wolf
	fearSpells[1] = Stress_PhobiaSpell_Dragons
	fearSpells[2] = Stress_PhobiaSpell_Vampires
	fearSpells[3] = Stress_PhobiaSpell_Mages
	fearSpells[4] = Stress_PhobiaSpell_Undead
	fearSpells[5] = Stress_PhobiaSpell_Bandits
	fearSpells[6] = Stress_PhobiaSpell_Falmer
	fearSpells[7] = Stress_PhobiaSpell_Forsworn
	fearSpells[8] = Stress_PhobiaSpell_Hagravens
	fearSpells[9] = Stress_PhobiaSpell_Ghosts
	fearSpells[10] = Stress_PhobiaSpell_Thalmor
	fearSpells[11] = Stress_PhobiaSpell_Bears
	fearSpells[12] = Stress_PhobiaSpell_Spiders
	fearSpells[13] = Stress_PhobiaSpell_Chaurus
	fearSpells[14] = Stress_PhobiaSpell_Sabrecats
	fearSpells[15] = Stress_PhobiaSpell_IceWraiths
	fearSpells[16] = Stress_PhobiaSpell_Werewolves
	fearSpells[17] = Stress_PhobiaSpell_Trolls
	fearSpells[18] = Stress_PhobiaSpell_Dremora
	fearSpells[19] = Stress_PhobiaSpell_Atronachs
	fearSpells[20] = Stress_PhobiaSpell_Dwarven
	fearSpells[21] = Stress_PhobiaSpell_Giants
	fearSpells[22] = Stress_PhobiaSpell_Spriggans
	fearSpells[23] = Stress_PhobiaSpell_Wispmothers

	fearPerks = new Perk[24]
	fearPerks[0] = Stress_Champion_Wolf
	fearPerks[1] = Stress_Champion_Dragons
	fearPerks[2] = Stress_Champion_Vampires
	fearPerks[3] = Stress_Champion_Mages
	fearPerks[4] = Stress_Champion_Undead
	fearPerks[5] = Stress_Champion_Bandits
	fearPerks[6] = Stress_Champion_Falmer
	fearPerks[7] = Stress_Champion_Forsworn
	fearPerks[8] = Stress_Champion_Hagravens
	fearPerks[9] = Stress_Champion_Ghosts
	fearPerks[10] = Stress_Champion_Thalmor
	fearPerks[11] = Stress_Champion_Bears
	fearPerks[12] = Stress_Champion_Spiders
	fearPerks[13] = Stress_Champion_Chaurus
	fearPerks[14] = Stress_Champion_Sabrecats
	fearPerks[15] = Stress_Champion_IceWraiths
	fearPerks[16] = Stress_Champion_Werewolves
	fearPerks[17] = Stress_Champion_Trolls
	fearPerks[18] = Stress_Champion_Dremora
	fearPerks[19] = Stress_Champion_Atronachs
	fearPerks[20] = Stress_Champion_Dwarven
	fearPerks[21] = Stress_Champion_Giants
	fearPerks[22] = Stress_Champion_Spriggans
	fearPerks[23] = Stress_Champion_Wispmothers

	; Message 1 is shown when the fear starts, message 2 when it is overcome; the _ALT versions are the corner notifications.
	fearMessage1 = new Message[24]
	fearMessage1[0] = Stress_Message_Wolves1
	fearMessage1[1] = Stress_Message_Dragons1
	fearMessage1[2] = Stress_Message_Vampires1
	fearMessage1[3] = Stress_Message_Mages1
	fearMessage1[4] = Stress_Message_Undead1
	fearMessage1[5] = Stress_Message_Bandits1
	fearMessage1[6] = Stress_Message_Falmer1
	fearMessage1[7] = Stress_Message_Forsworn1
	fearMessage1[8] = Stress_Message_Hagravens1
	fearMessage1[9] = Stress_Message_Ghosts1
	fearMessage1[10] = Stress_Message_Thalmor1
	fearMessage1[11] = Stress_Message_Bears1
	fearMessage1[12] = Stress_Message_Spiders1
	fearMessage1[13] = Stress_Message_Chaurus1
	fearMessage1[14] = Stress_Message_Sabrecats1
	fearMessage1[15] = Stress_Message_IceWraiths1
	fearMessage1[16] = Stress_Message_Werewolves1
	fearMessage1[17] = Stress_Message_Trolls1
	fearMessage1[18] = Stress_Message_Dremora1
	fearMessage1[19] = Stress_Message_Atronachs1
	fearMessage1[20] = Stress_Message_Dwarven1
	fearMessage1[21] = Stress_Message_Giants1
	fearMessage1[22] = Stress_Message_Spriggans1
	fearMessage1[23] = Stress_Message_Wispmothers1

	fearMessage2 = new Message[24]
	fearMessage2[0] = Stress_Message_Wolves2
	fearMessage2[1] = Stress_Message_Dragons2
	fearMessage2[2] = Stress_Message_Vampires2
	fearMessage2[3] = Stress_Message_Mages2
	fearMessage2[4] = Stress_Message_Undead2
	fearMessage2[5] = Stress_Message_Bandits2
	fearMessage2[6] = Stress_Message_Falmer2
	fearMessage2[7] = Stress_Message_Forsworn2
	fearMessage2[8] = Stress_Message_Hagravens2
	fearMessage2[9] = Stress_Message_Ghosts2
	fearMessage2[10] = Stress_Message_Thalmor2
	fearMessage2[11] = Stress_Message_Bears2
	fearMessage2[12] = Stress_Message_Spiders2
	fearMessage2[13] = Stress_Message_Chaurus2
	fearMessage2[14] = Stress_Message_Sabrecats2
	fearMessage2[15] = Stress_Message_IceWraiths2
	fearMessage2[16] = Stress_Message_Werewolves2
	fearMessage2[17] = Stress_Message_Trolls2
	fearMessage2[18] = Stress_Message_Dremora2
	fearMessage2[19] = Stress_Message_Atronachs2
	fearMessage2[20] = Stress_Message_Dwarven2
	fearMessage2[21] = Stress_Message_Giants2
	fearMessage2[22] = Stress_Message_Spriggans2
	fearMessage2[23] = Stress_Message_Wispmothers2

	fearMessage1Alt = new Message[24]
	fearMessage1Alt[0] = Stress_Message_Wolves1_ALT
	fearMessage1Alt[1] = Stress_Message_Dragons1_ALT
	fearMessage1Alt[2] = Stress_Message_Vampires1_ALT
	fearMessage1Alt[3] = Stress_Message_Mages1_ALT
	fearMessage1Alt[4] = Stress_Message_Undead1_ALT
	fearMessage1Alt[5] = Stress_Message_Bandits1_ALT
	fearMessage1Alt[6] = Stress_Message_Falmer1_ALT
	fearMessage1Alt[7] = Stress_Message_Forsworn1_ALT
	fearMessage1Alt[8] = Stress_Message_Hagravens1_ALT
	fearMessage1Alt[9] = Stress_Message_Ghosts1_ALT
	fearMessage1Alt[10] = Stress_Message_Thalmor1_ALT
	fearMessage1Alt[11] = Stress_Message_Bears1_ALT
	fearMessage1Alt[12] = Stress_Message_Spiders1_ALT
	fearMessage1Alt[13] = Stress_Message_Chaurus1_ALT
	fearMessage1Alt[14] = Stress_Message_Sabrecats1_ALT
	fearMessage1Alt[15] = Stress_Message_IceWraiths1_ALT
	fearMessage1Alt[16] = Stress_Message_Werewolves1_ALT
	fearMessage1Alt[17] = Stress_Message_Trolls1_ALT
	fearMessage1Alt[18] = Stress_Message_Dremora1_ALT
	fearMessage1Alt[19] = Stress_Message_Atronachs1_ALT
	fearMessage1Alt[20] = Stress_Message_Dwarven1_ALT
	fearMessage1Alt[21] = Stress_Message_Giants1_ALT
	fearMessage1Alt[22] = Stress_Message_Spriggans1_ALT
	fearMessage1Alt[23] = Stress_Message_Wispmothers1_ALT

	fearMessage2Alt = new Message[24]
	fearMessage2Alt[0] = Stress_Message_Wolves2_ALT
	fearMessage2Alt[1] = Stress_Message_Dragons2_ALT
	fearMessage2Alt[2] = Stress_Message_Vampires2_ALT
	fearMessage2Alt[3] = Stress_Message_Mages2_ALT
	fearMessage2Alt[4] = Stress_Message_Undead2_ALT
	fearMessage2Alt[5] = Stress_Message_Bandits2_ALT
	fearMessage2Alt[6] = Stress_Message_Falmer2_ALT
	fearMessage2Alt[7] = Stress_Message_Forsworn2_ALT
	fearMessage2Alt[8] = Stress_Message_Hagravens2_ALT
	fearMessage2Alt[9] = Stress_Message_Ghosts2_ALT
	fearMessage2Alt[10] = Stress_Message_Thalmor2_ALT
	fearMessage2Alt[11] = Stress_Message_Bears2_ALT
	fearMessage2Alt[12] = Stress_Message_Spiders2_ALT
	fearMessage2Alt[13] = Stress_Message_Chaurus2_ALT
	fearMessage2Alt[14] = Stress_Message_Sabrecats2_ALT
	fearMessage2Alt[15] = Stress_Message_IceWraiths2_ALT
	fearMessage2Alt[16] = Stress_Message_Werewolves2_ALT
	fearMessage2Alt[17] = Stress_Message_Trolls2_ALT
	fearMessage2Alt[18] = Stress_Message_Dremora2_ALT
	fearMessage2Alt[19] = Stress_Message_Atronachs2_ALT
	fearMessage2Alt[20] = Stress_Message_Dwarven2_ALT
	fearMessage2Alt[21] = Stress_Message_Giants2_ALT
	fearMessage2Alt[22] = Stress_Message_Spriggans2_ALT
	fearMessage2Alt[23] = Stress_Message_Wispmothers2_ALT

	tablesBuilt = true
EndFunction

; The player overcomes the fear of this kind of enemy: the permanent perk, the spell gone, a short pause, the sound, the message, and 50 points of stress freed.
Function OvercomeFear(int aiKind)
	PlayerRef.AddPerk(fearPerks[aiKind])
	PlayerRef.RemoveSpell(fearSpells[aiKind])
	Utility.Wait(3)
	UISkillIncrease.Play(PlayerRef)
	Utility.Wait(0.3)
	If Stress_BoxMessages.GetValue() == 1
		fearMessage2[aiKind].Show()
	Else
		fearMessage2Alt[aiKind].Show()
	EndIf
	PlayerRef.EquipItem(Stress_Potion_Reduce50, false, true)
EndFunction

; The player has barely survived this kind of enemy: the fear spell, the screen effect, the trauma line, and the message.
Function DevelopFear(int aiKind)
	PlayerRef.AddSpell(fearSpells[aiKind], false)
	Stress_PhobiaImageEffect.Apply()
	Utility.Wait(2.5)
	PlayerRef.Say(Stress_Quest_Trauma)
	Utility.Wait(2.5)
	If Stress_BoxMessages.GetValue() == 1
		fearMessage1[aiKind].Show()
	Else
		fearMessage1Alt[aiKind].Show()
	EndIf
EndFunction

Event OnEffectFinish(Actor akTarget, Actor akCaster)
	int random = Utility.RandomInt(1, 11)

	If random > 8 && Phobias_Enabled.GetValue() > 0 && PlayerRef.IsDead() == false
		Actor lastAttacker = Stress_LastAttacker.GetAt(0) as Actor
		If lastAttacker == None
			Return
		EndIf

		int kind = ClassifyAttacker(lastAttacker)
		If kind < 0
			Return
		EndIf

		BuildTables()
		If random > 9 && PlayerRef.HasSpell(fearSpells[kind])
			OvercomeFear(kind)
		ElseIf PlayerRef.HasPerk(fearPerks[kind]) == false && PlayerRef.GetActorValuePercentage("Health") < 0.25 && PlayerRef.HasSpell(fearSpells[kind]) == false
			DevelopFear(kind)
		EndIf
	EndIf
EndEvent
