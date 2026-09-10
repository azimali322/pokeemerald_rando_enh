# Randomizable Items — tiering worksheet

**The candidate pool for item randomization: 188 items.**

Everything `RandomItemId()` (`src/item.c:1217`) can return — visible item balls, hidden items, and
held items on scripted Pokémon. Key items and HMs are excluded by the existing code and never appear here.

Mark a tier in each `[ ]`: `1`=S `2`=A `3`=B `4`=C `5`=D `6`=junk, `X`=exclude.

## Columns

- `HOLD=` — battle hold effect. **`NONE` means the item does nothing when held.** This is the key
  signal for a hold-item-focused pool.
- `price` — rough proxy for intended value.
- `field` / `battle` — where the item can be used. Shown only where it matters.

## Suggested tiering strategy

Items differ from abilities and moves: an item's value depends on **whether it does anything at all**
on a randomly chosen Pokémon. A useful ordering:

1. **Universal hold items** — work on any species (Leftovers, Choice Band, Shell Bell, Bright Powder).
2. **Hold berries** — Lum and Sitrus are near-universal; the pinch berries need the right stat.
3. **Type-boosting items** — good only when the holder's type matches. The Phase 6 STAB guarantee
   raises their hit rate, so they're better here than in vanilla.
4. **Healing and balls** — low ceiling but always useful. Keep common for playability, not power.
5. **Species-locked items** — Soul Dew, Thick Club, Light Ball, Metal Powder, Lucky Punch, Stick,
   Deep Sea Tooth/Scale. Elite in competitive play, **near-worthless in a randomizer** because they
   only work on one species. Demote them despite their reputation.
6. **Flavour and sellables** — mail, shards, contest berries. The junk tail.

> **Playability constraint:** healing items must stay common enough to finish the game. If Potions and
> Revives land in a 2% bottom tier, the early game becomes unwinnable.

## Counts by group

| Group | Count |
|---|---:|
| Battle hold items | 49 |
| Berries with hold effects | 22 |
| Poké Balls | 12 |
| Healing / medicine | 27 |
| Vitamins & progression consumables | 9 |
| Evolution stones | 6 |
| Flavour berries (no hold effect) | 21 |
| Mail | 12 |
| Everything else | 19 |
| Battle-only dead items | 11 |
| **Total** | **188** |

Use these when you set weights — a tier's weight is divided across its members, so a small group
given a big weight becomes over-represented per item. See Appendix D of the plan.

---

## Battle hold items (49)

Held-only. `HOLD=` is the effect; these are the core of a 'good hold item' pool.

```
  [ ] ITEM_BERRY_JUICE         Berry Juice    price=  100  HOLD=RESTORE_HP(20)
  [ ] ITEM_BRIGHT_POWDER       BrightPowder   price=   10  HOLD=EVASION_UP(10)
  [ ] ITEM_WHITE_HERB          White Herb     price=  100  HOLD=RESTORE_STATS(0)
  [ ] ITEM_MACHO_BRACE         Macho Brace    price= 3000  HOLD=MACHO_BRACE(0)
  [ ] ITEM_EXP_SHARE_SMALL     Exp. Share S   price= 6000  HOLD=EXP_SHARE(0)
  [ ] ITEM_QUICK_CLAW          Quick Claw     price=  100  HOLD=QUICK_CLAW(20)
  [ ] ITEM_SOOTHE_BELL         Soothe Bell    price=  100  HOLD=FRIENDSHIP_UP(0)
  [ ] ITEM_MENTAL_HERB         Mental Herb    price=  100  HOLD=CURE_ATTRACT(0)
  [ ] ITEM_CHOICE_BAND         Choice Band    price=  100  HOLD=CHOICE_BAND(0)
  [ ] ITEM_KINGS_ROCK          King's Rock    price=  100  HOLD=FLINCH(10)
  [ ] ITEM_SILVER_POWDER       SilverPowder   price=  100  HOLD=BUG_POWER(10)
  [ ] ITEM_AMULET_COIN         Amulet Coin    price=  100  HOLD=DOUBLE_PRIZE(10)
  [ ] ITEM_CLEANSE_TAG         Cleanse Tag    price=  200  HOLD=REPEL(0)
  [ ] ITEM_SOUL_DEW            Soul Dew       price=  200  HOLD=SOUL_DEW(0)
  [ ] ITEM_DEEP_SEA_TOOTH      DeepSeaTooth   price=  200  HOLD=DEEP_SEA_TOOTH(0)
  [ ] ITEM_DEEP_SEA_SCALE      DeepSeaScale   price=  200  HOLD=DEEP_SEA_SCALE(0)
  [ ] ITEM_SMOKE_BALL          Smoke Ball     price=  200  HOLD=CAN_ALWAYS_RUN(0)
  [ ] ITEM_EVERSTONE           Everstone      price=  200  HOLD=PREVENT_EVOLVE(0)
  [ ] ITEM_FOCUS_BAND          Focus Band     price=  200  HOLD=FOCUS_BAND(10)
  [ ] ITEM_LUCKY_EGG           Lucky Egg      price=  200  HOLD=LUCKY_EGG(0)
  [ ] ITEM_SCOPE_LENS          Scope Lens     price=  200  HOLD=SCOPE_LENS(0)
  [ ] ITEM_METAL_COAT          Metal Coat     price=  100  HOLD=STEEL_POWER(10)
  [ ] ITEM_LEFTOVERS           Leftovers      price=  200  HOLD=LEFTOVERS(10)
  [ ] ITEM_DRAGON_SCALE        Dragon Scale   price= 2100  HOLD=DRAGON_SCALE(10)
  [ ] ITEM_LIGHT_BALL          Light Ball     price=  100  HOLD=LIGHT_BALL(0)
  [ ] ITEM_SOFT_SAND           Soft Sand      price=  100  HOLD=GROUND_POWER(10)
  [ ] ITEM_HARD_STONE          Hard Stone     price=  100  HOLD=ROCK_POWER(10)
  [ ] ITEM_MIRACLE_SEED        Miracle Seed   price=  100  HOLD=GRASS_POWER(10)
  [ ] ITEM_BLACK_GLASSES       BlackGlasses   price=  100  HOLD=DARK_POWER(10)
  [ ] ITEM_BLACK_BELT          Black Belt     price=  100  HOLD=FIGHTING_POWER(10)
  [ ] ITEM_MAGNET              Magnet         price=  100  HOLD=ELECTRIC_POWER(10)
  [ ] ITEM_MYSTIC_WATER        Mystic Water   price=  100  HOLD=WATER_POWER(10)
  [ ] ITEM_SHARP_BEAK          Sharp Beak     price=  100  HOLD=FLYING_POWER(10)
  [ ] ITEM_POISON_BARB         Poison Barb    price=  100  HOLD=POISON_POWER(10)
  [ ] ITEM_NEVER_MELT_ICE      NeverMeltIce   price=  100  HOLD=ICE_POWER(10)
  [ ] ITEM_SPELL_TAG           Spell Tag      price=  100  HOLD=GHOST_POWER(10)
  [ ] ITEM_TWISTED_SPOON       TwistedSpoon   price=  100  HOLD=PSYCHIC_POWER(10)
  [ ] ITEM_CHARCOAL            Charcoal       price= 9800  HOLD=FIRE_POWER(10)
  [ ] ITEM_DRAGON_FANG         Dragon Fang    price=  100  HOLD=DRAGON_POWER(10)
  [ ] ITEM_SILK_SCARF          Silk Scarf     price=  100  HOLD=NORMAL_POWER(10)
  [ ] ITEM_UP_GRADE            Up-Grade       price= 2100  HOLD=UP_GRADE(0)
  [ ] ITEM_SHELL_BELL          Shell Bell     price=  200  HOLD=SHELL_BELL(8)
  [ ] ITEM_SEA_INCENSE         Blue Incense   price= 9600  HOLD=WATER_POWER(5)
  [ ] ITEM_LAX_INCENSE         Lax Incense    price= 9600  HOLD=EVASION_UP(5)
  [ ] ITEM_LUCKY_PUNCH         Lucky Punch    price=   10  HOLD=LUCKY_PUNCH(0)
  [ ] ITEM_METAL_POWDER        Metal Powder   price=   10  HOLD=METAL_POWDER(0)
  [ ] ITEM_THICK_CLUB          Thick Club     price=  500  HOLD=THICK_CLUB(0)
  [ ] ITEM_STICK               Stick          price=  200  HOLD=STICK(0)
  [ ] ITEM_FAIRY_GEM           Fairy Gem      price=  100  HOLD=FAIRY_POWER(10)
```

## Berries with hold effects (22)

**Both** consumable and good held. Kept under the dead-item filter.

```
  [ ] ITEM_CHERI_BERRY         Cheri Berry    price=   20  HOLD=CURE_PAR(0)
  [ ] ITEM_CHESTO_BERRY        Chesto Berry   price=   20  HOLD=CURE_SLP(0)
  [ ] ITEM_PECHA_BERRY         Pecha Berry    price=   20  HOLD=CURE_PSN(0)
  [ ] ITEM_RAWST_BERRY         Rawst Berry    price=   20  HOLD=CURE_BRN(0)
  [ ] ITEM_ASPEAR_BERRY        Aspear Berry   price=   20  HOLD=CURE_FRZ(0)
  [ ] ITEM_LEPPA_BERRY         Leppa Berry    price=   20  HOLD=RESTORE_PP(10)
  [ ] ITEM_ORAN_BERRY          Oran Berry     price=   20  HOLD=RESTORE_HP(10)
  [ ] ITEM_PERSIM_BERRY        Persim Berry   price=   20  HOLD=CURE_CONFUSION(0)
  [ ] ITEM_LUM_BERRY           Lum Berry      price=   20  HOLD=CURE_STATUS(0)
  [ ] ITEM_SITRUS_BERRY        Sitrus Berry   price=   20  HOLD=RESTORE_PCT_HP(25)
  [ ] ITEM_FIGY_BERRY          Figy Berry     price=   20  HOLD=CONFUSE_SPICY(8)
  [ ] ITEM_WIKI_BERRY          Wiki Berry     price=   20  HOLD=CONFUSE_DRY(8)
  [ ] ITEM_MAGO_BERRY          Mago Berry     price=   20  HOLD=CONFUSE_SWEET(8)
  [ ] ITEM_AGUAV_BERRY         Aguav Berry    price=   20  HOLD=CONFUSE_BITTER(8)
  [ ] ITEM_IAPAPA_BERRY        Iapapa Berry   price=   20  HOLD=CONFUSE_SOUR(8)
  [ ] ITEM_LIECHI_BERRY        Liechi Berry   price=   20  HOLD=ATTACK_UP(4)
  [ ] ITEM_GANLON_BERRY        Ganlon Berry   price=   20  HOLD=DEFENSE_UP(4)
  [ ] ITEM_SALAC_BERRY         Salac Berry    price=   20  HOLD=SPEED_UP(4)
  [ ] ITEM_PETAYA_BERRY        Petaya Berry   price=   20  HOLD=SP_ATTACK_UP(4)
  [ ] ITEM_APICOT_BERRY        Apicot Berry   price=   20  HOLD=SP_DEFENSE_UP(4)
  [ ] ITEM_LANSAT_BERRY        Lansat Berry   price=   20  HOLD=CRITICAL_UP(4)
  [ ] ITEM_STARF_BERRY         Starf Berry    price=   20  HOLD=RANDOM_STAT_UP(4)
```

## Poké Balls (12)

Exempt from the no-battle-items challenge. Always keep available.

```
  [ ] ITEM_MASTER_BALL         Master Ball    price=    0
  [ ] ITEM_ULTRA_BALL          Ultra Ball     price= 1200
  [ ] ITEM_GREAT_BALL          Great Ball     price=  600
  [ ] ITEM_POKE_BALL           Poké Ball      price=  200
  [ ] ITEM_SAFARI_BALL         Safari Ball    price=  200
  [ ] ITEM_NET_BALL            Net Ball       price= 1000
  [ ] ITEM_DIVE_BALL           Dive Ball      price= 1000
  [ ] ITEM_NEST_BALL           Nest Ball      price= 1000
  [ ] ITEM_REPEAT_BALL         Repeat Ball    price= 1000
  [ ] ITEM_TIMER_BALL          Timer Ball     price= 1000
  [ ] ITEM_LUXURY_BALL         Luxury Ball    price= 1000
  [ ] ITEM_PREMIER_BALL        Premier Ball   price=  200
```

## Healing / medicine (27)

Field-usable, so **not** dead under the challenge. Keep reasonably common or the game is unplayable.

```
  [ ] ITEM_POTION              Potion         price=  300
  [ ] ITEM_ANTIDOTE            Antidote       price=  100
  [ ] ITEM_BURN_HEAL           Burn Heal      price=  250
  [ ] ITEM_ICE_HEAL            Ice Heal       price=  250
  [ ] ITEM_AWAKENING           Awakening      price=  250
  [ ] ITEM_PARALYZE_HEAL       Parlyz Heal    price=  200
  [ ] ITEM_FULL_RESTORE        Full Restore   price= 3000
  [ ] ITEM_MAX_POTION          Max Potion     price= 2500
  [ ] ITEM_HYPER_POTION        Hyper Potion   price= 1200
  [ ] ITEM_SUPER_POTION        Super Potion   price=  700
  [ ] ITEM_FULL_HEAL           Full Heal      price=  600
  [ ] ITEM_REVIVE              Revive         price= 1500
  [ ] ITEM_MAX_REVIVE          Max Revive     price= 4000
  [ ] ITEM_FRESH_WATER         Fresh Water    price=  200
  [ ] ITEM_SODA_POP            Soda Pop       price=  300
  [ ] ITEM_LEMONADE            Lemonade       price=  350
  [ ] ITEM_MOOMOO_MILK         Moomoo Milk    price=  500
  [ ] ITEM_ENERGY_POWDER       EnergyPowder   price=  500
  [ ] ITEM_ENERGY_ROOT         Energy Root    price=  800
  [ ] ITEM_HEAL_POWDER         Heal Powder    price=  450
  [ ] ITEM_REVIVAL_HERB        Revival Herb   price= 2800
  [ ] ITEM_ETHER               Ether          price= 2000
  [ ] ITEM_MAX_ETHER           Max Ether      price= 4000
  [ ] ITEM_ELIXIR              Elixir         price= 6000
  [ ] ITEM_MAX_ELIXIR          Max Elixir     price=12000
  [ ] ITEM_LAVA_COOKIE         Lava Cookie    price=  200
  [ ] ITEM_BLUE_FLUTE          Blue Flute     price=  100
```

## Vitamins & progression consumables (9)

Permanent stat/level gains.

```
  [ ] ITEM_HP_UP               HP Up          price= 1350
  [ ] ITEM_PROTEIN             Protein        price= 1350
  [ ] ITEM_IRON                Iron           price= 1350
  [ ] ITEM_CARBOS              Carbos         price= 1350
  [ ] ITEM_CALCIUM             Calcium        price= 1350
  [ ] ITEM_RARE_CANDY          Rare Candy     price= 4800
  [ ] ITEM_PP_UP               PP Up          price=20000
  [ ] ITEM_ZINC                Zinc           price= 1350
  [ ] ITEM_PP_MAX              PP Max         price=20000
```

## Evolution stones (6)

```
  [ ] ITEM_SUN_STONE           Sun Stone      price= 2100
  [ ] ITEM_MOON_STONE          Moon Stone     price= 2100
  [ ] ITEM_FIRE_STONE          Fire Stone     price= 2100
  [ ] ITEM_THUNDER_STONE       ThunderStone   price= 2100
  [ ] ITEM_WATER_STONE         Water Stone    price= 2100
  [ ] ITEM_LEAF_STONE          Leaf Stone     price= 2100
```

## Flavour berries (no hold effect) (21)

Pokéblock / contest use only.

```
  [ ] ITEM_RAZZ_BERRY          Razz Berry     price=   20
  [ ] ITEM_BLUK_BERRY          Bluk Berry     price=   20
  [ ] ITEM_NANAB_BERRY         Nanab Berry    price=   20
  [ ] ITEM_WEPEAR_BERRY        Wepear Berry   price=   20
  [ ] ITEM_PINAP_BERRY         Pinap Berry    price=   20
  [ ] ITEM_POMEG_BERRY         Pomeg Berry    price=   20
  [ ] ITEM_KELPSY_BERRY        Kelpsy Berry   price=   20
  [ ] ITEM_QUALOT_BERRY        Qualot Berry   price=   20
  [ ] ITEM_HONDEW_BERRY        Hondew Berry   price=   20
  [ ] ITEM_GREPA_BERRY         Grepa Berry    price=   20
  [ ] ITEM_TAMATO_BERRY        Tamato Berry   price=   20
  [ ] ITEM_CORNN_BERRY         Cornn Berry    price=   20
  [ ] ITEM_MAGOST_BERRY        Magost Berry   price=   20
  [ ] ITEM_RABUTA_BERRY        Rabuta Berry   price=   20
  [ ] ITEM_NOMEL_BERRY         Nomel Berry    price=   20
  [ ] ITEM_SPELON_BERRY        Spelon Berry   price=   20
  [ ] ITEM_PAMTRE_BERRY        Pamtre Berry   price=   20
  [ ] ITEM_WATMEL_BERRY        Watmel Berry   price=   20
  [ ] ITEM_DURIN_BERRY         Durin Berry    price=   20
  [ ] ITEM_BELUE_BERRY         Belue Berry    price=   20
  [ ] ITEM_ENIGMA_BERRY        Enigma Berry   price=   20
```

## Mail (12)

No battle relevance.

```
  [ ] ITEM_ORANGE_MAIL         Orange Mail    price=   50
  [ ] ITEM_HARBOR_MAIL         Harbor Mail    price=   50
  [ ] ITEM_GLITTER_MAIL        Glitter Mail   price=   50
  [ ] ITEM_MECH_MAIL           Mech Mail      price=   50
  [ ] ITEM_WOOD_MAIL           Wood Mail      price=   50
  [ ] ITEM_WAVE_MAIL           Wave Mail      price=   50
  [ ] ITEM_BEAD_MAIL           Bead Mail      price=   50
  [ ] ITEM_SHADOW_MAIL         Shadow Mail    price=   50
  [ ] ITEM_TROPIC_MAIL         Tropic Mail    price=   50
  [ ] ITEM_DREAM_MAIL          Dream Mail     price=   50
  [ ] ITEM_FAB_MAIL            Fab Mail       price=   50
  [ ] ITEM_RETRO_MAIL          Retro Mail     price= 5000
```

## Everything else (19)

Shards, flutes with field use, repels, escape rope, sellables, misc.

```
  [ ] ITEM_BLACK_FLUTE         Black Flute    price=  400
  [ ] ITEM_WHITE_FLUTE         White Flute    price=  500
  [ ] ITEM_SACRED_ASH          Sacred Ash     price=  200
  [ ] ITEM_SHOAL_SALT          Shoal Salt     price=   20
  [ ] ITEM_SHOAL_SHELL         Shoal Shell    price=   20
  [ ] ITEM_RED_SHARD           Red Shard      price=  200
  [ ] ITEM_BLUE_SHARD          Blue Shard     price=  200
  [ ] ITEM_YELLOW_SHARD        Yellow Shard   price=  200
  [ ] ITEM_GREEN_SHARD         Green Shard    price=  200
  [ ] ITEM_FERTILIZER          Growth Mulch   price= 2500
  [ ] ITEM_SUPER_REPEL         Super Repel    price=  500
  [ ] ITEM_MAX_REPEL           Max Repel      price=  700
  [ ] ITEM_ESCAPE_ROPE         Escape Rope    price=  550
  [ ] ITEM_REPEL               Repel          price=  350
  [ ] ITEM_RED_SCARF           Red Scarf      price=  100
  [ ] ITEM_BLUE_SCARF          Blue Scarf     price=  100
  [ ] ITEM_PINK_SCARF          Pink Scarf     price=  100
  [ ] ITEM_GREEN_SCARF         Green Scarf    price=  100
  [ ] ITEM_YELLOW_SCARF        Yellow Scarf   price=  100
```

## Battle-only dead items (11)

**Always bottom tier**, and hard-filtered when `tx_Challenges_NoItemPlayer` is on. No hold effect, no field use, not a ball.

```
  [ ] ITEM_YELLOW_FLUTE        Yellow Flute   price=  200  battle=Medicine
  [ ] ITEM_RED_FLUTE           Red Flute      price=  300  battle=Medicine
  [ ] ITEM_GUARD_SPEC          Guard Spec.    price=  700  battle=StatIncrease
  [ ] ITEM_DIRE_HIT            Dire Hit       price=  650  battle=StatIncrease
  [ ] ITEM_X_ATTACK            X Attack       price=  500  battle=StatIncrease
  [ ] ITEM_X_DEFEND            X Defend       price=  550  battle=StatIncrease
  [ ] ITEM_X_SPEED             X Speed        price=  350  battle=StatIncrease
  [ ] ITEM_X_ACCURACY          X Accuracy     price=  950  battle=StatIncrease
  [ ] ITEM_X_SPECIAL           X Special      price=  350  battle=StatIncrease
  [ ] ITEM_POKE_DOLL           Poké Doll      price= 1000  battle=Escape
  [ ] ITEM_FLUFFY_TAIL         Fluffy Tail    price= 1000  battle=Escape
```

