#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

/*
 * Internal representation
 */
 typedef struct Complex
 {
     double real;
     double imaginary;
 } Complex;

PG_FUNCTION_INFO_V1(complex_in);
PG_FUNCTION_INFO_V1(complex_out);

/*
* Input function
* Converts text -> internal binary
*/
Datum
complex_in(PG_FUNCTION_ARGS)
{
    char *str;
    double real;
    double imag;

    Complex *result;

    str = PG_GETARG_CSTRING(0);

    /*
    * Parse input string
    */
    if (sscanf(str, "(%lf,%lf)", &real, &imag) != 2)
    {
        ereport(ERROR,
                (
                    errmsg("invalid complex_number input syntax: \"%s\"", str)
                ));
    }

    /*
    * Allocate memory for internal object
    */
    result = (Complex *) palloc(sizeof(Complex));

    result->real = real;
    result->imaginary = imag;

    PG_RETURN_POINTER(result);
}

/*
* Output function
* Converts internal binary -> text
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