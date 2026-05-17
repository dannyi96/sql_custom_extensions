
CREATE FUNCTION fail_if_negative(int)
RETURNS int
AS 'MODULE_PATHNAME', 'fail_if_negative'
LANGUAGE C STRICT;
