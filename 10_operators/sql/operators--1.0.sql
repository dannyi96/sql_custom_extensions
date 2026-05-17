CREATE TYPE complex_number;

/*
 * Input/output functions
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
 * Type definition
 */
CREATE TYPE complex_number (
    INTERNALLENGTH = 16,
    INPUT = complex_in,
    OUTPUT = complex_out,
    ALIGNMENT = double
);

/*
 * Addition function
 */
CREATE FUNCTION complex_add(
    complex_number,
    complex_number
)
RETURNS complex_number
AS 'MODULE_PATHNAME', 'complex_add'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Equality function
 */
CREATE FUNCTION complex_equal(
    complex_number,
    complex_number
)
RETURNS boolean
AS 'MODULE_PATHNAME', 'complex_equal'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Inequality function
 */
CREATE FUNCTION complex_not_equal(
    complex_number,
    complex_number
)
RETURNS boolean
AS 'MODULE_PATHNAME', 'complex_not_equal'
LANGUAGE C IMMUTABLE STRICT;

/*
 * Magnitude function
 */
CREATE FUNCTION complex_magnitude(
    complex_number
)
RETURNS float8
AS 'MODULE_PATHNAME', 'complex_magnitude'
LANGUAGE C IMMUTABLE STRICT;

/*
 * + operator
 */
CREATE OPERATOR + (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_add,
    COMMUTATOR = +
);

/*
 * = operator
 */
CREATE OPERATOR = (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_equal,
    COMMUTATOR = =,
    NEGATOR = <>
);

/*
 * <> operator
 */
CREATE OPERATOR <> (
    LEFTARG = complex_number,
    RIGHTARG = complex_number,
    PROCEDURE = complex_not_equal,
    COMMUTATOR = <>,
    NEGATOR = =
);