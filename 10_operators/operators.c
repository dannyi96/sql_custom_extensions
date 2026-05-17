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

PG_FUNCTION_INFO_V1(complex_add);
PG_FUNCTION_INFO_V1(complex_equal);
PG_FUNCTION_INFO_V1(complex_not_equal);
PG_FUNCTION_INFO_V1(complex_magnitude);

/*
 * Input function
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
                    errmsg("invalid complex_number input: \"%s\"", str)
                ));
    }

    result = (Complex *) palloc(sizeof(Complex));

    result->real = real;
    result->imaginary = imag;

    PG_RETURN_POINTER(result);
}

/*
 * Output function
 */
Datum
complex_out(PG_FUNCTION_ARGS)
{
    Complex *complex;

    char *result;

    complex = (Complex *) PG_GETARG_POINTER(0);

    result = psprintf(
        "(%g,%g)",
        complex->real,
        complex->imaginary
    );

    PG_RETURN_CSTRING(result);
}

/*
 * Addition operator function
 */
Datum
complex_add(PG_FUNCTION_ARGS)
{
    Complex *a;
    Complex *b;

    Complex *result;

    a = (Complex *) PG_GETARG_POINTER(0);
    b = (Complex *) PG_GETARG_POINTER(1);

    result = (Complex *) palloc(sizeof(Complex));

    result->real =
        a->real + b->real;

    result->imaginary =
        a->imaginary + b->imaginary;

    PG_RETURN_POINTER(result);
}

/*
 * Equality operator
 */
Datum
complex_equal(PG_FUNCTION_ARGS)
{
    Complex *a;
    Complex *b;

    a = (Complex *) PG_GETARG_POINTER(0);
    b = (Complex *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(
        a->real == b->real &&
        a->imaginary == b->imaginary
    );
}

/*
 * Inequality operator
 */
Datum
complex_not_equal(PG_FUNCTION_ARGS)
{
    Complex *a;
    Complex *b;

    a = (Complex *) PG_GETARG_POINTER(0);
    b = (Complex *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(
        a->real != b->real ||
        a->imaginary != b->imaginary
    );
}

/*
 * Magnitude function
 */
Datum
complex_magnitude(PG_FUNCTION_ARGS)
{
    Complex *a;

    double magnitude;

    a = (Complex *) PG_GETARG_POINTER(0);

    magnitude =
        sqrt(
            a->real * a->real +
            a->imaginary * a->imaginary
        );

    PG_RETURN_FLOAT8(magnitude);
}