CREATE TYPE complex_number;

/*
 * IO functions
 */
CREATE FUNCTION complex_in(cstring)
RETURNS complex_number
AS 'MODULE_PATHNAME', 'complex_in'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_out(complex_number)
RETURNS cstring
AS 'MODULE_PATHNAME', 'complex_out'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Type
 */
CREATE TYPE complex_number (
    INTERNALLENGTH = 16,
    INPUT = complex_in,
    OUTPUT = complex_out,
    ALIGNMENT = double
);

/*
 * Comparison function
 */
CREATE FUNCTION complex_cmp(
    complex_number,
    complex_number
)
RETURNS int4
AS 'MODULE_PATHNAME', 'complex_cmp'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Operators
 */
CREATE FUNCTION complex_lt(
    complex_number,
    complex_number
)
RETURNS bool
AS 'MODULE_PATHNAME', 'complex_lt'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_le(
    complex_number,
    complex_number
)
RETURNS bool
AS 'MODULE_PATHNAME', 'complex_le'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_eq(
    complex_number,
    complex_number
)
RETURNS bool
AS 'MODULE_PATHNAME', 'complex_eq'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_ge(
    complex_number,
    complex_number
)
RETURNS bool
AS 'MODULE_PATHNAME', 'complex_ge'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_gt(
    complex_number,
    complex_number
)
RETURNS bool
AS 'MODULE_PATHNAME', 'complex_gt'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Operators
 */
CREATE OPERATOR < (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_lt,
    COMMUTATOR = >,
    NEGATOR = >=
);

CREATE OPERATOR <= (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_le,
    COMMUTATOR = >=,
    NEGATOR = >
);

CREATE OPERATOR = (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_eq,
    COMMUTATOR = =,
    NEGATOR = <>
);

CREATE OPERATOR >= (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_ge,
    COMMUTATOR = <=,
    NEGATOR = <
);

CREATE OPERATOR > (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_gt,
    COMMUTATOR = <,
    NEGATOR = <=
);

/*
 * B-tree operator class
 */
CREATE OPERATOR CLASS complex_ops
DEFAULT FOR TYPE complex_number
USING btree AS

    OPERATOR 1 <,
    OPERATOR 2 <=,
    OPERATOR 3 =,
    OPERATOR 4 >=,
    OPERATOR 5 >,

    FUNCTION 1 complex_cmp(
        complex_number,
        complex_number
    );