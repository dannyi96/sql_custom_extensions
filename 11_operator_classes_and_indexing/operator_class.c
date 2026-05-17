#include "postgres.h"
#include "fmgr.h"

#include <math.h>
#include <stdio.h>

PG_MODULE_MAGIC;

/*
 * Internal type
 */
typedef struct Complex
{
    double real;
    double imaginary;
} Complex;

/*
 * Function declarations
 */
PG_FUNCTION_INFO_V1(complex_in);
PG_FUNCTION_INFO_V1(complex_out);

PG_FUNCTION_INFO_V1(complex_cmp);

PG_FUNCTION_INFO_V1(complex_lt);
PG_FUNCTION_INFO_V1(complex_le);
PG_FUNCTION_INFO_V1(complex_eq);
PG_FUNCTION_INFO_V1(complex_ge);
PG_FUNCTION_INFO_V1(complex_gt);

/*
 * Helper
 */
static double
complex_magnitude_internal(Complex *c)
{
    return sqrt(
        c->real * c->real +
        c->imaginary * c->imaginary
    );
}

/*
 * Input
 */
Datum
complex_in(PG_FUNCTION_ARGS)
{
    char *str;

    double real;
    double imag;

    Complex *result;

    str = PG_GETARG_CSTRING(0);

    if (sscanf(str, "(%lf,%lf)", &real, &imag) != 2)
    {
        ereport(ERROR,
                (
                    errmsg("invalid complex_number input")
                ));
    }

    result = palloc(sizeof(Complex));

    result->real = real;
    result->imaginary = imag;

    PG_RETURN_POINTER(result);
}

/*
 * Output
 */
Datum
complex_out(PG_FUNCTION_ARGS)
{
    Complex *c;

    c = (Complex *) PG_GETARG_POINTER(0);

    PG_RETURN_CSTRING(
        psprintf(
            "(%g,%g)",
            c->real,
            c->imaginary
        )
    );
}

/*
 * Core comparison function
 *
 * Returns:
 *  <0 if a < b
 *   0 if a = b
 *  >0 if a > b
 */
Datum
complex_cmp(PG_FUNCTION_ARGS)
{
    Complex *a;
    Complex *b;

    double ma;
    double mb;

    a = (Complex *) PG_GETARG_POINTER(0);
    b = (Complex *) PG_GETARG_POINTER(1);

    ma = complex_magnitude_internal(a);
    mb = complex_magnitude_internal(b);

    if (ma < mb)
        PG_RETURN_INT32(-1);

    if (ma > mb)
        PG_RETURN_INT32(1);

    PG_RETURN_INT32(0);
}

/*
 * <
 */
Datum
complex_lt(PG_FUNCTION_ARGS)
{
    int32 cmp;

    cmp = DatumGetInt32(
        DirectFunctionCall2(
            complex_cmp,
            PG_GETARG_DATUM(0),
            PG_GETARG_DATUM(1)
        )
    );

    PG_RETURN_BOOL(cmp < 0);
}

/*
 * <=
 */
Datum
complex_le(PG_FUNCTION_ARGS)
{
    int32 cmp;

    cmp = DatumGetInt32(
        DirectFunctionCall2(
            complex_cmp,
            PG_GETARG_DATUM(0),
            PG_GETARG_DATUM(1)
        )
    );

    PG_RETURN_BOOL(cmp <= 0);
}

/*
 * =
 */
Datum
complex_eq(PG_FUNCTION_ARGS)
{
    int32 cmp;

    cmp = DatumGetInt32(
        DirectFunctionCall2(
            complex_cmp,
            PG_GETARG_DATUM(0),
            PG_GETARG_DATUM(1)
        )
    );

    PG_RETURN_BOOL(cmp == 0);
}

/*
 * >=
 */
Datum
complex_ge(PG_FUNCTION_ARGS)
{
    int32 cmp;

    cmp = DatumGetInt32(
        DirectFunctionCall2(
            complex_cmp,
            PG_GETARG_DATUM(0),
            PG_GETARG_DATUM(1)
        )
    );

    PG_RETURN_BOOL(cmp >= 0);
}

/*
 * >
 */
Datum
complex_gt(PG_FUNCTION_ARGS)
{
    int32 cmp;

    cmp = DatumGetInt32(
        DirectFunctionCall2(
            complex_cmp,
            PG_GETARG_DATUM(0),
            PG_GETARG_DATUM(1)
        )
    );

    PG_RETURN_BOOL(cmp > 0);
}