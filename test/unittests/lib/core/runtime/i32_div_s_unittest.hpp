#include <skypat/skypat.h>


#define _Bool bool
extern "C" {
#include <core/Runtime.h>
#include <dataTypes/Value.h>
}
#undef _Bool

SKYPAT_F(Runtime_i32_div_s, regular)
{
    // prepare
    Stack* stack = new_Stack((void (*)(void*))free_Value);
    Value *value1 = new_i32Value(20); //Dividend
    Value *value2 = new_i32Value(-3); //Divisor
    stack->entries->push(stack->entries, value1);
    stack->entries->push(stack->entries, value2);

    // run
    runtime_i32_div_s(stack);

    // check
    Value *check = NULL;
    stack->entries->pop(stack->entries, (void**)&check);
    EXPECT_EQ(check->value.i32, -6);

    // clean
    free_Value(check);
    free_Stack(stack);
}
