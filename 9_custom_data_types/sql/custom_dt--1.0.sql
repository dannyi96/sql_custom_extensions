/*
 * complex_number — Tutorial 9 user-defined type.
 *
 * Custom base types have a chicken-and-egg problem: the I/O functions
 * reference the type, and the type's CREATE TYPE references the I/O
 * functions. The canonical resolution is:
 *
 *   1. Declare a SHELL type (placeholder).
 *   2. Declare the I/O functions against the shell.
 *   3. Complete the type with the full CREATE TYPE.
 */

/* 1. Shell type. */
CREATE TYPE complex_number;

/* 2. I/O functions. */
CREATE FUNCTION complex_in(cstring)
RETURNS complex_number
AS 'MODULE_PATHNAME', 'complex_in'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_out(complex_number)
RETURNS cstring
AS 'MODULE_PATHNAME', 'complex_out'
LANGUAGE C IMMUTABLE STRICT;

/* 3. Complete the type.
 *    INTERNALLENGTH = sizeof(Complex)  = 2 * sizeof(double) = 16 bytes.
 *    ALIGNMENT      = double           (8-byte alignment for the leading double).
 *    Not PASSEDBYVALUE — 16 bytes don't fit in a 8-byte Datum, so it's
 *    stored by reference (PG palloc's and copies as needed).
 */
CREATE TYPE complex_number (
    INTERNALLENGTH = 16,
    INPUT          = complex_in,
    OUTPUT         = complex_out,
    ALIGNMENT      = double
);
