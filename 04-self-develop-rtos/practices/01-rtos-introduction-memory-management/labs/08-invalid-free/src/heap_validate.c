#include "heap.h"

/*
 * Validation is implemented in heap.c so it can inspect private block metadata.
 * This translation unit exists to keep the lab structure explicit and to host
 * future canary/poison extensions without exposing internals in heap.h.
 */

bool heap_validation_extension_available(void)
{
    return true;
}
