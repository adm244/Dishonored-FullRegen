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

#ifndef _PATCH_FULLREGEN_CPP_
#define _PATCH_FULLREGEN_CPP_

struct AttributeReplacer {
  u32 *id;
  int *value;
  bool *enabled;
};

//------------- Enumerables -------------//


//------------- Static pointers -------------//
//STATIC_POINTER(void, detour_attributes_init);
STATIC_POINTER(void, detour_disattributesentry_init);

STATIC_POINTER(void, hook_disattributesentry_init_ret);

//------------- Static variables -------------//
internal AttributeReplacer attributeReplacers[] = {
  { Attribute_HealthMax_ID, &patchSettings.health.iMax, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenAmount_ID, &patchSettings.health.iRegenAmount, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenInitialDelay_ID, &patchSettings.health.iRegenInitialDelay, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenRate_ID, &patchSettings.health.iRegenRate, &patchSettings.bHealthRegen },
  
  { Attribute_ManaMax_ID, &patchSettings.mana.iMax, &patchSettings.bManaRegen },
  { Attribute_ManaRegenAmount_ID, &patchSettings.mana.iRegenAmount, &patchSettings.bManaRegen },
  { Attribute_ManaRegenInitialDelay_ID, &patchSettings.mana.iRegenInitialDelay, &patchSettings.bManaRegen },
  { Attribute_ManaRegenStepTime_ID, &patchSettings.mana.iRegenStepTime, &patchSettings.bManaRegen },
};

internal bool gInitialized = false;

//------------- Functions -------------//


//------------- Detours -------------//
internal CDECL void Detour_Attributes_Init()
{
  if (patchSettings.bHealthRegen) {
    *Attribute_HealthRegenLimit_ID = *Attribute_HealthMax_ID;
  }
  
  if (patchSettings.bManaRegen) {
    *Attribute_ManaRegenAdditivePortion_ID = *Attribute_ManaMax_ID;
  }
}

internal CDECL void Detour_DisAttributesEntry_Init(DisAttributesEntry *entry, Attribute **attribute)
{
  if (!gInitialized) {
    Detour_Attributes_Init();
    gInitialized = true;
  }
  
  Attribute *attr = *attribute;
  for (int i = 0; i < arraysize(attributeReplacers); ++i) {
    u32 id = *attributeReplacers[i].id;
    int value = *attributeReplacers[i].value;
    bool enabled = *attributeReplacers[i].enabled;
    
    if (attr->id == id) {
      if (enabled && (value >= 0)) {
        attr->value = value;
      }
    }
  }
}

//------------- Hooks -------------//
//internal NAKED void Attributes_Init_Hook()
//{
//  __asm {
//    call Detour_Attributes_Init
//    ret
//  }
//}

internal NAKED void DisAttributesEntry_Init_Hook()
{
  __asm {
    push ebp
    mov ebp, esp
    push esi
    
    mov eax, [ebp+08h]
    push eax
    push ecx
    call Detour_DisAttributesEntry_Init
    pop ecx
    pop eax
    
    mov esi, ecx
    
    jmp [hook_disattributesentry_init_ret]
  }
}

//------------- Init -------------//
internal bool InitFullRegen()
{
  if (patchSettings.bHealthRegen || patchSettings.bManaRegen) {
    //if (!WriteDetour(detour_attributes_init, Attributes_Init_Hook, 0))
    //  return false;
    if (!WriteDetour(detour_disattributesentry_init, DisAttributesEntry_Init_Hook, 1))
      return false;
  }
  
  return true;
}

internal void InitFullRegenConfig()
{
  patchSettings.bHealthRegen = ini_read_bool(0, "bHealthRegen", true);
  patchSettings.health.iMax = ini_read_int(SETTINGS_HEALTH, "iMax", -1);
  patchSettings.health.iRegenAmount = ini_read_int(SETTINGS_HEALTH, "iRegenAmount", -1);
  patchSettings.health.iRegenInitialDelay = ini_read_int(SETTINGS_HEALTH, "iRegenInitialDelay", -1);
  patchSettings.health.iRegenRate = ini_read_int(SETTINGS_HEALTH, "iRegenRate", -1);
  
  patchSettings.bManaRegen = ini_read_bool(0, "bManaRegen", true);
  patchSettings.mana.iMax = ini_read_int(SETTINGS_MANA, "iMax", -1);
  patchSettings.mana.iRegenAmount = ini_read_int(SETTINGS_MANA, "iRegenAmount", -1);
  patchSettings.mana.iRegenInitialDelay = ini_read_int(SETTINGS_MANA, "iRegenInitialDelay", -1);
  patchSettings.mana.iRegenStepTime = ini_read_int(SETTINGS_MANA, "iRegenStepTime", -1);
}

#endif
