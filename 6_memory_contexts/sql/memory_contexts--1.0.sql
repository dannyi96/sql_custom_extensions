CREATE FUNCTION allocate_memory(int)
RETURNS int
AS 'MODULE_PATHNAME', 'allocate_memory'
LANGUAGE C STRICT;

CREATE FUNCTION context_info()
RETURNS void
AS 'MODULE_PATHNAME', 'context_info'
LANGUAGE C;

CREATE FUNCTION temporary_buffer_demo()
RETURNS void
AS 'MODULE_PATHNAME', 'temporary_buffer_demo'
LANGUAGE C;