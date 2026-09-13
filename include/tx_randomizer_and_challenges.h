#ifndef GUARD_DIFFICULTY_H
#define GUARD_DIFFICULTY_H
//tx_randomizer_and_challenges

// Balancing modes for tx_Random_Similar
#define TX_SIMILAR_OFF        0
#define TX_SIMILAR_BALANCED   1   // match by evolution stage
#define TX_SIMILAR_IMPROVED   2   // match by base stat total

// Gamemode preset the new-game menu opens on: 0 classic, 1 modern, 2 custom.
// Modern also forces the rest of the MODE page on (reusable TMs, modern types, Fairy,
// modern movepool, new stats, ...). It does not touch the features or randomizer pages.
#define TX_MODE_GAMEMODE                1

// default options set by the dev
#define TX_RANDOMIZER_ON                TRUE
#define TX_RANDOM_STARTER               TRUE
#define TX_RANDOM_WILD_POKEMON          TRUE
#define TX_RANDOM_TRAINER               TRUE
#define TX_RANDOM_STATIC                TRUE
#define TX_RANDOM_SIMILAR               TX_SIMILAR_IMPROVED
#define TX_RANDOM_MAP_BASED             TRUE
#define TX_RANDOM_INCLUDE_LEGENDARIES   TRUE
#define TX_RANDOM_TYPE                  FALSE
#define TX_RANDOM_MOVES                 TRUE
#define TX_RANDOM_ABILITIES             TRUE
#define TX_RANDOM_EVOLUTION             FALSE
#define TX_RANDOM_EVOLUTION_METHODE     FALSE
#define TX_RANDOM_TYPE_EFFECTIVENESS    FALSE
#define TX_RANDOM_ITEMS                 TRUE
#define TX_RANDOM_CHAOS_MODE            FALSE

// randomizer enhancements
#define TX_RANDOM_WILD_LEVEL_SCALED     TRUE
#define TX_RANDOM_LEGENDARIES           TRUE
#define TX_RANDOM_MOVES_VGC             TX_VGC_WEIGHTED
#define TX_RANDOM_GUARANTEE_STAB        FALSE
#define TX_RANDOM_ABILITIES_VGC         TX_VGC_WEIGHTED
#define TX_RANDOM_ITEMS_VGC             TX_VGC_WEIGHTED
#define TX_RANDOM_LEARNSETS             TRUE
#define TX_RANDOM_TMS_VGC               TX_VGC_WEIGHTED
#define TX_RANDOM_TM_MOVES              TRUE
#define TX_RANDOM_TM_COMPAT             TRUE    // TRUE = every Pokemon can learn every TM/HM
#define TX_FEATURES_CHEAP_BALLS         TRUE
#define TX_FEATURES_REROLL_CHEAT        TRUE

// VGC weighting modes, shared by the moves/abilities/items/TMs options
#define TX_VGC_OFF          0
#define TX_VGC_WEIGHTED     1
#define TX_VGC_STRICT       2

#define TX_NUZLOCKE_NUZLOCKE 1          // Normal (Easy and Hardcore both off)
#define TX_NUZLOCKE_NUZLOCKE_HARDCORE 0 //CAREFULL!!!!!
#define TX_NUZLOCKE_SPECIES_CLAUSE 1
#define TX_NUZLOCKE_SHINY_CLAUSE 1
#define TX_NUZLOCKE_NICKNAMING 0
#define TX_NUZLOCKE_DELETION 0

#define TX_DIFFICULTY_PARTY_LIMIT 0
#define TX_DIFFICULTY_LEVEL_CAP 1        // 0 off, 1 normal, 2 hard
#define TX_DIFFICULTY_EXP_MULTIPLIER 2   // 0 x1.0, 1 x1.5, 2 x2.0, 3 x0.0
#define TX_DIFFICULTY_NO_ITEM_PLAYER 1   // 0 Yes (items allowed), 1 No
#define TX_DIFFICULTY_NO_ITEM_TRAINER 0
#define TX_DIFFICULTY_NO_EVS 0
#define TX_DIFFICULTY_SCALING_IVS 0
#define TX_DIFFICULTY_SCALING_EVS 1      // 0 off, 1 Scale, 2 Hard, 3 Extreme
#define TX_DIFFICULTY_PKMN_CENTER 0 //0 no limit, 1 none
#define TX_DIFFICULTY_LIMIT_DIFFICULTY 0
#define TX_DIFFICULTY_MAX_PARTY_IVS 2    // 0 "Yes", 1 "No", 2 "No (HP)" -- IVs 30-31

#define TX_CHALLENGE_EVO_LIMIT 0 //0 off, 1 first, 2 none
#define TX_CHALLENGE_BASE_STAT_EQUALIZER 0 //0=off, 1=100, 2=255, 3=500
#define TX_CHALLENGE_TYPE_OFF 31
#define TX_CHALLENGE_TYPE TX_CHALLENGE_TYPE_OFF //TX_CHALLENGE_TYPE_OFF for off
#define TX_CHALLENGE_MIRROR 0
#define TX_CHALLENGE_PCHEAL 0
#define TX_CHALLENGE_MIRROR_THIEF 0
#define TX_CHALLENGES_LESS_ESCAPES 0
#define TX_MODE_ALTERNATE_SPAWNS 0
#define TX_FEATURES_SHINY_CHANCE 4       // 0 1/8192 .. 4 1/512
#define TX_FEATURES_ITEM_DROP 1
#define TX_MODE_INFINITE_TMS 0
#define TX_MODE_SURVIVE_POISON 0
#define TX_FEATURES_EASIER_FEEBAS 1
#define TX_NUZLOCKE_MINI_MODE 0
#define TX_FEATURES_RTC_TYPE 1           // 0 real RTC, 1 fake RTC
#define TX_FEATURES_WT 1
#define TX_FEATURES_UNLIMITED_WT 1
#define TX_MODE_NEW_SYNCHRONIZE 0
#define TX_MODE_MINTS 0
#define TX_MODE_NEW_CITRUS 0
#define TX_MODE_MODERN_TYPES 0
#define TX_MODE_FAIRY_TYPES 0
#define TX_MODE_NEW_STATS 0
#define TX_MODE_STURDY 0
#define TX_MODE_MODERN_MOVES 0
#define TX_MODE_LEGENDARY_ABILITIES 0
#define TX_MODE_NEW_LEGENDARIES 0
#define TX_CHALLENGES_EXPENSIVE 0
#define TX_DIFFICULTY_ESCAPE_ROPE_DIG 0
#define TX_DIFFICULTY_CATCH_RATE 3       // 0 normal, 1 half, 2 double, 3 triple
#define TX_DIFFICULTY_HARD_EXP 1         // 0 half EXP in Hard, 1 normal EXP
#define TX_FEATURES_FRONTIER_BANS 0
#define TX_MODE_TYPE_EFFECTIVENESS 0
#define TX_FEATURES_SHINY_COLORS 1

// randomization types
#define TX_RANDOM_T_WILD_POKEMON    0
#define TX_RANDOM_T_TRAINER         1
#define TX_RANDOM_T_MOVES           2
#define TX_RANDOM_T_ABILITY         3
#define TX_RANDOM_T_EVO             4
#define TX_RANDOM_T_EVO_METH        5
#define TX_RANDOM_T_STATIC          6

void CB2_InitTxRandomizerChallengesMenu(void);
void Task_ChooseChallenge_NoNewGame(u8 taskId);
void SaveData_TxRandomizerAndChallenges(void);

bool8 IsRandomizerActivated(void);
bool8 IsOneTypeChallengeActive(void);
bool8 AreAnyChallengesActive(void);
bool8 AreFeaturesActivated(void);
bool8 IsNuzlockeActive(void);
bool8 IsRegiUnlockRelaxed(void);
bool8 IsNuzlockeNicknamingActive(void);
bool8 IsPokecenterChallengeActivated(void);
bool8 IsLevelCapActive(void);
bool8 IsRandomItemsActivated(void);
bool8 HMsOverwriteOptionActive(void);

u8 NuzlockeFlagGet(u16 mapsec);
u8 NuzlockeFlagSet(u16 mapsec);
u8 NuzlockeFlagClear(u16 mapsec);
void NuzlockeDeletePartyMon(u8 position);
void NuzlockeDeletePartyMonOption(u8 position);
void NuzlockeDeleteFaintedPartyPokemon(void);
u8 GetMaxPartySize(void);
u8 GetCurrentPartyLevelCap(void);
u8 GetCurrentTrainerIVs(void);
u8 GetCurrentTrainerEVs(void);

void PrintTXSaveData(void);
void TestRandomizerValues(u8 type);

// constants
#define TX_MENU_ITEMS_PER_PAGE 6

extern struct tx_randomizer_OptionsMenu *sRandomizerOptions;
extern struct tx_challenges_OptionsMenu *sChallengesOptions;

#define TX_EXP_MULTIPLER_ONLY_ON_NUZLOCKE_AND_RANDOMIZER FALSE

#define TX_NUZLOCKE_CEMETERY_ICON_GRAY TRUE

#endif // GUARD_DIFFICULTY_H