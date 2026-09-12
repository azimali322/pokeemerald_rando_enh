# Randomizable Items — tier assignment

> **Berries are no longer in this pool.** All 43 moved to their own tiers for the berry-tree
> randomizer — see [BERRIES.md](BERRIES.md). Tier sizes here are 2/8/20/29/63, and the weights were
> rebalanced to keep the per-item ratios this tiering was built around.

Tiered for the way you play: a cheat heal item covers healing, EVs are set manually, and Ultra Balls
are buyable at ₽1. So healing, battle items, vitamins and balls carry almost no value here even though
a general tier list would rank some of them highly.

Pool: 122 non-berry items from `sRandomValidItems[]`, plus a TM band. Key items never reach this
path, and the 43 berries moved to [BERRIES.md](BERRIES.md) for the berry-tree pool.

**TMs can now turn up anywhere.** Before, the pocket check routed a found TM to another TM and
everything else to a non-TM, so a TM could only appear where one already was in the vanilla game.
A band between tiers 1 and 2 now yields one; *which* TM is still decided by the TM tiering.

## Weights

| Tier | Contents | n | Weight | Per-item | × uniform | vs next tier |
|---|---|---:|---:|---:|---:|---:|
| 1 | Premier hold items | 2 | 8.08% | 4.0400% | 6.95× | 1.6× |
| — | **TM band** (which TM is left to the TM tiering) | 50 | 13.89% | 0.2778% | 0.48× | — |
| 2 | Strong hold items | 8 | 20.20% | 2.5250% | 4.34× | 1.6× |
| 3 | Other battle hold items | 20 | 31.57% | 1.5785% | 2.72× | 2.5× |
| 4 | Evolution, balls, species-locked | 29 | 18.31% | 0.6314% | 1.09× | 5× |
| 5 | Healing, vitamins, utility, junk | 63 | 7.95% | 0.1262% | 0.22× | — |

Uniform baseline is 0.5814% per item across 122 items + 50 TMs. Ratios between item tiers:
**1.6× / 1.6× / 2.5× / 5×**, softened from 2× / 2× / 4× / 10× — tier 1 fell from 9.2× the uniform
rate to 6.9×. It lost Lum and Sitrus to the berry split, which is what had concentrated it.

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

