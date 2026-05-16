#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(add_one);
PG_FUNCTION_INFO_V1(is_even);

Datum
add_one(PG_FUNCTION_ARGS)
{
    int32 arg1 = PG_GETARG_INT32(0);
    PG_RETURN_INT32(arg1 + 1);
}

Datum
is_even(PG_FUNCTION_ARGS)
{
    int32 arg1 = PG_GETARG_INT32(0);
    PG_RETURN_BOOL(arg1 % 2 == 0);
}