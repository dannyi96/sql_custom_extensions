#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/memutils.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(allocate_memory);
PG_FUNCTION_INFO_V1(context_info);
PG_FUNCTION_INFO_V1(temporary_buffer_demo);

/*
* Allocate a large temporary buffer
*/
Datum
allocate_memory(PG_FUNCTION_ARGS)
{
    int32 size;
    char *buffer;

    size = PG_GETARG_INT32(0);
    buffer = palloc(size);

    memset(buffer, 'A', size);

    elog(INFO,
        "Allocated %d bytes at %p",
        size,
        buffer);

    PG_RETURN_INT32(size);
}

/*
* Print current memory context information
*/
Datum
context_info(PG_FUNCTION_ARGS)
{
    elog(INFO,
        "Current memory context: %s",
        CurrentMemoryContext->name);

    PG_RETURN_VOID();
}

/*
 * Demonstrates custom memory contexts
 */
Datum
temporary_buffer_demo(PG_FUNCTION_ARGS)
{
    MemoryContext temp_context;
    MemoryContext old_context;

    char *buffer1;
    char *buffer2;

    /*
    * Create new memory context
    */
    temp_context =
        AllocSetContextCreate(
            CurrentMemoryContext,
            "TemporaryBufferContext",
            ALLOCSET_DEFAULT_SIZES
        );

    /*
    * Switch into new context
    */
    old_context = MemoryContextSwitchTo(temp_context);

    buffer1 = palloc(1024 * 1024);
    buffer2 = palloc(1024 * 1024);

    elog(INFO,
        "Allocated temporary buffers");

    /*
    * Switch back
    */
    MemoryContextSwitchTo(old_context);

    /*
    * Destroy entire context
    */
    MemoryContextDelete(temp_context);

    elog(INFO,
        "Temporary context deleted");

    PG_RETURN_VOID();
}

