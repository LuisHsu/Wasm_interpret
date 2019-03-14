#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <dataTypes/Value.h>
#include <core/Runtime.h>
}
#undef _Bool

SKYPAT_F(Runtime_i64_popcnt, regular)
{
    // Prepare
    Stack *stack = new_Stack();
    Value *value1 = new_i64Value(-1);
    push_Value(stack, value1);

    // Run
    runtime_i64_popcnt(stack);

    // Check
    Value *check = NULL;
    pop_Value(stack,&check);
    EXPECT_EQ(check->value.i64, 64);

    // Clean
    free_Value(check);
    free_Stack(stack);
}
