CREATE FUNCTION sum_array(int[])
RETURNS bigint
AS 'MODULE_PATHNAME', 'sum_array'
LANGUAGE C STRICT;

CREATE FUNCTION double_array(int[])
RETURNS int[]
AS 'MODULE_PATHNAME', 'double_array'
LANGUAGE C STRICT;

CREATE FUNCTION count_nulls(anyarray)
RETURNS int
AS 'MODULE_PATHNAME', 'count_nulls'
LANGUAGE C;
