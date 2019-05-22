#ifndef WASMVM_CORE_DEF
#define WASMVM_CORE_DEF

#include <stdint.h>
#include <pthread.h>
#include <core/Stack.h>
#include <core/Store.h>
#include <instance/ModuleInst.h>

typedef enum {
    Core_Stop,
    Core_Running,
    Core_Paused,
} CoreStatus;

typedef struct Core_ {
    CoreStatus status;
    pthread_t thread;
    Store *store; // Share with other Core
    Stack *stack; // Core's private Stack
    ModuleInst* module;
    uint32_t startFuncAddr;
    int (*run)(struct Core_* core);
    int (*pause)(struct Core_* core);
    int (*resume)(struct Core_* core);
    int (*stop)(struct Core_* core);
} Core;

Core* new_Core(Store *store, ModuleInst* module, uint32_t startFuncAddr);
void free_Core(Core *core);

#endif