#include "postgres.h"
#include "fmgr.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "utils/lsyscache.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(sum_array);
PG_FUNCTION_INFO_V1(double_array);
PG_FUNCTION_INFO_V1(count_nulls);

/*
 * sum_array(int[]) -> bigint
 * Sums all non-NULL int4 elements. Uses int64 accumulator to avoid overflow.
 */
Datum
sum_array(PG_FUNCTION_ARGS)
{
    ArrayType *array = PG_GETARG_ARRAYTYPE_P(0);
    Datum     *elements;
    bool      *nulls;
    int        len;
    int64      sum = 0;

    if (ARR_ELEMTYPE(array) != INT4OID)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("sum_array expects int[]")));

    deconstruct_array(array, INT4OID, sizeof(int32), true, TYPALIGN_INT,
                      &elements, &nulls, &len);

    for (int i = 0; i < len; i++)
        if (!nulls[i])
            sum += DatumGetInt32(elements[i]);

    PG_RETURN_INT64(sum);
}

/*
 * double_array(int[]) -> int[]
 * Multiplies every element by 2; NULL slots are preserved.
 */
Datum
double_array(PG_FUNCTION_ARGS)
{
    ArrayType *array = PG_GETARG_ARRAYTYPE_P(0);
    Datum     *elements;
    bool      *nulls;
    int        len;
    int        dims[1];
    int        lbs[1] = {1};

    if (ARR_ELEMTYPE(array) != INT4OID)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("double_array expects int[]")));

    deconstruct_array(array, INT4OID, sizeof(int32), true, TYPALIGN_INT,
                      &elements, &nulls, &len);

    Datum *new_elems = (Datum *) palloc(len * sizeof(Datum));
    for (int i = 0; i < len; i++)
    {
        if (nulls[i])
            new_elems[i] = (Datum) 0;
        else
            new_elems[i] = Int32GetDatum(DatumGetInt32(elements[i]) * 2);
    }

    dims[0] = len;
    ArrayType *result = construct_md_array(new_elems, nulls, 1, dims, lbs,
                                           INT4OID, sizeof(int32), true,
                                           TYPALIGN_INT);

    PG_RETURN_ARRAYTYPE_P(result);
}

/*
 * count_nulls(anyarray) -> int
 * Generic over element type — looks up typlen/typbyval/typalign from the catalog.
 */
Datum
count_nulls(PG_FUNCTION_ARGS)
{
    ArrayType *array = PG_GETARG_ARRAYTYPE_P(0);
    Oid        elemtype = ARR_ELEMTYPE(array);
    int16      typlen;
    bool       typbyval;
    char       typalign;
    Datum     *elements;
    bool      *nulls;
    int        len;
    int32      count = 0;

    get_typlenbyvalalign(elemtype, &typlen, &typbyval, &typalign);

    deconstruct_array(array, elemtype, typlen, typbyval, typalign,
                      &elements, &nulls, &len);

    for (int i = 0; i < len; i++)
        if (nulls[i])
            count++;

    PG_RETURN_INT32(count);
}
