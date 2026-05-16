# SQL Custom Extensions

A hands-on curriculum for building PostgreSQL extensions—from your first C function through planner hooks, background workers, and reading production codebases.

| Path | Description |
|------|-------------|
| [`1_hello_word/`](1_hello_word/) | Minimal C extension example |
| [`pecip/`](pecip/) | PostgreSQL Extension Compatibility Intelligence Platform |

---

## Table of contents

- [Phase 1 — Basic Extension Infrastructure](#phase-1--basic-extension-infrastructure)
- [Phase 2 — PostgreSQL Internals Fundamentals](#phase-2--postgresql-internals-fundamentals)
- [Phase 3 — Custom Data Types](#phase-3--custom-data-types)
- [Phase 4 — PostgreSQL Planner and Executor](#phase-4--postgresql-planner-and-executor)
- [Phase 5 — Background Workers](#phase-5--background-workers)
- [Phase 6 — Advanced PostgreSQL Internals](#phase-6--advanced-postgresql-internals)
- [Phase 7 — Real Production Extensions](#phase-7--real-production-extensions)
- [Phase 8 — Debugging and Source Navigation](#phase-8--debugging-and-source-navigation)

---

## Phase 1 — Basic Extension Infrastructure

**Goal:** Understand how PostgreSQL loads extensions.

### Tutorial 1 — Build Your First Function Extension

**You learn**

- PGXS
- Shared libraries
- SQL install scripts
- `Datum`
- `PG_FUNCTION_ARGS`

**Build**

```sql
SELECT hello();
```

**Concepts**

- `PG_MODULE_MAGIC`
- `PG_FUNCTION_INFO_V1`
- `PG_RETURN_TEXT_P`

**Read**

- `contrib/intarray`
- `contrib/citext`

**Outcome**

You understand:

- How PostgreSQL loads native code
- How SQL maps to C

---

### Tutorial 2 — Integer and Boolean Functions

**Build**

```sql
SELECT add_one(5);
SELECT is_even(10);
```

**Learn**

Argument extraction macros:

- `PG_GETARG_INT32(0)`
- `PG_RETURN_BOOL(...)`

**Concepts**

- `Datum`
- Type conversion
- Pass-by-value types

**Important**

Learn why PostgreSQL uses `Datum`.

---

### Tutorial 3 — Text Functions

**Build**

```sql
SELECT reverse_text('hello');
```

**Learn**

- PostgreSQL `TEXT` internals
- `varlena`
- Memory allocation

**APIs**

- `text_to_cstring()`
- `cstring_to_text()`
- `palloc()`

**Concepts**

- PostgreSQL memory contexts
- Why `malloc()` is avoided

---

### Tutorial 4 — Arrays

**Build**

```sql
SELECT sum_array(ARRAY[1,2,3]);
```

**Learn**

- PostgreSQL arrays
- Deconstructing arrays
- Null bitmap handling

**APIs**

- `deconstruct_array()`
- `construct_array()`

**Important**

This teaches PostgreSQL internal binary structures.

---

### Tutorial 5 — Error Handling and Logging

**Build**

Functions that intentionally fail.

**Learn**

- `elog(INFO, ...)`
- `ereport(ERROR, ...)`
- `errmsg()`
- `errdetail()`

**Concepts**

- PostgreSQL error system
- Transaction abort behavior

---

## Phase 2 — PostgreSQL Internals Fundamentals

**Goal:** Understand memory and the execution model.

### Tutorial 6 — Memory Contexts

**Build**

A function allocating large temporary structures.

**Learn**

- `palloc`
- `pfree`
- Memory contexts

**Concepts**

PostgreSQL memory hierarchy:

- `TopMemoryContext`
- `CurrentMemoryContext`
- Transaction contexts

**Read source**

- `src/backend/utils/mmgr`

---

### Tutorial 7 — TOAST and Large Objects

**Build**

Functions manipulating huge `TEXT` values.

**Learn**

- TOAST
- External storage
- Detoasting

**APIs**

- `PG_DETOAST_DATUM()`
- `VARSIZE()`

**Concepts**

How PostgreSQL handles huge values efficiently.

---

### Tutorial 8 — SPI (Server Programming Interface)

**Build**

A C function that internally executes SQL:

```sql
SELECT count_users();
```

**Learn**

- `SPI_connect()`
- `SPI_execute()`
- `SPI_finish()`

**Concepts**

- Executing SQL inside extensions
- Internal executor usage

**Docs**

- [SPI Documentation](https://www.postgresql.org/docs/current/spi.html)

---

## Phase 3 — Custom Data Types

**Goal:** Build actual database features.

### Tutorial 9 — Create a Custom Type

**Build**

A `complex_number` type.

**Example**

```sql
SELECT '(1,2)'::complex_number;
```

**Learn**

- Type input/output functions
- Internal representation

**Functions**

- `complex_in()`
- `complex_out()`

**Concepts**

- `varlena` layout
- Serialization

**Docs**

- [User-defined types](https://www.postgresql.org/docs/current/xtypes.html)

---

### Tutorial 10 — Operators

**Build**

```sql
SELECT a + b;
```

…for your custom type.

**Learn**

- Operators
- Operator definitions
- Commutators
- Selectivity

**Concepts**

How the PostgreSQL planner understands operators.

---

### Tutorial 11 — Operator Classes and Indexing

**Build**

Index support for your custom type.

**Learn**

- B-tree operator classes
- GIN/GiST basics

**Concepts**

How indexes integrate with the planner.

**Read**

- `contrib/btree_gin`
- `contrib/pg_trgm`

---

## Phase 4 — PostgreSQL Planner and Executor

**Goal:** Understand query execution internals.

### Tutorial 12 — Query Execution Flow

**Study**

Execution path:

```
SQL → parser → rewrite → planner → executor
```

**Learn**

Key structs:

- `Query`
- `PlannedStmt`
- `Plan`
- `Expr`

**Read source**

- `src/backend/executor`
- `src/backend/optimizer`

---

### Tutorial 13 — Planner Hooks

**Build**

A hook logging all planned queries.

**Learn**

- `planner_hook`
- `standard_planner`

**Concepts**

- Planner interception
- Query transformation

**Docs**

- [Planner hooks example discussion](https://www.postgresql.org/docs/current/planner-optimizer.html)

---

### Tutorial 14 — Executor Hooks

**Build**

A query timing extension.

**Learn**

- `ExecutorStart_hook`
- `ExecutorRun_hook`
- `ExecutorFinish_hook`

**Concepts**

How query execution stages work.

---

### Tutorial 15 — ProcessUtility Hook

**Build**

A DDL audit extension. Track:

- `CREATE TABLE`
- `ALTER TABLE`
- `DROP TABLE`

**Learn**

- `ProcessUtility_hook`

**Concepts**

Intercepting utility commands.

---

## Phase 5 — Background Workers

**Goal:** Build PostgreSQL internal daemons.

### Tutorial 16 — Basic Background Worker

**Build**

A worker logging heartbeat messages.

**Learn**

- `BackgroundWorker`
- `RegisterBackgroundWorker()`

**Concepts**

- PostgreSQL processes
- Shared memory
- Signals

**Docs**

- [Background workers](https://www.postgresql.org/docs/current/bgworker.html)

---

### Tutorial 17 — Shared Memory

**Build**

Shared metrics storage.

**Learn**

- `ShmemInitStruct()`
- `LWLock`

**Concepts**

- Inter-process communication
- Lightweight locks

---

### Tutorial 18 — Custom Cache

**Build**

An in-memory cache extension.

**Learn**

- Shared hash tables
- Invalidation
- Synchronization

**Read**

- PostgreSQL buffer manager source

---

## Phase 6 — Advanced PostgreSQL Internals

**Goal:** Become capable of serious systems work.

### Tutorial 19 — WAL and Storage

**Learn**

- WAL records
- Pages
- Buffers
- Checkpoints

**Read source**

- `src/backend/access/transam`
- `src/backend/storage`

**Build**

Simple WAL logging extension.

---

### Tutorial 20 — Custom Index Access Method

**Build**

A toy index.

**Learn**

- `amhandler`
- `IndexAmRoutine`

**Concepts**

How PostgreSQL indexes actually work.

---

### Tutorial 21 — Custom Scan Nodes

**Build**

A custom executor node.

**Learn**

- Extensible planner
- Custom scans

**Concepts**

How extensions add execution strategies.

---

### Tutorial 22 — Logical Decoding

**Build**

A mini CDC extension.

**Learn**

- Replication slots
- WAL decoding

**Concepts**

How streaming systems integrate with PostgreSQL.

---

## Phase 7 — Real Production Extensions

**Goal:** Understand architecture from real codebases.

### Tutorial 23 — Read Small Extensions

**Study**

- `pg_trgm`
- `hstore`
- `cube`

**Focus on**

- SQL glue
- Operators
- Memory handling

---

### Tutorial 24 — Read Medium Extensions

**Study**

- `pgvector`
- `pg_hint_plan`

**Focus on**

- Planner integration
- Indexing

---

### Tutorial 25 — Read Large Extensions

**Study**

- TimescaleDB
- Citus

**Focus on**

- Distributed planning
- Chunking
- Executor changes

---

## Phase 8 — Debugging and Source Navigation

**Goal:** Become comfortable inside PostgreSQL source.

### Tutorial 26 — Debug PostgreSQL with gdb/lldb

**Learn**

- Breakpoints
- Stack traces
- Inspecting structs

**Example**

```bash
gdb --args postgres -D data
```

---

### Tutorial 27 — Build PostgreSQL from Source

**Learn**

```bash
./configure --enable-debug
make -j8
```

**Concepts**

- Assertions
- Debug builds

---

### Tutorial 28 — Trace a Query End-to-End

**Goal**

Follow:

```sql
SELECT * FROM users WHERE id = 1;
```

…through:

1. Parser
2. Planner
3. Executor
4. Heap access

> **Note:** This is one of the most important exercises in the curriculum.
