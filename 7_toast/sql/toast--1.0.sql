CREATE FUNCTION text_size_info(text)
RETURNS int
AS 'MODULE_PATHNAME', 'text_size_info'
LANGUAGE C STRICT;

CREATE FUNCTION force_detoast(text)
RETURNS text
AS 'MODULE_PATHNAME', 'force_detoast'
LANGUAGE C STRICT;

CREATE FUNCTION compare_storage(text, text)
RETURNS void
AS 'MODULE_PATHNAME', 'compare_storage'
LANGUAGE C STRICT;