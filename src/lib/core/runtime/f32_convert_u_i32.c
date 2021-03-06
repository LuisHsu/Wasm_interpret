#include <core/Runtime.h>

#include <stdlib.h>

int runtime_f32_convert_u_i32(Stack stack)
{
    Value* operand = NULL;
    pop_Value(stack, &operand);
    push_Value(stack, new_f32Value((float)(operand->value.i32 * (-(operand->value.i32 < 0) * 2 + 1))));
    free(operand);
    return 0;
}
