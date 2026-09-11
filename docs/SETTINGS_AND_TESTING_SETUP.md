# Settings & Testing Setup

What's built, what each toggle does, and how to set up a ROM to test it.

Companions: [`RANDOMIZER_ENHANCEMENTS_PLAN.md`](RANDOMIZER_ENHANCEMENTS_PLAN.md) (design) and
[`RANDOMIZER_ENHANCEMENTS_TESTING.md`](RANDOMIZER_ENHANCEMENTS_TESTING.md) (per-phase checklists).

---

## 1. Inventory — what's done

**15 of 17 phases implemented**, all merged to `master` via PRs #1-#8.

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
| 11 | Nature / ability reroll cheat | ✅ | #7 |

### What's left

Both optional — nothing is blocked:

| Item | Size | Why it was deferred |
|---|---|---|
| **Phase 10 — PC storage use** | M | Box Pokémon are `struct BoxPokemon` with no level or current-HP field; a bad round-trip corrupts them. Worth its own change. |
| ~~Per-type STAB tiering~~ | — | **Done.** Reuses the Phase 5 tiers — no separate ranking was needed, since all 8 types with 10+ STAB moves already spread across 3-4 tiers. Added a physical/special split at the same time. |

### Ideas considered and not built

- **BST mode for trainers** — Phase 3b applies to wild Pokémon only.
- **Trainer moveset de-duplication** — 47% of explicit-moveset trainer entries currently share movesets
  (see Appendix B). One-line fix, but it changes every trainer, so it wants a deliberate decision.
- **Tertu's dynamic species tables** — ruled out; EWRAM is at 99.6%. See Appendix G.

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
| `TX STAB GUARANTEE : … slot=… -> … (n candidates, type t)` | Phase 6 |
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
