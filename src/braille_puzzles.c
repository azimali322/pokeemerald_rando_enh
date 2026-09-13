#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "script.h"
#include "sound.h"
#include "task.h"
#include "constants/field_effects.h"
#include "constants/songs.h"
#include "constants/metatile_labels.h"
#include "fieldmap.h"
#include "party_menu.h"
#include "fldeff.h"
#include "tx_randomizer_and_challenges.h"

// Which wall the pending FLDEFF_USE_TOMB_PUZZLE_EFFECT is about to open.
enum {
    BRAILLE_PUZZLE_REGIROCK,
    BRAILLE_PUZZLE_REGISTEEL,
    BRAILLE_PUZZLE_REGICE,
    BRAILLE_PUZZLE_SEALED_CHAMBER,
};

EWRAM_DATA static u8 sBraillePuzzleId = 0;

static const u8 sRegicePathCoords[][2] =
{
    {4,  21},
    {5,  21},
    {6,  21},
    {7,  21},
    {8,  21},
    {9,  21},
    {10, 21},
    {11, 21},
    {12, 21},
    {12, 22},
    {12, 23},
    {13, 23},
    {13, 24},
    {13, 25},
    {13, 26},
    {13, 27},
    {12, 27},
    {12, 28},
    {4,  29},
    {5,  29},
    {6,  29},
    {7,  29},
    {8,  29},
    {9,  29},
    {10, 29},
    {11, 29},
    {12, 29},
    {4,  28},
    {4,  27},
    {3,  27},
    {3,  26},
    {3,  25},
    {3,  24},
    {3,  23},
    {4,  23},
    {4,  22},
};

static void Task_SealedChamberShakingEffect(u8);
static void DoBrailleRegirockEffect(void);
static void DoBrailleRegisteelEffect(void);
static void DoBrailleRegiceEffect(void);
static void OpenRegiChamberWall(u16 flagToSet);
static void UseRegiceHm_Callback(void);
static void UseSealedChamberHm_Callback(void);
static bool8 IsPlayerOnMap(u8 mapGroup, u8 mapNum);

bool8 ShouldDoBrailleDigEffect(void)
{
    if (!FlagGet(FLAG_SYS_BRAILLE_DIG)
     && (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(SEALED_CHAMBER_OUTER_ROOM)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEALED_CHAMBER_OUTER_ROOM)))
    {
        if (gSaveBlock1Ptr->pos.x == 10 && gSaveBlock1Ptr->pos.y == 3)
            return TRUE;
        if (gSaveBlock1Ptr->pos.x == 9 && gSaveBlock1Ptr->pos.y == 3)
            return TRUE;
        if (gSaveBlock1Ptr->pos.x == 11 && gSaveBlock1Ptr->pos.y == 3)
            return TRUE;
    }

    return FALSE;
}

void DoBrailleDigEffect(void)
{
    MapGridSetMetatileIdAt( 9 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopLeft);
    MapGridSetMetatileIdAt(10 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopMid);
    MapGridSetMetatileIdAt(11 + MAP_OFFSET, 1 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopRight);
    MapGridSetMetatileIdAt( 9 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomLeft | MAPGRID_COLLISION_MASK);
    MapGridSetMetatileIdAt(10 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomMid);
    MapGridSetMetatileIdAt(11 + MAP_OFFSET, 2 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomRight | MAPGRID_COLLISION_MASK);
    DrawWholeMapView();
    PlaySE(SE_BANG);
    FlagSet(FLAG_SYS_BRAILLE_DIG);
    UnlockPlayerFieldControls();
}

bool8 CheckRelicanthWailord(void)
{
    // tx_randomizer_and_challenges: a Magnezone/Vibrava party is not a reasonable ask once
    // species are randomized and a Nuzlocke gives no second chances. See IsRegiUnlockRelaxed.
    if (IsRegiUnlockRelaxed())
        return TRUE;

    // First comes Magnezone
    if (GetMonData(&gPlayerParty[0], MON_DATA_SPECIES_OR_EGG, 0) == SPECIES_MAGNEZONE)
    {
        CalculatePlayerPartyCount();
        // Last comes Vibrava
        if (GetMonData(&gPlayerParty[gPlayerPartyCount - 1], MON_DATA_SPECIES_OR_EGG, 0) == SPECIES_VIBRAVA)
            return TRUE;
    }
    return FALSE;
}

// THEORY: this was caused by block commenting out all of the older R/S braille functions but leaving the call to it itself, which creates the nullsub.
void ShouldDoBrailleRegirockEffectOld(void)
{
}

#define tDelayCounter  data[1]
#define tShakeCounter  data[2]
#define tVerticalPan   data[4]
#define tDelay         data[5]
#define tNumShakes     data[6]

void DoSealedChamberShakingEffect_Long(void)
{
    u8 taskId = CreateTask(Task_SealedChamberShakingEffect, 9);

    gTasks[taskId].tDelayCounter = 0;
    gTasks[taskId].tShakeCounter = 0;
    gTasks[taskId].tVerticalPan = 2;
    gTasks[taskId].tDelay = 5;
    gTasks[taskId].tNumShakes = 50;
    SetCameraPanningCallback(0);
}

void DoSealedChamberShakingEffect_Short(void)
{
    u8 taskId = CreateTask(Task_SealedChamberShakingEffect, 9);

    gTasks[taskId].tDelayCounter = 0;
    gTasks[taskId].tShakeCounter = 0;
    gTasks[taskId].tVerticalPan = 3;
    gTasks[taskId].tDelay = 5;
    gTasks[taskId].tNumShakes = 2;
    SetCameraPanningCallback(0);
}

static void Task_SealedChamberShakingEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->tDelayCounter++;
    if (task->tDelayCounter % task->tDelay == 0)
    {
        task->tDelayCounter = 0;
        task->tShakeCounter++;
        task->tVerticalPan = -task->tVerticalPan;
        SetCameraPanning(0, task->tVerticalPan);
        if (task->tShakeCounter == task->tNumShakes)
        {
            DestroyTask(taskId);
            ScriptContext_Enable();
            InstallCameraPanAheadCallback();
        }
    }
}

#undef tDelayCounter
#undef tShakeCounter
#undef tVerticalPan
#undef tDelay
#undef tNumShakes

bool8 ShouldDoBrailleRegirockEffect(void)
{
    if (!FlagGet(FLAG_SYS_REGIROCK_PUZZLE_COMPLETED)
        && gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(DESERT_RUINS)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(DESERT_RUINS))
    {
        if (gSaveBlock1Ptr->pos.x == 6 && gSaveBlock1Ptr->pos.y == 23)
        {
            sBraillePuzzleId = BRAILLE_PUZZLE_REGIROCK;
            return TRUE;
        }
        else if (gSaveBlock1Ptr->pos.x == 5 && gSaveBlock1Ptr->pos.y == 23)
        {
            sBraillePuzzleId = BRAILLE_PUZZLE_REGIROCK;
            return TRUE;
        }
        else if (gSaveBlock1Ptr->pos.x == 7 && gSaveBlock1Ptr->pos.y == 23)
        {
            sBraillePuzzleId = BRAILLE_PUZZLE_REGIROCK;
            return TRUE;
        }
    }

    return FALSE;
}

void SetUpPuzzleEffectRegirock(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    FieldEffectStart(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
}

void UseRegirockHm_Callback(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
    DoBrailleRegirockEffect();
}

// All three Regi chambers share a layout, so opening any of their walls is the same work
// apart from which flag records it.
static void OpenRegiChamberWall(u16 flagToSet)
{
    MapGridSetMetatileIdAt(7 + MAP_OFFSET, 19 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopLeft);
    MapGridSetMetatileIdAt(8 + MAP_OFFSET, 19 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopMid);
    MapGridSetMetatileIdAt(9 + MAP_OFFSET, 19 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_TopRight);
    MapGridSetMetatileIdAt(7 + MAP_OFFSET, 20 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomLeft | MAPGRID_COLLISION_MASK);
    MapGridSetMetatileIdAt(8 + MAP_OFFSET, 20 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomMid);
    MapGridSetMetatileIdAt(9 + MAP_OFFSET, 20 + MAP_OFFSET, METATILE_Cave_SealedChamberEntrance_BottomRight | MAPGRID_COLLISION_MASK);
    DrawWholeMapView();
    PlaySE(SE_BANG);
    FlagSet(flagToSet);
    UnlockPlayerFieldControls();
    UnfreezeObjectEvents();
}

static void DoBrailleRegirockEffect(void)
{
    OpenRegiChamberWall(FLAG_SYS_REGIROCK_PUZZLE_COMPLETED);
}

bool8 ShouldDoBrailleRegisteelEffect(void)
{
    if (!FlagGet(FLAG_SYS_REGISTEEL_PUZZLE_COMPLETED) && (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(ANCIENT_TOMB) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(ANCIENT_TOMB)))
    {
        if (gSaveBlock1Ptr->pos.x == 8 && gSaveBlock1Ptr->pos.y == 25)
        {
            sBraillePuzzleId = BRAILLE_PUZZLE_REGISTEEL;
            return TRUE;
        }
    }
    return FALSE;
}

void SetUpPuzzleEffectRegisteel(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    FieldEffectStart(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
}

void UseRegisteelHm_Callback(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
    DoBrailleRegisteelEffect();
}

static void DoBrailleRegisteelEffect(void)
{
    OpenRegiChamberWall(FLAG_SYS_REGISTEEL_PUZZLE_COMPLETED);
}

static void DoBrailleRegiceEffect(void)
{
    OpenRegiChamberWall(FLAG_SYS_BRAILLE_REGICE_COMPLETED);
}

static void UseRegiceHm_Callback(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
    DoBrailleRegiceEffect();
}

static void UseSealedChamberHm_Callback(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
    DoBrailleDigEffect();
    UnfreezeObjectEvents();
}

static bool8 IsPlayerOnMap(u8 mapGroup, u8 mapNum)
{
    return (gSaveBlock1Ptr->location.mapGroup == mapGroup
         && gSaveBlock1Ptr->location.mapNum == mapNum);
}

// tx_randomizer_and_challenges: under randomizer + Nuzlocke, Flash opens any Regi wall the
// player is standing in, from anywhere in the room - Sweet Scent, the Magnezone/Vibrava
// party and Regice's lap are all luck-dependent once species and learnsets are randomized.
// The vanilla methods still work; this only adds a route that is always available, because
// HMs are never randomized.
bool8 ShouldDoBrailleFlashUnlock(void)
{
    if (!IsRegiUnlockRelaxed())
        return FALSE;

    if (IsPlayerOnMap(MAP_GROUP(DESERT_RUINS), MAP_NUM(DESERT_RUINS))
        && !FlagGet(FLAG_SYS_REGIROCK_PUZZLE_COMPLETED))
    {
        sBraillePuzzleId = BRAILLE_PUZZLE_REGIROCK;
        return TRUE;
    }
    if (IsPlayerOnMap(MAP_GROUP(ANCIENT_TOMB), MAP_NUM(ANCIENT_TOMB))
        && !FlagGet(FLAG_SYS_REGISTEEL_PUZZLE_COMPLETED))
    {
        sBraillePuzzleId = BRAILLE_PUZZLE_REGISTEEL;
        return TRUE;
    }
    if (IsPlayerOnMap(MAP_GROUP(ISLAND_CAVE), MAP_NUM(ISLAND_CAVE))
        && !FlagGet(FLAG_SYS_BRAILLE_REGICE_COMPLETED))
    {
        sBraillePuzzleId = BRAILLE_PUZZLE_REGICE;
        return TRUE;
    }
    if (IsPlayerOnMap(MAP_GROUP(SEALED_CHAMBER_OUTER_ROOM), MAP_NUM(SEALED_CHAMBER_OUTER_ROOM))
        && !FlagGet(FLAG_SYS_BRAILLE_DIG))
    {
        sBraillePuzzleId = BRAILLE_PUZZLE_SEALED_CHAMBER;
        return TRUE;
    }

    return FALSE;
}

void SetUpPuzzleEffectFlashUnlock(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    FieldEffectStart(FLDEFF_USE_TOMB_PUZZLE_EFFECT);
}

// theory: another commented out DoBrailleWait and Task_BrailleWait.
static void UNUSED DoBrailleWait(void)
{
}

// this used to be FldEff_UseFlyAncientTomb . why did GF merge the 2 functions?
bool8 FldEff_UsePuzzleEffect(void)
{
    u8 taskId = CreateFieldMoveTask();
    void (*callback)(void);

    switch (sBraillePuzzleId)
    {
    case BRAILLE_PUZZLE_REGISTEEL:
        callback = UseRegisteelHm_Callback;
        break;
    case BRAILLE_PUZZLE_REGICE:
        callback = UseRegiceHm_Callback;
        break;
    case BRAILLE_PUZZLE_SEALED_CHAMBER:
        callback = UseSealedChamberHm_Callback;
        break;
    case BRAILLE_PUZZLE_REGIROCK:
    default:
        callback = UseRegirockHm_Callback;
        break;
    }
    gTasks[taskId].data[8] = (u32)callback >> 16;
    gTasks[taskId].data[9] = (u32)callback;
    return FALSE;
}

// The puzzle to unlock Regice's cave requires the player to interact with the braille message on the back wall,
// step on every space on the perimeter of the cave (and only those spaces) then return to the back wall.
bool8 ShouldDoBrailleRegicePuzzle(void)
{
    u8 i;

    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(ISLAND_CAVE)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(ISLAND_CAVE))
    {
        if (FlagGet(FLAG_SYS_BRAILLE_REGICE_COMPLETED))
            return FALSE;
        // Set when the player interacts with the braille message
        if (FlagGet(FLAG_TEMP_REGICE_PUZZLE_STARTED) == FALSE)
            return FALSE;
        // Cleared when the player interacts with the braille message
        if (FlagGet(FLAG_TEMP_REGICE_PUZZLE_FAILED) == TRUE)
            return FALSE;

        for (i = 0; i < ARRAY_COUNT(sRegicePathCoords); i++)
        {
            u8 xPos = sRegicePathCoords[i][0];
            u8 yPos = sRegicePathCoords[i][1];
            if (gSaveBlock1Ptr->pos.x == xPos && gSaveBlock1Ptr->pos.y == yPos)
            {
                // Player is standing on a correct space, set the corresponding bit
                if (i < 16)
                {
                    u16 val = VarGet(VAR_REGICE_STEPS_1);
                    val |= 1 << i;
                    VarSet(VAR_REGICE_STEPS_1, val);
                }
                else if (i < 32)
                {
                    u16 val = VarGet(VAR_REGICE_STEPS_2);
                    val |= 1 << (i - 16);
                    VarSet(VAR_REGICE_STEPS_2, val);
                }
                else
                {
                    u16 val = VarGet(VAR_REGICE_STEPS_3);
                    val |= 1 << (i - 32);
                    VarSet(VAR_REGICE_STEPS_3, val);
                }

                // Make sure a full lap has been completed. There are 36 steps in a lap, so 16+16+4 bits to check across the 3 vars.
                if (VarGet(VAR_REGICE_STEPS_1) != 0xFFFF || VarGet(VAR_REGICE_STEPS_2) != 0xFFFF || VarGet(VAR_REGICE_STEPS_3) != 0xF)
                    return FALSE;

                // A lap has been completed, the puzzle is complete when the player returns to the braille message.
                if (gSaveBlock1Ptr->pos.x == 8 && gSaveBlock1Ptr->pos.y == 21)
                    return TRUE;
                else
                    return FALSE;
            }
        }

        // Player stepped on an incorrect space, puzzle failed.
        FlagSet(FLAG_TEMP_REGICE_PUZZLE_FAILED);
        FlagClear(FLAG_TEMP_REGICE_PUZZLE_STARTED);
    }

    return FALSE;
}
