# STAB Move Pools by Type — tiering worksheet

Every move `EnsureStabMove()` can pick, grouped by type. A move qualifies if it is **damaging**
(`power > 1`) and **not an HM** — status moves cannot provide STAB, and HMs are excluded because
overwriting one can soft-lock progression.

A dual-type Pokémon draws from the **union** of both its types' lists, so the effective pool is
usually much larger than any single column below.

**Types with 10+ moves are worth tiering** (8 of 19). The rest stay uniform —
there is not enough there for weighting to mean anything.

Mark a tier in each `[ ]`: `1`=S `2`=A `3`=B `4`=C, or `X` to exclude.

| Type | Moves | Tier it? |
|---|---:|---|
| NORMAL | 52 | **yes** |
| FIGHTING | 18 | **yes** |
| FIRE | 12 | **yes** |
| GRASS | 12 | **yes** |
| WATER | 11 | **yes** |
| FLYING | 10 | **yes** |
| PSYCHIC | 10 | **yes** |
| GROUND | 10 | **yes** |
| ICE | 8 | no — uniform |
| ELECTRIC | 8 | no — uniform |
| POISON | 8 | no — uniform |
| BUG | 8 | no — uniform |
| DARK | 8 | no — uniform |
| ROCK | 7 | no — uniform |
| GHOST | 6 | no — uniform |
| STEEL | 6 | no — uniform |
| DRAGON | 5 | no — uniform |
| FAIRY | 2 | no — uniform |
| MYSTERY | 1 | no — uniform |

---

# Types worth tiering

## NORMAL (52)

```
  [ ] MOVE_EXPLOSION           pow=250  acc=100  PHYSICAL pp= 5  EXPLOSION
  [ ] MOVE_SELF_DESTRUCT       pow=200  acc=100  PHYSICAL pp= 5  EXPLOSION
  [ ] MOVE_HYPER_BEAM          pow=150  acc= 90  SPECIAL pp= 5  RECHARGE
  [ ] MOVE_DOUBLE_EDGE         pow=120  acc=100  PHYSICAL pp=15  DOUBLE_EDGE
  [ ] MOVE_MEGA_KICK           pow=120  acc= 75  PHYSICAL pp= 5  HIT
  [ ] MOVE_THRASH              pow=105  acc=100  PHYSICAL pp=10  RAMPAGE
  [ ] MOVE_EGG_BOMB            pow=100  acc= 90  PHYSICAL pp=10  HIT
  [ ] MOVE_SKULL_BASH          pow=100  acc=100  PHYSICAL pp=10  SKULL_BASH
  [ ] MOVE_SPIT_UP             pow=100  acc=100  SPECIAL pp=10  SPIT_UP
  [ ] MOVE_HYPER_VOICE         pow= 90  acc=100  SPECIAL pp=10  HIT
  [ ] MOVE_TAKE_DOWN           pow= 90  acc= 85  PHYSICAL pp=20  RECOIL
  [ ] MOVE_BODY_SLAM           pow= 85  acc=100  PHYSICAL pp=15  PARALYZE_HIT
  [ ] MOVE_EXTREME_SPEED       pow= 80  acc=100  PHYSICAL pp= 5  QUICK_ATTACK
  [ ] MOVE_HYPER_FANG          pow= 80  acc= 90  PHYSICAL pp=15  FLINCH_HIT
  [ ] MOVE_MEGA_PUNCH          pow= 80  acc= 85  PHYSICAL pp=20  HIT
  [ ] MOVE_RAZOR_WIND          pow= 80  acc=100  SPECIAL pp=10  RAZOR_WIND
  [ ] MOVE_SLAM                pow= 80  acc= 75  PHYSICAL pp=20  HIT
  [ ] MOVE_TRI_ATTACK          pow= 80  acc=100  SPECIAL pp=10  TRI_ATTACK
  [ ] MOVE_CRUSH_CLAW          pow= 75  acc= 95  PHYSICAL pp=10  DEFENSE_DOWN_HIT
  [ ] MOVE_DIZZY_PUNCH         pow= 70  acc=100  PHYSICAL pp=10  CONFUSE_HIT
  [ ] MOVE_FACADE              pow= 70  acc=100  PHYSICAL pp=20  FACADE
  [ ] MOVE_HEADBUTT            pow= 70  acc=100  PHYSICAL pp=15  FLINCH_HIT
  [ ] MOVE_SECRET_POWER        pow= 70  acc=100  PHYSICAL pp=20  SECRET_POWER
  [ ] MOVE_SLASH               pow= 70  acc=100  PHYSICAL pp=20  HIGH_CRITICAL
  [ ] MOVE_SMELLING_SALT       pow= 70  acc=100  PHYSICAL pp=10  SMELLINGSALT
  [ ] MOVE_UPROAR              pow= 70  acc=100  SPECIAL pp=10  UPROAR
  [ ] MOVE_HORN_ATTACK         pow= 65  acc=100  PHYSICAL pp=25  HIT
  [ ] MOVE_STOMP               pow= 65  acc=100  PHYSICAL pp=20  FLINCH_MINIMIZE_HIT
  [ ] MOVE_COVET               pow= 60  acc=100  PHYSICAL pp=20  THIEF
  [ ] MOVE_SWIFT               pow= 60  acc=  0  SPECIAL pp=20  ALWAYS_HIT
  [ ] MOVE_VICE_GRIP           pow= 55  acc=100  PHYSICAL pp=30  HIT
  [ ] MOVE_RAPID_SPIN          pow= 50  acc=100  PHYSICAL pp=40  RAPID_SPIN
  [ ] MOVE_SNORE               pow= 50  acc=100  SPECIAL pp=15  SNORE
  [ ] MOVE_STRUGGLE            pow= 50  acc=100  PHYSICAL pp= 1  RECOIL
  [ ] MOVE_WEATHER_BALL        pow= 50  acc=100  SPECIAL pp=10  WEATHER_BALL
  [ ] MOVE_FAKE_OUT            pow= 40  acc=100  PHYSICAL pp=10  FAKE_OUT
  [ ] MOVE_FALSE_SWIPE         pow= 40  acc=100  PHYSICAL pp=40  FALSE_SWIPE
  [ ] MOVE_PAY_DAY             pow= 40  acc=100  PHYSICAL pp=20  PAY_DAY
  [ ] MOVE_POUND               pow= 40  acc=100  PHYSICAL pp=35  HIT
  [ ] MOVE_QUICK_ATTACK        pow= 40  acc=100  PHYSICAL pp=30  QUICK_ATTACK
  [ ] MOVE_SCRATCH             pow= 40  acc=100  PHYSICAL pp=35  HIT
  [ ] MOVE_TACKLE              pow= 40  acc=100  PHYSICAL pp=35  HIT
  [ ] MOVE_COMET_PUNCH         pow= 30  acc=100  PHYSICAL pp=15  MULTI_HIT
  [ ] MOVE_CONSTRICT           pow= 25  acc=100  PHYSICAL pp=35  SPEED_DOWN_HIT
  [ ] MOVE_BARRAGE             pow= 20  acc=100  PHYSICAL pp=20  MULTI_HIT
  [ ] MOVE_DOUBLE_SLAP         pow= 20  acc= 85  PHYSICAL pp=10  MULTI_HIT
  [ ] MOVE_RAGE                pow= 20  acc=100  PHYSICAL pp=20  RAGE
  [ ] MOVE_SPIKE_CANNON        pow= 20  acc=100  PHYSICAL pp=15  MULTI_HIT
  [ ] MOVE_FURY_SWIPES         pow= 18  acc= 80  PHYSICAL pp=15  MULTI_HIT
  [ ] MOVE_BIND                pow= 15  acc= 85  PHYSICAL pp=20  TRAP
  [ ] MOVE_FURY_ATTACK         pow= 15  acc= 85  PHYSICAL pp=20  MULTI_HIT
  [ ] MOVE_WRAP                pow= 15  acc= 90  PHYSICAL pp=20  TRAP
```

## FIGHTING (18)

```
  [ ] MOVE_FOCUS_PUNCH         pow=150  acc=100  PHYSICAL pp=20  FOCUS_PUNCH
  [ ] MOVE_FOCUS_BLAST         pow=120  acc= 70  SPECIAL pp= 5  SPECIAL_DEFENSE_DOWN_H
  [ ] MOVE_SUPERPOWER          pow=120  acc=100  PHYSICAL pp= 5  SUPERPOWER
  [ ] MOVE_CROSS_CHOP          pow=100  acc= 80  PHYSICAL pp= 5  HIGH_CRITICAL
  [ ] MOVE_DYNAMIC_PUNCH       pow=100  acc= 50  PHYSICAL pp= 5  CONFUSE_HIT
  [ ] MOVE_HI_JUMP_KICK        pow=100  acc= 90  PHYSICAL pp=10  RECOIL_IF_MISS
  [ ] MOVE_JUMP_KICK           pow= 85  acc= 95  PHYSICAL pp=10  RECOIL_IF_MISS
  [ ] MOVE_SKY_UPPERCUT        pow= 85  acc= 90  PHYSICAL pp=15  SKY_UPPERCUT
  [ ] MOVE_SUBMISSION          pow= 80  acc= 80  PHYSICAL pp=20  RECOIL
  [ ] MOVE_BRICK_BREAK         pow= 75  acc=100  PHYSICAL pp=15  BRICK_BREAK
  [ ] MOVE_VITAL_THROW         pow= 70  acc=100  PHYSICAL pp=10  VITAL_THROW
  [ ] MOVE_REVENGE             pow= 60  acc=100  PHYSICAL pp=10  REVENGE
  [ ] MOVE_ROLLING_KICK        pow= 60  acc= 85  PHYSICAL pp=15  FLINCH_HIT
  [ ] MOVE_KARATE_CHOP         pow= 50  acc=100  PHYSICAL pp=25  HIGH_CRITICAL
  [ ] MOVE_MACH_PUNCH          pow= 40  acc=100  PHYSICAL pp=30  QUICK_ATTACK
  [ ] MOVE_DOUBLE_KICK         pow= 30  acc=100  PHYSICAL pp=30  DOUBLE_HIT
  [ ] MOVE_ARM_THRUST          pow= 15  acc=100  PHYSICAL pp=20  MULTI_HIT
  [ ] MOVE_TRIPLE_KICK         pow= 10  acc= 90  PHYSICAL pp=10  TRIPLE_KICK
```

## FIRE (12)

```
  [ ] MOVE_BLAST_BURN          pow=150  acc= 90  SPECIAL pp= 5  RECHARGE
  [ ] MOVE_ERUPTION            pow=150  acc=100  SPECIAL pp= 5  ERUPTION
  [ ] MOVE_OVERHEAT            pow=130  acc= 90  SPECIAL pp= 5  OVERHEAT
  [ ] MOVE_FIRE_BLAST          pow=110  acc= 85  SPECIAL pp= 5  BURN_HIT
  [ ] MOVE_SACRED_FIRE         pow=100  acc= 95  PHYSICAL pp= 5  THAW_HIT
  [ ] MOVE_HEAT_WAVE           pow= 95  acc= 90  SPECIAL pp=10  BURN_HIT
  [ ] MOVE_FLAMETHROWER        pow= 90  acc=100  SPECIAL pp=15  BURN_HIT
  [ ] MOVE_BLAZE_KICK          pow= 85  acc= 90  PHYSICAL pp=10  BLAZE_KICK
  [ ] MOVE_FIRE_PUNCH          pow= 75  acc=100  PHYSICAL pp=15  BURN_HIT
  [ ] MOVE_FLAME_WHEEL         pow= 60  acc=100  PHYSICAL pp=25  THAW_HIT
  [ ] MOVE_EMBER               pow= 40  acc=100  SPECIAL pp=25  BURN_HIT
  [ ] MOVE_FIRE_SPIN           pow= 35  acc= 95  SPECIAL pp=15  TRAP
```

## GRASS (12)

```
  [ ] MOVE_FRENZY_PLANT        pow=150  acc= 90  SPECIAL pp= 5  RECHARGE
  [ ] MOVE_SOLAR_BEAM          pow=120  acc=100  SPECIAL pp=10  SOLAR_BEAM
  [ ] MOVE_PETAL_DANCE         pow=100  acc=100  SPECIAL pp=10  RAMPAGE
  [ ] MOVE_LEAF_BLADE          pow= 90  acc=100  PHYSICAL pp=15  HIGH_CRITICAL
  [ ] MOVE_GIGA_DRAIN          pow= 75  acc=100  SPECIAL pp=10  ABSORB
  [ ] MOVE_MAGICAL_LEAF        pow= 60  acc=  0  SPECIAL pp=20  ALWAYS_HIT
  [ ] MOVE_NEEDLE_ARM          pow= 60  acc=100  PHYSICAL pp=15  FLINCH_MINIMIZE_HIT
  [ ] MOVE_RAZOR_LEAF          pow= 55  acc= 95  PHYSICAL pp=25  HIGH_CRITICAL
  [ ] MOVE_VINE_WHIP           pow= 45  acc=100  PHYSICAL pp=25  HIT
  [ ] MOVE_MEGA_DRAIN          pow= 40  acc=100  SPECIAL pp=15  ABSORB
  [ ] MOVE_ABSORB              pow= 20  acc=100  SPECIAL pp=25  ABSORB
  [ ] MOVE_BULLET_SEED         pow= 20  acc=100  PHYSICAL pp=30  MULTI_HIT
```

## WATER (11)

```
  [ ] MOVE_HYDRO_CANNON        pow=150  acc= 90  SPECIAL pp= 5  RECHARGE
  [ ] MOVE_WATER_SPOUT         pow=150  acc=100  SPECIAL pp= 5  ERUPTION
  [ ] MOVE_HYDRO_PUMP          pow=120  acc= 80  SPECIAL pp= 5  HIT
  [ ] MOVE_CRABHAMMER          pow= 90  acc= 90  PHYSICAL pp=10  HIGH_CRITICAL
  [ ] MOVE_BUBBLE_BEAM         pow= 65  acc=100  SPECIAL pp=20  SPEED_DOWN_HIT
  [ ] MOVE_OCTAZOOKA           pow= 65  acc= 85  SPECIAL pp=10  ACCURACY_DOWN_HIT
  [ ] MOVE_WATER_PULSE         pow= 60  acc=100  SPECIAL pp=20  CONFUSE_HIT
  [ ] MOVE_WATER_GUN           pow= 40  acc=100  SPECIAL pp=25  HIT
  [ ] MOVE_CLAMP               pow= 35  acc= 85  PHYSICAL pp=15  TRAP
  [ ] MOVE_WHIRLPOOL           pow= 35  acc= 95  SPECIAL pp=15  TRAP
  [ ] MOVE_BUBBLE              pow= 30  acc=100  SPECIAL pp=30  SPEED_DOWN_HIT
```

## FLYING (10)

```
  [ ] MOVE_SKY_ATTACK          pow=140  acc= 90  PHYSICAL pp= 5  SKY_ATTACK
  [ ] MOVE_AEROBLAST           pow=100  acc= 95  SPECIAL pp= 5  HIGH_CRITICAL
  [ ] MOVE_BOUNCE              pow= 85  acc= 85  PHYSICAL pp= 5  SEMI_INVULNERABLE
  [ ] MOVE_DRILL_PECK          pow= 85  acc=100  PHYSICAL pp=20  HIT
  [ ] MOVE_AIR_SLASH           pow= 75  acc= 95  SPECIAL pp=15  FLINCH_HIT
  [ ] MOVE_AERIAL_ACE          pow= 60  acc=  0  PHYSICAL pp=20  ALWAYS_HIT
  [ ] MOVE_AIR_CUTTER          pow= 60  acc= 95  SPECIAL pp=25  HIGH_CRITICAL
  [ ] MOVE_WING_ATTACK         pow= 60  acc=100  PHYSICAL pp=35  HIT
  [ ] MOVE_GUST                pow= 40  acc=100  SPECIAL pp=35  GUST
  [ ] MOVE_PECK                pow= 35  acc=100  PHYSICAL pp=35  HIT
```

## PSYCHIC (10)

```
  [ ] MOVE_PSYCHO_BOOST        pow=140  acc= 90  SPECIAL pp= 5  OVERHEAT
  [ ] MOVE_FUTURE_SIGHT        pow=120  acc=100  SPECIAL pp=10  FUTURE_SIGHT
  [ ] MOVE_DREAM_EATER         pow=100  acc=100  SPECIAL pp=15  DREAM_EATER
  [ ] MOVE_PSYCHIC             pow= 90  acc=100  SPECIAL pp=10  SPECIAL_DEFENSE_DOWN_H
  [ ] MOVE_EXTRASENSORY        pow= 80  acc=100  SPECIAL pp=20  FLINCH_MINIMIZE_HIT
  [ ] MOVE_LUSTER_PURGE        pow= 70  acc=100  SPECIAL pp= 5  SPECIAL_DEFENSE_DOWN_H
  [ ] MOVE_MIST_BALL           pow= 70  acc=100  SPECIAL pp= 5  SPECIAL_ATTACK_DOWN_HI
  [ ] MOVE_PSYCHO_CUT          pow= 70  acc=100  PHYSICAL pp=20  HIGH_CRITICAL
  [ ] MOVE_PSYBEAM             pow= 65  acc=100  SPECIAL pp=20  CONFUSE_HIT
  [ ] MOVE_CONFUSION           pow= 50  acc=100  SPECIAL pp=25  CONFUSE_HIT
```

## GROUND (10)

```
  [ ] MOVE_EARTHQUAKE          pow=100  acc=100  PHYSICAL pp=10  EARTHQUAKE
  [ ] MOVE_EARTH_POWER         pow= 90  acc=100  SPECIAL pp=10  SPECIAL_DEFENSE_DOWN_H
  [ ] MOVE_MUDDY_WATER         pow= 90  acc= 85  SPECIAL pp=10  ACCURACY_DOWN_HIT
  [ ] MOVE_DIG                 pow= 80  acc=100  PHYSICAL pp=10  SEMI_INVULNERABLE
  [ ] MOVE_BONE_CLUB           pow= 65  acc= 85  PHYSICAL pp=20  FLINCH_HIT
  [ ] MOVE_MUD_SHOT            pow= 55  acc= 95  SPECIAL pp=15  SPEED_DOWN_HIT
  [ ] MOVE_BONEMERANG          pow= 50  acc= 90  PHYSICAL pp=10  DOUBLE_HIT
  [ ] MOVE_SAND_TOMB           pow= 35  acc= 95  PHYSICAL pp=15  TRAP
  [ ] MOVE_BONE_RUSH           pow= 25  acc= 80  PHYSICAL pp=10  MULTI_HIT
  [ ] MOVE_MUD_SLAP            pow= 20  acc=100  SPECIAL pp=10  ACCURACY_DOWN_HIT
```

---

# Types left uniform

## ICE (8) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_BLIZZARD            pow=110  acc= 70  SPECIAL pp= 5  FREEZE_HIT
      MOVE_ICE_BEAM            pow= 90  acc=100  SPECIAL pp=10  FREEZE_HIT
      MOVE_ICE_PUNCH           pow= 75  acc=100  PHYSICAL pp=15  FREEZE_HIT
      MOVE_AURORA_BEAM         pow= 65  acc=100  SPECIAL pp=20  ATTACK_DOWN_HIT
      MOVE_ICY_WIND            pow= 55  acc= 95  SPECIAL pp=15  SPEED_DOWN_HIT
      MOVE_POWDER_SNOW         pow= 40  acc=100  SPECIAL pp=25  FREEZE_HIT
      MOVE_ICE_BALL            pow= 30  acc= 90  PHYSICAL pp=20  ROLLOUT
      MOVE_ICICLE_SPEAR        pow= 20  acc=100  PHYSICAL pp=30  MULTI_HIT
```

## ELECTRIC (8) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_VOLT_TACKLE         pow=120  acc=100  PHYSICAL pp=15  DOUBLE_EDGE
      MOVE_ZAP_CANNON          pow=120  acc= 50  SPECIAL pp= 5  PARALYZE_HIT
      MOVE_THUNDER             pow=110  acc= 70  SPECIAL pp=10  THUNDER
      MOVE_THUNDERBOLT         pow= 90  acc=100  SPECIAL pp=15  PARALYZE_HIT
      MOVE_THUNDER_PUNCH       pow= 75  acc=100  PHYSICAL pp=15  PARALYZE_HIT
      MOVE_SPARK               pow= 65  acc=100  PHYSICAL pp=20  PARALYZE_HIT
      MOVE_SHOCK_WAVE          pow= 60  acc=  0  SPECIAL pp=20  ALWAYS_HIT
      MOVE_THUNDER_SHOCK       pow= 40  acc=100  SPECIAL pp=30  PARALYZE_HIT
```

## POISON (8) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_SLUDGE_BOMB         pow= 90  acc=100  SPECIAL pp=10  POISON_HIT
      MOVE_POISON_JAB          pow= 80  acc=100  PHYSICAL pp=20  POISON_HIT
      MOVE_POISON_FANG         pow= 65  acc=100  PHYSICAL pp=15  POISON_FANG
      MOVE_SLUDGE              pow= 65  acc=100  SPECIAL pp=20  POISON_HIT
      MOVE_POISON_TAIL         pow= 50  acc=100  PHYSICAL pp=25  POISON_TAIL
      MOVE_ACID                pow= 40  acc=100  SPECIAL pp=30  SPECIAL_DEFENSE_DOWN_H
      MOVE_SMOG                pow= 30  acc= 70  SPECIAL pp=20  POISON_HIT
      MOVE_POISON_STING        pow= 15  acc=100  PHYSICAL pp=35  POISON_HIT
```

## BUG (8) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_MEGAHORN            pow=120  acc= 85  PHYSICAL pp=10  HIT
      MOVE_BUG_BUZZ            pow= 90  acc=100  SPECIAL pp=10  SPECIAL_DEFENSE_DOWN_H
      MOVE_SIGNAL_BEAM         pow= 75  acc=100  SPECIAL pp=15  CONFUSE_HIT
      MOVE_LEECH_LIFE          pow= 60  acc=100  PHYSICAL pp=10  ABSORB
      MOVE_SILVER_WIND         pow= 60  acc=100  SPECIAL pp= 5  ALL_STATS_UP_HIT
      MOVE_TWINEEDLE           pow= 25  acc=100  PHYSICAL pp=20  TWINEEDLE
      MOVE_FURY_CUTTER         pow= 20  acc= 95  PHYSICAL pp=20  FURY_CUTTER
      MOVE_PIN_MISSILE         pow= 20  acc= 95  PHYSICAL pp=20  MULTI_HIT
```

## DARK (8) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_CRUNCH              pow= 80  acc=100  PHYSICAL pp=15  DEFENSE_DOWN_HIT
      MOVE_DARK_PULSE          pow= 80  acc=100  SPECIAL pp=15  FLINCH_HIT
      MOVE_KNOCK_OFF           pow= 65  acc=100  PHYSICAL pp=20  KNOCK_OFF
      MOVE_BITE                pow= 60  acc=100  PHYSICAL pp=25  FLINCH_HIT
      MOVE_FAINT_ATTACK        pow= 60  acc=  0  PHYSICAL pp=20  ALWAYS_HIT
      MOVE_THIEF               pow= 50  acc=100  PHYSICAL pp=25  THIEF
      MOVE_PURSUIT             pow= 40  acc=100  PHYSICAL pp=20  PURSUIT
      MOVE_BEAT_UP             pow= 10  acc=100  PHYSICAL pp=10  BEAT_UP
```

## ROCK (7) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_POWER_GEM           pow= 80  acc=100  SPECIAL pp=20  HIT
      MOVE_ROCK_SLIDE          pow= 75  acc= 90  PHYSICAL pp=10  FLINCH_HIT
      MOVE_ANCIENT_POWER       pow= 60  acc=100  SPECIAL pp= 5  ALL_STATS_UP_HIT
      MOVE_ROCK_TOMB           pow= 60  acc= 95  PHYSICAL pp=15  SPEED_DOWN_HIT
      MOVE_ROCK_THROW          pow= 50  acc= 90  PHYSICAL pp=15  HIT
      MOVE_ROLLOUT             pow= 30  acc= 90  PHYSICAL pp=20  ROLLOUT
      MOVE_ROCK_BLAST          pow= 25  acc= 90  PHYSICAL pp=10  MULTI_HIT
```

## GHOST (6) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_SHADOW_BALL         pow= 80  acc=100  SPECIAL pp=15  SPECIAL_DEFENSE_DOWN_H
      MOVE_SHADOW_CLAW         pow= 70  acc=100  PHYSICAL pp=15  HIGH_CRITICAL
      MOVE_SHADOW_PUNCH        pow= 60  acc=  0  PHYSICAL pp=20  ALWAYS_HIT
      MOVE_NIGHT_SHADE         pow= 50  acc=100  SPECIAL pp=15  HIT
      MOVE_ASTONISH            pow= 30  acc=100  PHYSICAL pp=15  FLINCH_MINIMIZE_HIT
      MOVE_LICK                pow= 30  acc=100  PHYSICAL pp=30  PARALYZE_HIT
```

## STEEL (6) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_DOOM_DESIRE         pow=140  acc=100  SPECIAL pp= 5  FUTURE_SIGHT
      MOVE_IRON_TAIL           pow=100  acc= 75  PHYSICAL pp=15  DEFENSE_DOWN_HIT
      MOVE_METEOR_MASH         pow=100  acc= 90  PHYSICAL pp=10  ATTACK_UP_HIT
      MOVE_FLASH_CANNON        pow= 80  acc=100  SPECIAL pp=10  SPECIAL_DEFENSE_DOWN_H
      MOVE_STEEL_WING          pow= 70  acc= 90  PHYSICAL pp=25  DEFENSE_UP_HIT
      MOVE_METAL_CLAW          pow= 50  acc= 95  PHYSICAL pp=35  ATTACK_UP_HIT
```

## DRAGON (5) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_OUTRAGE             pow=100  acc=100  PHYSICAL pp=10  RAMPAGE
      MOVE_DRAGON_PULSE        pow= 85  acc=100  SPECIAL pp=10  HIT
      MOVE_DRAGON_CLAW         pow= 80  acc=100  PHYSICAL pp=15  HIT
      MOVE_TWISTER             pow= 65  acc=100  SPECIAL pp=20  TWISTER
      MOVE_DRAGON_BREATH       pow= 60  acc=100  SPECIAL pp=20  PARALYZE_HIT
```

## FAIRY (2) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_MOONBLAST           pow= 95  acc=100  SPECIAL pp=15  SPECIAL_ATTACK_DOWN_HI
      MOVE_PLAY_ROUGH          pow= 90  acc= 90  PHYSICAL pp=10  ATTACK_DOWN_HIT
```

## MYSTERY (1) — uniform, no tiering needed

Too few moves for weighting to matter; all equally likely.

```
      MOVE_HIDDEN_POWER        pow= 60  acc=100  SPECIAL pp=15  HIDDEN_POWER
```

---

## Note on Fairy

Fairy has only **2** damaging moves, so a pure-Fairy Pokémon would otherwise get the same move
almost every time. Own-type pools below 5 mix in Normal moves at **67% own-type / 33% Normal**.
Only 7 species are affected — Clefairy, Clefable, Cleffa, Togepi, Snubbull, Granbull, Sylveon.
Every Fairy dual-type is saved by the union rule and never falls back.
