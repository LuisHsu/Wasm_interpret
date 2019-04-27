#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <string.h>
#include <stdlib.h>
#include <core/Stack.h>
#include <dataTypes/Label.h>
#include <dataTypes/Frame.h>
#include <dataTypes/Value.h>
#include <instance/ModuleInst.h>
}
#undef _Bool

SKYPAT_F(Stack, create_delete)
{
    // Prepare
    Stack* stack = new_Stack();

    // Check
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);

    free_Stack(stack);
}

SKYPAT_F(Stack, push_Label)
{
    // Prepare
    Stack* stack = new_Stack();
    push_Label(stack, new_Label(0, 1, 2));

    // Check
    EXPECT_EQ(stack->entries->size, 1);
    EXPECT_NE(stack->entries->head, NULL);
    Label* result = NULL;
    stack->entries->top(stack->entries, (void**)&result);
    EXPECT_EQ(stack->curLabel, result);
    EXPECT_EQ(result->funcAddr, 0);
    EXPECT_EQ(result->instrIndex, 1);
    EXPECT_EQ(result->contInstr, 2);

    free_Stack(stack);
}

SKYPAT_F(Stack, push_Frame)
{
    // Prepare
    Stack* stack = new_Stack();
    char* moduleName = (char*) malloc(sizeof(char) * 5);
    strcpy(moduleName, "Test");
    ModuleInst* module = new_ModuleInst(moduleName);
    push_Frame(stack, new_Frame(module));

    // Check
    EXPECT_EQ(stack->entries->size, 1);
    EXPECT_NE(stack->entries->head, NULL);
    Frame* result = NULL;
    stack->entries->top(stack->entries, (void**)&result);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(stack->curFrame, result);
    EXPECT_EQ(result->moduleInst, module);

    free_Stack(stack);
}

SKYPAT_F(Stack, push_Value)
{
    // Prepare
    Stack* stack = new_Stack();
    push_Value(stack, new_i32Value(2));

    // Check
    EXPECT_EQ(stack->entries->size, 1);
    EXPECT_NE(stack->entries->head, NULL);
    Value* result = NULL;
    stack->entries->top(stack->entries, (void**)&result);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(stack->curFrame, NULL);
    EXPECT_EQ(result->value.i32, 2);

    free_Stack(stack);
}

SKYPAT_F(Stack, pop_Label_valid)
{
    // Prepare
    Stack* stack = new_Stack();
    push_Label(stack, new_Label(0, 1, 2));

    // Check
    Label* result = NULL;
    EXPECT_EQ(pop_Label(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);
    EXPECT_EQ(result->funcAddr, 0);
    EXPECT_EQ(result->instrIndex, 1);
    EXPECT_EQ(result->contInstr, 2);
    free_Label(result);

    result = NULL;
    push_Label(stack, new_Label(0, 5, 6));
    push_Value(stack, new_i32Value(10));
    push_Label(stack, new_Label(0, 3, 4));
    push_Value(stack, new_i32Value(20));

    EXPECT_EQ(pop_Label(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 2);
    EXPECT_NE(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel->funcAddr, 0);
    EXPECT_EQ(stack->curLabel->instrIndex, 5);
    EXPECT_EQ(stack->curLabel->contInstr, 6);
    EXPECT_EQ(result->funcAddr, 0);
    EXPECT_EQ(result->instrIndex, 3);
    EXPECT_EQ(result->contInstr, 4);
    free_Label(result);

    EXPECT_EQ(pop_Label(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(result->funcAddr, 0);
    EXPECT_EQ(result->instrIndex, 5);
    EXPECT_EQ(result->contInstr, 6);

    free_Label(result);
    free_Stack(stack);
}

SKYPAT_F(Stack, pop_Label_not_exist_curLabel)
{
    // Prepare
    Stack* stack = new_Stack();

    // Check
    Label* result = NULL;
    EXPECT_EQ(pop_Label(stack, &result), -1);

    free_Stack(stack);
}

SKYPAT_F(Stack, pop_Frame_valid)
{
    // Prepare
    Stack* stack = new_Stack();
    char* moduleName = (char*) malloc(sizeof(char) * 5);
    strcpy(moduleName, "Test");
    ModuleInst* module = new_ModuleInst(moduleName);
    push_Frame(stack, new_Frame(module));

    // Check
    Frame* result = NULL;
    EXPECT_EQ(pop_Frame(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(stack->curFrame, NULL);
    EXPECT_EQ(result->moduleInst, module);
    free_Frame(result);

    result = NULL;
    Label* label1 = new_Label(0, 1, 2);
    Frame* frame1 = new_Frame(module);
    push_Frame(stack, frame1);
    push_Label(stack, label1);
    push_Value(stack, new_i32Value(10));
    push_Frame(stack, new_Frame(module));
    push_Label(stack, new_Label(0, 3, 4));
    push_Value(stack, new_i32Value(20));

    EXPECT_EQ(pop_Frame(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 3);
    EXPECT_NE(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel, label1);
    EXPECT_EQ(stack->curFrame, frame1);
    EXPECT_EQ(result->moduleInst, module);
    free_Frame(result);

    EXPECT_EQ(pop_Frame(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(stack->curFrame, NULL);
    EXPECT_EQ(result->moduleInst, module);
    free_Frame(result);

    free_Stack(stack);
    free_ModuleInst(module);
}

SKYPAT_F(Stack, pop_Frame_not_exist_curFrame)
{
    // Prepare
    Stack* stack = new_Stack();

    // Check
    Frame* result = NULL;
    EXPECT_EQ(pop_Frame(stack, &result), -1);

    free_Stack(stack);
}

SKYPAT_F(Stack, pop_Value_valid)
{
    // Prepare
    Stack* stack = new_Stack();
    push_Value(stack, new_i32Value(3));

    // Check
    Value* result = NULL;
    EXPECT_EQ(pop_Value(stack, &result), 0);
    EXPECT_EQ(stack->entries->size, 0);
    EXPECT_EQ(stack->entries->head, NULL);
    EXPECT_EQ(stack->curLabel, NULL);
    EXPECT_EQ(stack->curFrame, NULL);
    EXPECT_EQ(result->value.i32, 3);

    free_Value(result);
    free_Stack(stack);
}

SKYPAT_F(Stack, pop_Value_not_exist)
{
    // Prepare
    Stack* stack = new_Stack();

    // Check
    Value* result = NULL;
    EXPECT_EQ(pop_Value(stack, &result), -1);
    EXPECT_EQ(result, NULL);

    free_Stack(stack);
}