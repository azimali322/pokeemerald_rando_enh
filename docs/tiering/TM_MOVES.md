# TM Move Assignment — tiering worksheet

**The candidate pool for TM randomization: 359 moves.**

This is every move that could be assigned to a TM slot when *Random TM Moves* (Phase 8c-b) is on —
the full randomizable move pool (367) minus the 8 HM moves, which must stay fixed.
It is **not** limited to the 50 vanilla Emerald TM moves; those are just the default assignment.

The same tiers drive both halves of Phase 8c:
- **(a)** which TM you find in an item ball — tier the TM by the move it currently teaches
- **(b)** which move gets assigned to each TM slot — tier the candidate directly

Mark a tier in each `[ ]`: `1`=S `2`=A `3`=B `4`=C `5`=D `6`=junk, `X`=exclude.
`[TM]` marks a move that is a TM in vanilla Emerald — useful reference, no special status.

> **Worth excluding outright:** moves that make no sense on a TM — Sketch, Metronome, Mimic,
> Transform, Struggle-likes, and the OHKO moves (Fissure, Horn Drill, Guillotine, Sheer Cold).
> A TM is a permanent, tutorable resource; these are either broken or useless as one.

## Candidate pool (359)

```
  [ ] MOVE_POUND                      pow= 40  acc=100  NORMAL   PHYS pp=35
  [ ] MOVE_KARATE_CHOP                pow= 50  acc=100  FIGHTING PHYS pp=25
  [ ] MOVE_DOUBLE_SLAP                pow= 20  acc= 85  NORMAL   PHYS pp=10
  [ ] MOVE_COMET_PUNCH                pow= 30  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_MEGA_PUNCH                 pow= 80  acc= 85  NORMAL   PHYS pp=20
  [ ] MOVE_PAY_DAY                    pow= 40  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_FIRE_PUNCH                 pow= 75  acc=100  FIRE     PHYS pp=15
  [ ] MOVE_ICE_PUNCH                  pow= 75  acc=100  ICE      PHYS pp=15
  [ ] MOVE_THUNDER_PUNCH              pow= 75  acc=100  ELECTRIC PHYS pp=15
  [ ] MOVE_SCRATCH                    pow= 40  acc=100  NORMAL   PHYS pp=35
  [ ] MOVE_VICE_GRIP                  pow= 55  acc=100  NORMAL   PHYS pp=30
  [ ] MOVE_GUILLOTINE                 pow=  1  acc= 30  NORMAL   PHYS pp= 5
  [ ] MOVE_RAZOR_WIND                 pow= 80  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_SWORDS_DANCE               pow=  0  acc=  0  NORMAL   STAT pp=20
  [ ] MOVE_GUST                       pow= 40  acc=100  FLYING   SPEC pp=35
  [ ] MOVE_WING_ATTACK                pow= 60  acc=100  FLYING   PHYS pp=35
  [ ] MOVE_WHIRLWIND                  pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_BIND                       pow= 15  acc= 85  NORMAL   PHYS pp=20
  [ ] MOVE_SLAM                       pow= 80  acc= 75  NORMAL   PHYS pp=20
  [ ] MOVE_VINE_WHIP                  pow= 45  acc=100  GRASS    PHYS pp=25
  [ ] MOVE_STOMP                      pow= 65  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_DOUBLE_KICK                pow= 30  acc=100  FIGHTING PHYS pp=30
  [ ] MOVE_MEGA_KICK                  pow=120  acc= 75  NORMAL   PHYS pp= 5
  [ ] MOVE_JUMP_KICK                  pow= 85  acc= 95  FIGHTING PHYS pp=10
  [ ] MOVE_ROLLING_KICK               pow= 60  acc= 85  FIGHTING PHYS pp=15
  [ ] MOVE_SAND_ATTACK                pow=  0  acc=100  GROUND   STAT pp=15
  [ ] MOVE_HEADBUTT                   pow= 70  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_HORN_ATTACK                pow= 65  acc=100  NORMAL   PHYS pp=25
  [ ] MOVE_FURY_ATTACK                pow= 15  acc= 85  NORMAL   PHYS pp=20
  [ ] MOVE_HORN_DRILL                 pow=  1  acc= 30  NORMAL   PHYS pp= 5
  [ ] MOVE_TACKLE                     pow= 40  acc=100  NORMAL   PHYS pp=35
  [ ] MOVE_BODY_SLAM                  pow= 85  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_WRAP                       pow= 15  acc= 90  NORMAL   PHYS pp=20
  [ ] MOVE_TAKE_DOWN                  pow= 90  acc= 85  NORMAL   PHYS pp=20
  [ ] MOVE_THRASH                     pow=105  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_DOUBLE_EDGE                pow=120  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_TAIL_WHIP                  pow=  0  acc=100  NORMAL   STAT pp=30
  [ ] MOVE_POISON_STING               pow= 15  acc=100  POISON   PHYS pp=35
  [ ] MOVE_TWINEEDLE                  pow= 25  acc=100  BUG      PHYS pp=20
  [ ] MOVE_PIN_MISSILE                pow= 20  acc= 95  BUG      PHYS pp=20
  [ ] MOVE_LEER                       pow=  0  acc=100  NORMAL   STAT pp=30
  [ ] MOVE_BITE                       pow= 60  acc=100  DARK     PHYS pp=25
  [ ] MOVE_GROWL                      pow=  0  acc=100  NORMAL   STAT pp=40
  [ ] MOVE_ROAR               [TM]    pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_SING                       pow=  0  acc= 55  NORMAL   STAT pp=15
  [ ] MOVE_SUPERSONIC                 pow=  0  acc= 55  NORMAL   STAT pp=20
  [ ] MOVE_SONIC_BOOM                 pow=  1  acc= 90  NORMAL   SPEC pp=20
  [ ] MOVE_DISABLE                    pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_ACID                       pow= 40  acc=100  POISON   SPEC pp=30
  [ ] MOVE_EMBER                      pow= 40  acc=100  FIRE     SPEC pp=25
  [ ] MOVE_FLAMETHROWER       [TM]    pow= 90  acc=100  FIRE     SPEC pp=15
  [ ] MOVE_MIST                       pow=  0  acc=  0  ICE      STAT pp=30
  [ ] MOVE_WATER_GUN                  pow= 40  acc=100  WATER    SPEC pp=25
  [ ] MOVE_HYDRO_PUMP                 pow=120  acc= 80  WATER    SPEC pp= 5
  [ ] MOVE_ICE_BEAM           [TM]    pow= 90  acc=100  ICE      SPEC pp=10
  [ ] MOVE_BLIZZARD           [TM]    pow=110  acc= 70  ICE      SPEC pp= 5
  [ ] MOVE_PSYBEAM                    pow= 65  acc=100  PSYCHIC  SPEC pp=20
  [ ] MOVE_BUBBLE_BEAM                pow= 65  acc=100  WATER    SPEC pp=20
  [ ] MOVE_AURORA_BEAM                pow= 65  acc=100  ICE      SPEC pp=20
  [ ] MOVE_HYPER_BEAM         [TM]    pow=150  acc= 90  NORMAL   SPEC pp= 5
  [ ] MOVE_PECK                       pow= 35  acc=100  FLYING   PHYS pp=35
  [ ] MOVE_DRILL_PECK                 pow= 85  acc=100  FLYING   PHYS pp=20
  [ ] MOVE_SUBMISSION                 pow= 80  acc= 80  FIGHTING PHYS pp=20
  [ ] MOVE_LOW_KICK                   pow=  1  acc=100  FIGHTING PHYS pp=20
  [ ] MOVE_COUNTER                    pow=  1  acc=100  FIGHTING PHYS pp=20
  [ ] MOVE_SEISMIC_TOSS               pow=  1  acc=100  FIGHTING PHYS pp=20
  [ ] MOVE_ABSORB                     pow= 20  acc=100  GRASS    SPEC pp=25
  [ ] MOVE_MEGA_DRAIN                 pow= 40  acc=100  GRASS    SPEC pp=15
  [ ] MOVE_LEECH_SEED                 pow=  0  acc= 90  GRASS    STAT pp=10
  [ ] MOVE_GROWTH                     pow=  0  acc=  0  NORMAL   STAT pp=20
  [ ] MOVE_RAZOR_LEAF                 pow= 55  acc= 95  GRASS    PHYS pp=25
  [ ] MOVE_SOLAR_BEAM         [TM]    pow=120  acc=100  GRASS    SPEC pp=10
  [ ] MOVE_POISON_POWDER              pow=  0  acc= 75  POISON   STAT pp=35
  [ ] MOVE_STUN_SPORE                 pow=  0  acc= 75  GRASS    STAT pp=30
  [ ] MOVE_SLEEP_POWDER               pow=  0  acc= 75  GRASS    STAT pp=15
  [ ] MOVE_PETAL_DANCE                pow=100  acc=100  GRASS    SPEC pp=10
  [ ] MOVE_STRING_SHOT                pow=  0  acc= 95  BUG      STAT pp=40
  [ ] MOVE_DRAGON_RAGE                pow=  1  acc=100  DRAGON   SPEC pp=10
  [ ] MOVE_FIRE_SPIN                  pow= 35  acc= 95  FIRE     SPEC pp=15
  [ ] MOVE_THUNDER_SHOCK              pow= 40  acc=100  ELECTRIC SPEC pp=30
  [ ] MOVE_THUNDERBOLT        [TM]    pow= 90  acc=100  ELECTRIC SPEC pp=15
  [ ] MOVE_THUNDER_WAVE               pow=  0  acc= 90  ELECTRIC STAT pp=20
  [ ] MOVE_THUNDER            [TM]    pow=110  acc= 70  ELECTRIC SPEC pp=10
  [ ] MOVE_ROCK_THROW                 pow= 50  acc= 90  ROCK     PHYS pp=15
  [ ] MOVE_EARTHQUAKE         [TM]    pow=100  acc=100  GROUND   PHYS pp=10
  [ ] MOVE_FISSURE                    pow=  1  acc= 30  GROUND   PHYS pp= 5
  [ ] MOVE_DIG                [TM]    pow= 80  acc=100  GROUND   PHYS pp=10
  [ ] MOVE_TOXIC              [TM]    pow=  0  acc= 70  POISON   STAT pp=10
  [ ] MOVE_CONFUSION                  pow= 50  acc=100  PSYCHIC  SPEC pp=25
  [ ] MOVE_PSYCHIC            [TM]    pow= 90  acc=100  PSYCHIC  SPEC pp=10
  [ ] MOVE_HYPNOSIS                   pow=  0  acc= 60  PSYCHIC  STAT pp=20
  [ ] MOVE_MEDITATE                   pow=  0  acc=  0  PSYCHIC  STAT pp=40
  [ ] MOVE_AGILITY                    pow=  0  acc=  0  PSYCHIC  STAT pp=30
  [ ] MOVE_QUICK_ATTACK               pow= 40  acc=100  NORMAL   PHYS pp=30
  [ ] MOVE_RAGE                       pow= 20  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_TELEPORT                   pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_NIGHT_SHADE                pow= 50  acc=100  GHOST    SPEC pp=15
  [ ] MOVE_MIMIC                      pow=  0  acc=100  NORMAL   STAT pp=10
  [ ] MOVE_SCREECH                    pow=  0  acc= 85  NORMAL   STAT pp=40
  [ ] MOVE_DOUBLE_TEAM        [TM]    pow=  0  acc=  0  NORMAL   STAT pp=15
  [ ] MOVE_RECOVER                    pow=  0  acc=  0  NORMAL   STAT pp= 5
  [ ] MOVE_HARDEN                     pow=  0  acc=  0  NORMAL   STAT pp=30
  [ ] MOVE_MINIMIZE                   pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_SMOKESCREEN                pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_CONFUSE_RAY                pow=  0  acc=100  GHOST    STAT pp=10
  [ ] MOVE_WITHDRAW                   pow=  0  acc=  0  WATER    STAT pp=40
  [ ] MOVE_DEFENSE_CURL               pow=  0  acc=  0  NORMAL   STAT pp=40
  [ ] MOVE_BARRIER                    pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_LIGHT_SCREEN       [TM]    pow=  0  acc=  0  PSYCHIC  STAT pp=30
  [ ] MOVE_HAZE                       pow=  0  acc=  0  ICE      STAT pp=30
  [ ] MOVE_REFLECT            [TM]    pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_FOCUS_ENERGY               pow=  0  acc=  0  NORMAL   STAT pp=30
  [ ] MOVE_BIDE                       pow=  1  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_METRONOME                  pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_MIRROR_MOVE                pow=  0  acc=  0  FLYING   STAT pp=20
  [ ] MOVE_SELF_DESTRUCT              pow=200  acc=100  NORMAL   PHYS pp= 5
  [ ] MOVE_EGG_BOMB                   pow=100  acc= 90  NORMAL   PHYS pp=10
  [ ] MOVE_LICK                       pow= 30  acc=100  GHOST    PHYS pp=30
  [ ] MOVE_SMOG                       pow= 30  acc= 70  POISON   SPEC pp=20
  [ ] MOVE_SLUDGE                     pow= 65  acc=100  POISON   SPEC pp=20
  [ ] MOVE_BONE_CLUB                  pow= 65  acc= 85  GROUND   PHYS pp=20
  [ ] MOVE_FIRE_BLAST         [TM]    pow=110  acc= 85  FIRE     SPEC pp= 5
  [ ] MOVE_CLAMP                      pow= 35  acc= 85  WATER    PHYS pp=15
  [ ] MOVE_SWIFT                      pow= 60  acc=  0  NORMAL   SPEC pp=20
  [ ] MOVE_SKULL_BASH                 pow=100  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_SPIKE_CANNON               pow= 20  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_CONSTRICT                  pow= 25  acc=100  NORMAL   PHYS pp=35
  [ ] MOVE_AMNESIA                    pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_KINESIS                    pow=  0  acc= 80  PSYCHIC  STAT pp=15
  [ ] MOVE_SOFT_BOILED                pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_HI_JUMP_KICK               pow=100  acc= 90  FIGHTING PHYS pp=10
  [ ] MOVE_GLARE                      pow=  0  acc=100  NORMAL   STAT pp=30
  [ ] MOVE_DREAM_EATER                pow=100  acc=100  PSYCHIC  SPEC pp=15
  [ ] MOVE_POISON_GAS                 pow=  0  acc= 90  POISON   STAT pp=40
  [ ] MOVE_BARRAGE                    pow= 20  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_LEECH_LIFE                 pow= 60  acc=100  BUG      PHYS pp=10
  [ ] MOVE_LOVELY_KISS                pow=  0  acc= 75  NORMAL   STAT pp=10
  [ ] MOVE_SKY_ATTACK                 pow=140  acc= 90  FLYING   PHYS pp= 5
  [ ] MOVE_TRANSFORM                  pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_BUBBLE                     pow= 30  acc=100  WATER    SPEC pp=30
  [ ] MOVE_DIZZY_PUNCH                pow= 70  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_SPORE                      pow=  0  acc=100  GRASS    STAT pp=15
  [ ] MOVE_PSYWAVE                    pow=  1  acc=100  PSYCHIC  SPEC pp=15
  [ ] MOVE_SPLASH                     pow=  0  acc=  0  NORMAL   STAT pp=40
  [ ] MOVE_ACID_ARMOR                 pow=  0  acc=  0  POISON   STAT pp=20
  [ ] MOVE_CRABHAMMER                 pow= 90  acc= 90  WATER    PHYS pp=10
  [ ] MOVE_EXPLOSION                  pow=250  acc=100  NORMAL   PHYS pp= 5
  [ ] MOVE_FURY_SWIPES                pow= 18  acc= 80  NORMAL   PHYS pp=15
  [ ] MOVE_BONEMERANG                 pow= 50  acc= 90  GROUND   PHYS pp=10
  [ ] MOVE_REST               [TM]    pow=  0  acc=  0  PSYCHIC  STAT pp= 5
  [ ] MOVE_ROCK_SLIDE                 pow= 75  acc= 90  ROCK     PHYS pp=10
  [ ] MOVE_HYPER_FANG                 pow= 80  acc= 90  NORMAL   PHYS pp=15
  [ ] MOVE_SHARPEN                    pow=  0  acc=  0  NORMAL   STAT pp=30
  [ ] MOVE_CONVERSION                 pow=  0  acc=  0  NORMAL   STAT pp=30
  [ ] MOVE_TRI_ATTACK                 pow= 80  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_SUPER_FANG                 pow=  1  acc= 90  NORMAL   PHYS pp=10
  [ ] MOVE_SLASH                      pow= 70  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_SUBSTITUTE                 pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_STRUGGLE                   pow= 50  acc=100  NORMAL   PHYS pp= 1
  [ ] MOVE_SKETCH                     pow=  0  acc=  0  NORMAL   STAT pp= 1
  [ ] MOVE_TRIPLE_KICK                pow= 10  acc= 90  FIGHTING PHYS pp=10
  [ ] MOVE_THIEF              [TM]    pow= 50  acc=100  DARK     PHYS pp=25
  [ ] MOVE_SPIDER_WEB                 pow=  0  acc=100  BUG      STAT pp=10
  [ ] MOVE_MIND_READER                pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_NIGHTMARE                  pow=  0  acc=100  GHOST    STAT pp=15
  [ ] MOVE_FLAME_WHEEL                pow= 60  acc=100  FIRE     PHYS pp=25
  [ ] MOVE_SNORE                      pow= 50  acc=100  NORMAL   SPEC pp=15
  [ ] MOVE_CURSE                      pow=  0  acc=  0  MYSTERY  STAT pp=10
  [ ] MOVE_FLAIL                      pow=  1  acc=100  NORMAL   PHYS pp=15
  [ ] MOVE_CONVERSION_2               pow=  0  acc=100  NORMAL   STAT pp=30
  [ ] MOVE_AEROBLAST                  pow=100  acc= 95  FLYING   SPEC pp= 5
  [ ] MOVE_COTTON_SPORE               pow=  0  acc=100  GRASS    STAT pp=40
  [ ] MOVE_REVERSAL                   pow=  1  acc=100  FIGHTING PHYS pp=15
  [ ] MOVE_SPITE                      pow=  0  acc=100  GHOST    STAT pp=10
  [ ] MOVE_POWDER_SNOW                pow= 40  acc=100  ICE      SPEC pp=25
  [ ] MOVE_PROTECT            [TM]    pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_MACH_PUNCH                 pow= 40  acc=100  FIGHTING PHYS pp=30
  [ ] MOVE_SCARY_FACE                 pow=  0  acc=100  NORMAL   STAT pp=10
  [ ] MOVE_FAINT_ATTACK               pow= 60  acc=  0  DARK     PHYS pp=20
  [ ] MOVE_SWEET_KISS                 pow=  0  acc= 75  FAIRY    STAT pp=10
  [ ] MOVE_BELLY_DRUM                 pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_SLUDGE_BOMB        [TM]    pow= 90  acc=100  POISON   SPEC pp=10
  [ ] MOVE_MUD_SLAP                   pow= 20  acc=100  GROUND   SPEC pp=10
  [ ] MOVE_OCTAZOOKA                  pow= 65  acc= 85  WATER    SPEC pp=10
  [ ] MOVE_SPIKES                     pow=  0  acc=  0  GROUND   STAT pp=20
  [ ] MOVE_ZAP_CANNON                 pow=120  acc= 50  ELECTRIC SPEC pp= 5
  [ ] MOVE_FORESIGHT                  pow=  0  acc=100  NORMAL   STAT pp=40
  [ ] MOVE_DESTINY_BOND               pow=  0  acc=  0  GHOST    STAT pp= 5
  [ ] MOVE_PERISH_SONG                pow=  0  acc=  0  NORMAL   STAT pp= 5
  [ ] MOVE_ICY_WIND                   pow= 55  acc= 95  ICE      SPEC pp=15
  [ ] MOVE_DETECT                     pow=  0  acc=  0  FIGHTING STAT pp= 5
  [ ] MOVE_BONE_RUSH                  pow= 25  acc= 80  GROUND   PHYS pp=10
  [ ] MOVE_LOCK_ON                    pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_OUTRAGE                    pow=100  acc=100  DRAGON   PHYS pp=10
  [ ] MOVE_SANDSTORM          [TM]    pow=  0  acc=  0  ROCK     STAT pp=10
  [ ] MOVE_GIGA_DRAIN         [TM]    pow= 75  acc=100  GRASS    SPEC pp=10
  [ ] MOVE_ENDURE                     pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_CHARM                      pow=  0  acc=100  FAIRY    STAT pp=20
  [ ] MOVE_ROLLOUT                    pow= 30  acc= 90  ROCK     PHYS pp=20
  [ ] MOVE_FALSE_SWIPE                pow= 40  acc=100  NORMAL   PHYS pp=40
  [ ] MOVE_SWAGGER                    pow=  0  acc= 85  NORMAL   STAT pp=15
  [ ] MOVE_MILK_DRINK                 pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_SPARK                      pow= 65  acc=100  ELECTRIC PHYS pp=20
  [ ] MOVE_FURY_CUTTER                pow= 20  acc= 95  BUG      PHYS pp=20
  [ ] MOVE_STEEL_WING         [TM]    pow= 70  acc= 90  STEEL    PHYS pp=25
  [ ] MOVE_MEAN_LOOK                  pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_ATTRACT            [TM]    pow=  0  acc=100  NORMAL   STAT pp=15
  [ ] MOVE_SLEEP_TALK                 pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_HEAL_BELL                  pow=  0  acc=  0  NORMAL   STAT pp= 5
  [ ] MOVE_RETURN             [TM]    pow=  1  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_PRESENT                    pow=  1  acc= 90  NORMAL   PHYS pp=15
  [ ] MOVE_FRUSTRATION        [TM]    pow=  1  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_SAFEGUARD          [TM]    pow=  0  acc=  0  NORMAL   STAT pp=25
  [ ] MOVE_PAIN_SPLIT                 pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_SACRED_FIRE                pow=100  acc= 95  FIRE     PHYS pp= 5
  [ ] MOVE_MAGNITUDE                  pow=  1  acc=100  GROUND   PHYS pp=30
  [ ] MOVE_DYNAMIC_PUNCH              pow=100  acc= 50  FIGHTING PHYS pp= 5
  [ ] MOVE_MEGAHORN                   pow=120  acc= 85  BUG      PHYS pp=10
  [ ] MOVE_DRAGON_BREATH              pow= 60  acc=100  DRAGON   SPEC pp=20
  [ ] MOVE_BATON_PASS                 pow=  0  acc=  0  NORMAL   STAT pp=40
  [ ] MOVE_ENCORE                     pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_PURSUIT                    pow= 40  acc=100  DARK     PHYS pp=20
  [ ] MOVE_RAPID_SPIN                 pow= 50  acc=100  NORMAL   PHYS pp=40
  [ ] MOVE_SWEET_SCENT                pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_IRON_TAIL          [TM]    pow=100  acc= 75  STEEL    PHYS pp=15
  [ ] MOVE_METAL_CLAW                 pow= 50  acc= 95  STEEL    PHYS pp=35
  [ ] MOVE_VITAL_THROW                pow= 70  acc=100  FIGHTING PHYS pp=10
  [ ] MOVE_MORNING_SUN                pow=  0  acc=  0  NORMAL   STAT pp= 5
  [ ] MOVE_SYNTHESIS                  pow=  0  acc=  0  GRASS    STAT pp= 5
  [ ] MOVE_MOONLIGHT                  pow=  0  acc=  0  FAIRY    STAT pp= 5
  [ ] MOVE_HIDDEN_POWER       [TM]    pow= 60  acc=100  MYSTERY  SPEC pp=15
  [ ] MOVE_CROSS_CHOP                 pow=100  acc= 80  FIGHTING PHYS pp= 5
  [ ] MOVE_TWISTER                    pow= 65  acc=100  DRAGON   SPEC pp=20
  [ ] MOVE_RAIN_DANCE         [TM]    pow=  0  acc=  0  WATER    STAT pp= 5
  [ ] MOVE_SUNNY_DAY          [TM]    pow=  0  acc=  0  FIRE     STAT pp= 5
  [ ] MOVE_CRUNCH                     pow= 80  acc=100  DARK     PHYS pp=15
  [ ] MOVE_MIRROR_COAT                pow=  1  acc=100  PSYCHIC  SPEC pp=20
  [ ] MOVE_PSYCH_UP                   pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_EXTREME_SPEED              pow= 80  acc=100  NORMAL   PHYS pp= 5
  [ ] MOVE_ANCIENT_POWER              pow= 60  acc=100  ROCK     SPEC pp= 5
  [ ] MOVE_SHADOW_BALL        [TM]    pow= 80  acc=100  GHOST    SPEC pp=15
  [ ] MOVE_FUTURE_SIGHT               pow=120  acc=100  PSYCHIC  SPEC pp=10
  [ ] MOVE_WHIRLPOOL                  pow= 35  acc= 95  WATER    SPEC pp=15
  [ ] MOVE_BEAT_UP                    pow= 10  acc=100  DARK     PHYS pp=10
  [ ] MOVE_FAKE_OUT                   pow= 40  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_UPROAR                     pow= 70  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_STOCKPILE                  pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_SPIT_UP                    pow=100  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_SWALLOW                    pow=  0  acc=  0  NORMAL   STAT pp=10
  [ ] MOVE_HEAT_WAVE                  pow= 95  acc= 90  FIRE     SPEC pp=10
  [ ] MOVE_HAIL               [TM]    pow=  0  acc=  0  ICE      STAT pp=10
  [ ] MOVE_TORMENT            [TM]    pow=  0  acc=100  DARK     STAT pp=15
  [ ] MOVE_FLATTER                    pow=  0  acc=100  DARK     STAT pp=15
  [ ] MOVE_WILL_O_WISP                pow=  0  acc= 85  FIRE     STAT pp=15
  [ ] MOVE_MEMENTO                    pow=  0  acc=100  DARK     STAT pp=10
  [ ] MOVE_FACADE             [TM]    pow= 70  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_FOCUS_PUNCH        [TM]    pow=150  acc=100  FIGHTING PHYS pp=20
  [ ] MOVE_SMELLING_SALT              pow= 70  acc=100  NORMAL   PHYS pp=10
  [ ] MOVE_FOLLOW_ME                  pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_NATURE_POWER               pow=  0  acc= 95  NORMAL   STAT pp=20
  [ ] MOVE_CHARGE                     pow=  0  acc=100  ELECTRIC STAT pp=20
  [ ] MOVE_TAUNT              [TM]    pow=  0  acc=100  DARK     STAT pp=20
  [ ] MOVE_HELPING_HAND               pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_TRICK                      pow=  0  acc=100  PSYCHIC  STAT pp=10
  [ ] MOVE_ROLE_PLAY                  pow=  0  acc=100  PSYCHIC  STAT pp=10
  [ ] MOVE_WISH                       pow=  0  acc=100  NORMAL   STAT pp=10
  [ ] MOVE_ASSIST                     pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_INGRAIN                    pow=  0  acc=100  GRASS    STAT pp=20
  [ ] MOVE_SUPERPOWER                 pow=120  acc=100  FIGHTING PHYS pp= 5
  [ ] MOVE_MAGIC_COAT                 pow=  0  acc=100  PSYCHIC  STAT pp=15
  [ ] MOVE_RECYCLE                    pow=  0  acc=100  NORMAL   STAT pp=10
  [ ] MOVE_REVENGE                    pow= 60  acc=100  FIGHTING PHYS pp=10
  [ ] MOVE_BRICK_BREAK        [TM]    pow= 75  acc=100  FIGHTING PHYS pp=15
  [ ] MOVE_YAWN                       pow=  0  acc=100  NORMAL   STAT pp=10
  [ ] MOVE_KNOCK_OFF                  pow= 65  acc=100  DARK     PHYS pp=20
  [ ] MOVE_ENDEAVOR                   pow=  1  acc=100  NORMAL   PHYS pp= 5
  [ ] MOVE_ERUPTION                   pow=150  acc=100  FIRE     SPEC pp= 5
  [ ] MOVE_SKILL_SWAP         [TM]    pow=  0  acc=100  PSYCHIC  STAT pp=10
  [ ] MOVE_IMPRISON                   pow=  0  acc=100  PSYCHIC  STAT pp=10
  [ ] MOVE_REFRESH                    pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_GRUDGE                     pow=  0  acc=100  GHOST    STAT pp= 5
  [ ] MOVE_SNATCH             [TM]    pow=  0  acc=100  DARK     STAT pp=10
  [ ] MOVE_SECRET_POWER       [TM]    pow= 70  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_ARM_THRUST                 pow= 15  acc=100  FIGHTING PHYS pp=20
  [ ] MOVE_CAMOUFLAGE                 pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_TAIL_GLOW                  pow=  0  acc=100  BUG      STAT pp=20
  [ ] MOVE_LUSTER_PURGE               pow= 70  acc=100  PSYCHIC  SPEC pp= 5
  [ ] MOVE_MIST_BALL                  pow= 70  acc=100  PSYCHIC  SPEC pp= 5
  [ ] MOVE_FEATHER_DANCE              pow=  0  acc=100  FLYING   STAT pp=15
  [ ] MOVE_TEETER_DANCE               pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_BLAZE_KICK                 pow= 85  acc= 90  FIRE     PHYS pp=10
  [ ] MOVE_MUD_SPORT                  pow=  0  acc=100  GROUND   STAT pp=15
  [ ] MOVE_ICE_BALL                   pow= 30  acc= 90  ICE      PHYS pp=20
  [ ] MOVE_NEEDLE_ARM                 pow= 60  acc=100  GRASS    PHYS pp=15
  [ ] MOVE_SLACK_OFF                  pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_HYPER_VOICE                pow= 90  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_POISON_FANG                pow= 65  acc=100  POISON   PHYS pp=15
  [ ] MOVE_CRUSH_CLAW                 pow= 75  acc= 95  NORMAL   PHYS pp=10
  [ ] MOVE_BLAST_BURN                 pow=150  acc= 90  FIRE     SPEC pp= 5
  [ ] MOVE_HYDRO_CANNON               pow=150  acc= 90  WATER    SPEC pp= 5
  [ ] MOVE_METEOR_MASH                pow=100  acc= 90  STEEL    PHYS pp=10
  [ ] MOVE_ASTONISH                   pow= 30  acc=100  GHOST    PHYS pp=15
  [ ] MOVE_WEATHER_BALL               pow= 50  acc=100  NORMAL   SPEC pp=10
  [ ] MOVE_AROMATHERAPY               pow=  0  acc=  0  GRASS    STAT pp= 5
  [ ] MOVE_FAKE_TEARS                 pow=  0  acc=100  DARK     STAT pp=20
  [ ] MOVE_AIR_CUTTER                 pow= 60  acc= 95  FLYING   SPEC pp=25
  [ ] MOVE_OVERHEAT           [TM]    pow=130  acc= 90  FIRE     SPEC pp= 5
  [ ] MOVE_ODOR_SLEUTH                pow=  0  acc=100  NORMAL   STAT pp=40
  [ ] MOVE_ROCK_TOMB          [TM]    pow= 60  acc= 95  ROCK     PHYS pp=15
  [ ] MOVE_SILVER_WIND                pow= 60  acc=100  BUG      SPEC pp= 5
  [ ] MOVE_METAL_SOUND                pow=  0  acc= 85  STEEL    STAT pp=40
  [ ] MOVE_GRASS_WHISTLE              pow=  0  acc= 55  GRASS    STAT pp=15
  [ ] MOVE_TICKLE                     pow=  0  acc=100  NORMAL   STAT pp=20
  [ ] MOVE_COSMIC_POWER               pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_WATER_SPOUT                pow=150  acc=100  WATER    SPEC pp= 5
  [ ] MOVE_SIGNAL_BEAM                pow= 75  acc=100  BUG      SPEC pp=15
  [ ] MOVE_SHADOW_PUNCH               pow= 60  acc=  0  GHOST    PHYS pp=20
  [ ] MOVE_EXTRASENSORY               pow= 80  acc=100  PSYCHIC  SPEC pp=20
  [ ] MOVE_SKY_UPPERCUT               pow= 85  acc= 90  FIGHTING PHYS pp=15
  [ ] MOVE_SAND_TOMB                  pow= 35  acc= 95  GROUND   PHYS pp=15
  [ ] MOVE_SHEER_COLD                 pow=  1  acc= 30  ICE      SPEC pp= 5
  [ ] MOVE_MUDDY_WATER                pow= 90  acc= 85  GROUND   SPEC pp=10
  [ ] MOVE_BULLET_SEED        [TM]    pow= 20  acc=100  GRASS    PHYS pp=30
  [ ] MOVE_AERIAL_ACE         [TM]    pow= 60  acc=  0  FLYING   PHYS pp=20
  [ ] MOVE_ICICLE_SPEAR               pow= 20  acc=100  ICE      PHYS pp=30
  [ ] MOVE_IRON_DEFENSE               pow=  0  acc=  0  STEEL    STAT pp=15
  [ ] MOVE_BLOCK                      pow=  0  acc=100  NORMAL   STAT pp= 5
  [ ] MOVE_HOWL                       pow=  0  acc=  0  NORMAL   STAT pp=40
  [ ] MOVE_DRAGON_CLAW        [TM]    pow= 80  acc=100  DRAGON   PHYS pp=15
  [ ] MOVE_FRENZY_PLANT               pow=150  acc= 90  GRASS    SPEC pp= 5
  [ ] MOVE_BULK_UP            [TM]    pow=  0  acc=  0  FIGHTING STAT pp=20
  [ ] MOVE_BOUNCE                     pow= 85  acc= 85  FLYING   PHYS pp= 5
  [ ] MOVE_MUD_SHOT                   pow= 55  acc= 95  GROUND   SPEC pp=15
  [ ] MOVE_POISON_TAIL                pow= 50  acc=100  POISON   PHYS pp=25
  [ ] MOVE_COVET                      pow= 60  acc=100  NORMAL   PHYS pp=20
  [ ] MOVE_VOLT_TACKLE                pow=120  acc=100  ELECTRIC PHYS pp=15
  [ ] MOVE_MAGICAL_LEAF               pow= 60  acc=  0  GRASS    SPEC pp=20
  [ ] MOVE_WATER_SPORT                pow=  0  acc=100  WATER    STAT pp=15
  [ ] MOVE_CALM_MIND          [TM]    pow=  0  acc=  0  PSYCHIC  STAT pp=20
  [ ] MOVE_LEAF_BLADE                 pow= 90  acc=100  GRASS    PHYS pp=15
  [ ] MOVE_DRAGON_DANCE               pow=  0  acc=  0  DRAGON   STAT pp=20
  [ ] MOVE_ROCK_BLAST                 pow= 25  acc= 90  ROCK     PHYS pp=10
  [ ] MOVE_SHOCK_WAVE         [TM]    pow= 60  acc=  0  ELECTRIC SPEC pp=20
  [ ] MOVE_WATER_PULSE        [TM]    pow= 60  acc=100  WATER    SPEC pp=20
  [ ] MOVE_DOOM_DESIRE                pow=140  acc=100  STEEL    SPEC pp= 5
  [ ] MOVE_PSYCHO_BOOST               pow=140  acc= 90  PSYCHIC  SPEC pp= 5
  [ ] MOVE_DARK_PULSE                 pow= 80  acc=100  DARK     SPEC pp=15
  [ ] MOVE_PSYCHO_CUT                 pow= 70  acc=100  PSYCHIC  PHYS pp=20
  [ ] MOVE_FOCUS_BLAST                pow=120  acc= 70  FIGHTING SPEC pp= 5
  [ ] MOVE_POWER_GEM                  pow= 80  acc=100  ROCK     SPEC pp=20
  [ ] MOVE_SHADOW_CLAW                pow= 70  acc=100  GHOST    PHYS pp=15
  [ ] MOVE_FLASH_CANNON               pow= 80  acc=100  STEEL    SPEC pp=10
  [ ] MOVE_AIR_SLASH                  pow= 75  acc= 95  FLYING   SPEC pp=15
  [ ] MOVE_BUG_BUZZ                   pow= 90  acc=100  BUG      SPEC pp=10
  [ ] MOVE_DRAGON_PULSE               pow= 85  acc=100  DRAGON   SPEC pp=10
  [ ] MOVE_EARTH_POWER                pow= 90  acc=100  GROUND   SPEC pp=10
  [ ] MOVE_PLAY_ROUGH                 pow= 90  acc= 90  FAIRY    PHYS pp=10
  [ ] MOVE_MOONBLAST                  pow= 95  acc=100  FAIRY    SPEC pp=15
  [ ] MOVE_POISON_JAB                 pow= 80  acc=100  POISON   PHYS pp=20
```

## The 50 TM slots — vanilla assignment (reference)

What each TM teaches today. Under (b) these get reassigned from the candidate pool above.

```
  TM01  MOVE_FOCUS_PUNCH       pow=150  acc=100  FIGHTING
  TM02  MOVE_DRAGON_CLAW       pow= 80  acc=100  DRAGON  
  TM03  MOVE_WATER_PULSE       pow= 60  acc=100  WATER   
  TM04  MOVE_CALM_MIND         pow=  0  acc=  0  PSYCHIC 
  TM05  MOVE_ROAR              pow=  0  acc=100  NORMAL  
  TM06  MOVE_TOXIC             pow=  0  acc= 70  POISON  
  TM07  MOVE_HAIL              pow=  0  acc=  0  ICE     
  TM08  MOVE_BULK_UP           pow=  0  acc=  0  FIGHTING
  TM09  MOVE_BULLET_SEED       pow= 20  acc=100  GRASS   
  TM10  MOVE_HIDDEN_POWER      pow= 60  acc=100  MYSTERY 
  TM11  MOVE_SUNNY_DAY         pow=  0  acc=  0  FIRE    
  TM12  MOVE_TAUNT             pow=  0  acc=100  DARK    
  TM13  MOVE_ICE_BEAM          pow= 90  acc=100  ICE     
  TM14  MOVE_BLIZZARD          pow=110  acc= 70  ICE     
  TM15  MOVE_HYPER_BEAM        pow=150  acc= 90  NORMAL  
  TM16  MOVE_LIGHT_SCREEN      pow=  0  acc=  0  PSYCHIC 
  TM17  MOVE_PROTECT           pow=  0  acc=  0  NORMAL  
  TM18  MOVE_RAIN_DANCE        pow=  0  acc=  0  WATER   
  TM19  MOVE_GIGA_DRAIN        pow= 75  acc=100  GRASS   
  TM20  MOVE_SAFEGUARD         pow=  0  acc=  0  NORMAL  
  TM21  MOVE_FRUSTRATION       pow=  1  acc=100  NORMAL  
  TM22  MOVE_SOLAR_BEAM        pow=120  acc=100  GRASS   
  TM23  MOVE_IRON_TAIL         pow=100  acc= 75  STEEL   
  TM24  MOVE_THUNDERBOLT       pow= 90  acc=100  ELECTRIC
  TM25  MOVE_THUNDER           pow=110  acc= 70  ELECTRIC
  TM26  MOVE_EARTHQUAKE        pow=100  acc=100  GROUND  
  TM27  MOVE_RETURN            pow=  1  acc=100  NORMAL  
  TM28  MOVE_DIG               pow= 80  acc=100  GROUND  
  TM29  MOVE_PSYCHIC           pow= 90  acc=100  PSYCHIC 
  TM30  MOVE_SHADOW_BALL       pow= 80  acc=100  GHOST   
  TM31  MOVE_BRICK_BREAK       pow= 75  acc=100  FIGHTING
  TM32  MOVE_DOUBLE_TEAM       pow=  0  acc=  0  NORMAL  
  TM33  MOVE_REFLECT           pow=  0  acc=  0  PSYCHIC 
  TM34  MOVE_SHOCK_WAVE        pow= 60  acc=  0  ELECTRIC
  TM35  MOVE_FLAMETHROWER      pow= 90  acc=100  FIRE    
  TM36  MOVE_SLUDGE_BOMB       pow= 90  acc=100  POISON  
  TM37  MOVE_SANDSTORM         pow=  0  acc=  0  ROCK    
  TM38  MOVE_FIRE_BLAST        pow=110  acc= 85  FIRE    
  TM39  MOVE_ROCK_TOMB         pow= 60  acc= 95  ROCK    
  TM40  MOVE_AERIAL_ACE        pow= 60  acc=  0  FLYING  
  TM41  MOVE_TORMENT           pow=  0  acc=100  DARK    
  TM42  MOVE_FACADE            pow= 70  acc=100  NORMAL  
  TM43  MOVE_SECRET_POWER      pow= 70  acc=100  NORMAL  
  TM44  MOVE_REST              pow=  0  acc=  0  PSYCHIC 
  TM45  MOVE_ATTRACT           pow=  0  acc=100  NORMAL  
  TM46  MOVE_THIEF             pow= 50  acc=100  DARK    
  TM47  MOVE_STEEL_WING        pow= 70  acc= 90  STEEL   
  TM48  MOVE_SKILL_SWAP        pow=  0  acc=100  PSYCHIC 
  TM49  MOVE_SNATCH            pow=  0  acc=100  DARK    
  TM50  MOVE_OVERHEAT          pow=130  acc= 90  FIRE    
```

## The 8 HMs — NEVER RANDOMIZE, NEVER TIER

Excluded from the candidate pool above. Randomizing any of these can soft-lock the playthrough.

```
  HM01  MOVE_CUT
  HM02  MOVE_FLY
  HM03  MOVE_SURF
  HM04  MOVE_STRENGTH
  HM05  MOVE_FLASH
  HM06  MOVE_ROCK_SMASH
  HM07  MOVE_WATERFALL
  HM08  MOVE_DIVE
```
