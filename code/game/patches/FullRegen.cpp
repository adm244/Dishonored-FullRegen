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
  r32 *value;
  bool *enabled;
};

//------------- Static pointers -------------//
STATIC_POINTER(void, detour_disattributesentry_init);
STATIC_POINTER(void, hook_disattributesentry_init_ret);

//------------- Static variables -------------//
internal AttributeReplacer attributeReplacers[] = {
  { Attribute_HealthMax_ID, &patchSettings.health.fMax, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenAmount_ID, &patchSettings.health.fRegenAmount, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenInitialDelay_ID, &patchSettings.health.fRegenInitialDelay, &patchSettings.bHealthRegen },
  { Attribute_HealthRegenRate_ID, &patchSettings.health.fRegenRate, &patchSettings.bHealthRegen },
  
  { Attribute_ManaMax_ID, &patchSettings.mana.fMax, &patchSettings.bManaRegen },
  { Attribute_ManaRegenAmount_ID, &patchSettings.mana.fRegenAmount, &patchSettings.bManaRegen },
  { Attribute_ManaRegenInitialDelay_ID, &patchSettings.mana.fRegenInitialDelay, &patchSettings.bManaRegen },
  { Attribute_ManaRegenStepTime_ID, &patchSettings.mana.fRegenStepTime, &patchSettings.bManaRegen },
};

internal bool gInitialized = false;

//------------- Functions -------------//
internal void ChangeLimitAttributes()
{
  if (patchSettings.bHealthRegen) {
    *Attribute_HealthRegenLimit_ID = *Attribute_HealthMax_ID;
  }
  
  if (patchSettings.bManaRegen) {
    *Attribute_ManaRegenAdditivePortion_ID = *Attribute_ManaMax_ID;
  }
}

//------------- Detours -------------//
internal CDECL void Detour_DisAttributesEntry_Init(DisAttributesEntry *entry, Attribute **attribute)
{
  if (!gInitialized) {
    ChangeLimitAttributes();
    gInitialized = true;
  }
  
  Attribute *attr = *attribute;
  for (int i = 0; i < arraysize(attributeReplacers); ++i) {
    u32 id = *attributeReplacers[i].id;
    r32 value = *attributeReplacers[i].value;
    bool enabled = *attributeReplacers[i].enabled;
    
    if (attr->id == id) {
      if (enabled && (value >= 0)) {
        attr->value = value;
      }
    }
  }
}

//------------- Hooks -------------//
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
    if (!WriteDetour(detour_disattributesentry_init, DisAttributesEntry_Init_Hook, 1))
      return false;
  }
  
  return true;
}

internal void InitFullRegenConfig()
{
  patchSettings.bHealthRegen = ini_read_bool(0, "bHealthRegen", true);
  patchSettings.health.fMax = ini_read_float(SETTINGS_HEALTH, "fMax", -1.0f);
  patchSettings.health.fRegenAmount = ini_read_float(SETTINGS_HEALTH, "fRegenAmount", -1.0f);
  patchSettings.health.fRegenInitialDelay = ini_read_float(SETTINGS_HEALTH, "fRegenInitialDelay", -1.0f);
  patchSettings.health.fRegenRate = ini_read_float(SETTINGS_HEALTH, "fRegenRate", -1.0f);
  
  patchSettings.bManaRegen = ini_read_bool(0, "bManaRegen", true);
  patchSettings.mana.fMax = ini_read_float(SETTINGS_MANA, "fMax", -1.0f);
  patchSettings.mana.fRegenAmount = ini_read_float(SETTINGS_MANA, "fRegenAmount", -1.0f);
  patchSettings.mana.fRegenInitialDelay = ini_read_float(SETTINGS_MANA, "fRegenInitialDelay", -1.0f);
  patchSettings.mana.fRegenStepTime = ini_read_float(SETTINGS_MANA, "fRegenStepTime", -1.0f);
}

#endif
