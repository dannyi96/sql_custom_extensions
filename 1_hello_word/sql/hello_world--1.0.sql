CREATE FUNCTION sql_hello_world()
RETURNS text
AS 'MODULE_PATHNAME', 'hello_world'
LANGUAGE C STRICT;

-- MODULE_PATHNAME dynamically set by Makefile ( dynamically resolves shared library path )