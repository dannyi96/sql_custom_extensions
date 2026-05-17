CREATE FUNCTION count_users()
RETURNS bigint
AS 'MODULE_PATHNAME', 'count_users'
LANGUAGE C;

CREATE FUNCTION insert_user(text)
RETURNS void
AS 'MODULE_PATHNAME', 'insert_user'
LANGUAGE C STRICT;

CREATE FUNCTION get_usernames()
RETURNS text
AS 'MODULE_PATHNAME', 'get_usernames'
LANGUAGE C;