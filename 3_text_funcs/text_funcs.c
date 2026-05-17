#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(reverse_text);

Datum reverse_text(PG_FUNCTION_ARGS) {
    text *arg1 = PG_GETARG_TEXT_PP(0);
    char *str = text_to_cstring(arg1);
    int len = strlen(str);
    char *reversed = palloc(len + 1);
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    text *result = cstring_to_text(reversed);
    PG_RETURN_TEXT_P(result);
}