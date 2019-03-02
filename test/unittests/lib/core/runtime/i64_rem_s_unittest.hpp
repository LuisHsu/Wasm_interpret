#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <core/Runtime.h>
#include <dataTypes/Value.h>
}
#undef _Bool

/*
 * This function applys runtime_i64_rem_s() unittest.
 * Check the result of runtime_i64_rem_s().
 */
void i64_rem_s_check(Stack* stack, int64_t value_1, int64_t value_2, int64_t expected)
{
    Value *check = NULL;
    Value *_value_1 = new_i64Value(value_1); // dividend
    Value *_value_2 = new_i64Value(value_2); // divisor

    stack->entries->push(stack->entries, _value_1);
    stack->entries->push(stack->entries, _value_2);
    runtime_i64_rem_s(stack);

    stack->entries->pop(stack->entries, (void**)&check);
    EXPECT_EQ(check->value.i64, expected);
    free_Value(check);
}

SKYPAT_F(Runtime_i64_rem_s, regular)
{
    Stack* stack = new_Stack((void (*)(void*))free_Value);

    // case 1: 20 << 32 % 3
    i64_rem_s_check(stack, 20L << 32, 3, 2);

    // case 2: (-20 << 32) % 3
    i64_rem_s_check(stack, -20L << 32, 3, -2);

    // case 3: 20 << 32 % (-3)
    i64_rem_s_check(stack, 20L << 32, -3, 2);

    // case 4: 15 << 32 % 1
    i64_rem_s_check(stack, 15L << 32, 1, 0);

    // clean
    free_Stack(stack);
}