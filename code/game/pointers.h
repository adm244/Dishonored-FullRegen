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

#ifndef _POINTERS_H_
#define _POINTERS_H_

//------------- Macros -------------//
// internal void (* ptr_<name>_addr) = (void *)(<addr>)
#define DEFINE_STATIC_POINTER(name, addr) internal void (* ptr_##name##_addr) = (void *)(addr)
// internal <type> (*const <name>) = (<type> *)ptr_<name>_addr;
#define STATIC_POINTER(type, name) internal type (*const name) = (type *)ptr_##name##_addr
// internal <type> (*const *const <name>) = (<type> **)ptr_<name>_addr
#define STATIC_POINTER_POINTER(type, name) internal type (*const *const name) = (type **)ptr_##name##_addr

// internal void (* ptr_<obj>_<func>_addr) = (void *)(<addr>)
#define DEFINE_MEMBER_FUNCTION(obj, func, addr) internal void (* ptr_##obj##_##func##_addr) = (void *)(addr)
// typedef <ret> (THISCALL * _<obj>_<func>)(obj *, ...)
#define DECLARE_MEMBER_FUNCTION(ret, obj, func, ...) typedef ret (THISCALL * _##obj##_##func)(obj *, __VA_ARGS__)
// internal _<obj>_<func> <obj>_<func> = (_<obj>_<func>)(ptr_<obj>_<func>_addr)
#define MEMBER_FUNCTION(obj, func) internal _##obj##_##func obj##_##func = (_##obj##_##func)(ptr_##obj##_##func##_addr)

// typedef <ret> (THISCALL * _<obj>_<func>)(obj *, ...)
#define DECLARE_VIRTUAL_FUNCTION(ret, obj, func, ...) DECLARE_MEMBER_FUNCTION(ret, obj, func, __VA_ARGS__)
// _<obj>_<func> <func>
#define VIRTUAL_FUNCTION(obj, func) _##obj##_##func func

// internal void (* ptr_<func>_addr) = (void *)(<addr>)
#define DEFINE_CDECL_FUNCTION(func, addr) internal void (* ptr_##func##_addr) = (void *)(addr)
// typedef <ret> (CDECL * _<func>)(...)
#define DECLARE_CDECL_FUNCTION(ret, func, ...) typedef ret (CDECL * _##func)(__VA_ARGS__)
// internal _<func> <func> = (_<func>)(ptr_<func>_addr)
#define CDECL_FUNCTION(func) internal _##func func = (_##func)(ptr_##func##_addr)

// internal void (* ptr_<func>_addr) = (void *)(<addr>)
#define DEFINE_STDCALL_FUNCTION(func, addr) DEFINE_CDECL_FUNCTION(func, addr)
// typedef <ret> (STDCALL * _<func>)(...)
#define DECLARE_STDCALL_FUNCTION(ret, func, ...) typedef ret (STDCALL * _##func)(__VA_ARGS__)
// internal _<func> <func> = (_<func>)(ptr_<func>_addr)
#define STDCALL_FUNCTION(func) CDECL_FUNCTION(func)

//------------- Dishonored.h --------------//
DEFINE_STATIC_POINTER(playerPawn, 0x01452DE8);

DEFINE_STATIC_POINTER(Attribute_HealthMax_ID, 0x01451774);
DEFINE_STATIC_POINTER(Attribute_HealthRegenAmount_ID, 0x01451784);
DEFINE_STATIC_POINTER(Attribute_HealthRegenInitialDelay_ID, 0x0145178C);
DEFINE_STATIC_POINTER(Attribute_HealthRegenLimit_ID, 0x01451794);
DEFINE_STATIC_POINTER(Attribute_HealthRegenRate_ID, 0x0145179C);

DEFINE_STATIC_POINTER(Attribute_ManaMax_ID, 0x014517E4);
DEFINE_STATIC_POINTER(Attribute_ManaRegenAmount_ID, 0x014517F4);
DEFINE_STATIC_POINTER(Attribute_ManaRegenInitialDelay_ID, 0x014517FC);
DEFINE_STATIC_POINTER(Attribute_ManaRegenAdditivePortion_ID, 0x01451804);
DEFINE_STATIC_POINTER(Attribute_ManaRegenStepTime_ID, 0x0145180C);

//DEFINE_MEMBER_FUNCTION(Attribute, GetAttributeByName, 0x004549C0);

//------------- FullRegen.cpp -------------//
//DEFINE_STATIC_POINTER(detour_attributes_init, 0x009EA355);
DEFINE_STATIC_POINTER(detour_disattributesentry_init, 0x00C88210);

DEFINE_STATIC_POINTER(hook_disattributesentry_init_ret, 0x00C88216);

#endif
