// Generated for the berry-tree randomizer -- see docs/tiering/BERRIES.md.
//
// Split on whether the berry does anything when held, which is the line the item data already
// draws: 22 berries have a holdEffect, 21 have none. The 22 are then graded by how much that
// effect is worth in a fight, and the 21 sit together at the bottom -- reachable, but rarely.

// Tier 1 -- Battle staples (5 berries, 36.70% weight, 7.340% each)
static const u16 sBerryTier1[] =
{
    ITEM_LUM_BERRY,
    ITEM_SITRUS_BERRY,
    ITEM_SALAC_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_PETAYA_BERRY,
};

// Tier 2 -- Strong situational (6 berries, 27.50% weight, 4.583% each)
static const u16 sBerryTier2[] =
{
    ITEM_CHESTO_BERRY,
    ITEM_LEPPA_BERRY,
    ITEM_GANLON_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_LANSAT_BERRY,
};

// Tier 3 -- Single status cure (6 berries, 17.00% weight, 2.833% each)
static const u16 sBerryTier3[] =
{
    ITEM_CHERI_BERRY,
    ITEM_PERSIM_BERRY,
    ITEM_ORAN_BERRY,
    ITEM_PECHA_BERRY,
    ITEM_RAWST_BERRY,
    ITEM_ASPEAR_BERRY,
};

// Tier 4 -- Pinch heal, may confuse (5 berries, 9.20% weight, 1.840% each)
static const u16 sBerryTier4[] =
{
    ITEM_FIGY_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_MAGO_BERRY,
    ITEM_AGUAV_BERRY,
    ITEM_IAPAPA_BERRY,
};

// Tier 5 -- No hold effect (21 berries, 9.60% weight, 0.457% each)
static const u16 sBerryTier5[] =
{
    ITEM_RAZZ_BERRY,
    ITEM_BLUK_BERRY,
    ITEM_NANAB_BERRY,
    ITEM_WEPEAR_BERRY,
    ITEM_PINAP_BERRY,
    ITEM_POMEG_BERRY,
    ITEM_KELPSY_BERRY,
    ITEM_QUALOT_BERRY,
    ITEM_HONDEW_BERRY,
    ITEM_GREPA_BERRY,
    ITEM_TAMATO_BERRY,
    ITEM_CORNN_BERRY,
    ITEM_MAGOST_BERRY,
    ITEM_RABUTA_BERRY,
    ITEM_NOMEL_BERRY,
    ITEM_SPELON_BERRY,
    ITEM_PAMTRE_BERRY,
    ITEM_WATMEL_BERRY,
    ITEM_DURIN_BERRY,
    ITEM_BELUE_BERRY,
    ITEM_ENIGMA_BERRY,
};
