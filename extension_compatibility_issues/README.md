# Phase 10 — Extension Compatibility Across PostgreSQL Versions

**Goal:** Understand why PostgreSQL extensions break between versions and how serious extensions maintain compatibility.

Part of the [SQL Custom Extensions](../README.md) curriculum. Tutorial 36 connects to [`pecip/`](../pecip/) (PostgreSQL Extension Compatibility Intelligence Platform).

---

## Table of contents

- [Tutorial 29 — Why Extensions Break Across Versions](#tutorial-29--why-extensions-break-across-versions)
- [Tutorial 30 — Simple Compatibility Break Example](#tutorial-30--simple-compatibility-break-example)
- [Tutorial 31 — Header and Struct Breakages](#tutorial-31--header-and-struct-breakages)
- [Tutorial 32 — SQL-Level Upgrade Scripts](#tutorial-32--sql-level-upgrade-scripts)
- [Tutorial 33 — ABI vs API Breakage](#tutorial-33--abi-vs-api-breakage)
- [Tutorial 34 — Extension Upgrade Testing Matrix](#tutorial-34--extension-upgrade-testing-matrix)
- [Tutorial 35 — Real Examples from Popular Extensions](#tutorial-35--real-examples-from-popular-extensions)
- [Tutorial 36 — Detecting Breaking Changes Automatically](#tutorial-36--detecting-breaking-changes-automatically)
- [Tutorial 37 — Common Compatibility Patterns](#tutorial-37--common-compatibility-patterns)
- [Tutorial 38 — Reading PostgreSQL Release Notes](#tutorial-38--reading-postgresql-release-notes-for-extension-authors)
- [Tutorial 39 — Multi-Version Extension Skeleton](#tutorial-39--building-a-multi-version-extension-skeleton)
- [Tutorial 40 — Surviving Planner Changes](#tutorial-40--advanced-surviving-planner-changes)

---

## Tutorial 29 — Why Extensions Break Across Versions

### Core idea

PostgreSQL **guarantees** SQL compatibility reasonably well.

PostgreSQL **does not** guarantee:

- Internal C APIs
- Planner structs
- Executor internals
- Hook signatures
- ABI stability

Extensions that depend on internals can break on upgrade.

### Types of breakages

| Breakage type | Example |
|---------------|---------|
| Struct field changes | `PlannerInfo` modified |
| Function signature changes | Additional args added |
| Header movement | Include path changes |
| Macro changes | Renamed APIs |
| Catalog changes | Internal catalog layout changes |
| Hook behavior changes | Planner lifecycle changes |
| Memory/context behavior | Assumptions invalidated |
| Build system changes | PGXS behavior differences |

---

## Tutorial 30 — Simple Compatibility Break Example

Suppose PostgreSQL 15 has:

```c
void my_func(int x);
```

PostgreSQL 16 changes to:

```c
void my_func(int x, bool flag);
```

Your extension breaks at compile time:

```text
too few arguments to function call
```

### Real PostgreSQL-style example

Imagine executor hook signature changes.

**Old:**

```c
ExecutorRun(queryDesc, direction, count);
```

**New:**

```c
ExecutorRun(queryDesc, direction, count, execute_once);
```

Extensions compiled against old assumptions fail.

### Fix using version macros

PostgreSQL exposes `PG_VERSION_NUM`.

**Example:**

```c
#if PG_VERSION_NUM >= 160000
    ExecutorRun(queryDesc, direction, count, true);
#else
    ExecutorRun(queryDesc, direction, count);
#endif
```

This pattern is extremely common in production extensions.

---

## Tutorial 31 — Header and Struct Breakages

### Problem

Internal structs often change, for example:

- `PlannerInfo`
- `RelOptInfo`
- `Path`
- `Query`

Fields may be added or removed between releases.

**PostgreSQL 15:**

```c
rel->rows
```

**PostgreSQL 16:**

```c
rel->tuples
```

Extension breaks with:

```text
no member named 'rows'
```

### Fix strategy

#### Option 1 — Version macros

```c
#if PG_VERSION_NUM >= 160000
    rel->tuples
#else
    rel->rows
#endif
```

#### Option 2 — Compatibility layer (preferred)

Create `compat.h`:

```c
#if PG_VERSION_NUM >= 160000
#define REL_ROWS(rel) ((rel)->tuples)
#else
#define REL_ROWS(rel) ((rel)->rows)
#endif
```

Then use:

```c
double rows = REL_ROWS(rel);
```

This keeps extension code maintainable across versions.

---

## Tutorial 32 — SQL-Level Upgrade Scripts

Extensions also break at the SQL/catalog level.

### Problem example

**Version 1.0:**

```sql
CREATE FUNCTION hello();
```

**Version 2.0** changes signature:

```sql
CREATE FUNCTION hello(text);
```

Existing installations fail without a migration path.

### Solution — upgrade scripts

Add:

```text
sql/myext--1.0--2.0.sql
```

**Example:**

```sql
DROP FUNCTION hello();

CREATE FUNCTION hello(text)
RETURNS text
AS 'MODULE_PATHNAME', 'hello'
LANGUAGE C;
```

Users apply the upgrade with:

```sql
ALTER EXTENSION myext UPDATE;
```

**Docs:** [Extension updates](https://www.postgresql.org/docs/current/extend-extensions.html#EXTEND-EXTENSIONS-UPDATES)

---

## Tutorial 33 — ABI vs API Breakage

This distinction is critical for extension authors.

### API break

Your code **fails to compile**.

Examples:

- Renamed function
- Changed signature

**Easy to detect** at build time.

### ABI break

Code **compiles** but **crashes at runtime**. Much worse.

**Example — before:**

```c
struct Foo {
    int x;
};
```

**After:**

```c
struct Foo {
    long x;
    void *ptr;
};
```

Binary layout changes. An old compiled extension may:

1. Load successfully
2. Read wrong memory
3. Crash PostgreSQL

### Important rule

**Recompile extensions for every PostgreSQL major version.** Production systems do this routinely.

---

## Tutorial 34 — Extension Upgrade Testing Matrix

Professional extensions test against many PostgreSQL versions.

### Example matrix

| PostgreSQL |
|------------|
| 13 |
| 14 |
| 15 |
| 16 |
| 17 |

### Use Docker

```bash
docker run postgres:15
docker run postgres:16
```

### CI example (GitHub Actions)

```yaml
strategy:
  matrix:
    pg: [13, 14, 15, 16, 17]
```

---

## Tutorial 35 — Real Examples from Popular Extensions

### Example 1 — pgvector

**Repository:** [pgvector on GitHub](https://github.com/pgvector/pgvector)

**Look for:**

- `PG_VERSION_NUM`
- Compatibility macros
- Planner integration changes

### Example 2 — TimescaleDB

**Repository:** [TimescaleDB on GitHub](https://github.com/timescale/timescaledb)

Massive compatibility layer. Study:

- `compat/`
- Version abstraction
- Planner wrappers

This is industrial-grade PostgreSQL compatibility engineering.

### Example 3 — PostGIS

**Repository:** [PostGIS on GitHub](https://github.com/postgis/postgis)

**Study:**

- Upgrade SQL scripts
- Extension migration handling

---

## Tutorial 36 — Detecting Breaking Changes Automatically

This tutorial connects directly to the **PECIP** project ([`pecip/`](../pecip/)).

### Build a compatibility scanner

**Inputs**

- PostgreSQL source trees
- Extension source trees

**Detect — C-level**

- Removed symbols
- Changed signatures
- Struct changes
- Macro changes

**Detect — SQL-level**

- Changed function signatures
- Catalog assumptions
- Deprecated APIs

### Useful techniques

**Parse headers** with:

- Clang AST
- tree-sitter
- libclang

**Detect struct diffs** — compare definitions across versions:

```c
typedef struct PlannerInfo
```

**Build a dependency graph**

```text
extension → PostgreSQL APIs used
```

Then detect incompatible changes automatically when PG internals change.

---

## Tutorial 37 — Common Compatibility Patterns

### Pattern 1 — Version macros

```c
#if PG_VERSION_NUM >= 170000
    /* PG 17+ */
#else
    /* older */
#endif
```

### Pattern 2 — Compatibility wrapper functions

```c
static inline void
my_executor_run(...)
{
#if PG_VERSION_NUM >= 170000
    /* PG 17+ path */
#else
    /* legacy path */
#endif
}
```

### Pattern 3 — Isolate PostgreSQL internals

Instead of spreading internals across the codebase:

```text
planner_compat.c
executor_compat.c
```

This reduces upgrade pain.

### Pattern 4 — Avoid deep internal dependencies

| Safer | Riskier |
|-------|---------|
| SPI | Planner structs |
| Stable hooks | Executor internals |
| SQL interfaces | Storage internals |

---

## Tutorial 38 — Reading PostgreSQL Release Notes for Extension Authors

An underrated skill: reading release notes strategically.

Every PostgreSQL release documents:

- Extension-impacting changes
- Removed APIs
- Behavior changes

**Docs:** [PostgreSQL release notes](https://www.postgresql.org/docs/current/release.html)

---

## Tutorial 39 — Building a Multi-Version Extension Skeleton

Recommended layout:

```text
src/
├── compat/
│   ├── pg13.h
│   ├── pg14.h
│   ├── pg15.h
│   └── compat.h
├── planner/
├── executor/
└── storage/
```

Centralize version differences in `compat/` rather than scattering `#if` blocks through business logic.

---

## Tutorial 40 — Advanced: Surviving Planner Changes

Planner internals are the **most unstable** area of PostgreSQL for extensions.

### Common breakages

- `Path` structs
- Costing APIs
- Join internals
- Custom scan APIs

### Strategies

1. Isolate planner-touching code in dedicated modules
2. Minimize assumptions about struct layout
3. Wrap APIs aggressively behind compat layers

Large extensions (TimescaleDB, Citus, PostGIS) spend significant maintenance effort here.

---

[← Back to main curriculum](../README.md)
