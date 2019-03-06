#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <dataTypes/Value.h>
#include <core/Runtime.h>
}
#undef _Bool

SKYPAT_F(Runtime_i64_const, regular)
{
    // Prepare
    Stack* stack = new_Stack((void (*)(void*))free_Value);

    // Run
    runtime_i64_const(stack, 10L << 32);

    // Check
    Value *check = NULL;
    stack->entries->pop(stack->entries, (void**)&check);
    EXPECT_EQ(check->value.i64, 10L << 32);

    // Clean
    free_Value(check);
    free_Stack(stack);
}
