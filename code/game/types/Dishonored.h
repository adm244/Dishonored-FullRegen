/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

//IMPORTANT(adm244): SCRATCH VERSION JUST TO GET IT UP WORKING

#ifndef _DISHONORED_H_
#define _DISHONORED_H_

struct DishonoredPlayerPawn;

//------------- Static pointers -------------//
STATIC_POINTER_POINTER(DishonoredPlayerPawn, playerPawn);

STATIC_POINTER(u32, Attribute_HealthMax_ID);
STATIC_POINTER(u32, Attribute_HealthRegenAmount_ID);
STATIC_POINTER(u32, Attribute_HealthRegenInitialDelay_ID);
STATIC_POINTER(u32, Attribute_HealthRegenLimit_ID);
STATIC_POINTER(u32, Attribute_HealthRegenRate_ID);

STATIC_POINTER(u32, Attribute_ManaMax_ID);
STATIC_POINTER(u32, Attribute_ManaRegenAmount_ID);
STATIC_POINTER(u32, Attribute_ManaRegenInitialDelay_ID);
STATIC_POINTER(u32, Attribute_ManaRegenAdditivePortion_ID);
STATIC_POINTER(u32, Attribute_ManaRegenStepTime_ID);

//------------- Structures -------------//
#pragma pack(4)

struct DishonoredPlayerPawn {
  u8 unk00[0x344-0x0];
  i32 healthCurrent; // 0x344
  i32 healthMax; // 0x348
  u8 unk34C[0xA60-0x34C];
  i32 manaCurrent; // 0xA60
  i32 manaMax; // 0xA64
};
assert_size(DishonoredPlayerPawn, 0xA68);

struct DisAttributesEntry {
  u32 id;
  u32 unk04;
  r32 value1; // 0x8
  u32 unk0C;
  u32 unk10;
  u32 unk14;
  r32 value2; // 0x18
  u32 unk1C;
};
assert_size(DisAttributesEntry, 0x20);

struct Attribute {
  u32 id;
  u32 unk04;
  u32 unk08;
  u32 unk0C;
  u32 unk10;
  u32 unk14;
  r32 value; // 0x18
};
assert_size(Attribute, 0x1C);

#pragma pack(pop)

#endif
