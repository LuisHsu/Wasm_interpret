#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <core/Runtime.h>
#include <dataTypes/Value.h>
}
#undef _Bool

/*
 * This function applys runtime_i64_eqz() unittest and check the result of runtime_i64_eqz().
 */
void i64_eqz_check(Stack* stack, int64_t value_1, int64_t expected)
{
    Value *check = NULL;
    Value *_value_1 = new_i64Value(value_1);

    stack->entries->push(stack->entries, _value_1);
    runtime_i64_eqz(stack);

    stack->entries->pop(stack->entries, (void**)&check);
    EXPECT_EQ(check->value.i32, expected);
    free_Value(check);
}

SKYPAT_F(Runtime_i64_eqz, regular)
{
    Stack* stack = new_Stack((void (*)(void*))free_Value);

    // case 1: 10
    i64_eqz_check(stack, 10, 0);

    // case 2: -10
    i64_eqz_check(stack, -10, 0);

    // case 3: 0
    i64_eqz_check(stack, 0, 1);

    // clean
    free_Stack(stack);
}
