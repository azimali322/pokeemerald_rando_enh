# Randomizer Enhancements — Implementation Plan

Fork of [resetes12/pokeemerald](https://github.com/resetes12/pokeemerald) (Pokémon Modern Emerald).
Companion doc: [`RANDOMIZER_ENHANCEMENTS_TESTING.md`](RANDOMIZER_ENHANCEMENTS_TESTING.md) — updated at the end of each phase.

**Revision 6** — **Phase 0 is complete, the ROM builds** (`make MODERN=1`); build revealed **EWRAM at 99.62%**,
which constrains several later phases; new Phase 2b (evolution stones + trade-evolution items at ₽1).
Trade evolution turns out to need **no new item** — Modern Emerald already has a Self-trader.

*Revision 5* — item worksheet regrouped by function for tiering ([`tiering/ITEMS.md`](tiering/ITEMS.md));
dead-item handling confirmed as **tier always + filter when the challenge is on**.

*Revision 4* — ability weights finalized at **40/30/22/6/2**; TM worksheet widened to the full 359-move
candidate pool; item randomization documented end-to-end
([Phase 8](#phase-8--vgc-weighted-hold-items-new-req-10)).

*Revision 3* — abilities re-tiered from `community-abilities-tierlist.png` on a 6-tier S/A/B/C/D/F scale
([Appendix A](#appendix-a--ability-tiers)); learnset overhaul and TM weighting added; Ultra Balls ₽1
everywhere; Tertu findings in
[Appendix F](#appendix-f--other-ideas-worth-taking-from-the-tertu-randomizer).

**Editable tier worksheets live in [`tiering/`](tiering/)** — one file per pool, generated directly from the
ROM source so nothing is invented:

| File | Contents |
|---|---|
| [`tiering/ABILITIES.md`](tiering/ABILITIES.md) | All 81 abilities, **already tiered** from the community list |
| [`tiering/MOVES.md`](tiering/MOVES.md) | All 367 randomizable moves + power/acc/type/category/PP, TM & HM flagged — *awaiting tiers* |
| [`tiering/TM_MOVES.md`](tiering/TM_MOVES.md) | **359 candidate moves** for TM assignment, + the 50 vanilla TM slots for reference, + 8 HMs (never randomize) — *awaiting tiers* |
| [`tiering/ITEMS.md`](tiering/ITEMS.md) | All 188 randomizable items, **grouped by function** (hold items, hold berries, balls, healing, vitamins, stones, flavour berries, mail, misc, dead) with hold effects and prices — *awaiting tiers* |

---

## Answering your questions first

### `GetPreEvolution` is not a setting — it's a function in the code

This is worth stating plainly because it changes how you read Phase 3. `GetPreEvolution` is a C function
at [`src/pokemon.c:10240`](../src/pokemon.c), declared `static` at line 79. It takes a species and brute-force
scans the evolution table for whatever evolves *into* it:

```c
static u16 GetPreEvolution(u16 species){
    int i, j;
    for (i = 1; i < NUM_SPECIES; i++)
        for (j = 0; j < EVOS_PER_MON; j++)
            if (gEvolutionTable[i][j].targetSpecies == species)
                ...
```

There is no menu entry for it and there never will be — it's plumbing that Phase 3 calls to walk a Salamence
back down to Bagon. The *player-facing* switch is the new **Level-Scaled Wilds** option from Phase 1.

**The setting you were thinking of is "Similar Evolution Level"** (`tx_Random_Similar`), which is the existing
balancing option on the randomizer page. It's related but different: it makes the randomizer swap
*like-for-like by evolution stage* — a first-stage mon becomes another first-stage mon, using the
`sRandomSpeciesEvo0/1/2` pools. That keeps the *shape* of the encounter table sane, but it has no idea what
level the encounter is, so a stage-2 slot on an early route still yields a stage-2 mon. Level-Scaled Wilds is
the piece that actually looks at the level. They stack, and Phase 3 covers the interaction.

**Performance note worth flagging now:** `GetPreEvolution` is 462 × 8 = ~3,700 iterations per call, and the
Phase 3 clamp can call it ~10 times per encounter (the walk plus the recursive min-level lookup). That's
~37,000 iterations on a 16 MHz CPU — a few milliseconds, once, at encounter start. Acceptable, but if it feels
sluggish the fix is to build a reverse-evolution lookup table once at boot instead of scanning every time.
Noted in Phase 3's risks.

### Legendary randomization — simplified per your feedback

You're right that it's cleaner. Phase 4 is now **"any legendary encounter yields a random legendary"**, not
"static encounters specifically." One rule, applied at the one chokepoint every legendary flows through
(`GetSpeciesRandomSeeded`), keyed off `EVO_TYPE_LEGENDARY`. Gift legendaries, battle legendaries, roamers, and
the plot legendaries all inherit it automatically. Details in Phase 4.

### Hand-tiering the weights — done, see the appendices

You offered to do this; you don't need to. The pools turned out to be small enough to enumerate exhaustively:

- **Abilities: 82 total** (`ABILITIES_COUNT` = 82, `include/constants/abilities.h`), so 81 usable. That's
  Gen 3's set plus four modern additions (Transistor, Dragon's Maw, Multitype, Pixilate). **Every one is
  tiered in [Appendix A](#appendix-a--ability-tiers).**
- **Moves: 369** (`MOVES_COUNT`), which is Gen 3 plus 14 modern coverage moves (Dark Pulse, Psycho Cut,
  Focus Blast, Power Gem, Shadow Claw, Flash Cannon, Air Slash, Bug Buzz, Dragon Pulse, Earth Power,
  Play Rough, Moonblast, Poison Jab, Heal Pulse). Tier 1 and 2 enumerated in
  [Appendix B](#appendix-b--move-tiers).
- **Items: ~190** in `sRandomValidItems[]` (`src/item.c:1019`). Tiered in
  [Appendix C](#appendix-c--hold-item-tiers).

[Appendix D](#appendix-d--weighting-methodology) documents the method so you can re-tier anything you
disagree with without reverse-engineering my reasoning. Tier assignment is opinion, and it's the part of this
plan most worth you overriding — the appendices are deliberately structured as flat editable lists.

---

## 1. What already exists (survey results)

Several requirements are **partly** implemented already, which changes the shape of the work.

| System | Where | State |
|---|---|---|
| Randomizer save flags | `include/global.h:1135-1204` (bitfields in `SaveBlock1`) | ~70 flags, packed bitfields |
| Randomizer options menu | `src/tx_rac_menu.c` | Paged menu, 6 items/page, clean add pattern |
| Settings viewer (read-only recap) | `src/tx_rac_viewer.c` | Mirrors every option; must be updated in lockstep |
| Core species randomizer | `src/pokemon.c:12273` `GetSpeciesRandomSeeded()` | Seeded + map-based, deterministic |
| Evolution-stage buckets | `src/pokemon.c:2470` `gSpeciesMapping[]` | Every species tagged `EVO_TYPE_0/1/2/SELF/LEGENDARY` |
| Species pools | `src/pokemon.c:2936-4290` | `sRandomSpecies`, `…Legendary`, `…Evo0/1/2/EvoLegendary` |
| Move pool | `src/pokemon.c:4565` `sRandomValidMoves[]` | Flat list, uniform pick |
| Move randomizer chokepoint | `src/pokemon.c:12320` `GetRandomMove(move, species)` | **Single** function; all call sites route through it |
| Ability randomizer | `src/pokemon.c:8258` `GetAbilityBySpecies()` | Species-substitution, *not* an ability pool — see Phase 7 |
| Item randomizer | `src/item.c:1217` `RandomItemId()` | Flat `sRandomValidItems[]` pick; TMs handled separately |
| Item randomizer entry points | `src/item.c:1247` `RandomItem()`, `:1255` `RandomItemHidden()` | Ground items vs hidden items — **separate**, which Phase 8 exploits |
| Wild encounter chokepoint | `src/wild_encounter.c:452` `CreateWildMon(species, level)` | **Single** entry for land / water / all rods / Feebas / outbreaks |
| Static encounter randomizer | `src/script_pokemon_util.c:106, 191` | `TX_RANDOM_T_STATIC` |
| Trainer party build | `src/battle_main.c:2348`, `:2563` | Two sites; moves seeded on the **original** species |
| Level cap | `src/tx_randomizer_and_challenges.c:382` `GetCurrentPartyLevelCap()` | Badge-count based, two tables + hard-mode variants |
| Rare Candy (cap-aware) | `src/party_menu.c:5376` `ItemUseCB_RareCandy()` | **Already** refuses past cap and evolves at cap |
| Cheat lady | `data/maps/OldaleTown/scripts.inc:61` | Gives `ITEM_INFINITE_REPEL`, `ITEM_INFINITE_RARE_CANDIES`, `ITEM_HEALING_HEART` |
| Item prices | `src/item.c:946` `ItemId_GetPrice()` | Single chokepoint, already has a difficulty multiplier |
| Shop inventories | `src/shop.c:772` `SetShopItemsForSale()` | `sShopInventories[badgeCount]` — **shared by all marts** |
| Battle move info box | `src/battle_controller_player.c:1747` | Already shows PWR / ACC / PRI + description |
| Battle move type + category icons | `src/battle_controller_player.c:1901, 1949` | `B_WIN_MOVE_TYPE_ICON` (7), `B_WIN_PSS_ICON` (8) already drawn |
| Type icon graphics | `sMoveTypeIcons_Gfx` / `_Pal`, 32×16 per type, palette slot 13 | Reusable as-is |
| Battle window templates | `src/battle_bg.c:239-419` **and** `:496-676` | Two parallel tables, both must be edited |
| Stat (EV/IV) editor | `src/ui_stat_editor.c:727-732` | Already renders nature + ability text |
| Nature override (mints) | `MON_DATA_HIDDEN_NATURE`, `include/pokemon.h:112` | Lets nature change **without** touching personality |
| PC storage menu | `src/pokemon_storage_system.c:114`, `:8244`, `:7935` | Extensible |

### Findings that shaped the plan

**The battle UI is ~80% done.** Comparing your screenshot to the current build: PWR, ACC, the description,
the move type icon, and the physical/special/status icon **all already render**. This build even adds PRI and
a STAB marker the screenshot lacks. The only genuinely new piece is the **opponent's type box** — Phase 9 is
therefore much smaller than it looked.

**Weighted VGC abilities cannot reuse the existing ability randomizer.** `GetAbilityBySpecies()` doesn't pick
an ability — it picks a random *species* and borrows that species' ability slot. There is no ability pool to
weight. Phase 7 adds a genuinely new code path that runs *instead of* the species-substitution.

**Trainer moves are seeded on the pre-randomization species.** At `src/battle_main.c:2348`, the call is
`GetRandomMove(partyData[i].moves[j], partyData[i].species)` — the **original** species, not the randomized
one. Harmless today, but Phase 6 (STAB guarantee) needs the *randomized* species' type, so it must read the
`species` local, not `partyData[i].species`. Easy to get wrong.

**Hidden items and ground items already have separate entry points.** `RandomItem()` (`src/item.c:1247`) and
`RandomItemHidden()` (`:1255`) both call `RandomItemId()` but from different scripts
(`data/scripts/obtain_item.inc:6` and `:187`). That split is exactly what Phase 8 needs to leave hidden items
on the existing uniform roll while weighting visible ones.

### Reference repos
You pointed at [Zetraphes/pokeemerald-expansion (tertu-randomizer)](https://github.com/Zetraphes/pokeemerald-expansion/tree/tertu-randomizer)
for the level-cap candy. Worth reading for UX, but **not worth porting** — that's pokeemerald-**expansion**
(different `gSpeciesInfo`, `gMovesInfo` vs this repo's `gBattleMoves`/`gMoveNames`, different party-menu
plumbing). This repo already has cap-aware rare candy logic; Phase 10 extends it rather than importing a
foreign implementation.

---

## Phase 0 — Build environment & baseline ROM — ✅ **COMPLETE**

**Status: the ROM builds.** `pokeemerald_modern.gba`, 32 MB, header `POKEMON EMER / BPEE`, exit code 0.

### What worked

devkitARM **was** already installed from your other project at `/opt/devkitpro/devkitARM` (gcc 16.1.0) —
it just wasn't on `PATH`, which is why `arm-none-eabi-gcc` appeared missing. libpng was already present from
your `brew install`. **agbcc was never needed** — the `MODERN=1` path uses devkitARM's own gcc.

Add to `~/.zshrc` so it persists:

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
export PATH=$DEVKITARM/bin:$PATH
```

Then build:

```bash
make MODERN=1 -j$(sysctl -n hw.ncpu)
```

`MODERN=1` is **required** here — the default `MODERN=0` path needs agbcc, which isn't installed. The output
is `pokeemerald_modern.gba` (not `pokeemerald.gba`), and it will never match `rom.sha1` — that hash is for a
byte-matching vanilla build, which is irrelevant for this project.

The 540 compiler warnings and the `LOAD segment with RWX permissions` linker warning are pre-existing and
harmless. Nothing in the log matched a real error (the 15 `-Werror` hits are host-tool build flags).

### ⚠️ EWRAM is at 99.62% — this constrains later phases

```
EWRAM:  261156 B / 256 KB   99.62%     <-- ~988 bytes free
IWRAM:   26288 B /  32 KB   80.22%
ROM:  23112952 B /  32 MB   68.88%
```

**Under 1 KB of EWRAM headroom.** ROM space is plentiful (10 MB free), so `const` data tables are fine —
every tier table in the appendices lives in ROM and costs nothing here. But anything needing *runtime* RAM is
effectively blocked:

| Plan item | EWRAM cost | Verdict |
|---|---|---|
| Tier tables (abilities/moves/items/TMs) | 0 — `const`, lives in ROM | ✅ Fine |
| Phase 8c **runtime remap** for TM moves | 0 — computed per call | ✅ **Use this** |
| Phase 8c *alternative* EWRAM shuffle table | ~100 B | ⚠️ Fits, but eats 10% of the remaining headroom |
| Phase 3 reverse-evolution lookup (perf fix) | ~924 B for `u16[462]` | ⚠️ Would consume nearly all of it |
| Tertu's dynamic species tables (Appendix F) | ~2.7 KB (6 B × 462) | ❌ **Does not fit** |
| Phase 8b learnset post-pass | small stack buffer | ✅ Fine if stack-local |

Three consequences for the plan:
1. **Phase 8c must use the runtime-remap option**, not the EWRAM shuffle table. The plan already recommended
   this; now it's effectively mandatory.
2. **Phase 3's performance optimization is off the table** unless something else is freed. Measure first
   (test T3.23) — the linear `GetPreEvolution` scan may well be fast enough.
3. **Appendix F's "dynamic species tables" idea is confirmed not worth pursuing**, on hard grounds rather
   than preference.

Check this number after every phase — `make` prints it on each link. If a phase pushes EWRAM to 100%, the
link fails.

### Emulator

```bash
brew install --cask mgba
```

Open **Tools → View Logs** and enable the **Debug** channel. The randomizer is instrumented with `MgbaPrintf`
traces at every decision point (`src/pokemon.c:12262`, `:12326`, `src/wild_encounter.c:462`) that print the
inputs *and* outputs of every randomization — by far the fastest way to verify Phases 3-8. They compile out
under `NDEBUG`; the `MODERN=1` build above leaves them in.

### Remaining Phase 0 checks

Code work can start now; these are gameplay smoke tests, best done while the ROM is fresh:

- [ ] ROM boots in mGBA to the title screen
- [ ] New Game reaches the randomizer options menu; an existing option persists across save + reload
- [ ] `MgbaPrintf` output appears in the log during a wild encounter

### Branch

```bash
git checkout -b feature/randomizer-enhancements
```

**Save-file warning for every phase from 1 onward:** adding bitfields to `SaveBlock1` shifts the struct layout
and **corrupts existing saves**. Start fresh after Phase 1 and keep that save.

---

## Phase 1 — Save flags & menu plumbing (foundation)

Add every new option's storage and UI up front, wired to stubs. Doing this once avoids eight separate
save-breaking layout changes.

### Files

| File | Change |
|---|---|
| `include/global.h` | New bitfields at the end of the `tx_*` block (~line 1204) |
| `include/tx_randomizer_and_challenges.h` | Matching `#define TX_*` dev defaults |
| `src/tx_rac_menu.c` | Menu enum, `sItemFunctions*` rows, `DrawChoices_*`, strings, save/load/reset (~1482, ~1559, ~1915, ~1933) |
| `src/tx_rac_viewer.c` | A `GetSel_*` accessor + row per option (~line 469) |
| `src/new_game.c` | Add to the "randomizer is active" check (~line 294) where applicable |
| `src/script.c` | Reset alongside the other flags (~line 534) where applicable |

### New flags

| Flag | Bits | Values | Requirement |
|---|---|---|---|
| `tx_Random_WildLevelScaled` | 1 | Off / On | Req 1 — evo stage matches encounter level |
| `tx_Random_Legendaries` | 1 | Off / On | Req 2 — legendary encounters → random legendary |
| `tx_Random_MovesVGC` | 2 | Off / Weighted / Strict | Req 6 |
| `tx_Random_GuaranteeStab` | 1 | Off / On | **New** Req 9 — ≥1 same-type move |
| `tx_Random_AbilitiesVGC` | 2 | Off / Weighted / Strict | Req 7 |
| `tx_Random_ItemsVGC` | 2 | Off / Weighted / Strict | **New** Req 10 — VGC hold items |
| `tx_Random_Learnsets` | 1 | Off / On | **New** Req 11 — learnset overhaul |
| `tx_Random_TMsVGC` | 2 | Off / Weighted / Strict | **New** Req 12a — which TM you find |
| `tx_Random_TMs` | 1 | Off / On | **New** Req 12b — which move a TM teaches |
| `tx_Features_CheapBalls` | 1 | Off / On | Req 3 |
| `tx_Features_RerollCheat` | 1 | Off / On | Req 8 (optional) |

**15 bits total**, out of ~4,384 available (see below). `Weighted` = biased roll, good stuff common but anything can appear. `Strict` = draw only
from tier 1. The extra bit on the three-state options costs nothing and avoids a second save-breaking change
if Weighted turns out too tame.

The Level Cap Candy (Req 4, now optional Phase 10) needs **no flag** — it's gated on
`tx_Challenges_LevelCap != 0`, per your instruction that it only be handed out when a level cap is set.

### Verify headroom

`SaveBlock1` is asserted at `src/save.c:80` to fit `SECTOR_DATA_SIZE * 4`.

**Measured after implementing Phase 1** (my earlier estimate was wrong — this ROM sets
`SECTOR_DATA_SIZE = 4084`, not vanilla's 3968):

| | Bytes |
|---|---:|
| `sizeof(struct SaveBlock1)` | 15,788 |
| Limit (`4084 × 4`) | 16,336 |
| **Free** | **548** (≈4,384 more bitfield bits) |

Save flags are **not** a scarce resource — there is room for many more options. The assert is still the gate;
if a build fails with `SaveBlock1FreeSpace`, reclaim space rather than moving the struct.

### Menu placement

Each new option sits next to the one it modifies: `WildLevelScaled` after `SIMILAR_EVOLUTION_LEVEL`,
`Legendaries` after `INCLUDE_LEGENDARIES`, `MovesVGC` and `GuaranteeStab` after `RANDOM_MOVES`, `AbilitiesVGC`
after `RANDOM_ABILITIES`, `ItemsVGC` after `RANDOM_ITEMS`. `CheapBalls` and `RerollCheat` go on the features
page. The menu pages automatically (`TX_MENU_ITEMS_PER_PAGE 6`), so no layout math.

### Risks
- `tx_rac_viewer.c` is easy to forget and produces a silently wrong settings recap. Same commit.
- Menu enums index parallel arrays; inserting mid-enum requires re-checking every table. Use designated
  initializers (`[MENUITEM_X] = …`) — the existing code already does.

---

## Phase 2 — Cheap Ultra Balls (Req 3)

**Your decision (revised): ₽1 Ultra Balls at every mart.** This simplifies the phase considerably — no
location gate needed, and the price and the stock now agree everywhere instead of the price being global
while the stock was Oldale-only.

### Implementation

1. **Stock.** Add `ITEM_ULTRA_BALL` to `sShopInventories[0]` (and `sShopInventories_PC[0]`) gated on
   `tx_Features_CheapBalls`. Marts are badge-indexed (`SetShopItemsForSale()`, `src/shop.c:772`) and shared
   across every location, so adding it at index 0 means "available from the first mart onward" — exactly the
   behaviour you want, and it falls out of the existing structure with no special-casing.

2. **Price.** In `ItemId_GetPrice()` (`src/item.c:946`), **before** the `tx_Challenges_Expensive` multiplier:

   ```c
   if (gSaveBlock1Ptr->tx_Features_CheapBalls && itemId == ITEM_ULTRA_BALL)
       return 1;
   ```

   Central chokepoint — shop list, quantity screen, and running total all read it, so all three stay
   consistent automatically.

### Side effect worth knowing
Sell price is `ItemId_GetPrice() / 2` (`src/item_menu.c:2265`), so Ultra Balls sell for ₽0 with this on.
Correct — it closes a buy-at-1/sell-at-600 money exploit. No action needed.

---

## Phase 2b — Evolution stones & trade-evolution items (New Req 13)

Bundles naturally with Phase 2 — same mechanism (mart stock + `ItemId_GetPrice` override), same ₽1 price.

### Trade evolution: **already solved, no new item needed**

Modern Emerald ships a **Self-trader** — a machine on **Lilycove City Department Store 1F**
(`data/maps/LilycoveCity_DepartmentStore_1F/scripts.inc:161`, `SCRIPT_SelfTrade_V1`). It picks a party
Pokémon, runs a real in-game trade round-trip (`CreateInGameTradePokemon` → `DoInGameTradeScene`), which
fires `EVO_MODE_TRADE` in `GetEvolutionTargetSpecies()` and evolves the mon. Trading with another game still
works too. README line 243 documents it.

**Cost: ₽10,000 per trade**, checked at `:150`.

So no Link Cable item is needed — and there isn't one in this ROM (I checked; the only "Link Cable" strings
are unrelated UI text). **The requirement is already met.**

Two things worth doing anyway:

1. **Drop the Self-trader price to ₽1** alongside the stones, for consistency with the rest of your cheat
   setup. It's a `checkmoney 10000, 0` / `removemoney 10000, 0` pair in the script — a two-line edit, no C
   changes. Without this, the ₽1 stones sit oddly next to a ₽10,000 trade.
2. **Stock the trade-evolution held items** — see below. This is the real gap.

### The actual gap: trade-evolution held items

Of the 15 trade evolutions, **11 need a specific held item**, and **none of those items is purchasable
anywhere in the game**:

| Item | Evolves |
|---|---|
| `ITEM_KINGS_ROCK` | Poliwhirl → Politoed, Slowpoke → Slowking |
| `ITEM_METAL_COAT` | Onix → Steelix, Scyther → Scizor |
| `ITEM_HARD_STONE` | Rhydon → Rhyperior |
| `ITEM_DRAGON_SCALE` | Seadra → Kingdra |
| `ITEM_UP_GRADE` | Porygon → Porygon2 → Porygon-Z |
| `ITEM_SPELL_TAG` | Dusclops → Dusknoir |
| `ITEM_DEEP_SEA_TOOTH` | Clamperl → Huntail |
| `ITEM_DEEP_SEA_SCALE` | Clamperl → Gorebyss |

(The other four — Kadabra, Machoke, Graveler, Haunter — are plain `EVO_TRADE` and need only the Self-trader.)

Without these on sale, the Self-trader can't complete 11 of the 15 trade evolutions. **Stocking these 8 items
is what actually finishes the feature.**

### Evolution stones are also unpurchasable today

The six stones (`ITEM_SUN_STONE`, `MOON_STONE`, `FIRE_STONE`, `THUNDER_STONE`, `WATER_STONE`, `LEAF_STONE`)
are sold in **no mart**. Current sources are a Sun Stone gift at the Mossdeep Space Center
(`MossdeepCity_SpaceCenter_1F/scripts.inc:118`) and shard trades with the Diving Treasure Hunter
(`Route124_DivingTreasureHuntersHouse/scripts.inc:232-247`). Base price is ₽2100.

### This is much easier than Phase 2 was

Regular Poké Marts call `pokemart 0`, which ignores the script list and serves the badge-indexed global
`sShopInventories[badgeCount]` — that's why "Oldale only" was awkward for Ultra Balls.

**Department store and specialty clerks are different.** They call `pokemart <label>` with a **static,
hardcoded `.2byte` list**, e.g. `LilycoveCity_DepartmentStore_3F_Pokemart_Vitamins` (`:15`). Those lists are
genuinely per-location, so adding items to one clerk affects only that clerk. No `SetShopItemsForSale` changes,
no map gate, no C code.

### Implementation

1. **Lilycove Department Store 3F** — add a stones list. 3F already has two static clerks
   (`_Pokemart_Vitamins` at `:15`, `_Pokemart_StatBoosters` at `:31`); add a third, or extend Vitamins:

   ```
   LilycoveCity_DepartmentStore_3F_Pokemart_EvoStones:
   	.2byte ITEM_SUN_STONE
   	.2byte ITEM_MOON_STONE
   	.2byte ITEM_FIRE_STONE
   	.2byte ITEM_THUNDER_STONE
   	.2byte ITEM_WATER_STONE
   	.2byte ITEM_LEAF_STONE
   	.2byte ITEM_KINGS_ROCK
   	.2byte ITEM_METAL_COAT
   	.2byte ITEM_HARD_STONE
   	.2byte ITEM_DRAGON_SCALE
   	.2byte ITEM_UP_GRADE
   	.2byte ITEM_SPELL_TAG
   	.2byte ITEM_DEEP_SEA_TOOTH
   	.2byte ITEM_DEEP_SEA_SCALE
   	.2byte ITEM_NONE
   ```

   Lilycove is the right home: the Self-trader is one floor below, so buying the item and doing the trade
   happen in the same building.

2. **Slateport** — add the same trade-evolution items to `SlateportCity_Pokemart_EnergyGuru`
   (`data/maps/SlateportCity/scripts.inc:146`), which is already a static list. Slateport is reachable much
   earlier than Lilycove, so this is what makes mid-game trade evolutions practical.

3. **Price** — extend the Phase 2 override in `ItemId_GetPrice()` (`src/item.c:946`) from one item to a set:

   ```c
   if (gSaveBlock1Ptr->tx_Features_CheapBalls && IsCheapCheatItem(itemId))
       return 1;
   ```

   `IsCheapCheatItem` covers Ultra Ball, the six stones, and the eight trade-evo items.

### Decision for you
This reuses the **`tx_Features_CheapBalls`** flag rather than adding a new one — one toggle for "cheap cheat
shop". If you'd rather control balls and evolution items separately, say so and it's a second bit in Phase 1
(there's room). **Recommend: one flag**, since you want both on together.

### Risks
- **Hold-effect side effects.** King's Rock (flinch), Hard Stone, Spell Tag, Deep Sea Tooth/Scale, and
  Dragon Scale all have *battle* hold effects. Selling them at ₽1 makes King's Rock flinch-stacking trivially
  available. Acceptable for a cheat shop, but it's a real balance change, not just convenience.
- **Sell price becomes ₽0** (`price / 2`), same as Ultra Balls. This is *desirable* here — stones are worth
  ₽2100, so a ₽1 buy price without the sell change would be an infinite money exploit. **Verify explicitly.**
- **Moon Stone** has no `EVO_ITEM` user in some Gen-3 line edits; confirm it's actually useful in this ROM
  before assuming stocking it completes anything.

---

## Phase 3 — Level-appropriate wild randomization (Req 1)

**Your reading confirmed:** the encounter table's own level, not the badge-based cap.

### Design: clamp, don't re-seed

The randomizer's contract is *determinism* — same route, same species, every encounter (that's what
`tx_Random_MapBased` buys you). Feeding `level` into the seed would break that: two encounter slots on the
same route with different levels would become unrelated species.

Instead: **randomize exactly as today, then walk the result down its own evolution chain until it's plausible
at that level.** Deterministic, preserves map-based consistency, and gives exactly your Salamence→Bagon
example — Route 102 grass (Lv3-4) yields Bagon; Route 102 Super Rod (Lv~30) yields Shelgon; late-game water
yields Salamence.

### Implementation

**`src/pokemon.c`** — two new functions near `GetSpeciesRandomSeeded()` (~line 12273):

```c
static u8 GetSpeciesMinLevel(u16 species);          // lowest plausible level for this species
u16 ClampSpeciesToLevel(u16 species, u8 level);     // walk down until it fits
```

`GetSpeciesMinLevel` walks `GetPreEvolution()` and reads `gEvolutionTable[prevo][i]` for the entry whose
`targetSpecies` matches:

| Evolution method | Level used |
|---|---|
| `EVO_LEVEL`, `EVO_LEVEL_ATK_GT_DEF`, `EVO_LEVEL_SILCOON`, … | `.param` directly |
| `EVO_ITEM` (stones) | **28** (estimate) |
| `EVO_TRADE`, `EVO_TRADE_ITEM` | **32** |
| `EVO_FRIENDSHIP`, `_DAY`, `_NIGHT` | **22** |
| `EVO_BEAUTY` (Milotic) | **30** |

Recurse and take the max against the pre-evo's own min level (Dragonite = max(55, Dragonair's 30) = 55).
Cap the walk at 3 iterations — no chain is longer. `SPECIES_NONE` from `GetPreEvolution` terminates it, so
single-stage and baby mons are safe.

**`src/wild_encounter.c:459`** — inside `CreateWildMon`:

```c
species = GetSpeciesRandomSeeded(species, TX_RANDOM_T_WILD_POKEMON, 0);
if (gSaveBlock1Ptr->tx_Random_WildLevelScaled)
    species = ClampSpeciesToLevel(species, level);
```

That one hook covers **every** wild path — `TryGenerateWildMon` (land + water), `GenerateFishingWildMon`
(Old/Good/Super Rod, `:539`), Feebas (`:925`), and mass outbreaks (`:555`) — because they all call
`CreateWildMon`. Per-rod behaviour is free: each rod's table carries its own levels, and `ChooseWildMonLevel`
(`:284`) has already picked one by then.

### Interaction with Similar Evolution Level
With `tx_Random_Similar` on, the randomizer already maps stage-0→stage-0, so clamping rarely fires — correct,
the two stack harmlessly. Clamping matters most with Similar **off**, which is exactly the configuration that
produces Lv4 Salamence today.

### Risks
- **Performance.** `GetPreEvolution` is a linear scan (462 × 8). The clamp can invoke it ~10× per encounter,
  ~37,000 iterations. Fine as a one-off at encounter start. **The obvious fix — a cached reverse-evolution
  table — is largely blocked:** it needs ~924 bytes and Phase 0 measured only ~988 bytes of EWRAM free.
  Measure first (test T3.23); if it does stutter, the table would have to go in ROM as a generated `const`,
  or something else must be freed.
- `GetPreEvolution` is `static` — lift the prototype or co-locate the new functions.
- Branching evolutions (Wurmple→Silcoon/Cascoon) have two entries; take the first match, levels are identical.
- Legendaries are `EVO_TYPE_SELF`/`LEGENDARY` with no pre-evo — the walk no-ops, so a Lv4 Mewtwo stays a
  Lv4 Mewtwo. That's your stated intent.

### Out of scope
Randomized **trainer** parties are not clamped — trainer mons have authored levels and changing them shifts
difficulty balance. Flag it if you want it; it's a small follow-up.

---

## Phase 3b — BST similarity mode / "Improved" balancing (New Req 14) — ✅ **IMPLEMENTED**

Pairs with Phase 3. Phase 3 controls **how evolved** a wild mon is; this controls **how strong** it is.
Background and worked examples in [Appendix G](#appendix-g--tertus-dynamic-species-tables-explained).

### The problem

`tx_Random_Similar` ("Similar Evolution Level") matches by evolution stage, and stage is a poor proxy for
power. The `EVO_TYPE_0` bucket holds 179 species from **Sunkern (BST 180)** to **Lapras (BST 535)** — so the
balance option itself puts a 3× power gap in one swap pool.

### The design — as built

The existing **BALANCING** option became three-state rather than gaining a separate toggle, since the three
are mutually exclusive answers to "what should this species become?":

| `tx_Random_Similar` | Menu label | Behaviour |
|---|---|---|
| 0 | **Off** | Any species → any species |
| 1 | **Balanced** | Evolution-stage matching (the original behaviour) |
| 2 | **Improved** | Swap within ±10.24% base stat total |

Field widened 1 → 2 bits; 0 and 1 keep their existing meanings.

**One wrinkle worth recording:** the old menu was *inverted* — `DrawChoices_Random_EvoStages` drew index 0 as
"On", and load/save applied a `!`. That negation is removed, so the mapping is now a plain 0/1/2. Anything
touching this option must not reintroduce it.

`PickRandomStarter` still treats the field as truthy, so *Improved* falls back to the stage-0 starter pool.
That is intended — starters should stay first-stage regardless of balancing mode.

### Implementation

**1. Generated sorted table** — `src/data/pokemon/species_by_bst.h`, species ids ascending by base stat total:

```c
static const u16 sSpeciesByBST[] = { /* generated at build time */ };
```

**924 bytes of ROM, zero EWRAM.** This is the key departure from Tertu: he builds it in RAM at boot because
pokeemerald-expansion has an open-ended species list that would make a static table go stale. Your list is
fixed at 462, so sorting once at build time is strictly better here.

Generate it with a script committed alongside (`tools/gen_bst_table.py`), parsing
`src/data/pokemon/species_info.h`. **Regenerate if you ever change base stats** — `tx_Mode_New_Stats` already
alters some, so decide whether the table should reflect the modified stats (it should; the modified values are
what the game actually uses).

**2. Band lookup** in `src/pokemon.c`:

```c
static u16 GetSpeciesBST(u16 species);                    // sums the 6 base stats
static u16 GetRandomSpeciesByBST(u16 species, u16 seed);  // two binary searches + seeded pick
```

Binary search `sSpeciesByBST` for the first entry ≥ `bst * 0.8976` and the first > `bst * 1.1024`, then pick a
seeded index between them. ~9 comparisons per bound versus a 462-entry scan — the whole reason a sorted table
exists.

Use integer maths, not floats: `lo = bst - (bst * 1024 / 10000)`. The GBA has no FPU and this is on the
encounter path.

**3. Hook** — in `GetRandomSpecies()` (`src/pokemon.c:12240`), add the mode-2 branch alongside the existing
`tx_Random_Similar` branch. Everything downstream is unchanged.

### Interaction with Phase 3
They compose cleanly and are worth running together: BST picks a species of appropriate power, then
`ClampSpeciesToLevel` walks it down to an appropriate evolution stage. A Lv4 Route 102 encounter would pick
something Poochyena-tier by BST, and clamping would rarely need to fire at all.

### Edge cases — resolved in the build
- **Narrow bands at the extremes.** Handled: the band widens up to 4× when it would hold fewer than 8
  candidates. Sunkern goes 6 → 17, Slaking → 32. Measured narrowest band across all 400 species is **13**
  (Azurill); none is empty.
- **Legendaries.** A ±10.24% band around Mewtwo (680) is almost entirely other legendaries, which is a sane
  outcome — but confirm it respects `tx_Random_IncludeLegendaries` rather than sneaking them in.
- **Species missing base stats.** The generator parsed 436 of ~462; the rest are forms or placeholders. Ensure
  the table only contains species that are valid randomizer targets, and that a species absent from the table
  falls back to the flat random path rather than reading past the end.

### Risks
- Getting the band arithmetic wrong in integer maths gives a silently too-wide or too-narrow pool. Unit-test
  the bounds against a few known BSTs before wiring it in.
- The table must stay sorted. If someone edits it by hand and breaks the ordering, binary search returns
  garbage rather than failing loudly — worth a debug-only `STATIC_ASSERT`-style ordering check at boot.

---

## Phase 4 — Random legendary encounters (Req 2) — ✅ **IMPLEMENTED**

**Simplified per your feedback: every legendary encounter in the game yields a random legendary.**

### Implementation

One rule at one chokepoint. In `GetSpeciesRandomSeeded()` (`src/pokemon.c:12273`), the early-out currently reads:

```c
if (slot == EVO_TYPE_SELF || (slot == EVO_TYPE_LEGENDARY && !gSaveBlock1Ptr->tx_Random_IncludeLegendaries))
    return species;
```

Insert **ahead of it**: if `tx_Random_Legendaries` is on and `gSpeciesMapping[species] == EVO_TYPE_LEGENDARY`,
draw from `sRandomSpeciesEvoLegendary[]` (`src/pokemon.c:4292`) and return — bypassing the early-out entirely.

Because it keys off `EVO_TYPE_LEGENDARY` at the shared chokepoint rather than off `TX_RANDOM_T_STATIC`, it
covers **every** route a legendary can reach the player:

- Battle legendaries — `CreateScriptedWildMon` (`src/script_pokemon_util.c:191`)
- Gift legendaries — `ScriptGiveMon` (`:106`)
- Wild legendaries — `CreateWildMon`, when *Include Legendaries* is on
- The third static site at `src/pokemon.c:5753`
- Trainer-owned legendaries, when *Random Trainer* is on

**Roamers** (Latias/Latios, `src/roamer.c`) use their own `CreateMon` path — verify and add the same call if
it doesn't inherit. That's the one place likely to need a second edit.

Non-legendary statics (Castform, Beldum, Wynaut, the fossils, gift Pokémon) are unaffected — they aren't
`EVO_TYPE_LEGENDARY`.

### The legendary pool
`sRandomSpeciesEvoLegendary[]` holds ~26 entries: Articuno, Zapdos, Moltres, Mewtwo, Mew, Raikou, Entei,
Suicune, Lugia, Ho-Oh, Celebi, Regirock, Regice, Registeel, Kyogre, Groudon, Rayquaza, Latias, Latios,
Jirachi, Deoxys, Arceus, Regidrago, Regieleki, Regigigas.

### Uniqueness — your answer: unique if feasible, else duplicates

**It's feasible.** The codebase already has the machinery: `ShuffleListU16()` with a fixed seed, used by
`PickRandomStarterForOneTypeChallenge()` (`src/pokemon.c:12038`). Approach:

1. Build the shuffled permutation of `sRandomSpeciesEvoLegendary[]` with a save-derived seed.
2. Index it by the *original* legendary's position in the list, not by a modulo of the species id.

That makes the mapping a bijection — every legendary maps to exactly one distinct legendary, deterministically,
with no collisions. Needs a small helper (`GetLegendaryIndex(species)`, a linear scan of ~26 entries) and the
shuffled array either cached in EWRAM or regenerated on demand (cheap at 26 elements).

**Built as unique.** Verified a bijection across 9,363 sampled trainer IDs. The shared `ShuffleListU16`
turned out to reuse one seed for every iteration, so `GetRandomLegendary` runs its own Fisher-Yates with a
per-iteration seed rather than changing shared code that starter selection depends on.

### Risks
- **Plot integrity — highest consequence in the plan.** Kyogre/Groudon/Rayquaza gate the Cave of Origin and
  Sky Pillar sequences. Verify those cutscenes don't hard-reference the species; a hard reference could
  strand a save. Test explicitly (T4.11).
- Level is preserved from the script, so a replacement appears at the original's level (Rayquaza's Lv70).
  Intended, but means a swapped-in Celebi shows up far above its usual level. Fine for a randomizer.

---

## Phase 5 — VGC-weighted move pool (Req 6)

### Implementation

**`src/pokemon.c`** — tiered tables next to `sRandomValidMoves[]` (`:4565`). Contents in
[Appendix B](#appendix-b--move-tiers).

```c
static const u16 sVGCMovesTier1[] = { /* ~55 */ };
static const u16 sVGCMovesTier2[] = { /* ~110 */ };
// everything else falls through to sRandomValidMoves
```

**`GetRandomMove()` (`src/pokemon.c:12320`)** — the whole change lives here:

```c
u16 GetRandomMove(u16 move, u16 species)
{
    if (gSaveBlock1Ptr->tx_Random_MovesVGC == VGC_STRICT)
        return sVGCMovesTier1[RandomSeededModulo(move + species, ARRAY_COUNT(sVGCMovesTier1))];

    if (gSaveBlock1Ptr->tx_Random_MovesVGC == VGC_WEIGHTED)
    {
        u16 roll = RandomSeededModulo(move + species + 0x3001, 100);   // NOTE: different offset
        if (roll < 45) return sVGCMovesTier1[RandomSeededModulo(move + species, ARRAY_COUNT(sVGCMovesTier1))];
        if (roll < 80) return sVGCMovesTier2[RandomSeededModulo(move + species, ARRAY_COUNT(sVGCMovesTier2))];
    }
    return sRandomValidMoves[RandomSeededModulo(move + species, RANDOM_MOVES_COUNT)];
}
```

**45% tier 1 / 35% tier 2 / 20% tail.** Keeping a 20% tail preserves the randomizer's chaos and stops every
mon converging on the same six moves.

### Why this is the whole job
`GetRandomMove` is the sole chokepoint. All call sites route through it: player learnsets
(`src/pokemon.c:6166-6285`, `10300-10375`) **and** trainer parties (`src/battle_main.c:2348`, `:2563`). Your
requirement — "all Pokémon including opposing trainers, wild and player" — is satisfied automatically, and the
trainer half is correctly conditional on `tx_Random_Trainer` because `battle_main.c` only calls it in that branch.

### Risks
- **Use a different seed offset for the tier roll** than the within-tier roll, or the two correlate and you
  get banding (all four moves from one tier).
- A wrong-but-valid `MOVE_*` constant is silent. Cross-check against `include/constants/moves.h`.
- **HM progression.** `HMsOverwriteOptionActive()` (`src/tx_randomizer_and_challenges.c:107`) already includes
  `tx_Random_Moves`. Add the VGC option there too, or HM-locked progression can **soft-lock the playthrough**.

---

## Phase 6 — Guaranteed same-type move (New Req 9) — ✅ **IMPLEMENTED**

*"All Pokémon have at least 1 move that matches their type when captured or in the wild. Same for trainers
with randomized Pokémon."*

Depends on Phase 5. Build it as a **post-pass** that runs after all four moves are assigned — that way it's
one function, applied identically everywhere, and it doesn't complicate `GetRandomMove`.

### Implementation

**`src/pokemon.c`** — new function:

```c
void EnsureStabMove(struct Pokemon *mon, u16 species);
```

1. Read `type1 = GetTypeBySpecies(species, 1)`, `type2 = GetTypeBySpecies(species, 2)`.
   **Use `GetTypeBySpecies`, not `gSpeciesInfo[].types`** — it already accounts for Modern/Fairy type modes
   and the type randomizer (`src/pokemon.c:12129`).
2. Scan the mon's four moves. If any **damaging** move (`gBattleMoves[m].power > 1`) matches either type,
   return — nothing to do.
3. Otherwise, collect candidate moves by scanning `gBattleMoves[]` for entries where
   `type == type1 || type == type2`, `power > 1`, and the move is in the valid pool. Pick one with
   `RandomSeededModulo(species + <offset>, candidateCount)` so it stays deterministic.
4. Replace a slot — pick the *weakest damaging move*, or slot 3, seeded. Don't blindly overwrite slot 0; that
   tends to delete the mon's best move.
5. `SetMonData(MON_DATA_MOVE1 + slot, &move)` **and** the matching `MON_DATA_PP1 + slot`. Forgetting PP
   leaves a 0-PP move — a classic bug here.

A runtime scan of `gBattleMoves[]` (369 entries) beats authoring 18 per-type tables and can't drift out of
sync when moves change. It runs once per mon creation, not per frame.

### Call sites

| Site | File |
|---|---|
| Wild + captured mons | `src/wild_encounter.c` — end of `CreateWildMon` (`:452`) |
| Static / gift mons | `src/script_pokemon_util.c` — `ScriptGiveMon` (`:106`), `CreateScriptedWildMon` (`:191`) |
| Trainer parties | `src/battle_main.c` — after the move loop at `:2348` and `:2563` |

**Resolved differently than planned.** The trainer sites' local `species` is only assigned inside the
Random-Trainer branch, so passing it would be stale when that option is off. `EnsureStabMove` takes only the
mon and reads the species back with `GetMonData` — correct in every branch, and impossible to get wrong at a
call site.

### Scope decision
This applies at **creation time** only, which matches your wording ("when captured or in the wild"). A mon
that later learns four non-STAB moves by levelling isn't re-corrected. Continuously enforcing STAB through
level-up would mean overriding player move choices, which is worse. Flag it if you disagree.

### Risks
- **Status-only species.** A few species have movepools where the scan might find nothing usable for their
  type. Always guard: if `candidateCount == 0`, leave the moveset alone rather than writing `MOVE_NONE`.
- **Interaction with Phase 5 Strict.** In Strict mode the four moves come from tier 1, and the STAB pass may
  inject a *non*-tier-1 move to satisfy the type constraint. That's correct — the constraint wins — but note
  it so it doesn't read as a Phase 5 bug.
- Dual-type mons only need one of the two types matched. Don't require both.
- **HM check again:** if the STAB pass can overwrite an HM move, it can soft-lock progression the same way
  Phase 5 can. Exclude HM moves from the overwrite candidates.

---

## Phase 7 — VGC-weighted abilities (Req 7)

### Why this is the hard one
`GetAbilityBySpecies()` (`src/pokemon.c:8258`) randomizes by *substituting a species* and borrowing its
ability slot. There is no ability pool. Weighting requires building one.

### Implementation

Tiered tables in `src/pokemon.c` — full contents in [Appendix A](#appendix-a--ability-tiers), mapped from
`docs/community-abilities-tierlist.png`:

```c
static const u16 sAbilitiesTierSA[] = { /* 16 — S and A merged */ };
static const u16 sAbilitiesTierB[]  = { /* 22 */ };
static const u16 sAbilitiesTierC[]  = { /* 26 */ };
static const u16 sAbilitiesTierD[]  = { /*  9 */ };
static const u16 sAbilitiesTierF[]  = { /*  6 — F + Negative */ };
// ABILITY_WONDER_GUARD and ABILITY_CACOPHONY appear in no table.
```

Cumulative thresholds on a seeded 0-99 roll — **40 / 30 / 22 / 6 / 2**, per the weighting analysis in
Appendix A (note this is *not* the 45/35/20 used by moves and items):

```c
//  0-39  S+A     40-69  B     70-91  C     92-97  D     98-99  F/Negative
```

**`GetAbilityBySpecies()`** — insert **before** the existing species-substitution block:

```c
if (gSaveBlock1Ptr->tx_Random_AbilitiesVGC)
{
    gLastUsedAbility = <weighted pick, seeded on species + abilityNum>;
    return gLastUsedAbility;
}
```

Returning early keeps it cleanly separate from the legendary / `tx_Mode_Legendary_Abilities` special cases
below it — those only need to run on the species-substitution path. Same 45/35/20 weighting.

### Risks — read before implementing
- **Determinism is non-negotiable.** `GetAbilityBySpecies` is called constantly, including mid-battle. Seed on
  `species + abilityNum` **only** — never call `Random()`. A non-deterministic ability flickers between frames.
- **Ability count is 2** (`include/pokemon.h:332`, `abilities[2]`) — no hidden-ability slot. Don't design for three.
- Hard-coded species signatures (Truant/Slaking, Forecast/Castform, the `tx_Mode_Legendary_Abilities` block at
  `:8267`) can interact oddly. Acceptable for a randomizer; worth a test pass.
- **Four abilities are excluded** from all tiers — see Appendix A for why.

---

## Phase 8 — VGC-weighted hold items (New Req 10)

*"Weighted VGC randomized good items, particularly hold items. Keep hidden items unaffected."*

### Reading your requirement

The item randomizer randomizes **ground items / item balls**, plus held items on scripted mons. It's not a
held-item-only system. Two things follow:

1. **The tiering covers the whole pool, biased toward hold items.** Good hold items go tier 1; your unwanted
   battle items (X Attack, Dire Hit, Guard Spec) go to the junk tail. That achieves "I don't want battle
   items" without a hold-item-only restriction.
2. **Healing items must stay well-represented in tier 2.** If the weighting starved Potions and Revives, the
   game becomes unplayable. This is a real failure mode — tier 2 is deliberately healing-heavy.

Full lists in [Appendix C](#appendix-c--hold-item-tiers).

### The species-specific trap

`sRandomValidItems[]` contains Light Ball, Thick Club, Soul Dew, Metal Powder, Lucky Punch, Stick, Deep Sea
Tooth, and Deep Sea Scale. These are **elite in VGC and useless in a randomizer** — each does nothing unless
held by one specific species, which a random item ball will essentially never pair with. A naive "VGC tier
list" import puts them in tier 1 and makes 8 of your best rolls dead weight. They're in **tier 3** in
Appendix C. Worth knowing, because it's exactly the kind of thing that looks like a bug later.

### Implementation

**`src/item.c`** — tiered tables near `sRandomValidItems[]` (`:1019`).

`RandomItemId()` (`:1217`) needs to distinguish visible from hidden. The two entry points are already
separate (`RandomItem()` at `:1247`, `RandomItemHidden()` at `:1255`), so add a parameter:

```c
u16 RandomItemId(u16 itemId, bool8 applyVGCWeighting);
```

- `RandomItem()` passes `TRUE` → weighted.
- `RandomItemHidden()` passes `FALSE` → existing uniform roll, **hidden items unaffected as you asked**.
- `CreateScriptedWildMon` (`src/script_pokemon_util.c:194`) passes `TRUE` — that's a held item, exactly what
  you want weighted.

Leave the TM/HM branch (`:1220-1240`) untouched; it has its own logic and HMs are already protected.

### How item randomization actually works (answering your question)

Worth laying out, because the structure determines what a "smarter" version can do.

**`RandomItemId(itemId)` (`src/item.c:1217`) has three branches:**

1. **TM/HM pocket** (`:1220`) — picks a random TM from TM01-TM50 with a dedupe loop so you don't find one you
   already own. **HM01-HM08 are explicitly excluded** (`:1222-1229`) — they pass through unchanged.
2. **Key items** (`:1241`) — excluded entirely, returned unchanged. This is what protects progression.
3. **Everything else** — one flat uniform pick:
   ```c
   itemId = sRandomValidItems[RandomSeededModulo(itemId + mapId, RANDOM_ITEM_COUNT)];
   ```
   Seeded on the original item **plus the current map section**, so it's deterministic and location-varying.

**Three call sites, two of them script-driven:**

| Caller | Reaches | Source |
|---|---|---|
| `RandomItem()` (`:1247`) | Visible item balls | `Std_ObtainItem`, `data/scripts/obtain_item.inc:6` |
| `RandomItemHidden()` (`:1255`) | Hidden items | `EventScript_HiddenItemScript`, `:187` |
| `CreateScriptedWildMon()` | Held items on scripted mons | `src/script_pokemon_util.c:194` |

The visible/hidden split already exists as two separate entry points, which is what lets Phase 8 weight
visible items while leaving hidden ones on the uniform roll, exactly as you asked.

**The pool is 188 items** — and it's much broader than "hold items". Breaking it down:

| Category | Count |
|---|---:|
| Has a battle hold effect | 71 |
| Usable in battle from the bag | 62 |
| **Both** (hold effect *and* bag-usable) | **11** |
| Battle-usable only, no hold effect | 51 |
| Neither (mail, shards, flutes, vitamins, stones…) | 66 |

That 11-item overlap is precisely the group you flagged: **Cheri, Chesto, Pecha, Rawst, Aspear, Leppa, Oran,
Persim, Lum, Sitrus, and Berry Juice** — berries that are both consumable *and* genuinely good held. Any rule
phrased as "drop battle items" has to keep these.

### Filtering out unusable items under the no-battle-items challenge

You want items you can't use excluded from the roll when `tx_Challenges_NoItemPlayer` is on. **I checked what
that option actually restricts, and the answer is narrower than it sounds.**

The restriction lives in `ItemMenu_UseInBattle()` (`src/item_menu.c:2137`):

```c
if (gSaveBlock1Ptr->tx_Challenges_NoItemPlayer && ItemId_GetBattleFunc(gSpecialVar_ItemId) != ItemUseInBattle_PokeBall)
```

Two consequences:

- **Poké Balls are explicitly exempt** — you can still catch Pokémon. All 12 ball types stay useful.
- **It only blocks the *battle* menu.** Potions, Revives, Ethers, and Full Heals are still fully usable from
  the bag and party menu between battles. They are *not* dead items; they're just no longer mid-battle
  panic buttons.

So the correct predicate is **not** "battle-usable". It's "has no hold effect **and** cannot be used outside
battle **and** isn't a Poké Ball":

```c
holdEffect == HOLD_EFFECT_NONE
  && battleUseFunc != NULL && battleUseFunc != ItemUseInBattle_PokeBall
  && (fieldUseFunc == NULL || fieldUseFunc == ItemUseOutOfBattle_CannotUse)
```

**That yields exactly 11 items** — every one verified against `src/data/items.h`:

```
ITEM_X_ATTACK     ITEM_X_DEFEND    ITEM_X_SPEED     ITEM_X_ACCURACY
ITEM_X_SPECIAL    ITEM_DIRE_HIT    ITEM_GUARD_SPEC          (stat boosters)
ITEM_POKE_DOLL    ITEM_FLUFFY_TAIL                          (escape items)
ITEM_YELLOW_FLUTE ITEM_RED_FLUTE                            (battle-only flutes)
```

All pinch berries (Liechi, Salac, Petaya, Ganlon, Apicot, Lansat, Starf) and all 11 hold-berries are
correctly **kept** — they have hold effects, so the predicate never fires on them. Blue Flute is kept too; it
has a field use, unlike Yellow and Red.

**Honest assessment: this is a small win.** 11 of 188 items is ~6% of the pool, so the felt improvement is
modest. It's cheap to implement — a single predicate in `RandomItemId`, no new data table, no new save flag
(it keys off the existing `tx_Challenges_NoItemPlayer`) — so it's worth doing. But if you were expecting it to
transform the item pool, the Phase 8 tier weighting is what actually does that; those 7 X-items are already
headed for the junk tier regardless.

**Where the real gain is:** the X-items and escape items are dead weight *even without* the challenge active,
because you said you don't play with battle items. Rather than gating the exclusion on
`tx_Challenges_NoItemPlayer`, consider just putting all 11 in the bottom tier permanently — same effect, works
whether or not the challenge is on, and it's already how Appendix C treats the X-items.

### Decision: both (confirmed)
1. **Tier the 11 into the junk tier** in Appendix C — always active, no conditional. They drop to ~2%
   frequency whether or not the challenge is on, which matches how you play regardless.
2. **Add the predicate as a hard filter** when `tx_Challenges_NoItemPlayer` is on — drops them to zero.

The two compose cleanly: the tier table is static and always applied, the filter is a conditional rejection
pass layered on top. Neither needs a new save flag.

Implement the filter as a **rejection pass** over the weighted result (re-roll if the pick is dead, bounded to
~8 attempts) rather than pre-filtering the pool. That keeps the tier tables static and avoids rebuilding
`sRandomValidItems` at runtime.

### Risks
- **Progression items.** Verify no *required* item (a key item, or an HM) can be replaced by a weighted roll.
  The existing `POCKET_KEY_ITEMS` guard at `:1241` handles key items — confirm it still fires on the new path.
- Weighting ground items toward hold items means fewer Potions from item balls. Watch early-game difficulty
  and rebalance tier 2 if it bites.
- `RandomItemId` is also reachable from the `RandomItem`/`RandomItemHidden` **specials** (`data/specials.inc:150-151`).
  Changing the signature means updating both wrappers, not the script side.

---

## Phase 8b — Learnset randomization overhaul (New Req 11)

### First: the Tertu learnset pattern doesn't exist

I read the branch. **`RANDOMIZE_LEARNSET` is declared but not implemented** — it's an enum entry in
`include/randomizer.h` with the comment *"Randomization of species learnset. Not yet implemented."* The same
is true of `RANDOMIZE_BASE_STATS` and `RANDOMIZE_MON_TYPES`. There is no learnset code in `src/randomizer.c`
to port. So there's no Tertu pattern to copy here — the design below is built for your ROM instead.

### What Modern Emerald already does

Learnsets **are** already randomized when `tx_Random_Moves` is on: each learnset entry is independently mapped
through `GetRandomMove(move, species)` at `src/pokemon.c:6166` and the sibling call sites. There's even a
partial quality guard already — for the **starter only** (`!FlagGet(FLAG_SYS_POKEMON_GET)`), a retry loop at
`src/pokemon.c:6176` re-rolls up to 100 times until it gets a damaging move, so you don't start the game with
four status moves.

That existing guard is the precedent for this whole phase: it's the right idea applied to exactly one mon.
The overhaul generalizes it.

### The four problems with the current approach

Each learnset entry is rolled independently, which produces:

1. **Duplicates** — the same move can be rolled for several learnset slots. `GiveMoveToBoxMon` rejects an
   exact duplicate at learn time, so the slot is silently wasted rather than visibly broken.
2. **No level appropriateness** — a Lv5 learnset entry can roll Hyper Beam; a Lv50 entry can roll Tackle.
   This is the move-side analogue of the Lv4-Salamence problem Phase 3 fixes for species.
3. **No guaranteed offense past the starter** — every other mon in the game can roll four status moves.
4. **No coherence** — no relationship between a mon's moves and its type or stats.

### Design

A **post-pass over the whole learnset**, not a change to `GetRandomMove`. Randomize as today, then fix up the
resulting set — same architectural choice as Phase 3 (clamp, don't re-seed) and Phase 6 (post-pass), and it
keeps all four features composable.

New function in `src/pokemon.c`:

```c
void RandomizeLearnsetForSpecies(u16 species, u16 *outMoves, u8 *outLevels, u8 count);
```

Applied in order:

1. **Dedupe.** Track rolled moves in a small bitfield; on collision, re-roll with an incremented offset
   (bounded retry, like the existing starter loop). Cheap and removes the wasted-slot problem entirely.

2. **Level-scaled power.** Bias each slot's roll by the level it's learned at. Reuse the Phase 5 tier
   machinery rather than inventing a second system:

   | Learn level | Pool |
   |---|---|
   | 1-15 | Tier 2 + tail, and cap `power <= 60` |
   | 16-35 | Tier 2 weighted, tier 1 allowed |
   | 36+ | Tier 1 weighted |

   This is the same idea as Phase 3, applied to moves: what you get should suit when you get it.

3. **Guaranteed early offense.** Generalize the existing starter guard to **every** species — the first
   learnset entry must be a damaging move. The code at `src/pokemon.c:6176` already does this; the change is
   dropping the `!FlagGet(FLAG_SYS_POKEMON_GET)` condition and moving it into the post-pass.

4. **Guaranteed early STAB.** Force the first entry to match one of the species' types. **This is Phase 6's
   `EnsureStabMove` applied to the learnset instead of the final moveset** — build Phase 6 first and reuse
   the candidate-scan helper rather than writing a second one.

### Interaction with Phase 6
Phase 6 guarantees STAB on the *final four moves at creation time*; this phase guarantees it *throughout the
learnset*, so a mon keeps STAB as it levels rather than only at capture. They're complementary, and both
should be on. If only one ships, Phase 6 is the higher-value half.

### Risks
- **The learnset arrays are `const`.** `gLevelUpLearnsets` / `gLevelUpLearnsets_Original` live in ROM. The
  post-pass must operate on a **copy** at mon-creation time, not attempt to rewrite the table. Don't try to
  make the global learnset mutable — that's a much larger change with save implications.
- **Performance.** This runs per mon creation and iterates the whole learnset with possible re-rolls. Combined
  with Phase 3's evolution walk and Phase 6's move scan, that's three post-passes per wild encounter. Measure
  encounter latency (test TX.11).
- **HM moves** — same soft-lock hazard as Phases 5 and 6. Exclude from re-rolls.
- **Determinism** — every re-roll must be seeded, never `Random()`. A learnset that differs between viewing
  the summary and entering battle is a serious bug.

---

## Phase 8c — TM randomization weighting (New Req 12)

### Two different things are called "TM randomization"

Worth separating, because this repo currently only has the first:

| | What it randomizes | State here |
|---|---|---|
| **(a) TM item drops** | *Which TM* you find in an item ball | **Exists** — `src/item.c:1232` |
| **(b) TM move mapping** | *Which move* a given TM teaches | **Does not exist** |

Your wording ("the Modern Emerald moves that can be randomized for TMs") points at **(b)**, but **(a)** is
where the current code is, and it's nearly free. I'd do both, (a) first.

### (a) Weight which TM you find — small

Today, `RandomItemId()` picks uniformly across TM01-TM50 with a dedupe loop:

```c
itemId = ITEM_TM01 + RandomSeededModulo(itemId, 50);
```

Replace the uniform pick with the same 45/35/20 tier roll used everywhere else, tiering the **50 TMs by the
move they teach**. Tiering is in [Appendix E](#appendix-e--tm-tiers). The existing dedupe loop
(`src/item.c:1233-1238`) is preserved — it stops you finding the same TM twice.

Note the HM guard at `src/item.c:1222-1229` already excludes HM01-HM08 from this path. Leave it alone.

### (b) Randomize which move each TM teaches — larger

**The obstacle:** TM→move is compile-time constant. `sTMHMMoves[]` (`src/data/party_menu.h:1145`) is generated
from the `FOREACH_TM` / `FOREACH_HM` macros in `include/constants/tms_hms.h`, and
`ItemIdToBattleMoveId()` (`src/party_menu.c:5052`) is a straight array index:

```c
u16 ItemIdToBattleMoveId(u16 item)
{
    u16 tmNumber = item - ITEM_TM01;
    return sTMHMMoves[tmNumber];
}
```

Two implementation options:

- **Runtime remap (recommended).** Leave `sTMHMMoves` alone and make `ItemIdToBattleMoveId` consult a
  randomized mapping when the option is on:

  ```c
  u16 ItemIdToBattleMoveId(u16 item)
  {
      u16 tmNumber = item - ITEM_TM01;
      if (gSaveBlock1Ptr->tx_Random_TMs && tmNumber < NUM_TECHNICAL_MACHINES)
          return GetRandomizedTMMove(tmNumber);   // seeded, weighted, deduped
      return sTMHMMoves[tmNumber];
  }
  ```

  Deterministic from the save seed, no save-layout change, no EWRAM table. `GetRandomizedTMMove` does a
  seeded weighted pick with a dedupe pass so two TMs don't teach the same move.

- **EWRAM shuffle table.** ~~Generate a 50-entry table at game start.~~ **Ruled out by the Phase 0 build:**
  EWRAM sits at 99.62% (~988 bytes free), so a 100-byte table would consume 10% of all remaining headroom for
  a marginal per-call saving. Use the runtime remap.

**HMs must be excluded, absolutely.** `tmNumber < NUM_TECHNICAL_MACHINES` above is what enforces it — HM01-08
sit past the TMs in the same array. Randomizing an HM breaks Cut/Surf/Strength/Fly gates and **soft-locks the
playthrough**. This is the single highest-risk line in the phase.

### Consequence worth accepting
Each species has a TM-compatibility bitfield (`CanMonLearnTMHM`). Once TM→move is randomized, "compatible with
TM01" no longer means "can learn Focus Punch" — it means "can learn whatever TM01 now teaches." That's normal
and expected in randomizers, but it makes compatibility effectively arbitrary. The alternative (recomputing
compatibility from the new move) would need a legality model this ROM doesn't have. Accept it.

### Flags
Two new bits in Phase 1: `tx_Random_TMsVGC` (2 bits: Off/Weighted/Strict) for (a), and `tx_Random_TMs`
(1 bit) for (b). **Add them in the Phase 1 batch** — that's the whole point of doing the plumbing once.

---

## Phase 9 — Opponent type display in battle (Req 5)

**Your decision: tied to the existing `optionTypeEffective` setting.**

### What's already there
The move info box is **already done** — PWR, ACC, description, move type icon, and category icon all render
today. The new work is only the **opponent's type box**.

### Implementation

1. **Two new windows.** `B_WIN_OPPONENT_TYPE_1` / `_2` in `include/constants/battle.h` (after
   `B_WIN_MOVE_DESCRIPTION` = 27). Add templates to **both** tables in `src/battle_bg.c` (`:239-419` and
   `:496-676`) — they're parallel copies and both are live.

   **Tile budget:** `B_WIN_MOVE_DESCRIPTION` sits at `baseBlock 0x350`, 18×6 = 108 tiles, ending at `0x3BC`.
   BG0 has 1024 (`0x400`), leaving ~68 free. Two 4×2 windows = 16 tiles. Place at `0x3BC` and `0x3C4`.

2. **Draw function** — `MoveSelectionDisplayOpponentTypes(u8 targetId)`, modelled on the existing blit:

   ```c
   LoadPalette(sMoveTypeIcons_Pal, 13 * 16, 32);
   FillWindowPixelBuffer(B_WIN_OPPONENT_TYPE_1, PIXEL_FILL(15));
   BlitBitmapToWindow(B_WIN_OPPONENT_TYPE_1,
                      (const u8 *)&sMoveTypeIcons_Gfx[(type1 * 0x100) / 4], 0, 0, 32, 16);
   ```

   Read types from `gBattleMons[targetId].type1 / .type2`. **Use `gBattleMons`, not `gSpeciesInfo`** — it's
   live battle state, so it's automatically correct under Modern/Fairy type modes, the type randomizer, and
   mid-battle type changes (Conversion, Camouflage).

   Mono-type mons have `type1 == type2`; hide the second window rather than drawing a duplicate.

3. **Call sites.** Mirror how the existing type icons refresh:
   - Singles: inside `MoveSelectionDisplayMoveType()` (`:1949`), which already computes `targetId`.
   - Doubles: inside `MoveSelectionDisplayMoveTypeDoubles(targetId)` (`:1901`), already called from the four
     target-cycling handlers (`:514`, `:606`, `:649`, `:733`). **Doubles works for free** — the box follows
     your target selection.

4. **Gate on `gSaveBlock2Ptr->optionTypeEffective == 0`**, matching the effectiveness and STAB indicators.

5. **Hide on teardown** so the box doesn't linger over "What will X do?".

### Risks
- Two window template tables — updating only one breaks exactly one battle type. Easy to miss.
- Tile budget is tight-ish; these break first if the description window ever widens.
- Wild battles resolve `targetId` slightly differently — verify no garbage renders.

---

## Phase 10 — Level Cap Rare Candy (Req 4) — **OPTIONAL**

Deprioritized at your request. Build after the core randomizer work lands.

### Behaviour, per your specification

Not a single jump to the cap. **Stop at every interesting level, then continue:**

1. Compute `cap = GetCurrentPartyLevelCap()`.
2. Level the mon **one level at a time** toward the cap.
3. At each level that has a **level-up move**, prompt to learn it — the player may accept or decline.
4. At each level that triggers an **evolution**, run the evolution.
5. Continue until the cap is reached.

Effectively an automated Rare Candy spam that can't overshoot the cap — the player keeps every choice they'd
have had, without the button mashing. That's a fair bit more work than a single jump, which is the main reason
this is optional.

### Gating, per your specification

**Only given when a level cap is set.** The cheat lady checks `tx_Challenges_LevelCap != 0` before handing it
over. Needs a new script special (e.g. `IsLevelCapActive`) registered in `data/specials.inc` alongside the
existing `IsRandomItemsActivated` (`:149`), then a `goto_if_eq` branch in
`data/maps/OldaleTown/scripts.inc:61`.

Note `GetCurrentPartyLevelCap()` also returns `MAX_LEVEL` after becoming Champion
(`src/tx_randomizer_and_challenges.c:389`). Decide whether the item should then jump mons to 100 or refuse.
**Recommend: refuse post-Champion**, or it becomes an instant-max-level button.

### Implementation sketch

**Item** — `include/constants/items.h`: reuse the `ITEM_0E4` dummy slot (228), immediately after
`ITEM_INFINITE_RARE_CANDIES` (227), rather than growing `ITEMS_COUNT`.

`src/data/items.h` — **this file contains the item table twice** (`:2820` and `:7675`); both copies need it.
Model on `ITEM_INFINITE_RARE_CANDIES` but with `.type = ITEM_USE_PARTY_MENU` (not `BAG_MENU` — it must target
a specific mon) and `.importance = 1` so it's never consumed.

**Party menu** — `src/party_menu.c`. The existing `ItemUseCB_RareCandy` (`:5376`) already does one level
correctly, including the learn-move prompt and `Task_RareCandyEvoAtCap`. The new handler is a **task-driven
state machine** that re-enters that per-level logic until the cap:

```
STATE_LEVEL_ONE  → apply one level
STATE_CHECK_MOVE → if a move is learnable, hand off to the existing prompt; resume here when it returns
STATE_CHECK_EVO  → if evolving, hand off to PartyMenuTryEvolution; resume here when it returns
STATE_LOOP       → level < cap ? STATE_LEVEL_ONE : STATE_DONE
```

**The async prompts are the hard part.** Both the move prompt and the evolution sequence are multi-frame tasks
that take over the task function; the loop has to store its state and re-enter cleanly. This is where the bugs
will be — budget for it.

**PC storage** (`src/pokemon_storage_system.c`) — new `MENU_LEVEL_CAP` in the enum (`:114`), text (`:8244`),
builder (`:7935`), handler (`:2655` region). Box mons are `struct BoxPokemon` — no level, no current HP — so
you must `BoxMonToMon()` → apply → `CalculateMonStats` → write back. **Do not attempt evolution or move
learning in the box**; neither has the party-menu task context. A box mon should level to the cap and defer
both until withdrawn. Ship this as its own commit after the party-menu version works.

---

## Phase 11 — Ability / nature reroll cheat (Req 8) — **OPTIONAL**

### Implementation

`src/ui_stat_editor.c` already renders both values (nature `:727-729`, ability `:731-732`).

**Nature — use the mint system, not personality.** `MON_DATA_HIDDEN_NATURE` exists
(`include/pokemon.h:112`, `GetNature(mon, TRUE)` at `src/pokemon.c:9268`):

```c
SetMonData(mon, MON_DATA_HIDDEN_NATURE, &newNature);
```

**Never reroll `MON_DATA_PERSONALITY`** — it also determines gender, shininess, ability slot, Unown letter,
Wurmple's evolution branch, and Spinda's spots.

**Ability** — flip `MON_DATA_ABILITY_NUM` between 0 and 1. Guard species where
`abilities[1] == ABILITY_NONE`: the reroll must be a no-op with a message, never a write that yields
`ABILITY_NONE`.

**UI** — a new `inputMode` reached by a shoulder button, moving the cursor to the nature/ability rows in
`WINDOW_3`. Avoid extending the 6×2 EV/IV grid; its sprite coordinate table (`:747`) is hard-coded.

Gate behind `tx_Features_RerollCheat`.

### Risk
With *Random Abilities* or *VGC Abilities* on, `GetAbilityBySpecies` **overrides** the stored `abilityNum` —
the reroll button will appear to do nothing. Either disable it while those are active or message the player.
**Left unhandled this reads as a broken feature, not a documented limitation.**

---

## Phase summary

**Progress: 6 of 17 phases implemented** (all building clean, `feature/randomizer-enhancements`).

| ✔ | Phase | Requirement | Size | Risk | Key files |
|:-:|---|---|---|---|---|
| ✅ | 0 | Build env | S | Low | — |
| ✅ | 1 | Save + menu plumbing | M | **Med** (save layout) | `global.h`, `tx_rac_menu.c`, `tx_rac_viewer.c` |
| ✅ | 2 | Cheap Ultra Balls | S | Low | `item.c` |
| ✅ | 2b | Evo stones + trade-evo items ₽1 | S | Low | `item.c`, 2 map scripts |
| ✅ | 3 | Level-aware wild randomization | M | Med | `pokemon.c`, `wild_encounter.c` |
| ✅ | 3b | BST similarity ("Improved") | M | Low | `pokemon.c`, `species_by_bst.h` |
| ✅ | 4 | Random legendaries (everywhere) | S | **Med** (plot) | `pokemon.c`, `roamer.c` |
| ☐ | 5 | VGC moves | M | Low | `pokemon.c` — **blocked on move tiers** |
| ✅ | 6 | Guaranteed STAB move | M | Med | `pokemon.c`, `wild_encounter.c`, `battle_main.c` |
| ☐ | 7 | VGC abilities | **L** | **Med-High** | `pokemon.c` — tiers ready (Appendix A) |
| ☐ | 8 | VGC hold items | M | Med | `item.c` — **blocked on item tiers** |
| ☐ | 8b | Learnset overhaul | **L** | Med | `pokemon.c` |
| ☐ | 8c | TM weighting + TM move remap | M | **Med-High** (HM soft-lock) | `item.c`, `party_menu.c` — **blocked on TM tiers** |
| ☐ | 9 | Opponent type box | M | Med | `battle_bg.c`, `battle_controller_player.c`, `battle.h` |
| ☐ | 10 | Level Cap Candy *(optional)* | **L** | **High** | `items.h`, `party_menu.c`, `pokemon_storage_system.c` |
| ☐ | 11 | Reroll cheat *(optional)* | M | Med | `ui_stat_editor.c` |

**Blocked on your input:** Phases 5, 8 and 8c need tiers filled in
([`tiering/MOVES.md`](tiering/MOVES.md), [`tiering/ITEMS.md`](tiering/ITEMS.md),
[`tiering/TM_MOVES.md`](tiering/TM_MOVES.md)). Phase 7's tiers are already done in Appendix A, so it can
proceed any time.

**Everything implemented so far is verified only by offline simulation and a clean build.** None of it has
been run in an emulator — see the test plan for what still needs a human at the controls.

## Decisions — resolved

| # | Question | **Your answer** |
|---|---|---|
| 1 | Phase 3: encounter level or badge cap? | **Encounter table level** |
| 2 | Phase 2: which marts? | **Every mart, ₽1** *(revised — was Oldale-only)* |
| 3 | Phase 4: unique legendaries? | **Unique if feasible** — it is (shuffled permutation); duplicates only as fallback |
| 4 | Phase 10: skip level-up moves? | **No** — stop at each move/evolution level, prompt, continue to cap |
| 5 | Phase 7: include Wonder Guard? | **Excluded** |
| 6 | Phase 9: always on or gated? | **Tied to `optionTypeEffective`** |
| 7 | Phase 8: tier the dead items, or filter them? | **Both** — always bottom-tier, plus a hard filter when `tx_Challenges_NoItemPlayer` is on |

### Still open
- **Phase 2b**: one `tx_Features_CheapBalls` flag for all cheap cheat items, or split balls from evolution
  items? (Recommend: one flag.) And drop the Self-trader from ₽10,000 to ₽1?
- **Phase 10**: should the candy refuse post-Champion, when the cap lifts to 100? (Recommend: refuse.)
- **Phase 6**: creation-time only, or re-enforce STAB on level-up? (Recommend: creation-time only.)
- **Phase 8c**: implement both (a) TM-drop weighting and (b) TM-move remapping, or just (a)?
  (Recommend: both, (a) first — it's nearly free.)
- **Appendix F**: which of Tertu's other ideas do you want? BST-mode and per-reason seed streams are the two
  I'd actually take.

---

# Appendices — tier lists

These are the weight tables. **Tier assignment is opinion** — edit freely; each is a flat list, and moving an
entry between tiers is a one-line change with no other code impact.

## Appendix A — Ability tiers

**Source: `docs/community-abilities-tierlist.png`.** Every ability in Modern Emerald was located on that
tier list and assigned its community tier. Editable worksheet: [`tiering/ABILITIES.md`](tiering/ABILITIES.md).

Verified against `include/constants/abilities.h`: **79 tiered + 2 excluded = 81 usable** (82 minus
`ABILITY_NONE`). No duplicates, no omissions, no invented constants.

### Tier counts

| Tier | Grade | Count | Share of pool |
|---|---|---:|---:|
| 1 | S | 7 | 8.9% |
| 2 | A | 9 | 11.4% |
| **1+2** | **S+A** | **16** | **20.3%** |
| 3 | B | 22 | 27.8% |
| 4 | C | 26 | 32.9% |
| 5 | D | 9 | 11.4% |
| 6 | F / Negative | 6 | 7.6% |
| — | Excluded | 2 | — |

### Final weights — **40 / 30 / 22 / 6 / 2**

| Tier | Weight | Count | **Per-ability** | vs uniform (1.27%) |
|---|---:|---:|---:|---:|
| S+A | 40% | 16 | **2.500%** | 1.97× |
| B | 30% | 22 | **1.364%** | 1.08× |
| C | **22%** | 26 | **0.846%** | 0.67× |
| D | **6%** | 9 | **0.667%** | 0.53× |
| F/Neg | 2% | 6 | **0.333%** | 0.26× |

Strictly decreasing per-ability at every step. S+A land at roughly **2× uniform**, F/Negative at **1/4
uniform** — your stated intent, with the tier ordering actually holding.

<details>
<summary><b>Why C is 22% and D is 6% rather than your original 20/8</b></summary>

Your first pass divided out like this:

| Tier | Weight | Count | **Per-ability** | vs uniform (1.27%) |
|---|---:|---:|---:|---:|
| S+A | 40% | 16 | **2.500%** | 1.97× |
| B | 30% | 22 | **1.364%** | 1.08× |
| C | 20% | 26 | **0.769%** | 0.61× |
| D | 8% | 9 | **0.889%** | 0.70× |
| F/Neg | 2% | 6 | **0.333%** | 0.26× |

**A D-tier ability would have been more likely than a C-tier ability** — 0.889% vs 0.769% each. C has 26
members sharing 20%, while D has only 9 sharing 8%, so the ordering broke between tiers 4 and 5 and Magma
Armor would have out-appeared Shed Skin. Moving 2 points from D to C fixes it; nothing else changed.

</details>

**Optional S/A split.** The 40% bucket gives S and A the same per-ability odds (2.5%), because the counts
happen to divide that way. To make S genuinely better than A, split it **20% / 20%** — S becomes 2.857% each
and A 2.222% each. Purely your call; the flat 40% is fine.

### Implementation note
Weights are cumulative thresholds on a 0-99 seeded roll. With the recommended numbers:

```c
//  0-39  S+A  (tier1 + tier2 merged into one 16-entry table)
// 40-69  B
// 70-91  C
// 92-97  D
// 98-99  F/Negative
```

Store S and A as one combined table unless you take the optional split, in which case keep them separate and
use `0-19` / `20-39`.

### Tier 1 — S (7)

```
ABILITY_ARENA_TRAP     ABILITY_DRIZZLE        ABILITY_DROUGHT        ABILITY_HUGE_POWER
ABILITY_INTIMIDATE     ABILITY_LEVITATE       ABILITY_SPEED_BOOST
```

### Tier 2 — A (9)

```
ABILITY_CHLOROPHYLL    ABILITY_CLEAR_BODY     ABILITY_COMPOUND_EYES  ABILITY_FLASH_FIRE
ABILITY_GUTS           ABILITY_PURE_POWER     ABILITY_SAND_STREAM    ABILITY_SERENE_GRACE
ABILITY_SHADOW_TAG
```

### Tier 3 — B (22)

```
ABILITY_BATTLE_ARMOR   ABILITY_BLAZE          ABILITY_DRAGONS_MAW    ABILITY_EFFECT_SPORE
ABILITY_FLAME_BODY     ABILITY_HUSTLE         ABILITY_IMMUNITY       ABILITY_INNER_FOCUS
ABILITY_LIGHTNING_ROD  ABILITY_MAGNET_PULL    ABILITY_MARVEL_SCALE   ABILITY_NATURAL_CURE
ABILITY_PIXILATE       ABILITY_ROCK_HEAD      ABILITY_ROUGH_SKIN     ABILITY_STATIC
ABILITY_STURDY         ABILITY_SWIFT_SWIM     ABILITY_THICK_FAT      ABILITY_TRACE
ABILITY_VOLT_ABSORB    ABILITY_WATER_ABSORB
```

### Tier 4 — C (26)

```
ABILITY_AIR_LOCK       ABILITY_CLOUD_NINE     ABILITY_CUTE_CHARM     ABILITY_DAMP
ABILITY_EARLY_BIRD     ABILITY_HYPER_CUTTER   ABILITY_INSOMNIA       ABILITY_LIMBER
ABILITY_LIQUID_OOZE    ABILITY_OBLIVIOUS      ABILITY_OVERGROW       ABILITY_OWN_TEMPO
ABILITY_POISON_POINT   ABILITY_PRESSURE       ABILITY_RAIN_DISH      ABILITY_SAND_VEIL
ABILITY_SHED_SKIN      ABILITY_SHELL_ARMOR    ABILITY_SHIELD_DUST    ABILITY_SOUNDPROOF
ABILITY_SWARM          ABILITY_SYNCHRONIZE    ABILITY_TORRENT        ABILITY_TRANSISTOR
ABILITY_VITAL_SPIRIT   ABILITY_WHITE_SMOKE
```

### Tier 5 — D (9)

```
ABILITY_COLOR_CHANGE   ABILITY_FORECAST       ABILITY_KEEN_EYE       ABILITY_MAGMA_ARMOR
ABILITY_MULTITYPE      ABILITY_STENCH         ABILITY_STICKY_HOLD    ABILITY_SUCTION_CUPS
ABILITY_WATER_VEIL
```

### Tier 6 — F / Negative (6)

```
ABILITY_ILLUMINATE     ABILITY_MINUS          ABILITY_PICKUP         ABILITY_PLUS
ABILITY_RUN_AWAY       ABILITY_TRUANT
```

Truant is the tier list's only *Negative* entry present in Modern Emerald; the other five are F. Merged per
your instruction. At 2% across 6 entries, Truant lands ~0.33% of the time.

### Excluded (2)

| Ability | Why |
|---|---|
| `ABILITY_WONDER_GUARD` | **Your decision.** Sits in A on the community list, but it's game-breaking on a random mon. |
| `ABILITY_CACOPHONY` | Not on the community list. Unimplemented dummy in Gen 3 — does nothing. |

### Changes from Revision 2

The community list disagreed with my earlier hand-tiering in several places worth knowing about, since these
shift real gameplay:

- **Transistor → C** (I had it tier 1). The community rates it far lower than I did.
- **Multitype → D**, not excluded. It's tiered here rather than banned; it stays in as a live roll.
  If Arceus-signature behaviour on a random mon causes problems in testing, re-exclude it.
- **Forecast → D**, likewise no longer excluded.
- **Blaze → B** while **Overgrow and Torrent → C**. Asymmetric, but that's the community's call.
- **Immunity, Battle Armor, Effect Spore, Hustle → B**, notably higher than I had them.
- **Compound Eyes, Serene Grace → A**, also higher.


## Appendix B — Move tiers

> **Status: pending re-tiering.** The 3-tier list below is my hand-tiering from Revision 2. You asked to redo
> it the way we redid abilities, against a real tier list. The full editable pool —
> **367 moves with power / accuracy / type / category / PP**, TM and HM moves flagged — is in
> [`tiering/MOVES.md`](tiering/MOVES.md). Once you mark tiers there (or point me at a move tier list image),
> this appendix gets regenerated with counts and the same per-move overweighting check.
>
> **Watch the count asymmetry here especially.** With 367 moves, a small S tier gets an enormous per-move
> boost. If S ends up with ~15 moves at 40%, that's 2.7% each versus a 0.27% uniform baseline — a 10× boost,
> far steeper than the ~2× the ability list produces. Expect to want gentler weights for moves, or a larger
> top tier.

`sRandomValidMoves[]` covers all 367 usable moves. Tier 1 and 2 are listed; everything else is the tail.

### Tier 1 — 45% weight (~55 moves)

**Physical attackers**
```
MOVE_EARTHQUAKE      MOVE_ROCK_SLIDE      MOVE_BODY_SLAM       MOVE_DOUBLE_EDGE
MOVE_BRICK_BREAK     MOVE_SHADOW_BALL     MOVE_CRUNCH          MOVE_AERIAL_ACE
MOVE_DRAGON_CLAW     MOVE_IRON_TAIL       MOVE_EXTREME_SPEED   MOVE_HYDRO_PUMP
MOVE_SHADOW_CLAW     MOVE_PSYCHO_CUT      MOVE_POISON_JAB      MOVE_PLAY_ROUGH
MOVE_SKY_UPPERCUT    MOVE_MEGAHORN        MOVE_CROSS_CHOP      MOVE_ROCK_TOMB
```

**Special attackers**
```
MOVE_ICE_BEAM        MOVE_THUNDERBOLT     MOVE_FLAMETHROWER    MOVE_SURF
MOVE_PSYCHIC         MOVE_GIGA_DRAIN      MOVE_SLUDGE_BOMB     MOVE_FIRE_BLAST
MOVE_THUNDER         MOVE_BLIZZARD        MOVE_DARK_PULSE      MOVE_FOCUS_BLAST
MOVE_POWER_GEM       MOVE_FLASH_CANNON    MOVE_AIR_SLASH       MOVE_BUG_BUZZ
MOVE_DRAGON_PULSE    MOVE_EARTH_POWER     MOVE_MOONBLAST       MOVE_SOLAR_BEAM
```

**All 56 tier-1 moves above are verified to exist in this ROM's `include/constants/moves.h`.** Note there is
no Energy Ball in this build, so Solar Beam is the Grass special option.

**Setup / status / utility**
```
MOVE_SWORDS_DANCE    MOVE_CALM_MIND       MOVE_DRAGON_DANCE    MOVE_PROTECT
MOVE_SUBSTITUTE      MOVE_RECOVER         MOVE_REST            MOVE_THUNDER_WAVE
MOVE_TOXIC           MOVE_WILL_O_WISP     MOVE_LEECH_SEED      MOVE_SPIKES
MOVE_BATON_PASS      MOVE_TAUNT           MOVE_ENCORE          MOVE_SOFT_BOILED
```

Note how many of the 14 modern additions land here — Dark Pulse, Focus Blast, Air Slash, Bug Buzz,
Dragon Pulse, Earth Power, Moonblast, Poison Jab, Shadow Claw, Psycho Cut, Flash Cannon, Power Gem are all
strictly better than their Gen 3 equivalents, so they carry a lot of tier 1's weight.

### Tier 2 — 35% weight (~110 moves)

Defined by rule rather than enumeration, so it stays maintainable:

- **Any damaging move with `power >= 65` and `accuracy >= 85`** not already in tier 1.
- **Multi-hit and priority moves**: Quick Attack, Mach Punch, Bullet Seed, Rock Blast, Fury Cutter, Vital Throw.
- **Secondary-effect attackers**: Ice Punch, Thunder Punch, Fire Punch, Bite, Headbutt, Waterfall, Crabhammer.
- **Support**: Light Screen, Reflect, Haze, Roar, Whirlwind, Sing/Sleep Powder/Hypnosis, Confuse Ray,
  Screech, Charm, Growl, Leer, Sandstorm, Rain Dance, Sunny Day, Hail, Safeguard, Heal Pulse, Moonlight,
  Synthesis, Milk Drink, Aromatherapy, Refresh, Belly Drum, Bulk Up, Agility, Amnesia, Barrier, Acid Armor,
  Curse, Endure, Counter, Mirror Coat, Destiny Bond, Perish Song, Pain Split, Knock Off, Torment,
  Snatch, Trick, Skill Swap, Imprison, Bulk Up, Agility.

  **This build has no Energy Ball, Roost, Aqua Jet, Sucker Punch, or Zen Headbutt** — it's Gen 3 plus 14
  specific additions, not a full modern movepool. Verify any move you add to these tiers against
  `include/constants/moves.h` first; a non-existent constant is a build break, and a *wrong* one is silent.
- **HM moves** — deliberately tier 2 so they stay reachable and progression doesn't soft-lock.

The `power >= 65 && accuracy >= 85` rule can be evaluated at build time or asserted in a comment; it makes
tier 2 self-maintaining if move data changes.

### Tail — 20% (everything else)

Splash, Constrict, Barrier-on-a-physical-mon, one-hit-KO moves, Sketch, Metronome, Mimic, low-power
starters (Tackle, Scratch, Pound), and the long list of Gen-1 filler. This is what preserves randomizer chaos.

### Explicit exclusions

Moves that should **never** be rolled regardless of tier:

- `MOVE_NONE` (0) — obvious, but the modulo must not reach it.
- Consider excluding the OHKO moves (Fissure, Horn Drill, Guillotine, Sheer Cold) — a wild mon with one is
  purely a rage-quit generator. Your call; they're in the tail either way (~0.4% of rolls).

---

## Appendix C — Hold item tiers

> **Status: pending re-tiering.** The full editable pool of **188 items** is in
> [`tiering/ITEMS.md`](tiering/ITEMS.md), **grouped by function** so the tiering is tractable rather than a
> flat 188-line list:
>
> | Group | Count | | Group | Count |
> |---|---:|---|---|---:|
> | Battle hold items | 49 | | Flavour berries | 21 |
> | Berries with hold effects | 22 | | Mail | 12 |
> | Poké Balls | 12 | | Everything else | 19 |
> | Healing / medicine | 27 | | **Battle-only dead items** | **11** |
> | Vitamins & progression | 9 | | Evolution stones | 6 |
>
> Each entry carries its **hold effect** (`HOLD=NONE` = does nothing when held) and price. The worksheet also
> documents a suggested tiering strategy — the short version is that an item's randomizer value depends on
> whether it does anything at all on a *randomly chosen* Pokémon, which demotes the species-locked elites.
>
> **Constraint to preserve when you re-tier:** healing items must stay common enough to play the game. If
> Potions and Revives land in a 2%-weight bottom tier, early-game becomes unwinnable.

Drawn from `sRandomValidItems[]` (`src/item.c:1019`, 188 entries).

### Tier 1 — 45% weight: universally useful hold items

```
ITEM_LEFTOVERS       ITEM_CHOICE_BAND     ITEM_SHELL_BELL      ITEM_FOCUS_BAND
ITEM_BRIGHT_POWDER   ITEM_QUICK_CLAW      ITEM_SCOPE_LENS      ITEM_KINGS_ROCK
ITEM_LUM_BERRY       ITEM_SITRUS_BERRY    ITEM_CHESTO_BERRY    ITEM_WHITE_HERB
ITEM_MENTAL_HERB     ITEM_SALAC_BERRY     ITEM_LIECHI_BERRY    ITEM_PETAYA_BERRY
```

Every one works on **any** species — that's the tier 1 criterion, not raw VGC power.

### Tier 2 — 35% weight: type-boosters, healing, situational

**Type-boosting hold items** (good on a matching-type mon, which the Phase 6 STAB guarantee makes likely):
```
ITEM_SILVER_POWDER   ITEM_SOFT_SAND       ITEM_HARD_STONE      ITEM_MIRACLE_SEED
ITEM_BLACK_GLASSES   ITEM_BLACK_BELT      ITEM_MAGNET          ITEM_MYSTIC_WATER
ITEM_SHARP_BEAK      ITEM_POISON_BARB     ITEM_NEVER_MELT_ICE  ITEM_SPELL_TAG
ITEM_TWISTED_SPOON   ITEM_CHARCOAL        ITEM_DRAGON_FANG     ITEM_SILK_SCARF
ITEM_SEA_INCENSE     ITEM_LAX_INCENSE     ITEM_FAIRY_GEM
```

**Remaining pinch berries + status berries:**
```
ITEM_APICOT_BERRY    ITEM_GANLON_BERRY    ITEM_STARF_BERRY     ITEM_LANSAT_BERRY
ITEM_CHERI_BERRY     ITEM_PECHA_BERRY     ITEM_RAWST_BERRY     ITEM_ASPEAR_BERRY
ITEM_PERSIM_BERRY    ITEM_LEPPA_BERRY     ITEM_ORAN_BERRY
```

**Healing and revival — deliberately kept here so the game stays playable:**
```
ITEM_FULL_RESTORE    ITEM_MAX_POTION      ITEM_HYPER_POTION    ITEM_SUPER_POTION
ITEM_POTION          ITEM_REVIVE          ITEM_MAX_REVIVE      ITEM_FULL_HEAL
ITEM_ETHER           ITEM_MAX_ETHER       ITEM_ELIXIR          ITEM_MAX_ELIXIR
ITEM_SACRED_ASH      ITEM_MOOMOO_MILK     ITEM_LEMONADE        ITEM_BERRY_JUICE
```

**Progression / vitamins:**
```
ITEM_RARE_CANDY      ITEM_PP_UP           ITEM_PP_MAX          ITEM_HP_UP
ITEM_PROTEIN         ITEM_IRON            ITEM_CARBOS          ITEM_CALCIUM
ITEM_ZINC            ITEM_ULTRA_BALL      ITEM_GREAT_BALL      ITEM_TIMER_BALL
ITEM_NET_BALL        ITEM_DIVE_BALL       ITEM_NEST_BALL       ITEM_REPEAT_BALL
```

### Tier 3 — 20% tail

**Species-locked items — powerful in VGC, dead weight in a randomizer (see Phase 8):**
```
ITEM_SOUL_DEW        ITEM_LIGHT_BALL      ITEM_THICK_CLUB      ITEM_METAL_POWDER
ITEM_LUCKY_PUNCH     ITEM_STICK           ITEM_DEEP_SEA_TOOTH  ITEM_DEEP_SEA_SCALE
```

**Battle items you said you won't use:**
```
ITEM_X_ATTACK        ITEM_X_DEFEND        ITEM_X_SPEED         ITEM_X_ACCURACY
ITEM_X_SPECIAL       ITEM_DIRE_HIT        ITEM_GUARD_SPEC
```

**Everything else:** mail (12 kinds), shards, flutes, Shoal Salt/Shell, Fertilizer, the flavour berries
(Figy through Belue), Poké Doll, Fluffy Tail, repels, Escape Rope, evolution stones, Everstone, Cleanse Tag,
Smoke Ball, Amulet Coin, Lucky Egg, Soothe Bell, Macho Brace, Exp. Share, Up-Grade, Dragon Scale,
Metal Coat, contest scarves, Master Ball, Safari Ball, Premier Ball, Luxury Ball, Poké Ball.

**Master Ball note:** it's in the tail, so it's a rare but real drop. That's arguably a *good* randomizer
moment. Move it to the exclusion list if you'd rather it stay unique.

---

## Appendix D — Weighting methodology

So you can re-tier anything without reverse-engineering the reasoning.

### Always divide the weight by the tier's count

**This is the most important rule, and it's the one that bit us.** A tier's weight is shared across every
member, so the number that matters to a player is not the tier weight — it's:

```
per-entry probability = tier weight / number of entries in that tier
```

Tier weights alone tell you nothing. In the ability list, 20% across C's 26 entries gave 0.769% each, while
8% across D's 9 entries gave 0.889% — so **D outranked C per ability** even though C is the better tier. The
weights looked monotonic; the actual odds weren't.

**Whenever you re-tier anything, recompute the per-entry column and check it decreases at every step.** If it
doesn't, shift a few points between the two offending tiers — that's all the fix ever needs to be.

Useful reference point: uniform probability is `100 / pool size`. Divide each per-entry figure by that to get
a "× uniform" multiplier, which is the honest measure of how much a tier is actually being favoured.

### Count asymmetry sets how steep you can go

Pool size changes what a given weight *means*:

| Pool | Size | Uniform | A 40% top tier of ~15 gives | Multiplier |
|---|---:|---:|---:|---:|
| Abilities | 79 | 1.27% | 2.67% each | ~2× |
| TMs | 50 | 2.00% | 2.67% each | ~1.3× |
| Moves | 367 | 0.27% | 2.67% each | **~10×** |

Same weight, wildly different effect. The ability weights are gentle because the pool is small; applying them
unchanged to 367 moves would make the top tier ten times overrepresented and flatten the variety you wanted.
**Expect moves to need either a larger top tier or gentler weights.**

### The tier criterion is "value on a random recipient", not "value in competitive play"

A real tier list ranks options **on the specific Pokémon that runs them**. A randomizer assigns them to an
**arbitrary** Pokémon. Those give different answers:

| Thing | Competitive rank | Randomizer rank | Why they differ |
|---|---|---|---|
| Soul Dew | S | Low | Only works on Latias/Latios |
| Thick Club | S | Low | Only works on Cubone/Marowak |
| Huge Power | S | S | Works on anything with an Attack stat |
| Leftovers | S | S | Works on anything |

**An option belongs in the top tier only if it's strong on a mon chosen at random.** Species-locked power
drops regardless of pedigree.

This is also where the community ability list and a randomizer diverge: the list rates Multitype (Arceus) and
Forecast (Castform) as D — reasonable competitively, but on a random mon they're closer to inert. They're
tiered as-is per your instruction; revisit if testing shows they misbehave off-species.

### The six-tier scale

Used for abilities, and the recommended scale for the remaining appendices:

| Tier | Grade | Weight | Meaning |
|---|---|---:|---|
| 1 | S | 40% (with A) | Defines a Pokémon. Strong on any recipient. |
| 2 | A | — | Excellent, near-universal. |
| 3 | B | 30% | Solid; strong given a common precondition. |
| 4 | C | 22% | Situational but real. |
| 5 | D | 6% | Weak; rarely matters. |
| 6 | F / Negative | 2% | Useless or actively harmful. |

S and A share one 40% bucket unless you split them (see Appendix A). The 2% floor is deliberate — it keeps
runs distinct rather than converging on the same handful of options.

### Strict mode

`Strict` bypasses the roll and draws only from tier 1. Expect heavy repetition — 7 S-tier abilities means a
6-mon party will almost certainly duplicate. Consider having Strict draw from S+A (16) instead of S alone.

### Excluding vs. demoting

**Demote** when something is merely weak. **Exclude** when it is:
- unimplemented (`ABILITY_CACOPHONY`),
- actively run-ruining (`ABILITY_WONDER_GUARD`, per your call),
- or progression-critical and must never be touched (**HM moves** — the highest-stakes case, since a
  randomized HM can soft-lock a save).

Exclusions live in a separate list from the tiers, so they never appear at any weight. Demoted entries still
appear, just rarely.

---

## Appendix E — TM tiers

> **Status: pending re-tiering**, same as B and C. Worksheet: [`tiering/TM_MOVES.md`](tiering/TM_MOVES.md) —
> all **50 TMs** with the move each teaches plus power/accuracy/type/category, and the **8 HMs** listed
> separately under a do-not-randomize warning.

TMs are a much smaller pool than moves (50 vs 367), so the per-entry maths is far friendlier: a uniform TM
roll is already 2% each, and even a 10-entry S tier at 40% only reaches 4% each — a 2× boost, in line with
the ability list. **TM tiering is the easiest of the three to get right.**

Tier the 50 TMs by the move they teach. The same tier assignment serves both halves of Phase 8c:

- **(a) which TM you find** — weight the item-ball roll toward high-tier TMs.
- **(b) which move a TM teaches** — weight the move assigned to each TM slot.

**The 8 HMs (Cut, Fly, Surf, Strength, Flash, Rock Smash, Waterfall, Dive) are never randomized and never
tiered.** They gate progression; randomizing one can soft-lock the playthrough. The existing guard at
`src/item.c:1222-1229` already excludes them from TM item randomization — preserve it, and add the equivalent
`tmNumber < NUM_TECHNICAL_MACHINES` guard on the (b) path.

---

## Appendix F — Other ideas worth taking from the Tertu randomizer

You asked what else Tertu does that's worth adopting. I read `src/randomizer.c`, `include/randomizer.h`, and
`include/config/randomizer.h` on the `tertu-randomizer` branch. Here's what's actually implemented, ranked by
what I'd take.

### Worth taking

**1. BST-based similarity mode — the strongest idea there.**
Tertu's `MON_RANDOM_BST` mode matches species by **base stat total** rather than evolution stage, using a
±10.24% band around the original's BST (`GetGroupRange`, with a binary search over a BST-sorted table).

Modern Emerald only has evolution-stage matching (`tx_Random_Similar`, via `sRandomSpeciesEvo0/1/2`). Stage is
a crude proxy for power — the `EVO_TYPE_0` bucket spans **BST 180 (Sunkern) to 535 (Lapras)**, a 3× spread
inside the option that exists to keep things fair. BST matching is far more even, and it composes cleanly
with the Phase 3 level clamp (BST picks *how strong*, the clamp picks *how evolved*).

Worked through with real numbers in [Appendix G](#appendix-g--tertus-dynamic-species-tables-explained);
implementation in [Phase 3b](#phase-3b--bst-similarity-mode-new-req-14).

*(Correction: an earlier revision claimed single-stage mons are `EVO_TYPE_SELF` and never randomize. That was
wrong — Tauros, Lapras and Skarmory are all `EVO_TYPE_0` and do randomize. Only 27 species are
`EVO_TYPE_SELF`. The spread problem above is the real and only argument for BST mode.)*

**2. Per-reason independent random streams.**
Tertu seeds each randomization category separately via `enum RandomizerReason` (`RANDOMIZER_REASON_WILD_ENCOUNTER`,
`_TRAINER_PARTY`, `_ABILITIES`, `_FIELD_ITEM`, …), folded into the seed at `src/randomizer.c:182`.

This directly fixes the hazard I flagged in Phases 5-8: Modern Emerald uses ad-hoc additive offsets
(`species + move`, `+ 0x3001`) that can correlate and produce banding — all four moves from one tier, or a
mon's ability tracking its species roll. Adopting a reason enum is cheap and makes every future weighted roll
independent by construction. **If you only take one thing from Tertu, take this** — it's small and it prevents
a whole class of subtle bug.

**3. Trainer-ID-derived seed.**
`RANDOMIZER_SEED_IS_TRAINER_ID` makes the seed the player's trainer ID, so each new save randomizes
differently while staying reproducible within a save. Worth checking what Modern Emerald keys off today
(`typeRandomizerSeed` exists in `SaveBlock1` at `include/global.h:1131`) — if the mapping is fixed across
saves, every playthrough gets identical randomization, which gets stale fast.

**4. Egg randomization** (`RANDOMIZE_EGG_MON`). Modern Emerald randomizes eggs only incidentally via
`src/egg_hatch.c:323`. Tertu treats it as a first-class feature with its own table. Small, self-contained.

### Interesting but lower priority

**5. `IsRandomizationPossible(tableSpecies, matchSpecies)`** — answers "could X have become Y?" Useful for
dex hints or an in-game "what did this become" helper. Nice-to-have.

**6. Dynamic EWRAM species tables** (`RANDOMIZER_DYNAMIC_SPECIES`) — see the detailed breakdown in
[Appendix G](#appendix-g--tertus-dynamic-species-tables-explained). Short version: **the implementation is
blocked by EWRAM, but the one feature it uniquely enables (BST mode) is not** — it can be done with a
ROM-resident sorted table instead.

### Not worth taking

**7. Their ability whitelist is weaker than ours.** `src/data/randomizer/ability_whitelist.h` is a **flat
include/exclude list with no tiers and no weights** — and it *includes* Wonder Guard. Appendix A is strictly
more sophisticated. Nothing to port.

**8. Learnset / base stat / type randomization: not implemented.** `RANDOMIZE_LEARNSET`,
`RANDOMIZE_BASE_STATS`, and `RANDOMIZE_MON_TYPES` are enum entries with *"Not yet implemented"* comments and
no code. (Modern Emerald already has type randomization via `tx_Random_Type`, so it's ahead there.)

### Architectural note
Tertu is built on **pokeemerald-expansion**, a substantially different base — `gSpeciesInfo`/`gMovesInfo` vs
this repo's `gBattleMoves`/`gMoveNames`, a different RNG (`Sfc32State` streams), different party plumbing.
Nothing here is copy-pasteable. Treat these as **design ideas to reimplement**, not code to port.


---

## Appendix G — Tertu's dynamic species tables, explained

### What they physically are

Three parallel `u16` arrays, one entry per species, built in EWRAM at runtime
(`struct SpeciesTable`, `src/randomizer.c:330`):

```c
struct SpeciesTable
{
    u16 groupData[RANDOMIZER_SPECIES_COUNT];            // each species' "group" value
    u16 speciesToGroupIndex[RANDOMIZER_SPECIES_COUNT];  // species id  -> sorted index
    u16 groupIndexToSpecies[RANDOMIZER_SPECIES_COUNT];  // sorted index -> species id
};
```

3 × 2 bytes × 462 species = **~2.7 KB of EWRAM**, which is what the config means by *"consumes 6 bytes for
each species present."*

### What they actually accomplish

They are a **sorted index that makes range queries cheap.** The table is heap-sorted by group
(`BuildRandomizerSpeciesTable`, `:560`), so the randomizer can answer *"give me a random species whose group
falls in [min, max]"* with a **binary search** (`GetIndicesFromGroupRange`, `:384`) instead of scanning all
462 species.

"Group" means something different per mode — that's the clever part, one table structure serving four
behaviours:

| Mode | Group value | Equivalent in Modern Emerald |
|---|---|---|
| `MON_RANDOM` | all one group | `sRandomSpecies[]` |
| `MON_RANDOM_LEGEND_AWARE` | legendary vs not | `sRandomSpeciesLegendary[]` |
| `MON_EVOLUTION` | evolution stage | `gSpeciesMapping[]` + `sRandomSpeciesEvo0/1/2` |
| **`MON_RANDOM_BST`** | **the species' base stat total** | **nothing** |

### So what's genuinely missing without them

**Exactly one thing: BST-similarity matching.**

Three of the four modes already have hardcoded `const` equivalents in this ROM, and hardcoded is *better*
here — it costs ROM (10 MB spare) instead of EWRAM (988 bytes spare), and needs no build step at boot.

`MON_RANDOM_BST` is the exception. Modern Emerald cannot express it, and here is exactly why that matters.

### Why Tertu built this — a worked example

Modern Emerald's only "balance" option is `tx_Random_Similar`, which matches by **evolution stage**. It sorts
every species into one of five buckets (`gSpeciesMapping[]`) and swaps within the bucket:

| Bucket | Species | BST range | Spread |
|---|---:|---|---:|
| `EVO_TYPE_0` | 179 | Sunkern **180** → Lapras **535** | **355** |
| `EVO_TYPE_1` | 156 | — | — |
| `EVO_TYPE_2` | 60 | Butterfree **395** → Slaking **670** | **275** |
| `EVO_TYPE_LEGENDARY` | 28 | — | — |
| `EVO_TYPE_SELF` | 27 | never randomized | — |

**A bucket labelled "balanced" spans a 3× power difference.** Turn on *Similar Evolution Level* — the option
whose entire purpose is fairness — and Sunkern is in the same swap pool as Lapras:

```
SUNKERN  (BST 180, EVO_TYPE_0)
  Similar pool : 179 species — strongest are Lapras(535), Aerodactyl(515), Tauros(510), Shuckle(505)
  BST pool     :   6 species — strongest are Ralts(198), Caterpie(195), Weedle(195), Azurill(190)

MAGIKARP (BST 200, EVO_TYPE_0)
  Similar pool : 179 species — same Lapras/Aerodactyl/Tauros at the top
  BST pool     :  22 species — strongest are Poochyena(220), Lotad(220), Seedot(220), Happiny(220)
```

Stage-0 means "hasn't evolved yet", which says nothing about power. Lapras and Aerodactyl never evolve, so
they're stage 0 — sitting in the same pool as Caterpie. On Route 101 you either get a Sunkern or a Lapras,
and *Similar Evolution Level* is what put them both on the table.

**BST matching is the fix.** Group by base stat total, swap within ±10.24%, and Sunkern's 6 candidates are
all genuinely Sunkern-tier. That's the whole feature.

### Why it needs a sorted table

This is the part that explains the machinery, and it comes down to one distinction:

- **Evolution stage is an exact-match query.** "Give me a stage-0 species." You can precompute one flat list
  per bucket and index it — which is precisely what `sRandomSpeciesEvo0/1/2[]` are.
- **BST is a *range* query.** "Give me a species with BST between 269 and 331." There is no fixed bucket to
  precompute, because every species has a different band.

Answering a range query naively means scanning all 462 species, collecting matches into a buffer, and picking
one — every single call. The randomizer is called constantly: every wild encounter, every trainer party slot,
every ability lookup. That cost is not acceptable per-call on a 16 MHz CPU.

Sort the species by BST once, and the same query becomes **two binary searches** (~9 comparisons each) for the
band's start and end, then a random index between them. No scan, no buffer. That is all
`GetIndicesFromGroupRange` (`src/randomizer.c:384`) does, and it is the entire reason the table exists.

So: **Tertu built the table because BST mode needs a sorted index, and the other three modes are along for the
ride** — once you have the machinery, expressing "same evolution stage" as a degenerate range where min == max
is free.

### Why he built it in RAM, and why you shouldn't

This is a difference in constraints, not a difference in judgement.

Tertu's branch targets **pokeemerald-expansion**, where the species list is open-ended — people add Pokémon
routinely, and the project supports 1000+. A hardcoded sorted table would silently go stale the moment
someone inserted a species. Building it at boot means it is always correct, for any fork, with no build step.
That is the right call *for a widely-forked base*.

**Your species list is fixed at 462 and you are not adding Pokémon.** So you can sort once at build time and
ship the result in ROM:

```c
static const u16 sSpeciesByBST[NUM_SPECIES] = { /* generated, ascending by BST */ };
```

**924 bytes of ROM, zero EWRAM** — versus 2.7 KB of EWRAM you do not have. Same binary search, same feature,
none of the cost. See [Phase 3b](#phase-3b--bst-similarity-mode-new-req-14).

### The other thing they enable, which you do not need

Switching randomization mode mid-game and rebuilding the tables on the fly (`PreloadRandomizationTables`).
Modern Emerald settles its options at new-game time and keeps them in the save, so there is nothing to
rebuild. No loss.
