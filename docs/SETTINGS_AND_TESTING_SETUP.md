# Settings & Testing Setup

What's built, what each toggle does, and how to set up a ROM to test it.

Companions: [`RANDOMIZER_ENHANCEMENTS_PLAN.md`](RANDOMIZER_ENHANCEMENTS_PLAN.md) (design) and
[`RANDOMIZER_ENHANCEMENTS_TESTING.md`](RANDOMIZER_ENHANCEMENTS_TESTING.md) (per-phase checklists).

---

## 1. Inventory — what's done

**Everything requested is built and on `master`**, verified by checking the code rather than this table
— it has drifted before. The one exception is noted under *What's left*.

| Phase | Feature | Status | PR |
|---|---|:-:|---|
| 0 | Build environment | ✅ | — |
| 1 | Save flags + menu plumbing | ✅ | #1 |
| 2 | Cheap Ultra Balls (₽1) | ✅ | #1 |
| 2b | Evolution stones + trade-evo items ₽1, Self-trader ₽1 | ✅ | #1 |
| 3 | Level-scaled wild randomization | ✅ | #1 |
| 3b | "Improved" balancing (base stat total) | ✅ | #1 |
| 4 | Random legendary encounters | ✅ | #1 |
| 5 | Weighted move pool (community tiers) | ✅ | #5 |
| 6 | Guaranteed same-type move | ✅ | #1 |
| 7 | Weighted ability pool (community tiers) | ✅ | #1 |
| 8 | Weighted item pool | ✅ | #8 |
| 8b | Smart learnsets | ✅ | #3 |
| 8c | Weighted TM drops + randomized TM moves | ✅ | #6 |
| 9 | Opponent type display in battle | ✅ | #2 |
| 10 | Level Cap Candy (party menu) | ✅ | #7 |
| 11 | Nature reroll cheat | ✅ | #7 |
| 12 | Per-type STAB tiering + physical/special split | ✅ | #12 |
| 14 | Free TM/HM use — any TM or HM on any Pokemon (`FREE TM/HM USE`) | ✅ | #16 |
| 13 | STAB/learnset refinements — level-capped STAB injection, guaranteed uncapped same-type move above Lv30, 40% same-type bias on ordinary slots, own-type pool dealt without replacement | ✅ | #13 |

### What's left

Nothing is blocked, and every requested feature is built:

| Item | Size | Why it was deferred |
|---|---|---|
| **Play-testing** | — | Everything is verified by reproducing the algorithms against the ROM's own data tables. In-game behaviour is confirmed only where it has actually been played. **The only work genuinely outstanding.** |
| ~~Per-type STAB tiering~~ | — | **Done** (#12). Reuses the Phase 5 tiers — no separate ranking was needed, since all 8 types with 10+ STAB moves already spread across 3-4 tiers. Added a physical/special split at the same time. |

### Ideas considered and not built

- **Cap Level on box Pokemon** — asked for, scoped, and **declined**. The data side is easy: a box
  Pokemon's level is derived from its EXP, `BoxMonToMon` converts it and writing back is a struct copy.
  The blocker is the prompts. `pokemon_storage_system.c` contains **no** reference to
  `MonTryLearningNewMove`, `BeginEvolutionScene` or `DisplayMonNeedsToReplaceMove` — the PC hosts none
  of that flow, only a yes/no menu. So an in-PC version either silently skips move choices and the
  evolution scene, stops at the first obstacle (which for anything with an evolution in range is
  almost immediately), or needs that whole UI rebuilt in the storage system. The party-menu Cap Level
  covers the need; withdraw the Pokemon first.

- **BST mode for trainers** — Phase 3b applies to wild Pokémon only.
- **Trainer moveset de-duplication** — 47% of explicit-moveset trainer entries currently share movesets
  (see Appendix B). One-line fix, but it changes every trainer, so it wants a deliberate decision.
- **Tertu's dynamic species tables** — ruled out; EWRAM is at 99.6%. See Appendix G.
- **Reserving a pool move for the guaranteed late slot** — evaluated and **declined**. For a 2-move type
  (Fairy) the earlier slots can use the pool up before the promoted post-Lv30 slot is reached, so that
  slot falls through to the general pool 0.9% of the time overall. Letting guaranteed slots draw first
  would fix it but reshuffles 69.5% of all learnsets; a narrower "don't take the last move" rule fixes it
  for 0.9%. Neither was taken: the mon still learns its same-type moves, just earlier, which is the
  intended behaviour for an exhausted pool.
- **General-pool slots repeating each other** — 7.3% of non-type-leaning slots duplicate another slot in
  the same learnset, because the re-roll chain feeds its own result back and different slots can converge.
  Pre-existing. Mon creation retries on duplicates; a level-up does not, so that level-up silently does
  nothing. Not investigated further.

---

## 2. Every toggle, and what it needs

All randomizer settings live in the **New Game options menu**. They are set at the start of a run and
stored in the save — you cannot change them mid-playthrough.

### Randomizer page

| Setting | Values | **Requires** | Phase |
|---|---|---|:-:|
| `RANDOMIZER` | Off / On | — **master switch, everything needs this** | — |
| `WILD PKMN` | Off / On | Randomizer | — |
| `TRAINER` | Off / On | Randomizer | — |
| `STATIC` | Off / On | Randomizer | — |
| `BALANCING` | Off / **Balanced** / **Improved** | Randomizer + one of Wild/Starter/Trainer/Static | 3b |
| `LEVEL-SCALED WILDS` | Off / On | Randomizer + **Wild Pkmn**, and Chaos off | 3 |
| `INCLUDE LEGENDARIES` | Off / On | Randomizer + a species option | — |
| `RANDOM LEGENDARIES` | Off / On | Randomizer, Chaos off | 4 |
| `MOVES` | Off / On | Randomizer | — |
| `VGC MOVE POOL` | Off / Weighted / Strict | Randomizer + **Moves** | 5 |
| `GUARANTEE STAB` | Off / On | Randomizer + **Moves** | 6 |
| `SMART LEARNSETS` | Off / On | Randomizer + **Moves** | 8b |
| `ABILITIES` | Off / On | Randomizer | — |
| `VGC ABILITY POOL` | Off / Weighted / Strict | Randomizer + **Abilities** | 7 |
| `ITEMS` | Off / On | Randomizer | — |
| `VGC ITEM POOL` | Off / Weighted / Strict | Randomizer + **Items** | 8 |
| `VGC TM POOL` | Off / Weighted / Strict | Randomizer + **Items** | 8c(a) |
| `RANDOM TM MOVES` | Off / On | Randomizer | 8c(b) |

### Features / challenges page

| Setting | Values | Requires | Phase |
|---|---|---|:-:|
| `CHEAP SHOP` | Off / On | — | 2, 2b |
| `REROLL CHEAT` | Off / On | — | 11 |

### Not in the randomizer menu

Three features are driven from elsewhere:

| Feature | Controlled by | Where |
|---|---|:-:|
| **Opponent type box in battle** | `optionTypeEffective` — the existing battle-hints option | Options menu, not the randomizer menu |
| **Level Cap Candy** | `LEVEL CAP` being set to Normal or Hard | Difficulty page. The cheat lady only hands the item over if a cap is set |
| **Trade evolutions** | Nothing — always available | Self-trader, Lilycove Dept. Store 1F (₽1 with *Cheap Shop*) |

### Is GUARANTEE STAB still worth it once SMART LEARNSETS is on?

Yes, and the reason is the **mid-game**. Smart Learnsets guarantees a same-type move is *learned* — the
first entry, and one above level 30. It cannot guarantee the move is still in one of the four slots later,
because a Pokemon holds four moves and a learnset has around twenty. The level 1 guaranteed move is long
gone by level 25, and the late one has not arrived yet.

Measured with *Smart Learnsets* on and *Guarantee STAB* **off** — share of generated Pokemon holding no
same-type damaging move at all:

| generated at | Lv5 | Lv10 | Lv15 | Lv20 | Lv25 | Lv30 | Lv40 | Lv50 | Lv70 |
|---|---|---|---|---|---|---|---|---|---|
| no same-type move | 6.2% | 8.8% | 17.5% | 24.6% | **25.4%** | 23.8% | 16.9% | 9.8% | 7.0% |

So a quarter of mid-game opponents and encounters would have no STAB at all. *Guarantee STAB* is a final
per-Pokemon check applied after the moveset is built, on all six generation paths, so it closes that
window outright. It also still matters when *Smart Learnsets* is off, and it is what covers static and
gift Pokemon.

Keep both on for play. Turn *Guarantee STAB* off only when testing learnsets in isolation, per section 3b.

### How TM compatibility actually works here (it is not vanilla)

Two independent things are randomized, and it is worth being precise about both.

**1. What each TM teaches** — `RANDOM TM MOVES` re-rolls every TM's move from the weighted tier pool.
The roll is not type-aware, and moves that are not TMs in Emerald at all (Ice Punch, Dragon Dance, …) can
land on a TM slot.

**2. Who can learn that TM** — this is the surprising one. `CanMonLearnTMHM()` in `src/pokemon.c:10200`
does this before looking anything up:

```c
if (gSaveBlock1Ptr->tx_Random_Moves)
    species = GetSpeciesRandomSeeded(species, TX_RANDOM_T_MOVES, 0);
```

With `MOVES` on, most Pokemon therefore read a **different species'** compatibility list. It is stable per
species across a save, but unrelated to what the Pokemon can canonically learn. This is stock upstream
behaviour, not something this fork added.

It does **not** apply to everything. `GetSpeciesRandomSeeded` returns the species unchanged when its
`gSpeciesMapping` slot is `EVO_TYPE_SELF`, or when it is a legendary and *Include Legendaries* is off:

| mapping slot | species | TM list used |
|---|---|---|
| `EVO_TYPE_0 / 1 / 2` | **395** | a substituted species' list |
| `EVO_TYPE_SELF` | 27 | its own, vanilla |
| `EVO_TYPE_LEGENDARY` | 28 | another legendary's if *Random Legendaries* is on; its own if *Include Legendaries* is off; otherwise substituted |

So **88% of species** do not use their own TM compatibility once `MOVES` is on.

**Worked example.** Articuno can learn 23 TM slots in vanilla and cannot learn TM01. Ice Punch is not a TM
in Emerald at all — it is a Move Tutor move. With `MOVES` + `RANDOM TM MOVES` on, Ice Punch can land on a
TM slot, and because Articuno is `EVO_TYPE_LEGENDARY`, whether it can use that TM depends on the settings:
with *Random Legendaries* on it reads another legendary's list; with *Include Legendaries* off it reads its
own. Either way canon "can Articuno learn Ice Punch" never enters into it.

**What that means for type coverage.** Modelling the substitution (and the species that escape it), a species has on average **0.96**
same-type damaging TMs it can actually learn, and **39.4%** have none at all. So TMs are a thin safety net
for a Pokemon with no same-type move — thinner than the stock TM set, where the spread across types is
deliberate.

**`FREE TM/HM USE` removes the restriction entirely.** Turn it On and any TM or HM can be taught to
any Pokemon. That is the [Randolocke](https://www.pokecommunity.com/threads/pok%C3%A9mon-randolocke-v1-1.537596/)
approach, and the equivalent of the Universal Pokemon Randomizer's *Full compatibility* setting.

Why this rather than restoring the vanilla lists: the compatibility table is a poor fit for a randomizer
either way, and upstream's substitution makes it arbitrary rather than merely restrictive. Measured against
each Pokemon's real list, the borrowed list wrongly allows about **7** TMs and wrongly denies **7** more,
and a Magikarp -- which can learn **0** TMs in the base game -- ends up able to learn 30. Handing the
decision to the player is simpler than arguing about which wrong answer is less wrong.

**HMs are included.** This removes the one genuine soft-lock in a randomized run -- a party where nobody can
learn Surf -- at the cost of making HM slaves unnecessary. If you would rather keep HM eligibility as a
constraint, leave this Off.

UPR's own tooltip for the equivalent option warns that it "might make the game too easy", and that is worth
taking seriously: pair it with a `LEVEL CAP` if you want the difficulty to come from somewhere. Randolocke
does exactly that, with caps from 14 to 63.

Note this option does **not** require `MOVES` -- it overrides the compatibility check regardless of whether
anything is randomized.

Levers, if TMs are meant to be your fallback:

- Leave **`RANDOM TM MOVES` off**. The stock TM list is spread across types on purpose. (Compatibility is
  still substituted whenever `MOVES` is on — that part cannot be turned off separately.)
- Turn **`MOVES` off** if you want genuine vanilla TM compatibility. That also disables Smart Learnsets,
  Guarantee STAB and the VGC move pool, so it is a big trade.
- **`REUSABLE TMS`** removes the single-use limit. It is **on automatically under `GAMEMODE: Modern`**, off
  under Classic, and freely editable under Custom.

### Two behaviours worth understanding

**`VGC ABILITY POOL` replaces the base ability randomizer, it does not add to it.** With it on,
`GetAbilityBySpecies` returns a tier-weighted ability directly and the species-substitution path never
runs. You still have to turn `ABILITIES` on to unlock the option in the menu, but once set, VGC is what
decides the ability.

**`BALANCING` has three states, and the menu used to be inverted.** It now reads plainly: Off / Balanced
(evolution stage, the original behaviour) / Improved (base stat total). If *Off* ever appears to behave
like *Balanced*, that inversion has come back — it's the first thing to check.

---

## 3. Recommended settings for a full-feature run

Everything on, tuned for nuzlocke play:

```
RANDOMIZER ............ On
  WILD PKMN ........... On
  TRAINER ............. On
  STATIC .............. On
  BALANCING ........... Improved          <- base stat total, the new mode
  LEVEL-SCALED WILDS .. On                <- no Lv4 Salamence
  INCLUDE LEGENDARIES . On                <- optional; legendaries in the wild
  RANDOM LEGENDARIES .. On                <- legendary encounters stay legendary
  MOVES ............... On
  VGC MOVE POOL ....... Weighted
  GUARANTEE STAB ...... On
  SMART LEARNSETS ..... On
  ABILITIES ........... On
  VGC ABILITY POOL .... Weighted
  ITEMS ............... On
  VGC ITEM POOL ....... Weighted
  VGC TM POOL ......... Weighted
  RANDOM TM MOVES ..... On

LEVEL CAP ............. Normal (or Hard)  <- required for the Cap Candy
CHEAP SHOP ............ On
REROLL CHEAT .......... On
```

Use **Strict** rather than Weighted only if you want a deliberately overpowered run — Strict draws from the
top two tiers only, so a party will repeat options.

---

## 3b. Testing Smart Learnsets specifically

Smart Learnsets is the most intricate feature in the fork and the one with the most moving parts, so it
gets its own recipe. The goal is to isolate it: turn on exactly what it needs, and turn **off** everything
that would make its output hard to read.

### Settings to use

```
RANDOMIZER ............ On     <- master switch; nothing works without it
  WILD PKMN ........... On     <- gives you mons to inspect
  TRAINER ............. On     <- second source of randomized movesets
  MOVES ............... On     <- REQUIRED: unlocks the three sub-options below
  VGC MOVE POOL ....... Weighted
  SMART LEARNSETS ..... On     <- the feature under test
  GUARANTEE STAB ...... Off    <- see below
```

### Settings to deliberately turn OFF

| Turn off | Why it interferes |
|---|---|
| **`GUARANTEE STAB`** | It injects a same-type move *at catch time*, on top of the learnset. With it on you cannot tell whether a same-type move came from the learnset or from the injection. Turn it on again for a second pass once the learnset behaviour looks right. |
| **`CHAOS`** | Replaces the seeded roll with `Random()`, so nothing is reproducible and the same mon gives a different answer every time. It also disables Level-Scaled Wilds and Random Legendaries. |
| **`VGC MOVE POOL` = Strict** | Draws from the top two tiers only, so every mon's moves look alike and the tier weighting cannot be observed. Use **Weighted**. |
| **`TYPE`** (type randomizer) | Re-types species, so "does this move match its type" needs you to look up the randomized type first. Fine later, noise now. |
| **`RANDOM TM MOVES`, `VGC TM POOL`, `VGC ITEM POOL`** | Unrelated to learnsets; they just add log traffic. |
| `MODERN MOVES`, `FAIRY TYPES`, `MODERN TYPES` | Each swaps which learnset table and type chart are used. Leave at defaults for the first pass, then re-test with them on — the code paths are genuinely different. |

`ABILITIES`, `STATIC`, `BALANCING`, and the difficulty page do not affect learnsets and can be set however
you like.

### What to actually look at

The **Move Relearner** is the best window into a learnset — it lists every move the species learns, so you
see the whole randomized table at once rather than four moves at a time. The summary screen's level-up
list works too.

Check these in order:

1. **Power scales with level.** Nothing over 60 power below level 16, nothing over 100 below level 36.
   Above 35 there is no cap, and that is intentional.
2. **The first move is same-type.** Every species, no exceptions.
3. **One entry above level 30 is same-type and can be full power.** Which entry is fixed per species.
   59 of 461 species end their learnset at or below level 30 and correctly have no such entry.
4. **Roughly half the other damaging moves match the type.** Measured at 46%. If *every* damaging move
   matches, something is over-firing; if almost none do, the bias is not running.
5. **No repeated same-type move.** A Pokémon should not learn Thunderbolt twice. Shallow types are the
   ones to check: **Sylveon** is the only mono-Fairy and has just two Fairy moves — it must learn each
   once, then fall back to the general pool.
6. **Status slots stay status.** A slot that taught a status move should still teach one.

### Reading the log

With mGBA's Debug channel on, every learnset entry prints one line:

```
TX LEARNSET TYPE   : 282=KADABRA Lv36 PSYCHIC -> 248=FUTURE SIGHT (120 pow, cap 65535)
TX LEARNSET GEN    : 282=KADABRA Lv21 PSYBEAM -> 016=GUST (40 pow, cap 100)
```

`TYPE` means the slot was dealt a same-type move; `GEN` means it rolled the general pool. `cap 65535` is
the sentinel for "no power cap" — anything at level 31+. A `TYPE` line whose power exceeds its cap is a
bug, **except** for a mono-Fairy below level 16: Fairy's weakest damaging move here is Play Rough at 90,
so the cap is lifted rather than leaving the mon with nothing.

### Fastest way to see a lot of it

Give yourself a full party and use the Move Relearner on each — six species per look. Trainer battles are
the other high-volume source, since every opposing mon is generated fresh. A Pokémon caught and then
levelled is the case that exercises `MonTryLearningNewMove`, which is a different code path from the one
that builds a mon's starting moveset; both need a look.

---

## 4. Setting up a ROM to test

### Build

```bash
export DEVKITPRO=/opt/devkitpro && export DEVKITARM=$DEVKITPRO/devkitARM && export PATH=$DEVKITARM/bin:$PATH
```

```bash
make MODERN=1 -j$(sysctl -n hw.ncpu)
```

**`MODERN=1` is required** — agbcc isn't installed and the default path will fail. Output is
`pokeemerald_modern.gba` (32 MB), which will never match `rom.sha1`; that hash is for a byte-matching
vanilla build and is irrelevant here.

If graphics or map data look stale after a pull: `make clean` first.

### Emulator

```bash
brew install --cask mgba
```

**Open Tools → View Logs and enable the Debug channel before testing anything.** This is the single most
useful testing tool in the project. Every randomizer decision prints its inputs *and* outputs:

| Log line | Confirms |
|---|---|
| `TX RANDOM WILD POKEMON: species=… speciesResult=… EVO TYPE n-->>EVO TYPE m` | Phase 3 — original → randomized, and both evo stages |
| `TX CLAMP TO LEVEL : level=… result=…` | Phase 3 — the clamp firing |
| `%S: BST …(…) -->> …(…)` | Phase 3b — Improved balancing, with both BSTs |
| `TX RANDOM LEGENDARY: … -->> …` | Phase 4 |
| `TX VGC MOVE : move=… -> …` | Phase 5 |
| `TX STAB GUARANTEE : … Lv… slot=… -> … (p pow, n candidates <= c pow, type t)` | Phase 6 — note the **power cap** `c` and that `p` never exceeds it |
| `TX LEARNSET TYPE  : … Lv… orig -> new (p pow, cap c)` | Phase 8b — this slot was **dealt a same-type move** |
| `TX LEARNSET GEN   : … Lv… orig -> new (p pow, cap c)` | Phase 8b — this slot rolled the **general pool** |
| `TX VGC ABILITY : … num=… -> …` | Phase 7 |

Without the log, the statistical phases (5, 7, 8, 8c) are close to untestable by eye.

### Save discipline

**Start a fresh save.** The `SaveBlock1` layout changed, so any save from before these phases is invalid.
Symptoms of a stale save: garbage in the randomizer menu, options that won't stick, a corrupted party, or a
hang on load. Delete the `.sav` before debugging anything else.

### Save states are the main tool

Randomization is **seeded from your trainer ID**, so the same save always produces the same results.
That makes save states far more useful than usual:

- Park a state in front of a trigger — Route 102 grass, the Oldale cheat lady, Sky Pillar, a trainer's move
  menu — and reload it to repeat the exact same roll.
- **That reproducibility is itself a test.** If reloading the same state gives a *different* species,
  move or ability, something is using `Random()` where it should be seeded. That's a real bug class, and
  T3.13, T4.12, T5.11 and T7.4 all check for it.

---

## 5. A practical testing order

Roughly cheapest-first, and each step de-risks the next.

### Step 1 — Smoke test (10 minutes)

Do this before anything else. If it fails, nothing below is diagnosable.

1. ROM boots to the title screen
2. New Game reaches the randomizer options menu
3. All 11 new options appear, each beneath the option it modifies
4. Each greys out when its parent is off, with a sensible hint
5. Three-state options cycle Off → Weighted → Strict
6. Save, soft reset, reload — every setting survives
7. The settings **viewer** shows the same values as the menu *(most commonly-missed piece)*
8. A wild encounter produces `MgbaPrintf` output in the log

### Step 2 — Cheap in-game checks (30 minutes)

| Test | What to do |
|---|---|
| **Ultra Balls ₽1** | Oldale Mart. Check the list, quantity screen and confirmation all say 1 |
| **Sell price ₽0** | Try selling one. **If it sells for more than 0, stop** — that's an infinite money loop |
| **Cheap evolution shop** | Lilycove Dept. Store 3F: six stones + eight trade items, all ₽1 |
| **Self-trader ₽1** | Lilycove 1F. With ₽0 it should refuse; with ₽1 it should trade |
| **Opponent type box** | Any battle → Fight. Icons match the opponent; no overlap with the move description |

### Step 3 — The randomizer proper (a route or two)

Route 102 with the log open:

1. **Level-scaled wilds off** — confirm you *can* find over-evolved species. This proves the later test means something
2. **Turn it on** — every encounter is a plausible first-stage Pokémon
3. **Old Rod vs Super Rod on the same route** — Super Rod's higher levels should allow higher evo stages
4. **Reload a save state 5×** — same species every time
5. **Guarantee STAB** — every caught Pokémon has a damaging move of its own type
6. **Smart learnsets** — no Hyper Beam at level 5, no duplicate moves

### Step 4 — The ones that need patience

| Test | Why it matters |
|---|---|
| **T4.11 — plot legendaries** | Cave of Origin and Sky Pillar with a swapped Kyogre/Groudon/Rayquaza. A hard-coded species reference could strand the save. **Highest consequence in the project** |
| **T8c.10 — HM safety** | Every HM still teaches its original move. A randomized Surf or Strength soft-locks the run |
| **T8c.11 — TM agreement** | The bag description, the teach prompt and the move actually learned must match. 13 call sites |
| **T8b.14 — relearner agreement** | Move Relearner list == moves learned on level-up == summary's list |
| **T7.4 — ability stability** | Check a Pokémon's ability, switch out, switch back. It must not change |
| **T10.25 — Cap Candy interrupt** | Soft-reset mid-climb during a move prompt, then reload. **Phase 10 is the least-verified work here** |

---

## 6. What has and hasn't been verified

Being explicit, because it changes how much to trust each phase.

**Verified by offline simulation** — the algorithm was reproduced against the ROM's own data tables and
measured over tens of thousands of rolls:

| Phase | Evidence |
|---|---|
| 3 | Salamence@Lv4 → Bagon, @Lv30 → Shelgon; all species × 5 levels, no invalid results |
| 3b | Both BST tables sorted; no legendary leak; no empty band |
| 4 | Bijection across 9,363 trainer IDs; deterministic |
| 5 | 367/367 reachable; distribution within 0.2%; monotonic |
| 6 | Every type has candidates; no species with both types empty |
| 7 | 878 pairs; distribution within 0.2%; banned abilities never appear |
| 8 | 165/165 reachable; ratios 2.0/2.0/4.0/9.9; exclusions hold |
| 8b | All five call sites agree; zero level-cap violations; shape preserved 100% |
| 8c | Zero HM leaks, zero duplicates across 65 trainer IDs |

**Not verified at all** — build-clean and reasoned, but never executed:

- **Every UI interaction.** Menus, the battle type box, prompts, message flow
- **Phase 10's stepwise loop.** A multi-frame async chain with several exit points. Treat as unproven
- **Every cross-feature interaction** in actual play
- **Plot safety** with randomized legendaries

No phase has run in an emulator. Offline verification catches algorithmic errors — wrong distributions,
unreachable entries, non-termination — and it caught several real ones. It cannot catch a window drawn in
the wrong place, a prompt that never appears, or a soft-lock.
