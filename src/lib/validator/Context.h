#ifndef WASMVM_VALIDATOR_CONTEXT_DEF
#define WASMVM_VALIDATOR_CONTEXT_DEF

#include <stdint.h>
#include <dataTypes/Value.h>
#include <dataTypes/vector.h>
#include <structures/WasmModule.h>
#include <structures/WasmFunc.h>

typedef struct {
    vector* label_types; // ValueType
    vector* end_types; // ValueType
    _Bool unreachable;
} ctrl_frame;

typedef struct {
    WasmModule* module;
    vector* locals; // ValueType
    vector* returns; // ValueType
} Context;

ctrl_frame* new_ctrl_frame();
void free_ctrl_frame(ctrl_frame* frame);
Context* new_Context(WasmModule* module, WasmFunc* func);
void free_Context(Context* context);

#endif