#include "global.h"
#include "decompress.h"
#include "graphics.h"
#include "item_icon.h"
#include "malloc.h"
#include "sprite.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "data.h"
#include "party_menu.h"
#include "tx_randomizer_and_challenges.h"

// EWRAM vars
EWRAM_DATA u8 *gItemIconDecompressionBuffer = NULL;
EWRAM_DATA u8 *gItemIcon4x4Buffer = NULL;

// const rom data
#include "data/item_icon_table.h"

static const struct OamData sOamData_ItemIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 2,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_ItemIcon[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_ItemIcon[] =
{
    sSpriteAnim_ItemIcon
};

const struct SpriteTemplate gItemIconSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &sOamData_ItemIcon,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// code
bool8 AllocItemIconTemporaryBuffers(void)
{
    gItemIconDecompressionBuffer = Alloc(0x120);
    if (gItemIconDecompressionBuffer == NULL)
        return FALSE;

    gItemIcon4x4Buffer = AllocZeroed(0x200);
    if (gItemIcon4x4Buffer == NULL)
    {
        Free(gItemIconDecompressionBuffer);
        return FALSE;
    }

    return TRUE;
}

void FreeItemIconTemporaryBuffers(void)
{
    Free(gItemIconDecompressionBuffer);
    Free(gItemIcon4x4Buffer);
}

void CopyItemIconPicTo4x4Buffer(const void *src, void *dest)
{
    u8 i;

    for (i = 0; i < 3; i++)
        CpuCopy16(src + i * 96, dest + i * 128, 0x60);
}

u8 AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (!AllocItemIconTemporaryBuffers())
    {
        return MAX_SPRITES;
    }
    else
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        LZDecompressWram(GetItemIconPicOrPalette(itemId, 0), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPicOrPalette(itemId, 1);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(&gItemIconSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

        FreeItemIconTemporaryBuffers();
        Free(spriteTemplate);

        return spriteId;
    }
}

u8 AddCustomItemIconSprite(const struct SpriteTemplate *customSpriteTemplate, u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (!AllocItemIconTemporaryBuffers())
    {
        return MAX_SPRITES;
    }
    else
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        LZDecompressWram(GetItemIconPicOrPalette(itemId, 0), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPicOrPalette(itemId, 1);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(customSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

        FreeItemIconTemporaryBuffers();
        Free(spriteTemplate);

        return spriteId;
    }
}

//tx_randomizer_and_challenges
// Every TM shares one disc graphic and is told apart by a type-coloured palette, so a randomized
// TM kept the colour of the move it taught in the base game. This picks the palette from the move
// it teaches now.
//
// Bug, Fairy and Mystery have no TM palette -- no base-game TM teaches those types -- so they fall
// back to the Normal disc rather than reading a null entry.
static const u32 *const sTMPaletteByType[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL]   = gItemIconPalette_NormalTMHM,
    [TYPE_FIGHTING] = gItemIconPalette_FightingTMHM,
    [TYPE_FLYING]   = gItemIconPalette_FlyingTMHM,
    [TYPE_POISON]   = gItemIconPalette_PoisonTMHM,
    [TYPE_GROUND]   = gItemIconPalette_GroundTMHM,
    [TYPE_ROCK]     = gItemIconPalette_RockTMHM,
    [TYPE_GHOST]    = gItemIconPalette_GhostTMHM,
    [TYPE_STEEL]    = gItemIconPalette_SteelTMHM,
    [TYPE_FIRE]     = gItemIconPalette_FireTMHM,
    [TYPE_WATER]    = gItemIconPalette_WaterTMHM,
    [TYPE_GRASS]    = gItemIconPalette_GrassTMHM,
    [TYPE_ELECTRIC] = gItemIconPalette_ElectricTMHM,
    [TYPE_PSYCHIC]  = gItemIconPalette_PsychicTMHM,
    [TYPE_ICE]      = gItemIconPalette_IceTMHM,
    [TYPE_DRAGON]   = gItemIconPalette_DragonTMHM,
    [TYPE_DARK]     = gItemIconPalette_DarkTMHM,
};

static const void *GetRandomizedTMPalette(u16 itemId)
{
    u16 move = ItemIdToBattleMoveId(itemId);
    u8 type;

    if (move == MOVE_NONE || move >= MOVES_COUNT)
        return gItemIconTable[itemId][1];

    type = gBattleMoves[move].type;
    if (type >= NUMBER_OF_MON_TYPES || sTMPaletteByType[type] == NULL)
        return gItemIconPalette_NormalTMHM;

    return sTMPaletteByType[type];
}

const void *GetItemIconPicOrPalette(u16 itemId, u8 which)
{
    if (itemId == ITEM_LIST_END)
        itemId = ITEMS_COUNT; // Use last icon, the "return to field" arrow
    else if (itemId >= ITEMS_COUNT)
        itemId = 0;

    //tx_randomizer_and_challenges: palette only, and only for TMs. HMs are never randomized.
    if (which == 1
        && gSaveBlock1Ptr->tx_Random_TMs
        && itemId >= ITEM_TM01 && itemId < ITEM_TM01 + NUM_TECHNICAL_MACHINES)
        return GetRandomizedTMPalette(itemId);

    return gItemIconTable[itemId][which];
}
