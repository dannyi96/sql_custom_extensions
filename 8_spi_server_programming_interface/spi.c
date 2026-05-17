#include "postgres.h"
#include "fmgr.h"

#include "catalog/pg_type.h"      /* TEXTOID */
#include "executor/spi.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(count_users);
PG_FUNCTION_INFO_V1(insert_user);
PG_FUNCTION_INFO_V1(get_usernames);

/*
 * Count rows in users table
 */
Datum
count_users(PG_FUNCTION_ARGS)
{
    int     ret;
    Datum   d;
    bool    isnull;
    int64   count;

    SPI_connect();

    ret = SPI_execute("SELECT count(*) FROM users", true, 0);
    if (ret != SPI_OK_SELECT || SPI_processed == 0)
    {
        SPI_finish();
        ereport(ERROR,
                (errcode(ERRCODE_INTERNAL_ERROR),
                 errmsg("SPI_execute(SELECT count(*) FROM users) failed: %s",
                        SPI_result_code_string(ret))));
    }

    d = SPI_getbinval(SPI_tuptable->vals[0],
                      SPI_tuptable->tupdesc,
                      1,
                      &isnull);

    count = isnull ? 0 : DatumGetInt64(d);

    SPI_finish();

    PG_RETURN_INT64(count);
}

/*
 * Insert user into table
 */
Datum
insert_user(PG_FUNCTION_ARGS)
{
    text   *name_text = PG_GETARG_TEXT_PP(0);
    Oid     argtypes[1] = { TEXTOID };
    Datum   values[1]   = { PointerGetDatum(name_text) };
    /* nulls[]: " " = not null, "n" = null. We're STRICT, so never null. */
    char    nulls[1]    = { ' ' };
    int     ret;

    SPI_connect();

    ret = SPI_execute_with_args(
              "INSERT INTO users(name) VALUES($1)",
              1, argtypes, values, nulls,
              false /* read-only */,
              0     /* count: 0 = unlimited */);

    if (ret != SPI_OK_INSERT)
    {
        SPI_finish();
        ereport(ERROR,
                (errcode(ERRCODE_INTERNAL_ERROR),
                 errmsg("INSERT into users failed: %s",
                        SPI_result_code_string(ret))));
    }

    SPI_finish();
    PG_RETURN_VOID();
}

/*
 * Return all usernames as text
 */
Datum
get_usernames(PG_FUNCTION_ARGS)
{
    StringInfoData result;
    int            ret;
    uint64         i;

    initStringInfo(&result);

    SPI_connect();

    ret = SPI_execute("SELECT name FROM users ORDER BY id", true, 0);
    if (ret != SPI_OK_SELECT)
    {
        SPI_finish();
        ereport(ERROR,
                (errcode(ERRCODE_INTERNAL_ERROR),
                 errmsg("SELECT from users failed: %s",
                        SPI_result_code_string(ret))));
    }

    for (i = 0; i < SPI_processed; i++)
    {
        bool    isnull;
        Datum   d = SPI_getbinval(SPI_tuptable->vals[i],
                                  SPI_tuptable->tupdesc,
                                  1,
                                  &isnull);
        if (!isnull)
        {
            char *name = TextDatumGetCString(d);
            appendStringInfo(&result, "%s\n", name);
            pfree(name);
        }
    }

    SPI_finish();

    PG_RETURN_TEXT_P(cstring_to_text(result.data));
}
