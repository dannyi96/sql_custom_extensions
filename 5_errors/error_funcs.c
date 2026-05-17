#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(fail_if_negative);

Datum fail_if_negative(PG_FUNCTION_ARGS) {
    int32 value = PG_GETARG_INT32(0);
    if (value < 0) {
        ereport(ERROR,
            (
                errmsg("Negative numbers are not allowed"),
                errdetail("Input value was %d", value)
            ));
    }
    PG_RETURN_INT32(value);
}