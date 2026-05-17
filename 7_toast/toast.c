#include "postgres.h"
#include "fmgr.h"
#include "varatt.h"

#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(text_size_info);
PG_FUNCTION_INFO_V1(force_detoast);
PG_FUNCTION_INFO_V1(compare_storage);

/*
 * Show internal size information
 */
Datum
text_size_info(PG_FUNCTION_ARGS)
{
    text *txt;

    int32 raw_size;
    int32 data_size;

    txt = PG_GETARG_TEXT_PP(0);

    raw_size = VARSIZE(txt);
    data_size = VARSIZE_ANY_EXHDR(txt);

    elog(INFO,
         "Total size including header: %d bytes",
         raw_size);

    elog(INFO,
         "Actual data size: %d bytes",
         data_size);

    PG_RETURN_INT32(data_size);
}

/*
 * Force detoasting of value
 */
Datum
force_detoast(PG_FUNCTION_ARGS)
{
    text *txt;
    text *detoasted;

    txt = PG_GETARG_TEXT_PP(0);

    detoasted = (text *) pg_detoast_datum(txt);

    elog(INFO,
         "Successfully detoasted value");

    PG_RETURN_TEXT_P(detoasted);
}

/*
 * Compare compressibility
 */
Datum
compare_storage(PG_FUNCTION_ARGS)
{
    text *txt1;
    text *txt2;

    txt1 = PG_GETARG_TEXT_PP(0);
    txt2 = PG_GETARG_TEXT_PP(1);

    elog(INFO,
         "Text1 size: %d",
         VARSIZE_ANY_EXHDR(txt1));

    elog(INFO,
         "Text2 size: %d",
         VARSIZE_ANY_EXHDR(txt2));

    PG_RETURN_VOID();
}