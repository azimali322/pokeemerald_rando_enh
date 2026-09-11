# Randomizer Enhancements — Test Plan

Companion to [`RANDOMIZER_ENHANCEMENTS_PLAN.md`](RANDOMIZER_ENHANCEMENTS_PLAN.md).

**Start here instead if you are setting up to test:**
[`SETTINGS_AND_TESTING_SETUP.md`](SETTINGS_AND_TESTING_SETUP.md) — what every toggle does and what it
requires, a recommended full-feature configuration, and a suggested testing order. This file is the
detailed per-phase checklist.

**Revision 5** — dead items are now **both** permanently bottom-tiered and hard-filtered under the challenge;
T8.15-T8.23 cover both layers. *Revision 4* added the dead-item tests and widened the TM tests to the
359-move candidate pool. *Revision 3* added Phase 8b (learnset overhaul) and Phase 8c (TM weighting), ₽1 Ultra Balls everywhere,
and the 6-tier ability weighting.

**How this doc works:** written ahead of implementation, filled in as we go. Each phase starts as
*Not implemented*; when it lands, status flips to *Ready to test* and the checkboxes go live. Record the
result and build hash so a regression can be bisected later.

---

## Standing setup

### Build

```bash
make -j$(sysctl -n hw.ncpu)
```

Falls back to `make MODERN=1` if agbcc won't cooperate on Apple Silicon (output `pokeemerald_modern.gba`).
Clean rebuild when things get weird — generated graphics/map data go stale:

```bash
make clean && make -j$(sysctl -n hw.ncpu)
```

### Emulator

mGBA, with **Tools → View Logs** open and the **Debug** channel enabled. This is the most important testing
tool in the project — the randomizer traces its inputs *and* outputs at every decision point:

| Log line | From | Tells you |
|---|---|---|
| `******** CreateWildMon ********` | `src/wild_encounter.c:462` | A wild encounter is being randomized |
| `TX RANDOM WILD POKEMON: species=… speciesResult=… EVO TYPE n-->>EVO TYPE m` | `src/pokemon.c:12262` | Original → randomized species, **and both evo stages** |
| `TX RANDOM MOVE : GetRandomMove: move=… final=…` | `src/pokemon.c:12326` | Original → randomized move |
| `TX RANDOM TYPEn: species=… type=…` | `src/pokemon.c:12222` | Type randomization |

Compiled out under `NDEBUG` — **build without it while testing.**

**Consider adding your own traces** for the new systems as you build them (which tier a roll landed in, which
move the STAB pass injected, which pre-evo the clamp walked to). Phases 3, 5, 6, 7, and 8 are all far easier
to verify from a log than from a summary screen, and statistical tests (T5.4, T7.13, T8.5) are impractical
without one.

### Save file discipline

**Phase 1 changes the `SaveBlock1` layout and invalidates every existing save.** Start fresh after Phase 1
and keep that save for all later phases.

Symptoms of a stale save: garbage in the randomizer menu, options that won't stick, corrupted party, hang on
load. If you see those after a struct change, delete the `.sav` before debugging anything else.

### Save states

Park a save state before each test trigger — the Oldale cheat lady, Route 102 grass, Sky Pillar, a trainer
battle's move menu. Randomization is *seeded*, so reloading reproduces the result exactly. That determinism is
itself testable, and several phases depend on it.

### Results log

| Phase | Date | Build | Result | Notes |
|---|---|---|---|---|
| 0 | | | | |

---

## Phase 0 — Build environment

**Status:** ✅ Build works — gameplay smoke tests outstanding

- [x] **T0.1** `arm-none-eabi-gcc --version` → devkitARM gcc 16.1.0 at `/opt/devkitpro/devkitARM`.
      *Was already installed, just not on `PATH`.*
- [x] **T0.2** `shasum baserom.gba` = `f3ae088181bf583e55daf962a92bb46f4f1d07b7` ✓
- [x] **T0.3** `make MODERN=1 -j$(sysctl -n hw.ncpu)` → exit 0. Output `pokeemerald_modern.gba`, 32 MB,
      header `POKEMON EMER / BPEE`. **`MODERN=1` is required** — agbcc is not installed.
- [x] **T0.3b** Memory at first link, record after every phase: **EWRAM 99.62%** (~988 B free),
      IWRAM 80.22%, ROM 68.88%. **EWRAM is the binding constraint** — see the plan's Phase 0.
- [ ] **T0.4** ROM boots in mGBA to the title screen.
- [ ] **T0.5** New Game reaches the randomizer options menu; toggling an existing option (e.g. *Random Wild
      Pokémon*) sticks after save + reload.
- [ ] **T0.6** mGBA log shows `MgbaPrintf` output during a wild encounter — confirms `NDEBUG` is off.
- [ ] **T0.7** Self-trader on Lilycove Dept. Store 1F works: trade a Kadabra/Machoke/Graveler/Haunter and it
      evolves. Baseline for Phase 2b — confirms trade evolution needs no new item.

**Gate:** T0.5 and T0.6 must pass before writing code. If the stock randomizer doesn't work, nothing built on
it can be diagnosed.

---

## Phase 1 — Save flags & menu plumbing

**Status:** ✅ Implemented (commit `78f6f11`) — **needs in-game verification**

Build passes, `SaveBlock1FreeSpace` assert holds. Measured: `sizeof(struct SaveBlock1)` = 15,788 / 16,336,
**548 bytes free**. EWRAM unchanged at 99.62%.

New options — randomizer page: *Level-Scaled Wilds*, *Random Legendaries*, *VGC Move Pool*,
*Guarantee STAB*, *Smart Learnsets*, *VGC Ability Pool*, *VGC Item Pool*, *VGC TM Pool*, *Random TM Moves*.
Features page: *Cheap Shop*, *Reroll Cheat*.

**Each new option is gated** — it greys out unless its parent is on (e.g. *VGC Move Pool* needs
*Moves*; *Level-Scaled Wilds* needs *Wild Pkmn*). Verify the grey-out messages in T1.10.

Eleven new options exist, display, persist, and default correctly — with no behaviour behind them yet.

- [x] **T1.1 — Build gate.** Compiles clean; no `SaveBlock1FreeSpace` failure. ✓
- [ ] **T1.2 — All eleven present.** *Level-Scaled Wilds*, *Random Legendaries*, *VGC Moves*, *Guarantee STAB*,
      *VGC Abilities*, *VGC Items*, *Random Learnsets*, *VGC TMs*, *Random TM Moves*, *Cheap Balls*,
      *Reroll Cheat* — each next to the option it modifies.
- [ ] **T1.3 — Paging intact.** Cursor moves through every page without skipping or duplicating; nothing
      orphaned past the last page (6 items/page).
- [ ] **T1.4 — Persistence.** Set each to a non-default value → save → soft reset → reload. All survive.
- [ ] **T1.5 — Viewer agreement.** The settings viewer (`tx_rac_viewer.c`) shows the **same** values as the
      menu for all eleven. *Most commonly-missed piece of the plumbing.*
- [ ] **T1.6 — Defaults.** Fresh New Game shows every new option Off, matching the `TX_*` defines.
- [ ] **T1.7 — Three-state options.** *VGC Moves*, *VGC Abilities*, *VGC Items*, and *VGC TMs* each cycle
      Off → Weighted → Strict → Off, with the label matching the stored value at each step.
- [ ] **T1.8 — No regression.** Every pre-existing option still displays and persists. Bitfield insertion can
      silently shift neighbours.
- [x] **T1.9 — Save size.** 15 bits used; **548 bytes (~4,384 bits) still free.** Save flags are not scarce. ✓
- [ ] **T1.10 — Gating.** Each new option greys out when its parent is off, with the right hint text:
      *VGC Move Pool* / *Guarantee STAB* / *Smart Learnsets* need *Moves*; *VGC Ability Pool* needs
      *Abilities*; *VGC Item Pool* and *VGC TM Pool* need *Items*; *Level-Scaled Wilds* needs *Wild Pkmn*.
- [ ] **T1.11 — Menu ordering.** Each new option appears directly beneath the option it modifies.

---

## Phase 2 — Cheap Ultra Balls (every mart)

**Status:** ✅ Implemented (PR #1) — **needs in-game verification**

`ItemId_GetPrice()` returns ₽1 for Ultra Ball when *Cheap Shop* is on. Ultra Balls added to the
0-badge shop inventory, so they are stocked from the first mart onward.

- [ ] **T2.1 — Off (control).** Ultra Balls not stocked at 0 badges; cost ₽1200 where they legitimately appear.
- [ ] **T2.2 — On, stocked.** New game → Oldale Mart lists Ultra Ball.
- [ ] **T2.3 — Price ₽1.** Verify in the shop list, the quantity-select screen, **and** the final
      confirmation — all three read `ItemId_GetPrice`, so all three must agree.
- [ ] **T2.4 — Bulk purchase.** Buy ×99. Money decreases by exactly 99; bag shows 99.
- [ ] **T2.5 — Unlimited.** Buy to bag cap. No overflow, no negative money, no wraparound.
- [ ] **T2.6 — Catch works.** Behaves as a normal Ultra Ball in battle.
- [ ] **T2.7 — Every mart.** Petalburg, Rustboro, Slateport marts also stock Ultra Balls at ₽1. This is the
      revised requirement — availability everywhere, not just Oldale.
- [ ] **T2.8 — Later badges.** After badges 1-3, Ultra Balls still stocked at ₽1 and other stock grew normally
      (`sShopInventories[badgeCount]` moves to a different row each badge — verify the option applies to every
      row that matters, not just index 0).
- [ ] **T2.9 — Sell price ₽0.** Expected (`price / 2`). Confirm no buy-low/sell-high loop.
- [ ] **T2.10 — No cross-contamination.** Great Ball and Poké Ball untouched — an over-broad `itemId` check is
      the likely bug.
- [ ] **T2.11 — Stacks with Expensive.** With `tx_Challenges_Expensive` on, Ultra Balls still ₽1 (early
      return), everything else inflated.

---

## Phase 2b — Evolution stones & trade-evolution items

**Status:** ✅ Implemented (commit `78f6f11`) — **needs in-game verification**

- Lilycove Dept. Store **3F left clerk** now stocks the 6 stones + the 8 trade-evolution held items
  alongside the vitamins.
- **Self-trader (1F) dropped from ₽10,000 → ₽1** — both the `checkmoney` gate and the `removemoney`
  deduction were changed.
- `ItemId_GetPrice()` returns **₽1** for Ultra Ball, the 6 stones, and the 8 trade items when
  *Cheap Shop* is on (`IsCheapCheatItem`, `src/item.c`).

Note: **8 distinct items** cover the 11 trade-item evolutions — King's Rock, Metal Coat and Up-Grade each
serve two lines.

- [ ] **T2b.1 — Control.** Before the change, no mart sells any evolution stone.
- [ ] **T2b.2 — Stones stocked.** Lilycove Dept. Store 3F sells all six: Sun, Moon, Fire, Thunder, Water, Leaf.
- [ ] **T2b.3 — Stones ₽1.** All six priced ₽1 in list, quantity, and confirmation screens.
- [ ] **T2b.4 — Stones work.** Buy a Fire Stone, use it on a valid species (e.g. Vulpix), it evolves.
- [ ] **T2b.5 — Trade-evo items stocked at Lilycove 3F (left clerk).** King's Rock, Metal Coat, Hard Stone,
      Dragon Scale, Up-Grade, Spell Tag, Deep Sea Tooth, Deep Sea Scale — all eight, all ₽1.
- [ ] **T2b.6 — Slateport (NOT YET DONE).** The 8 trade items are not yet on the Energy Guru's list. Decide
      whether you still want them there — Lilycove alone means no trade evolution before Lilycove.
- [ ] **T2b.7 — Full trade-item evolution.** Buy a Metal Coat, give it to a Scyther, use the Self-trader on
      Lilycove 1F → **Scizor**. This is the end-to-end test that the requirement is actually satisfied.
- [ ] **T2b.8 — Plain trade evolution.** Kadabra / Machoke / Graveler / Haunter evolve via the Self-trader
      with no held item.
- [ ] **T2b.9 — Held item consumed.** After a trade-item evolution the item is gone (`SetMonData(HELD_ITEM,
      ITEM_NONE)` at `src/pokemon.c:9463`).
- [ ] **T2b.10 — Sell price ₽0 (exploit check).** Stones normally sell for ₽1050 (half of ₽2100). At ₽1 buy
      price they must sell for ₽0. **If they sell for anything, that's an infinite money loop** — buy at ₽1,
      sell for more. Highest-priority check in this phase.
- [ ] **T2b.11 — Other marts unaffected.** Regular Poké Marts (which use the badge-indexed global list) don't
      suddenly stock stones.
- [ ] **T2b.12 — Self-trader ₽1.** Both `checkmoney` and `removemoney` changed. With ₽0 the machine should
      still refuse; with ₽1 it should trade and deduct exactly ₽1.
- [ ] **T2b.13 — King's Rock balance note.** ₽1 King's Rock makes flinch-stacking trivially available.
      Expected, but confirm it's the intent.

---

## Phase 3 — Level-appropriate wild randomization

**Status:** ✅ Implemented — **needs in-game verification**

`ClampSpeciesToLevel()` + `GetSpeciesMinLevel()` in `src/pokemon.c`, hooked into `CreateWildMon`
(`src/wild_encounter.c`). Zero EWRAM cost — all logic, no tables.

**Verified offline** by simulating the algorithm against the real `src/data/pokemon/evolution.h`:

| Species | Encounter Lv | Min Lv | Result |
|---|---:|---:|---|
| Salamence | 4 | 50 | **Bagon** ← your example |
| Salamence | 30 | 50 | **Shelgon** ← Super Rod case |
| Salamence | 55 | 50 | Salamence (unchanged) |
| Dragonite | 4 | 55 | Dratini |
| Ninetales (stone) | 5 | 28 | Vulpix |
| Machamp (trade) | 5 | 32 | Machop |
| Crobat (friendship) | 5 | 22 | Zubat |
| Milotic (beauty) | 5 | 30 | Feebas |
| Beautifly (branching) | 3 | 10 | Wurmple |
| Tauros / Lapras (single-stage) | 5 | 1 | unchanged |
| Mewtwo / Kyogre (legendary) | 4 | 1 | unchanged |
| Pichu (baby) | 5 | 1 | unchanged |

Swept all species × 5 levels: no invalid results, no non-terminating walks. **T3.15, T3.16, T3.17, T3.18,
and T3.19 are covered by this** — the in-game checks below are for integration, not algorithm correctness.

The headline feature. Test against the mGBA log — it prints both evo stages (`EVO TYPE 2-->>EVO TYPE 0`),
making the clamp directly visible.

### Core behaviour

- [ ] **T3.1 — Off (control).** *Random Wild* On, *Level-Scaled* Off, *Similar* Off. Route 102 grass should
      still produce fully-evolved / pseudo-legendary species at Lv3-4 — **the bug the feature fixes must still
      reproduce.** Confirming this first is what makes the later tests meaningful.
- [ ] **T3.2 — On, early route.** Same save, *Level-Scaled* On. Route 102 grass: every encounter is a
      first-stage / low-level-viable species. No Salamence, Dragonite, Tyranitar, Metagross.
- [ ] **T3.3 — Log confirmation.** Find a `speciesResult` in the log that is a stage-2 mon, and confirm the
      mon actually sent out is its base form. Direct proof the clamp fired.
- [ ] **T3.4 — Mid-game route.** ~Lv25-30: stage-1 appears; stage-2 that evolves above the encounter level
      still doesn't.
- [ ] **T3.5 — Late-game.** Victory Road / Lv45+: fully-evolved mons appear normally. The clamp must not
      over-suppress.

### Per-encounter-method (your explicit requirement)

- [ ] **T3.6 — Old Rod, Route 102.** Low-level table → base forms only.
- [ ] **T3.7 — Good Rod, Route 102.** Higher levels → correspondingly higher stages allowed.
- [ ] **T3.8 — Super Rod, Route 102.** Lv~30 → stage-1, and stage-2 where the evo level permits.
      **T3.6 → T3.8 on one map is the exact scenario you specified.** Compare back to back.
- [ ] **T3.9 — Surfing.** Water tables have their own levels; clamping tracks them.
- [ ] **T3.10 — Feebas.** The dedicated path (`src/wild_encounter.c:925`) clamps.
- [ ] **T3.11 — Mass outbreak.** Outbreak encounters (`:555`) clamp.
- [ ] **T3.12 — Rock Smash / headbutt-likes**, if present in this build.

### Correctness edges

- [ ] **T3.13 — Determinism preserved.** Save state → encounter → note species → reload → encounter →
      **same species**. Repeat 5×. Clamping must not introduce per-encounter randomness.
- [ ] **T3.14 — Map-based consistency.** With `tx_Random_MapBased` On, the same route keeps the same mapping
      across save/reload.
- [ ] **T3.15 — Single-stage species.** Tauros, Lapras, etc. at low level pass through unchanged, not mangled
      to `SPECIES_NONE`. **A `SPECIES_NONE` leak crashes the battle** — hunt for it deliberately.
- [ ] **T3.16 — Baby forms.** Pichu/Cleffa/Igglybuff don't walk below themselves.
- [ ] **T3.17 — Branching evolutions.** Wurmple→Silcoon/Cascoon, Poliwag line. No infinite loop; terminates
      within the 3-iteration guard.
- [ ] **T3.18 — Non-level evolutions.** Stone (Vulpix→Ninetales), trade (Machoke→Machamp), friendship
      (Golbat→Crobat), Beauty (Feebas→Milotic). Each uses its estimate; a stone evolution should not appear
      at Lv5.
- [ ] **T3.19 — Legendaries unaffected.** *Include Legendaries* On + *Level-Scaled* On: a legendary can still
      appear at low level. Your stated intent — legendaries have no pre-evo, so the walk no-ops.
- [ ] **T3.20 — Stacks with Similar.** *Similar Evolution Level* On + *Level-Scaled* On: no conflict, no
      double-demotion below base form.
- [ ] **T3.21 — Catch & evolve.** Catch a clamped mon, level it past its evolution threshold. It evolves
      normally into the line it was clamped from.
- [ ] **T3.22 — Pokédex.** Caught clamped mon registers the correct species.
- [ ] **T3.23 — Performance.** Time the delay between stepping into grass and the battle transition.
      `GetPreEvolution` is a ~3,700-iteration scan called up to ~10× per encounter. **If encounters feel
      sluggish, that's the cause** — note it and consider the reverse-lookup-table optimization.

---

## Phase 3b — BST similarity mode ("Improved" balancing)

**Status:** ✅ Implemented — **needs in-game verification**

The existing **BALANCING** option is now three-state: **Off / Balanced / Improved**.
*Balanced* is the old evolution-stage behaviour; *Improved* matches by base stat total (±10.24%).

`tx_Random_Similar` widened 1→2 bits. **The menu's old inversion was removed** — it used to draw index 0 as
"On" and store `!selection`. Now 0=Off, 1=Balanced, 2=Improved with no negation.

Tables generated by `tools/randomizer/gen_bst_table.py` into `src/data/pokemon/species_by_bst.h`:
**400 entries** (non-legendary, BST 180-670) and **425** (with legendaries, 180-720). ~2.9 KB ROM, zero EWRAM.

**Verified offline** against the generated tables:

| Check | Result |
|---|---|
| Both tables sorted ascending (binary search depends on it) | ✅ |
| Legendaries in the non-legendary table | **0** ✅ |
| Legendaries in the legendary table | 25 ✅ |
| Any band with 0 candidates | **none** ✅ |
| Narrowest band | Azurill 13, Dragonite/Metagross/Salamence 15 |
| Widest band | Victreebel/Vileplume/Weezing 171 |
| Band widening fires where expected | Sunkern 6→17 (×2), Slaking →32 (×2) |

Sample bands: Dratini/Bagon (BST 300) → 84 candidates; Tauros (510) → 147; Salamence (600) → 15.

**T3b.3, T3b.6, T3b.8, T3b.9 and T3b.10 are covered by this.** The in-game checks below are for
integration.

- [ ] **T3b.1 — Control.** *Similar* = Evolution stage: Sunkern can still swap into Lapras-tier species
      (the 180→535 spread). Confirm the problem reproduces before testing the fix.
- [ ] **T3b.2 — BST mode on.** *Similar* = Base stats: sample 20 wild encounters on an early route. Every
      result is within roughly ±10% BST of what the slot originally held.
- [ ] **T3b.3 — Sunkern band.** A BST-180 species yields only very weak species (Azurill/Caterpie/Weedle/Ralts
      tier) — 6 candidates in band. Expect visible repetition; confirm it's tolerable or widen the band.
- [ ] **T3b.4 — Mid band.** A BST-300 species (Dratini, Bagon, Larvitar) draws from ~84 candidates — good variety.
- [ ] **T3b.5 — High band.** A BST-510 species (Tauros) draws from ~147 candidates.
- [ ] **T3b.6 — Legendaries.** With *Include Legendaries* **off**, a high-BST band must not sneak legendaries in.
      With it on, they may appear. **Most likely place for a leak** — the band around Mewtwo (680) is nearly
      all legendary.
- [ ] **T3b.7 — Menu inversion removed.** The old code drew index 0 as "On" and stored `!selection`.
      Verify the three states now read correctly: selecting **Off** stores 0, **Balanced** stores 1,
      **Improved** stores 2 — and that the viewer agrees. **A leftover negation here would silently swap
      Off and Balanced**, which is the most likely regression in this phase.
- [ ] **T3b.14 — Starters.** `PickRandomStarter` treats *Similar* as truthy, so *Improved* falls back to the
      stage-0 starter pool. That is intended — starters should stay first-stage. Confirm starters still work.
- [ ] **T3b.8 — Table ordering.** Add a debug check that `sSpeciesByBST` is sorted ascending. A mis-sorted
      table makes binary search return wrong results **silently** rather than crashing.
- [ ] **T3b.9 — Integer band maths.** Verify the computed bounds for a few known BSTs (180, 300, 510) match
      the ±10.24% intent. Off-by-one in integer maths gives a silently wrong pool.
- [ ] **T3b.10 — Missing species.** A species absent from the generated table (forms, placeholders) falls back
      to the flat random path — no read past the end of the array.
- [ ] **T3b.11 — Stacks with Phase 3.** BST mode + *Level-Scaled Wilds* together: BST picks power, the clamp
      picks stage. On Route 102 the clamp should rarely need to fire, because BST already picked a weak mon.
- [ ] **T3b.12 — Determinism.** Same route, same species across save/reload.
- [ ] **T3b.13 — Performance.** Encounter latency unchanged vs Phase 3 alone — binary search should be
      unmeasurable next to the existing `GetPreEvolution` scans.

---

## Phase 4 — Random legendary encounters

**Status:** ✅ Implemented — **needs in-game verification**

`GetRandomLegendary()` in `src/pokemon.c`, hooked into `GetSpeciesRandomSeeded` **ahead of** the existing
early-out (which would otherwise pass legendaries through untouched whenever *Include Legendaries* is off).
Because the hook sits at the shared chokepoint and keys off `EVO_TYPE_LEGENDARY`, it covers battle
legendaries, gift legendaries, wild legendaries and trainer-owned ones in one place.

**Roamers needed a second hook** — `CreateInitialRoamerMon` (`src/roamer.c`) assigns
`ROAMER->species` directly and never reaches the chokepoint. Added there too.

**Uniqueness: guaranteed.** The 25-entry pool is shuffled once with a trainer-ID-derived seed and each
legendary takes the entry at its own index — a bijection, so no two legendaries collide.

**Verified offline** by reproducing the RNG maths exactly:

| Check | Result |
|---|---|
| Valid bijection (no duplicates, nothing lost) | ✅ **all 9,363 sampled trainer IDs** |
| Deterministic (same save → same mapping) | ✅ |
| Distinct permutations across saves | 9,363 / 9,363 |
| Saves with ≥8 of 25 legendaries mapping to themselves | 0.55% |

*Implementation note:* the shared `ShuffleListU16` feeds the **same** seed to every iteration, so one 16-bit
value decides the whole permutation — that left ~1% of saves mapping 8+ legendaries onto themselves, and one
save in 65,536 mapping 16. `GetRandomLegendary` runs its own Fisher-Yates with a per-iteration seed instead,
halving that tail, without touching shared code that starter selection also uses.

**T4.4 (uniqueness) and T4.12 (determinism) are covered by this.** A legendary occasionally mapping to
itself is expected — that's normal for a permutation, not a bug.

- [ ] **T4.1 — Off (control).** Legendaries are their canonical species.
- [ ] **T4.2 — On, battle legendary.** Sky Pillar Rayquaza → a **different legendary**, never a regular mon.
- [ ] **T4.3 — Multiple statics.** Check at least four across a playthrough (Regirock, Regice, Registeel,
      Kyogre/Groudon, Rayquaza, Latias/Latios). All become legendaries.
- [ ] **T4.4 — Uniqueness.** Record every legendary obtained. With the shuffled-permutation implementation,
      **no duplicates**. If the modulo fallback was used instead, duplicates are expected — confirm which was
      built and that it matches the plan.
- [ ] **T4.5 — Gift legendaries.** Any legendary given via `ScriptGiveMon` also randomizes.
- [ ] **T4.6 — Wild legendaries.** *Include Legendaries* On: a wild legendary encounter also yields a random
      legendary, not its canonical species.
- [ ] **T4.7 — Trainer legendaries.** *Random Trainer* On: a trainer-owned legendary randomizes too.
- [ ] **T4.8 — Roamers.** Latias/Latios roaming encounter. Hook added at `CreateInitialRoamerMon`; verify the
      roamer is a random legendary and that it still roams, flees and can be caught normally.
- [ ] **T4.9 — Non-legendary statics unaffected.** Beldum, Castform, Wynaut, the fossils, gift Pokémon still
      follow plain *Random Static* rules.
- [ ] **T4.10 — Level preserved.** A randomized Rayquaza is at Rayquaza's scripted level (70), not the
      replacement's canonical level.
- [ ] **T4.11 — Plot integrity.** **Highest-consequence test in the plan.** Cave of Origin and Sky Pillar
      sequences complete without softlock when the species changed. A hard-coded species reference in a
      cutscene would strand the save — test before committing to a long playthrough.
- [ ] **T4.12 — Determinism.** Save state before the encounter, reload, re-trigger. Same replacement.

---

## Phase 5 — VGC-weighted move pool

**Status:** ✅ Implemented (PR #5) — **needs in-game verification**

All 367 randomizable moves tiered from `community-moves-tierlist.png`, weighted **4/24/38/27/6/1**.
Nuzlocke overrides: Self-Destruct, Explosion, Memento, Perish Song and Curse forced to tier 6; every
recoil move dropped one tier.

**Verified offline** over 146,832 rolls: all 367 reachable, distribution within 0.2% of target, per-move
odds strictly decreasing, nuzlocke-forced moves at ~0.23× uniform.

This PR also fixed a seed-correlation bug that had made **66 of 367 moves unreachable** — see the plan's
Appendix B. **T5.12 (all moves legal) is covered.**

Statistical — test with volume. Read 50 lines of `TX RANDOM MOVE` out of the mGBA log rather than checking 50
summary screens by hand.

- [ ] **T5.1 — Off (control).** *Random Moves* On, *VGC* Off: movesets look like today's, plenty of junk.
- [ ] **T5.2 — Weighted.** Sample ~50 moves. Noticeably more tier-1 staples, **but still some junk** — the 20%
      tail must be present. Zero junk means the tier roll isn't working.
- [ ] **T5.3 — Strict.** Sample ~50. **Only** tier-1 moves.
- [ ] **T5.4 — Rough distribution.** Over ~100 samples, roughly 45/35/20. Not precise — just checking no tier
      is starved or dominant.
- [ ] **T5.5 — No banding.** Consecutive moves on one mon aren't all from one tier. Banding means the tier
      roll and the within-tier roll share a seed — the plan's explicit warning.
- [ ] **T5.6 — Player mons.** Your own Pokémon's learnset moves are weighted.
- [ ] **T5.7 — Wild mons.** Caught wild mons have weighted moves.
- [ ] **T5.8 — Trainers.** *Random Trainer* **On** → opponent movesets weighted.
- [ ] **T5.9 — Trainers, control.** *Random Trainer* **Off** → trainer movesets are **unchanged vanilla**.
      Your stated condition.
- [ ] **T5.10 — Level-up learns.** Moves learned on level-up mid-game are also weighted.
- [ ] **T5.11 — Determinism.** Same mon, same species → same moves across save/reload.
- [ ] **T5.12 — All moves legal.** No `MOVE_NONE`, no out-of-range index. **A bad index reads garbage and can
      crash the battle** — needs a long session, not a spot check.
- [ ] **T5.13 — HM progression.** Verify `HMsOverwriteOptionActive()` accounts for the VGC option. Without it,
      Surf/Strength/Rock Smash can become unobtainable and **soft-lock the run.** Test by reaching a Surf gate.
- [ ] **T5.14 — PP sane.** Correct PP; nothing shows 0/0.
- [ ] **T5.15 — Battle-safe.** Several full battles, no crash from an unusual move effect on an unexpected species.

---

## Phase 6 — Guaranteed same-type move

**Status:** ✅ Implemented — **needs in-game verification**

`EnsureStabMove()` in `src/pokemon.c`, called from **six** sites: wild (both the Cute Charm and normal
paths), static/gift (`ScriptGiveMon`), scripted wild (`CreateScriptedWildMon`), and both trainer party
builders. Gated on *Guarantee STAB* **and** *Random Moves*.

**Level cap added after the fact.** The first implementation ignored the mon's level entirely, so a Lv5
special Fire-type had an **89.7%** chance of an injected move over 60 power (Eruption at 27.9%). That
contradicted Phase 8b, which caps levels 1–15 at 60 power and 16–35 at 100. `EnsureStabMove` now reads
`MON_DATA_LEVEL` and shares Phase 8b's bands via `GetStabPowerCapForLevel()`. Swept offline across
5,700 type × category × level combinations: **0 cap violations**, one legitimate relaxation (Fairy, see
T6.17).

**The trainer-species trap was avoided by design.** The plan warned that `src/battle_main.c` seeds moves on
`partyData[i].species` (pre-randomization). It's worse than that — the local `species` is only assigned
*inside* the Random-Trainer branch, so it's stale otherwise. `EnsureStabMove` therefore reads the species
back off the created mon with `GetMonData`, which is correct in every branch.

**Verified offline** against the real move and species data:

| Check | Result |
|---|---|
| Types with zero damaging non-HM moves | **none** ✅ |
| Species where *both* types have no candidates | **0** ✅ |
| HM moves excluded from the candidate pool | ✅ by construction |

Candidates per type (power > 1, non-HM): Normal 52, Fighting 18, Fire/Grass 12, Water 11, Flying/Psychic/
Ground 10, Ice/Electric/Poison/Bug/Dark 8, Rock 7, Ghost/Steel 6, Dragon 5, **Fairy 2**.

**Narrow-type handling.** Fairy has only 2 candidates, so a pure-Fairy mon would get the same move nearly
every time. Pools smaller than 5 therefore mix in Normal moves: **33% own-type, 67% Normal**. Own-type still
wins a third of the time — far above the 2/54 (≈4%) it would get from simply merging the two lists.

**Dual-types draw from the union of both types and are unaffected.** A Fairy/Dragon mon picks from all 7
Fairy+Dragon moves; only mono-types of a narrow type fall back. Verified:

| | |
|---|---|
| Species triggering the Normal blend | **7** — Clefairy, Clefable, Cleffa, Togepi, Snubbull, Granbull, Sylveon (all pure Fairy) |
| Dual-types containing Fairy that **don't** trigger | 17 — Jigglypuff (Normal/Fairy → 54), Mr. Mime (Psychic/Fairy → 12), Togetic (Fairy/Flying → 12), Marill (Water/Fairy → 13) |
| Dragon (5), Ghost (6), Steel (6) | never trigger — above the threshold |

**T6.12 (no candidates) is covered by this** — the guard exists but cannot fire with current data.

Slot choice: first empty slot → else the weakest damaging non-HM move → else slot 3. Status moves are
preserved where possible, and the function refuses outright rather than overwrite an HM.

- [ ] **T6.1 — Off (control).** Option Off: randomized mons frequently have zero same-type damaging moves.
- [ ] **T6.2 — On, wild mons.** Sample 20 wild encounters. **Every one** has ≥1 damaging move matching one of
      its types.
- [ ] **T6.3 — On, captured.** The caught version of that mon retains the STAB move.
- [ ] **T6.4 — Dual types.** A dual-type mon needs only **one** of its two types matched, and draws from the
      **union** of both types' moves. Check a Fairy/Flying mon (Togetic): its STAB should sometimes be Fairy
      and sometimes Flying, from the combined 12-move pool.
- [ ] **T6.4b — Narrow mono-type blend.** Sample ~15 pure-Fairy encounters (Clefairy, Togepi, Snubbull,
      Sylveon). Roughly **2 in 3** should get a Fairy move; the rest a Normal move. Both outcomes are correct.
- [ ] **T6.4c — Blend does NOT fire elsewhere.** A pure-Dragon, pure-Ghost or pure-Steel mon should still get
      its own type every time — those pools are above the threshold.
- [ ] **T6.5 — Trainers.** *Random Trainer* + *Random Moves* + *Guarantee STAB* On: every opponent mon has a
      matching-type move.
- [ ] **T6.6 — Trainers use the randomized species.** Handled by reading species off the created mon rather
      than from `partyData`. Verify in-game that an opponent's STAB matches the mon you actually see, in both
      *Random Trainer* on and off configurations.
- [ ] **T6.7 — Static / gift mons.** Also get a STAB move.
- [ ] **T6.8 — PP set.** The injected move has correct PP, not 0/0. Classic omission.
- [ ] **T6.9 — Doesn't delete the best move.** Injection prefers an empty slot, then the *weakest* damaging
      move. Confirm a mon's strongest attack survives. Also confirm status moves are usually preserved.
- [ ] **T6.10 — HM moves preserved.** The STAB pass never overwrites an HM move. **Can soft-lock progression**
      the same way Phase 5 can.
- [ ] **T6.11 — Already-has-STAB case.** A mon that already rolled a matching move is left completely alone —
      no wasted slot, no duplicate.
- [ ] **T6.12 — No candidates.** A type/species combination where the scan finds nothing leaves the moveset
      untouched rather than writing `MOVE_NONE`. Deliberately hunt for this.
- [ ] **T6.13 — Interaction with Strict.** *VGC Moves* = Strict + *Guarantee STAB* On: the STAB pass may inject
      a non-tier-1 move to satisfy the constraint. **Correct behaviour** — the constraint wins. Confirm it
      doesn't read as a Phase 5 bug.
- [ ] **T6.14 — Type modes.** With `tx_Mode_Modern_Types` / `tx_Mode_Fairy_Types` on, STAB is computed against
      the *variant* typing — confirms `GetTypeBySpecies` was used, not `gSpeciesInfo[].types`.
- [ ] **T6.15 — Type randomizer.** With `tx_Random_Type` On, STAB matches the *randomized* type.
- [ ] **T6.16 — Determinism.** Same mon → same injected move across save/reload.
- [ ] **T6.17 — Level-appropriate power.** A Lv2–15 catch never receives a >60-power injected move, and a
      Lv16–35 one never receives >100. Watch for Eruption / Blast Burn / Hydro Cannon on early catches —
      those were the worst offenders before the cap existed. The **one** legitimate exception is a
      mono-**Fairy** mon below Lv16 (`tx_Mode_Fairy_Types` on): Fairy's weakest damaging move in this ROM is
      Play Rough at 90, so the cap is lifted rather than leaving the mon without STAB.
- [ ] **T6.18 — Cap relaxes before failing, not before category.** A mon whose type has no move of its
      preferred category under the cap gets a level-appropriate move of the *other* category, not an
      over-powered one of the right category. Order is: category first, power cap last.
- [ ] **T6.19 — Existing moves are untouched.** The guarantee is one *added* move, not a whole moveset. A
      low-level mon's other one or two moves may still be off-type or the wrong category.
      **Decided, not a gap:** a special attacker that rolls three physical moves is a bad randomizer roll and
      stays one. The STAB pass guarantees coverage, it does not launder the rest of the moveset. Do not
      "fix" this. Confirm the two features coexist (T8b.12).

---

## Phase 7 — VGC-weighted abilities

**Status:** ✅ Implemented — **needs in-game verification**

`GetVGCAbility()` in `src/pokemon.c`, returning **early** from `GetAbilityBySpecies` so the legendary and
Modern-Types special cases below it only run on the species-substitution path they were written for.
Tiers generated into `src/data/pokemon/ability_tiers.h` from Appendix A: **16 / 22 / 26 / 9 / 6 = 79**,
with Wonder Guard and Cacophony in no table.

**Verified offline** by reproducing the RNG maths over 878 (species, abilityNum) pairs:

| Tier | Target | Actual | n | Per-ability | × uniform |
|---|---:|---:|---:|---:|---:|
| S+A | 40% | 40.1% | 16 | 2.506% | 1.98× |
| B | 30% | 30.1% | 22 | 1.367% | 1.08× |
| C | 22% | 21.9% | 26 | 0.841% | 0.66× |
| D | 6% | 6.2% | 9 | 0.683% | 0.54× |
| F | 2% | 1.8% | 6 | 0.304% | 0.24× |

| Check | Result |
|---|---|
| Per-ability odds strictly decreasing S+A > B > C > D > F | ✅ **the inversion is fixed** |
| Wonder Guard or Cacophony ever produced | **never** ✅ |
| Deterministic for the same (species, abilityNum) | ✅ |
| Distinct abilities seen | 78 of 79 |

**T7.6, T7.13, T7.14 and T7.15 are covered by this.** T7.4 (no flicker in battle) still needs the emulator —
the seed is species-only by construction, but only play confirms nothing else re-rolls it.

- [ ] **T7.1 — Off (control).** *Random Abilities* On, *VGC* Off: today's species-substitution behaviour.
- [ ] **T7.2 — Weighted.** Sample ~30 mons. More Intimidate / Levitate / Speed Boost / Thick Fat than chance
      would give, with a visible tail.
- [ ] **T7.3 — Strict.** Only tier-1 abilities. Expect duplicates across a 6-mon party — tier 1 is only 24 entries.
- [ ] **T7.4 — Determinism (critical).** Open a mon's summary, close, reopen — **same ability**. Then in
      battle: check ability, switch out, switch back — **same ability**. A flicker means
      `GetAbilityBySpecies` isn't seeded purely on `species + abilityNum`. **The single most important test
      in the phase** — that function is called constantly, including per-frame in battle.
- [ ] **T7.5 — Battle matches summary.** An Intimidate mon actually drops the opponent's Attack on entry.
- [ ] **T7.6 — Exclusions enforced.** Across a long sample, **Wonder Guard and Cacophony never appear.**
      Note Multitype and Forecast are now *tiered* (D), not excluded — they should appear rarely. If either
      misbehaves off-species in testing, re-exclude it.
- [ ] **T7.7 — No `ABILITY_NONE`.** No mon shows a blank ability. Check single-ability species specifically.
- [ ] **T7.8 — Trainers.** Opponent abilities weighted and triggering correctly.
- [ ] **T7.9 — Hard-coded species.** Slaking (Truant), Castform (Forecast), and the
      `tx_Mode_Legendary_Abilities` block (`src/pokemon.c:8267`) behave sanely — cleanly overridden or cleanly
      preserved, not half-applied.
- [ ] **T7.10 — Weather abilities.** Drizzle/Drought/Sand Stream set weather correctly; the battle doesn't stall.
- [ ] **T7.11 — Ability-dependent mechanics.** Synchronize (nature, `src/wild_encounter.c:430`), Cute Charm
      (gender, `:475`), Magnet Pull / Static (encounter influence, `:505`) still work — these read
      `GetMonAbility` on your lead and will now see randomized abilities.
- [ ] **T7.12 — Sturdy tier check.** If `tx_Mode_Sturdy` gives modern "survive at 1 HP" behaviour, Sturdy
      should be promoted to tier 1. Verify which behaviour this build has and re-tier if needed.
- [ ] **T7.13 — Rough distribution.** ~40/30/22/6/2 across S+A / B / C / D / F over a decent sample.
- [ ] **T7.14 — Per-ability ordering (the inversion check).** Sample ~200 abilities and bucket by tier, then
      **divide each tier's observed count by its member count**. The per-ability rate must *decrease*
      S+A → B → C → D → F. If D out-appears C, the recommended 22/6 split wasn't applied — that's the exact
      bug Appendix A identified in the original 20/8 weights.
- [ ] **T7.15 — Tier tables complete.** 16 + 22 + 26 + 9 + 6 = 79 entries across the five tables, with
      Wonder Guard and Cacophony in none of them.
- [ ] **T7.16 — Truant tolerance.** If a Truant roll lands on one of your mons, decide whether the tail is
      tolerable or Truant should move to the exclusion list.

---

## Phase 8 — Weighted item pool

**Status:** ✅ Implemented (PR #8) — **needs in-game verification**

188-item pool tiered for nuzlocke play, weighted **18.59 / 18.59 / 46.47 / 14.52 / 1.83** across tiers of
4 / 8 / 40 / 50 / 63. Per-item ratios 2× / 2× / 4× / 10×.

**23 items never roll**: 12 mail + 11 battle-only items.

**Verified offline** over 125,020 rolls: all 165 tiered items reachable, per-item odds strictly decreasing,
adjacent ratios 2.0/2.0/4.0/9.9, no excluded item ever produced. **T8.16-T8.23 are covered.**

- [ ] **T8.1 — Off (control).** *Random Items* On, *VGC* Off: uniform rolls, plenty of mail and shards.
- [ ] **T8.2 — Weighted, ground items.** Sample ~30 item balls. Noticeably more Leftovers / Choice Band /
      Sitrus / Lum, with a tail.
- [ ] **T8.3 — Strict.** Only tier-1 items.
- [ ] **T8.4 — Hidden items unaffected (your requirement).** Sample ~15 hidden items with the option On.
      Distribution matches the **uniform** roll, not the weighted one — mail and shards should still turn up
      at normal frequency. Confirms `RandomItemHidden()` passes `FALSE`.
- [ ] **T8.5 — Battle items suppressed.** X Attack / X Defend / X Speed / X Accuracy / X Special / Dire Hit /
      Guard Spec are rare (tail only). Your stated goal.
- [ ] **T8.6 — Healing items still common.** Potions, Revives, and status healers appear regularly in tier-2
      rolls. **If early-game becomes unplayable for lack of healing, tier 2 needs rebalancing** — this is a
      real failure mode, not a theoretical one.
- [ ] **T8.7 — Species-locked items are rare.** Soul Dew, Light Ball, Thick Club, Metal Powder, Lucky Punch,
      Stick, Deep Sea Tooth/Scale appear only in the tail. Getting these often would mean the tier list was
      built from a raw VGC ranking rather than the randomizer-adjusted one.
- [ ] **T8.8 — Held items on scripted mons.** `CreateScriptedWildMon` (`src/script_pokemon_util.c:194`) gives
      weighted held items.
- [ ] **T8.9 — TMs unaffected.** The TM/HM branch (`src/item.c:1220-1240`) still works; TMs are still randomized
      among TMs, and **HMs are still never randomized**.
- [ ] **T8.10 — Key items protected.** No key item is ever replaced. The `POCKET_KEY_ITEMS` guard at `:1241`
      must still fire on the new path.
- [ ] **T8.11 — Progression safe.** No required progression item is replaced by a weighted roll. Play far
      enough to confirm.
- [ ] **T8.12 — Specials still work.** The `RandomItem` / `RandomItemHidden` specials (`data/specials.inc:150-151`)
      still resolve after the signature change — a broken special shows up as an item ball giving nothing.
- [ ] **T8.13 — Determinism.** Same item ball → same item across save/reload.
### Dead-item filtering (no-battle-items challenge)

- [ ] **T8.15 — Junk tier permanently.** Even with the challenge **off**, the 7 X-items, 2 escape items, and
      Yellow/Red Flute appear only at bottom-tier frequency.
- [ ] **T8.16 — Filter on.** With `tx_Challenges_NoItemPlayer` **on**, sample ~40 item balls: **none** of the
      11 dead items appear at all.
- [ ] **T8.17 — Berries kept (your explicit requirement).** With the challenge on, hold-effect berries still
      appear: Cheri, Chesto, Pecha, Rawst, Aspear, Leppa, Oran, Persim, **Lum**, **Sitrus**, Berry Juice.
      These are both consumable *and* good held, so the filter must not catch them.
- [ ] **T8.18 — Pinch berries kept.** Liechi, Salac, Petaya, Ganlon, Apicot, Lansat, Starf still appear.
- [ ] **T8.19 — Poké Balls kept.** All ball types still drop — the challenge exempts them
      (`src/item_menu.c:2139`), so the filter must too. **If balls stop dropping, the run is unwinnable.**
- [ ] **T8.20 — Medicine kept.** Potions, Revives, Ethers, Full Heals still appear. They remain field-usable
      under the challenge (it only blocks the *battle* menu), so they are **not** dead items.
- [ ] **T8.21 — Blue Flute kept, Yellow/Red dropped.** Blue Flute has a field use; the other two don't. A good
      check that the predicate reads `fieldUseFunc` and isn't just pattern-matching on the name.
- [ ] **T8.22 — Rejection loop terminates.** The re-roll pass is bounded (~8 attempts). Confirm no hang and no
      `ITEM_NONE` leak when a roll repeatedly lands on dead items.
- [ ] **T8.23 — Filter off when challenge off.** Turning `tx_Challenges_NoItemPlayer` off restores the 11
      items at junk-tier frequency.

- [ ] **T8.14 — Master Ball.** It's in the tail, so it's a rare real drop. Confirm that's desirable; move to
      exclusions if not.

---

## Phase 8b — Learnset randomization overhaul

**Status:** ✅ Implemented — **needs in-game verification**

`GetRandomLearnsetMove()` in `src/pokemon.c`, replacing the raw `GetRandomMove` call at **all five**
functions that map learnset entries: `GiveBoxMonInitialMoveset`, `MonTryLearningNewMove`,
`GetMoveRelearnerMoves`, `GetLevelUpMovesBySpecies` (each with two branches for the Modern Moves modes).

**Built without the move tiers**, which are still outstanding — level scaling uses raw power thresholds
instead of tier tables, so nothing here blocks on `tiering/MOVES.md`.

What it does, given the slot it is filling:

| Constraint | Rule |
|---|---|
| Level-appropriate power | ≤ 60 at levels 1-15, ≤ 100 at 16-30, unbounded at 31+ |
| Shape preserved | a damaging slot stays damaging, a status slot stays status |
| First move | always a same-type **attack** — previously the starter only |
| One late move | one entry above level 30 is also a same-type attack, drawn with **no power cap** |
| Type bias | every *other* damaging slot leans same-type `LEARNSET_STAB_BIAS_PCT` (40%) of the time |
| Dealt, not drawn | all type-leaning slots share one deal of the own-type pool — no slot repeats another's move, and once the pool is spent the rest of the learnset is plain randomized |
| Duplicates | re-rolled, so a collision no longer silently wastes the slot |

**The two guaranteed slots did not actually deliver.** Both were implemented as rejection sampling:
re-roll up to `LEARNSET_MAX_REROLLS` (24) times and accept the first roll that happens to match. A
same-type move under the level's power cap is a thin slice of 367, so 24 draws usually missed it. The
"guaranteed" first move was same-type only **38.5%** of the time overall — and **5.2%** for Psychic,
**5.4%** for Electric. Both guaranteed slots now call `PickStabMove()`, which enumerates the candidate
pool instead of re-rolling into it, so the slot always fills. Non-guaranteed slots still use rejection
sampling, which is fine — they express preferences, not guarantees.

**Verified offline** by reproducing the RNG maths:

| Check | Result |
|---|---|
| First slot is a same-type attack | 38.5% → **100%** (20,460 cases) |
| A same-type attack exists above level 30 | 25.0% → **100%** (18,660 cases) |
| Ordinary damaging slot is same-type | 10.5% → **46.3%** (65,218 cases) |
| Repeated same-type moves within one learnset | 20.1% → **2.8%** (45,811 dealt moves) |
| Duplicates originating in the deal itself | **0** |
| Of the 4 moves known at Lv50, mean matching type | 0.25 → **1.25** |
| Pokemon with *zero* type-matching moves at Lv50 | 77.7% → **17.0%** |
| All five sites agree for the same (move, species, level) | ✅ |
| Level-cap violations | **0** |
| Max power at levels 1-15 / 16-30 / 31+ | 60 / 100 / 250 |
| Average power by band | 22.0 → 33.6 → 41.3 |
| Damaging/status shape preserved | **100%** |

Determinism across all five sites is the load-bearing property here: the move relearner and the
level-up-moves list must show exactly what the Pokémon actually learns. Since the re-roll chain is a pure
function of (original move, species, learn level), they cannot diverge.

**T8b.2, T8b.3, T8b.7 and T8b.8 are covered by this.**

Note: Tertu's `RANDOMIZE_LEARNSET` is unimplemented, so there's no reference behaviour to compare against.
These test the design in the plan.

- [ ] **T8b.1 — Off (control).** *Random Moves* On, *Random Learnsets* Off: learnsets show the current
      behaviour — occasional duplicate moves, Hyper Beam at Lv5, mons with four status moves.
- [ ] **T8b.2 — No duplicates.** Sample 10 randomized learnsets end to end. **No move appears twice** in the
      same species' learnset.
- [ ] **T8b.3 — Level-scaled power.** Moves learned at Lv1-15 are weak (power ≤ ~60); moves learned at Lv31+
      skew strong. Spot-check 5 species across the whole learnset, not just the first few entries.
- [ ] **T8b.4 — Early offense guaranteed.** **Every** species' first learnset entry is a damaging move — not
      just the starter. Verify on wild mons, which is where the old code didn't apply the guard.
- [ ] **T8b.14 — Relearner agreement (critical).** Open the Move Relearner and compare its list against the
      moves the Pokémon actually learns on level-up, and against the summary's level-up move list. **All three
      must match.** They are computed independently and only agree because the roll is deterministic — a
      mismatch means one site is passing a different learn level.
- [ ] **T8b.5 — Early STAB guaranteed.** The first entry matches one of the species' types. This used
      to be a claim rather than a guarantee — see the note below — so check it on a narrow type
      (Psychic, Electric, Dragon) rather than on a Normal-type where it almost always held anyway.
- [ ] **T8b.5a — Late STAB guaranteed.** One entry learned above level 30 is a same-type attack with
      **no power cap**. Level a mon past 35 and confirm it learns a full-power same-type move. Which
      entry gets promoted is fixed per species, so it is the same slot on every playthrough with the
      same trainer ID.
- [ ] **T8b.5b — Promoted slot keeps its shape.** The promoted entry is chosen from the species' late
      *damaging* entries where any exist (398 of 402 species), so a status slot is normally not eaten.
      For the 4 species whose late entries are all status, one is converted — that is intended.
- [ ] **T8b.5c — Species with no late entry.** 59 of 461 species end their learnset at or below level 30.
      They get no late guarantee and must behave exactly as before. Confirm no crash and no empty slot.
- [ ] **T8b.5h — Narrow types exhaust instead of repeating.** Fairy has **2** damaging moves in this
      ROM, Dragon 5, Ghost and Steel 6. A mono-Fairy (Sylveon is the only one) must learn Moonblast
      and Play Rough **once each**, and every later type slot falls through to the general tiered
      pool. Before dealing, a Sylveon could roll Moonblast four times in one learnset.
- [ ] **T8b.5i — Dual types continue on the other type.** The pool is the union, so a Fairy/Flying mon
      keeps drawing Flying moves after the two Fairy ones are spent. No separate fallback to code.
- [ ] **T8b.5j — Deal is consistent across all five call sites.** The deal is a forward walk keyed on
      species, so truncating the learnset at a mon's current level cannot change what earlier slots
      got. Check the move relearner, the summary-screen level-up list, and an actual level-up all
      show the same move for the same entry.
- [ ] **T8b.5e — Type bias, not type lock.** Ordinary damaging slots match the species' type about
      **46%** of the time, up from 10.5%. Off-type attacks must still be common — if a mon's damaging
      moves are *all* same-type, the bias is mis-set. Check several mons, not one: at 40% a six-slot
      learnset rolls zero same-type hits about 8% of the time, which is variance, not a bug.
      (Kadabra on trainer ID `0x7F3C` is a real example of that unlucky case.)
- [ ] **T8b.5f — Biased picks are tier-weighted.** A same-type move handed out by the bias should skew
      good: tier 2 runs ~17% against ~6% for an untiered draw from the same pool, tier 5 ~8% against
      ~18%. Watch for a mon whose same-type moves are all junk — that would mean the tier tables are
      being bypassed.
- [ ] **T8b.5g — Category stays mixed.** The bias deliberately does *not* force the mon's attacking
      stat, only the two guaranteed slots do. A special attacker should still pick up the odd physical
      same-type move.
- [ ] **T8b.5d — Not a duplicate of the catch-time move.** The Phase 6 injected move and the promoted
      learnset move are drawn with different seed salts. They can still coincide for a narrow type
      (~37% when the species has only 3-5 eligible moves, ~9% at 11+); when they do, the level-up is
      simply a no-op and the mon keeps the move. Confirm this does not read as a lost level-up.
- [ ] **T8b.6 — Starter unaffected in a bad way.** The existing starter damaging-move guard
      (`src/pokemon.c:6176`) still works, or is cleanly superseded. No double-application, no regression.
- [ ] **T8b.7 — Learnsets are not corrupted.** `gLevelUpLearnsets` is `const` ROM data. Confirm the post-pass
      operates on a **copy** — if the same species randomizes differently on a second encounter, or other
      species' learnsets shift, something is writing to ROM-shadow memory. **Serious bug; check early.**
- [ ] **T8b.8 — Determinism.** Same species → same learnset across save/reload, and identical between the
      summary screen and battle.
- [ ] **T8b.9 — HM moves preserved.** No HM move is re-rolled out of a learnset in a way that blocks
      progression.
- [ ] **T8b.10 — Level-up learning works.** Levelling a mon mid-game learns the randomized learnset moves at
      the right levels, with working prompts.
- [ ] **T8b.11 — Evolution.** A mon that evolves picks up the evolved species' randomized learnset correctly.
- [ ] **T8b.12 — Stacks with Phase 6.** *Guarantee STAB* + *Random Learnsets* both on: no conflict, no
      double-injection that wastes two move slots on the same type.
- [ ] **T8b.13 — Performance.** Encounter latency with Phases 3, 6, and 8b all active. Three post-passes now
      run per wild encounter — if it stutters, this is the most expensive one.

---

## Phase 8c — TM randomization weighting

**Status:** ✅ Implemented — **needs in-game verification**

**(a)** `PickWeightedTM()` in `src/item.c` — weights which TM drops by the tier of the move it teaches.
**(b)** `GetRandomizedTMMove()` in `src/party_menu.c` — reassigns what each TM teaches, drawn from the
tiered move pool. `BattleMoveIdToItemId` now routes through `ItemIdToBattleMoveId` so the forward and
reverse mappings cannot disagree.

**Verified offline** across 65 trainer IDs:

| Check | Result |
|---|---|
| TM tables containing an HM move | **0** ✅ |
| TM tables containing a duplicate | **0** ✅ |
| Tier spread of assigned TM moves | 3.0 / 21.5 / 39.2 / 29.3 / 5.8 / 1.1% |
| All six tiers present among the 50 TMs (so (a) never falls back) | ✅ |

**T8c.5, T8c.8 and T8c.10 are covered by this.** T8c.11 (every caller agreeing) still needs the emulator —
there are 13 callers of `ItemIdToBattleMoveId` and only play confirms the bag description, the teach prompt
and the move actually learned all match.

### (a) Which TM you find

- [ ] **T8c.1 — Off (control).** *VGC TMs* Off: TM item drops are uniform across TM01-TM50.
- [ ] **T8c.2 — Weighted.** Sample ~25 TM drops. Skew toward high-tier TMs, with a tail.
- [ ] **T8c.3 — Strict.** Only top-tier TMs drop.
- [ ] **T8c.4 — Dedupe preserved.** The existing loop (`src/item.c:1233-1238`) still prevents finding a TM you
      already own. Weighting must not break it — a weighted roll that keeps hitting the same tier could spin
      the retry loop; confirm it still terminates.
- [ ] **T8c.5 — HMs never dropped.** The guard at `src/item.c:1222-1229` still excludes HM01-HM08.

### (b) Which move a TM teaches

- [ ] **T8c.6 — Off (control).** *Random TM Moves* Off: TM01 teaches Focus Punch, TM02 Dragon Claw, etc. —
      matching `FOREACH_TM` in `include/constants/tms_hms.h`.
- [ ] **T8c.7 — On.** TMs teach different moves than their vanilla assignments.
- [ ] **T8c.8 — No duplicate moves across TMs.** Two different TMs shouldn't teach the same move. Check all 50.
- [ ] **T8c.9 — Weighted.** The 50 assigned moves skew toward high-tier moves, drawn from the **359-move
      candidate pool** (all randomizable moves minus the 8 HMs) — not just the 50 vanilla TM moves.
- [ ] **T8c.9b — Excluded moves absent.** No TM teaches Sketch, Metronome, Mimic, Transform, or an OHKO move
      (Fissure, Horn Drill, Guillotine, Sheer Cold) if those were excluded in the worksheet.
- [ ] **T8c.10 — HMs untouched (critical).** HM01 Cut, HM02 Fly, HM03 Surf, HM04 Strength, HM05 Flash,
      HM06 Rock Smash, HM07 Waterfall, HM08 Dive **all still teach their original moves**. The
      `tmNumber < NUM_TECHNICAL_MACHINES` guard is what enforces this. **A randomized HM soft-locks the
      playthrough — this is the single highest-risk test in the phase.** Verify all eight explicitly.
- [ ] **T8c.11 — Consistent everywhere.** The move shown in the bag description, in the party-menu teach
      prompt, and the move actually learned all agree. `ItemIdToBattleMoveId` is called from several places
      (`src/party_menu.c:2092`, `src/item_use.c:934`, `src/daycare.c:803`, `src/apprentice.c:384`) — a remap
      applied in only some paths shows up as the bag describing one move and teaching another.
- [ ] **T8c.12 — Determinism.** Same TM → same move across save/reload and across separate battles.
- [ ] **T8c.13 — Compatibility is arbitrary (expected).** A species "compatible with TM01" now learns whatever
      TM01 teaches, which may be off-type. Documented consequence, not a bug — confirm it doesn't crash.
- [ ] **T8c.14 — Move Tutor / Apprentice unaffected.** `src/apprentice.c` reads `ItemIdToBattleMoveId`;
      confirm those flows still work and don't offer `MOVE_NONE`.
- [ ] **T8c.15 — Daycare move inheritance.** `src/daycare.c:803` compares against TM moves — verify egg move
      inheritance doesn't break.

---

## Phase 9 — Opponent type display in battle

**Status:** ✅ Implemented — **needs in-game verification**

Two new windows, `B_WIN_OPPONENT_TYPE_1/2`, drawn by `MoveSelectionDisplayOpponentTypes()` in
`src/battle_controller_player.c`. Reuses the existing move-type icon sheet (one 32×16 icon per type,
palette 13) — no new graphics.

Placed in the empty region **right of the move description**: columns 21-28, rows 47-48, directly above the
existing move-type and category icons.

| Check | Result |
|---|---|
| Templates added to **both** window tables | ✅ standard **and** arena |
| Tile budget | 0x3BC-0x3CC used; **52 of 1024 still free** |
| Screen-rect overlap with any other window | **none** ✅ |
| Tile-range overlap with any other window | **none** ✅ |

Reads `gBattleMons[targetId].type1/type2` — **live battle state, not `gSpeciesInfo`** — so it is
automatically correct under the Modern/Fairy type modes, the type randomizer, and mid-battle type changes.

Mono-type mons store the same type twice, so the second window is cleared rather than drawing a duplicate.
Gated on `optionTypeEffective`, matching the effectiveness and STAB hints. Hidden on B-button exit so it
does not linger over "What will X do?", and hidden in doubles until a target is actually selected.

**T9.5, T9.11 and T9.17 are partly covered** by the overlap and budget checks above — but only play
confirms it *looks* right.

- [ ] **T9.1 — Renders.** Trainer battle → Fight menu → opponent's type icon(s) visible.
- [ ] **T9.2 — Correct types.** Icons match the opponent's actual types; cross-check against the Pokédex.
- [ ] **T9.3 — Dual type.** Both icons show.
- [ ] **T9.4 — Mono type.** Only one icon; the second window is hidden, **not** a duplicate or garbage.
- [ ] **T9.5 — No overlap.** No collision with the move description, PP, move type icon, category icon, STAB,
      or effectiveness indicators. Check with the longest move description you can find.
- [ ] **T9.6 — Updates on switch.** Opponent switches → box updates.
- [ ] **T9.7 — Doubles.** Cycling targets updates the box to the currently-targeted opponent.
- [ ] **T9.8 — Doubles, both opponents.** Each shows its own types.
- [ ] **T9.9 — Doubles, ally targeting.** Targeting your partner shows something sensible — not stale
      opponent data.
- [ ] **T9.10 — Wild battle.** Renders correctly (or is intentionally hidden); no garbage.
- [ ] **T9.11 — Both window tables.** Verify in **both** a normal battle and a Battle Frontier / arena battle —
      `src/battle_bg.c` has two parallel template tables and updating only one breaks exactly one battle type.
- [ ] **T9.12 — Cleanup.** Backing out of Fight removes the box; it doesn't linger over "What will X do?".
- [ ] **T9.13 — Option gate (your decision).** `optionTypeEffective` off → box hidden along with the other
      hints. On → box visible.
- [ ] **T9.14 — Type randomizer.** With `tx_Random_Type` On, the box shows the **randomized** type — the payoff
      for reading `gBattleMons` instead of `gSpeciesInfo`.
- [ ] **T9.15 — Modern/Fairy modes.** Icons match those variant typings.
- [ ] **T9.16 — Mid-battle type change.** Conversion / Camouflage updates the box.
- [ ] **T9.17 — No tile corruption.** No graphical glitching anywhere in the battle UI. ~68 free tiles of 1024
      on BG0 — an overrun corrupts the move description window first.

---

## Phase 10 — Level Cap Rare Candy

**Status:** ✅ Implemented (party menu) — **needs in-game verification**

`ITEM_LEVEL_CAP_CANDY` (slot 228, was the `ITEM_0E4` dummy), a **key item** that is never consumed.
Given by the Oldale cheat lady **only when `tx_Challenges_LevelCap` is set**, via a new `IsLevelCapActive`
special.

Implemented as the normal Rare Candy applied repeatedly rather than one jump, which is what preserves the
per-level prompts. Three re-entry points carry the loop: after a level with nothing to learn, after the
move-learning chain, and after an evolution.

⚠️ **This is the least-verified phase in the project.** The loop threads through a multi-frame async chain
with several exit points, and none of it has been run. Build is clean and the logic is reasoned, but
**T10.25 (soft-reset mid-climb) and T10.10-T10.16 (the prompts) genuinely need the emulator.**

**PC storage use (T10.26-T10.34) is NOT implemented** — deliberately deferred, as the plan advised shipping
it separately.

### Gating (your requirement)

- [ ] **T10.1 — Not given without a cap.** `tx_Challenges_LevelCap` = 0 → the cheat lady does **not** hand
      over the candy.
- [ ] **T10.2 — Given with Normal cap.** `LevelCap` = 1 → candy is given.
- [ ] **T10.3 — Given with Hard cap.** `LevelCap` = 2 → candy is given.
- [ ] **T10.4 — Post-Champion.** After the E4, `GetCurrentPartyLevelCap()` returns 100
      (`src/tx_randomizer_and_challenges.c:389`). Confirm the chosen behaviour (recommended: refuse) — otherwise
      this becomes an instant-max-level button.

### Item

- [ ] **T10.5** Both copies of the item table in `src/data/items.h` (`:2820`, `:7675`) updated — no desync.
- [ ] **T10.6** Appears in **Key Items** with a sane name, description, and icon.
- [ ] **T10.7** Cannot be tossed or sold (`.importance = 1`); never consumed on use.
- [ ] **T10.8** Selecting it opens the **party menu**, not a bag message — confirms `ITEM_USE_PARTY_MENU`.

### Stepwise levelling (your specified behaviour)

- [ ] **T10.9 — Reaches the cap.** Lv5 mon, cap 15 → ends at **exactly Lv15**, never overshoots.
- [ ] **T10.10 — Stops at every level-up move.** A mon crossing several learn levels is prompted **at each
      one**, in ascending level order. This is the core of your requirement — verify the count of prompts
      matches the learnset between start and cap.
- [ ] **T10.11 — Declining works.** Declining a move prompt continues the climb; it doesn't abort at that level.
- [ ] **T10.12 — Accepting works.** Accepting learns the move (including the replace-a-move flow when the mon
      already has four) and then continues.
- [ ] **T10.13 — Stops at evolution levels.** A mon crossing its evolution level **evolves mid-climb**, then
      continues levelling toward the cap as the evolved species.
- [ ] **T10.14 — Post-evolution learnset.** After evolving mid-climb, subsequent move prompts come from the
      **evolved** species' learnset. Subtle and easy to get wrong.
- [ ] **T10.15 — Multiple evolutions.** A mon that crosses two evolution levels in one climb (e.g. Lv5 →
      cap 46 on a three-stage line) evolves twice.
- [ ] **T10.16 — Cancelling evolution.** Pressing B during evolution behaves as it normally would; the climb
      continues sanely afterward.
- [ ] **T10.17 — Nothing to do.** A mon already at cap: refuses with a message, or evolves if it can at cap
      (the existing `Task_RareCandyEvoAtCap` path). No crash.
- [ ] **T10.18 — Above cap.** A mon above cap is not level-*reduced*.

### Correctness

- [ ] **T10.19 — Stats recalculated.** All six stats match a manually-candied mon of the same level, species,
      nature, and IVs. Check the **summary screen**, not the level-up popup.
- [ ] **T10.20 — Current HP sane.** Not 0, not above max.
- [ ] **T10.21 — Egg.** Cannot be used on an egg.
- [ ] **T10.22 — Fainted mon.** Works or refuses gracefully; no corruption.
- [ ] **T10.23 — Full party.** All six capped in sequence; no state leak between mons.
- [ ] **T10.24 — Normal Rare Candy unaffected.** Still +1 level, still consumed, still cap-aware.
- [ ] **T10.25 — Interrupt safety.** Soft-reset mid-climb (during a move prompt). Reload: the save isn't
      corrupted and the mon is in a coherent state. **The state machine is the risky part of this phase** —
      test it adversarially.

### PC storage *(ship as a separate commit)*

Back up your `.sav` before this section.

- [ ] **T10.26** Menu entry appears on a box mon **only** when the candy is in the bag.
- [ ] **T10.27** Hidden without the item; hidden on empty slots and eggs.
- [ ] **T10.28** Box mon levels to the cap.
- [ ] **T10.29 — Withdraw round-trip.** Level, stats, moves, held item, nickname, OT, IVs, EVs, friendship,
      and ribbons all intact. **Check every one** — `BoxMonToMon`/`MonToBoxMon` round-trips are exactly where
      fields get dropped.
- [ ] **T10.30 — Deferred evolution and moves.** Per the plan, a box mon doesn't evolve or learn moves in the
      box, but **does** once withdrawn. Confirm nothing is silently lost.
- [ ] **T10.31 — Box integrity.** Neighbouring mons untouched; check slots before and after.
- [ ] **T10.32 — Save/reload.** Close PC, save, reload: the levelled box mon persisted.
- [ ] **T10.33 — Boundaries.** Repeat in a non-default box and at slot 0 / slot 29.
- [ ] **T10.34 — Cancel.** Backing out makes no change.

---

## Phase 11 — Ability / nature reroll cheat

**Status:** ✅ Implemented — **needs in-game verification**

In the stat editor, gated on *Reroll Cheat*: **SELECT** rerolls nature, **START** rerolls the ability slot.
L/R/A/B/D-pad were all already bound.

Nature goes through `MON_DATA_HIDDEN_NATURE` (the Mint field), **not** the personality value — personality
also decides gender, shininess, Unown letter, Wurmple's branch and Spinda's spots.

Two refusals, both playing `SE_FAILURE` rather than silently doing nothing:
- species whose `abilities[1]` is `ABILITY_NONE` (writing it would blank the summary)
- **either ability randomizer being on** — `GetAbilityBySpecies` ignores the stored `abilityNum` in that
  case, so the button would appear broken. This was `T11.10`.

- [ ] **T11.1 — Gated.** `tx_Features_RerollCheat` Off → no reroll UI.
- [ ] **T11.2 — Visible when on** and discoverable.
- [ ] **T11.3 — Nature rerolls.** Displayed nature changes; summary agrees.
- [ ] **T11.4 — Nature affects stats.** The boost/penalty updates — confirms `GetNature(mon, TRUE)` is read
      everywhere it matters.
- [ ] **T11.5 — Personality untouched (critical).** After a nature reroll, verify **gender**, **shininess**,
      **Unown letter**, and **Spinda spots** are unchanged. Any change means `MON_DATA_PERSONALITY` was
      rerolled instead of `MON_DATA_HIDDEN_NATURE` — the bug this design exists to avoid.
- [ ] **T11.6 — Ability rerolls** between the species' two slots.
- [ ] **T11.7 — Single-ability species.** Graceful "no effect" message rather than writing `ABILITY_NONE`.
      Check the summary isn't left blank.
- [ ] **T11.8 — Persists** across save/reload.
- [ ] **T11.9 — Battle agreement.** The rerolled ability actually triggers.
- [ ] **T11.10 — Randomizer interaction.** With *Random Abilities* or *VGC Abilities* On,
      `GetAbilityBySpecies` overrides the stored `abilityNum` — the button appears to do nothing. Verify the
      chosen handling (disabled, or a message). **Unhandled, this reads as a broken feature.**
- [ ] **T11.11 — Existing editor intact.** EV/IV editing still works; the cursor moves correctly through the
      6×2 grid and the new mode doesn't strand it.
- [ ] **T11.12 — Mint interaction.** Rerolling on a mon with a Mint already applied overwrites cleanly.

---

## Cross-phase integration

Run once everything is in, on one save with **all** options enabled.

- [ ] **TX.1** All options On → new game → boots, playable, no crash in the first hour.
- [ ] **TX.2** Full randomizer + all new features → reach the first gym without a softlock.
- [ ] **TX.3** Save/reload with everything on: all settings persist; viewer agrees with the menu.
- [ ] **TX.4** *Level-Scaled Wilds* + *Include Legendaries* + *Random Legendaries* together — no conflict.
- [ ] **TX.5** *VGC Moves* + *Guarantee STAB* + *VGC Abilities* + *Random Trainer* — trainer battles are hard
      but winnable, not degenerate.
- [ ] **TX.5b** *Random Learnsets* + *VGC TMs* + *Random TM Moves* all on — no move-source conflicts, and
      every HM still teaches its original move.
- [ ] **TX.6** *VGC Items* + *Random Items* — enough healing to survive; the economy isn't broken.
- [ ] **TX.7** Level Cap Candy + *Level-Scaled Wilds* — cap a clamped low-stage mon; it levels, prompts, and
      evolves correctly through the whole chain.
- [ ] **TX.8** Battle UI clean in singles, doubles, wild, and Frontier battles with everything on.
- [ ] **TX.9** No `MgbaPrintf` errors or asserts across a long session.
- [ ] **TX.10** ROM size within cartridge limits — watch for linker overflow after the new data tables in
      Phases 5-8.
- [ ] **TX.11** Encounter latency acceptable with Phases 3, 5, 6, and 8b all active — four post-passes now
      run per wild encounter. Time it; this is the most likely place for a visible stutter.
- [ ] **TX.12** Full playthrough to at least badge 3 with all options on, no softlock.
