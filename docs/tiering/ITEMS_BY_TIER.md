# Randomizable Items — tier assignment

Tiered for the way you play: a cheat heal item covers healing, EVs are set manually, and Ultra Balls
are buyable at ₽1. So healing, battle items, vitamins and balls carry almost no value here even though
a general tier list would rank some of them highly.

Pool: the 188 items in `sRandomValidItems[]`. TMs and key items never reach this path.

## Weights

| Tier | Contents | n | Weight | Per-item | × uniform | vs next tier |
|---|---|---:|---:|---:|---:|---:|
| 1 | Premier hold items | 4 | 20.61% | 5.1525% | 9.69× | 2× |
| 2 | Strong hold items | 8 | 20.61% | 2.5762% | 4.84× | 2× |
| 3 | Other battle hold items | 40 | 51.53% | 1.2883% | 2.42× | 10× |
| 4 | Evolution, balls, flavour berries, species-locked | 50 | 6.44% | 0.1288% | 0.24× | 10× |
| 5 | Healing, vitamins, utility, junk | 63 | 0.81% | 0.0129% | 0.02× | — |

Uniform baseline is 0.5319% per item. Ratios are exactly as specified: 2× / 2× / 10× / 10×.

## Never rolled

**23 items appear in no tier table.**

- **Mail (12)** — no battle value at all, as you asked
- **Battle-only items (11)** — X-items, Dire Hit, Guard Spec, Poké Doll, Fluffy Tail, Yellow/Red Flute:
  no hold effect, no field use, so with the no-battle-items challenge on they cannot be used at all

```
  BEAD_MAIL
  DIRE_HIT
  DREAM_MAIL
  FAB_MAIL
  FLUFFY_TAIL
  GLITTER_MAIL
  GUARD_SPEC
  HARBOR_MAIL
  MECH_MAIL
  ORANGE_MAIL
  POKE_DOLL
  RED_FLUTE
  RETRO_MAIL
  SHADOW_MAIL
  TROPIC_MAIL
  WAVE_MAIL
  WOOD_MAIL
  X_ACCURACY
  X_ATTACK
  X_DEFEND
  X_SPECIAL
  X_SPEED
  YELLOW_FLUTE
```

---

## Tier 1 — Premier hold items (4)

```
  CHOICE_BAND
  LEFTOVERS
  LUM_BERRY
  SITRUS_BERRY
```

## Tier 2 — Strong hold items (8)

```
  BRIGHT_POWDER
  FOCUS_BAND
  KINGS_ROCK
  QUICK_CLAW
  SCOPE_LENS
  SHELL_BELL
  SILK_SCARF
  WHITE_HERB
```

## Tier 3 — Other battle hold items (40)

```
  AGUAV_BERRY
  APICOT_BERRY
  ASPEAR_BERRY
  BERRY_JUICE
  BLACK_BELT
  BLACK_GLASSES
  CHARCOAL
  CHERI_BERRY
  CHESTO_BERRY
  DRAGON_FANG
  FAIRY_GEM
  FIGY_BERRY
  GANLON_BERRY
  HARD_STONE
  IAPAPA_BERRY
  LANSAT_BERRY
  LAX_INCENSE
  LEPPA_BERRY
  LIECHI_BERRY
  MAGNET
  MAGO_BERRY
  MENTAL_HERB
  MIRACLE_SEED
  MYSTIC_WATER
  NEVER_MELT_ICE
  ORAN_BERRY
  PECHA_BERRY
  PERSIM_BERRY
  PETAYA_BERRY
  POISON_BARB
  RAWST_BERRY
  SALAC_BERRY
  SEA_INCENSE
  SHARP_BEAK
  SILVER_POWDER
  SOFT_SAND
  SPELL_TAG
  STARF_BERRY
  TWISTED_SPOON
  WIKI_BERRY
```

## Tier 4 — Evolution, balls, flavour berries, species-locked (50)

```
  BELUE_BERRY
  BLUK_BERRY
  CORNN_BERRY
  DEEP_SEA_SCALE
  DEEP_SEA_TOOTH
  DIVE_BALL
  DRAGON_SCALE
  DURIN_BERRY
  ENIGMA_BERRY
  FIRE_STONE
  GREAT_BALL
  GREPA_BERRY
  HONDEW_BERRY
  KELPSY_BERRY
  LEAF_STONE
  LIGHT_BALL
  LUCKY_PUNCH
  LUXURY_BALL
  MAGOST_BERRY
  MASTER_BALL
  METAL_COAT
  METAL_POWDER
  MOON_STONE
  NANAB_BERRY
  NEST_BALL
  NET_BALL
  NOMEL_BERRY
  PAMTRE_BERRY
  PINAP_BERRY
  POKE_BALL
  POMEG_BERRY
  PREMIER_BALL
  QUALOT_BERRY
  RABUTA_BERRY
  RAZZ_BERRY
  REPEAT_BALL
  SAFARI_BALL
  SOUL_DEW
  SPELON_BERRY
  STICK
  SUN_STONE
  TAMATO_BERRY
  THICK_CLUB
  THUNDER_STONE
  TIMER_BALL
  ULTRA_BALL
  UP_GRADE
  WATER_STONE
  WATMEL_BERRY
  WEPEAR_BERRY
```

## Tier 5 — Healing, vitamins, utility, junk (63)

```
  AMULET_COIN
  ANTIDOTE
  AWAKENING
  BLACK_FLUTE
  BLUE_FLUTE
  BLUE_SCARF
  BLUE_SHARD
  BURN_HEAL
  CALCIUM
  CARBOS
  CLEANSE_TAG
  ELIXIR
  ENERGY_POWDER
  ENERGY_ROOT
  ESCAPE_ROPE
  ETHER
  EVERSTONE
  EXP_SHARE_SMALL
  FERTILIZER
  FRESH_WATER
  FULL_HEAL
  FULL_RESTORE
  GREEN_SCARF
  GREEN_SHARD
  HEAL_POWDER
  HP_UP
  HYPER_POTION
  ICE_HEAL
  IRON
  LAVA_COOKIE
  LEMONADE
  LUCKY_EGG
  MACHO_BRACE
  MAX_ELIXIR
  MAX_ETHER
  MAX_POTION
  MAX_REPEL
  MAX_REVIVE
  MOOMOO_MILK
  PARALYZE_HEAL
  PINK_SCARF
  POTION
  PP_MAX
  PP_UP
  PROTEIN
  RARE_CANDY
  RED_SCARF
  RED_SHARD
  REPEL
  REVIVAL_HERB
  REVIVE
  SACRED_ASH
  SHOAL_SALT
  SHOAL_SHELL
  SMOKE_BALL
  SODA_POP
  SOOTHE_BELL
  SUPER_POTION
  SUPER_REPEL
  WHITE_FLUTE
  YELLOW_SCARF
  YELLOW_SHARD
  ZINC
```

