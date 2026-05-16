EXTENSION = hello_world
MODULES = hello_world

DATA = sql/hello_world--1.0.sql

PG_CONFIG = pg_config

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# Homebrew pg_config points at MacOSX26.sdk, which may be missing from CLT.
PG_SYSROOT := $(shell xcrun --show-sdk-path)
override CPPFLAGS := $(subst -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX26.sdk,-isysroot $(PG_SYSROOT),$(CPPFLAGS))