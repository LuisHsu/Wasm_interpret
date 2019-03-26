#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <structures/WasmImport.h>
#include <structures/WasmModule.h>
#include "sections.h"
}
#undef _Bool

SKYPAT_F(decode_import, valid)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x70\x01\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x7f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), 0);
    WasmImport* import = (WasmImport*)module->imports->at(module->imports, 0);
    // Test 1
    EXPECT_FALSE(strcmp(import->module, "Test"));
    EXPECT_FALSE(strcmp(import->name, "Test1"));
    EXPECT_EQ(import->descType, Desc_Func);
    EXPECT_EQ(import->desc.typeidx, 0);
    // Test 2
    import = (WasmImport*)module->imports->at(module->imports, 1);
    EXPECT_FALSE(strcmp(import->module, "Test"));
    EXPECT_FALSE(strcmp(import->name, "Test2"));
    EXPECT_EQ(import->descType, Desc_Table);
    EXPECT_EQ(import->desc.limits.min, 3);
    EXPECT_EQ(import->desc.limits.max, 5);
    // Test 3
    import = (WasmImport*)module->imports->at(module->imports, 2);
    EXPECT_FALSE(strcmp(import->module, "Test"));
    EXPECT_FALSE(strcmp(import->name, "Test3"));
    EXPECT_EQ(import->descType, Desc_Mem);
    EXPECT_EQ(import->desc.limits.min, 2);
    EXPECT_EQ(import->desc.limits.max, 4);
    // Test 4
    import = (WasmImport*)module->imports->at(module->imports, 3);
    EXPECT_FALSE(strcmp(import->module, "Test"));
    EXPECT_FALSE(strcmp(import->name, "Test4"));
    EXPECT_EQ(import->descType, Desc_Global);
    EXPECT_EQ(import->desc.global.valueType, Value_i32);
    EXPECT_EQ(import->desc.global.mut, 0);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, unknown_import_type)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x0a\x00\x04Test\x05Test2\x01\x70\x01\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x7f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -1);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, table_element_not_anyfunc)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x77\x01\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x7f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -2);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, unknown_table_limit_flag)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x70\x03\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x7f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -3);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, unknown_memory_limit_flag)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x70\x01\x03\x05\x04Test\x05Test3\x02\x05\x02\x04\x04Test\x05Test4\x03\x7f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -4);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, unknown_global_type)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x70\x01\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x6f\x00";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -5);
    free_WasmModule(module);
}

SKYPAT_F(decode_import, unknown_global_mut)
{
    WasmModule* module = new_WasmModule((char*)"Test");
    uint8_t* testBin = (uint8_t*) "\x02\x3b\x04\x04Test\x05Test1\x00\x00\x04Test\x05Test2\x01\x70\x01\x03\x05\x04Test\x05Test3\x02\x01\x02\x04\x04Test\x05Test4\x03\x7f\x08";
    EXPECT_EQ(parse_import_section(module, &testBin, testBin + 60), -6);
    free_WasmModule(module);
}