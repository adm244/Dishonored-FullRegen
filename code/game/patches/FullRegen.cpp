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
  char *name;
  u32 *id;
  r32 *value;
  bool *enabled;
};

//------------- Static pointers -------------//
STATIC_POINTER(void, detour_disattributesentry_init);
STATIC_POINTER(void, hook_disattributesentry_init_ret);

//------------- Static variables -------------//
internal AttributeReplacer attributeReplacers[] = {
  { "HealthMax", Attribute_HealthMax_ID, &patchSettings.health.fMax, &patchSettings.bHealthRegen },
  { "HealthRegenAmount", Attribute_HealthRegenAmount_ID, &patchSettings.health.fRegenAmount, &patchSettings.bHealthRegen },
  { "HealthRegenInitialDelay", Attribute_HealthRegenInitialDelay_ID, &patchSettings.health.fRegenInitialDelay, &patchSettings.bHealthRegen },
  { "HealthRegenRate", Attribute_HealthRegenRate_ID, &patchSettings.health.fRegenRate, &patchSettings.bHealthRegen },
  
  { "ManaMax", Attribute_ManaMax_ID, &patchSettings.mana.fMax, &patchSettings.bManaRegen },
  { "ManaRegenAmount", Attribute_ManaRegenAmount_ID, &patchSettings.mana.fRegenAmount, &patchSettings.bManaRegen },
  { "ManaRegenInitialDelay", Attribute_ManaRegenInitialDelay_ID, &patchSettings.mana.fRegenInitialDelay, &patchSettings.bManaRegen },
  { "ManaRegenStepTime", Attribute_ManaRegenStepTime_ID, &patchSettings.mana.fRegenStepTime, &patchSettings.bManaRegen },
};

internal bool gInitialized = false;

//------------- Functions -------------//
internal void ChangeLimitAttributes()
{
  if (patchSettings.bHealthRegen) {
    *Attribute_HealthRegenLimit_ID = *Attribute_HealthMax_ID;
    Log(LogInfo, "Health regeneration limit is set to health maximum value.");
  }
  
  if (patchSettings.bManaRegen) {
    *Attribute_ManaRegenAdditivePortion_ID = *Attribute_ManaMax_ID;
    Log(LogInfo, "Mana regeneration portion is set to mana maximum value.");
  }
}

//struct AttributeInfo {
//  u32 unk00;
//  u32 unk04;
//  u32 cookedId; // 0x08
//  AttributeInfo *next;
//  char str[0];
//};
//
//internal u32 GetUncookedID(AttributeInfo *attributeInfo)
//{
//  u32 cookedID = attributeInfo->cookedId;
//  
//  //if (attributeInfo->cookedId & 1) {
//  //  IsUnicodeString(attributeInfo->str)
//  //}
//  
//  return ((attributeInfo->cookedId & (~1)) / 2);
//}

//------------- Detours -------------//
internal CDECL void Detour_DisAttributesEntry_Init(DisAttributesEntry *entry, Attribute **attribute)
{
  if (!gInitialized) {
    ChangeLimitAttributes();
    gInitialized = true;
    
    //DEBUG: dump all attributes
    // from 0x013AA038 to 0x013EA034
    //FILE *logFile = fopen("fullregen.log", "w");
    //assert(logFile != 0);
    //
    //AttributeInfo **attributesHashtable = (AttributeInfo **)0x013AA038;
    //for (int i = 0; i < 65535; ++i) {
    //  if (attributesHashtable[i] == 0)
    //    continue;
    //  
    //  AttributeInfo *attributeInfo = attributesHashtable[i];
    //  fprintf(logFile, "%ID: %i, Name: %s\n", GetUncookedID(attributeInfo), attributeInfo->str);
    //  
    //  attributeInfo = attributeInfo->next;
    //  while (attributeInfo) {
    //    fprintf(logFile, "%ID: %i, Name: %s\n", GetUncookedID(attributeInfo), attributeInfo->str);
    //    attributeInfo = attributeInfo->next;
    //  }
    //}
    //
    //fclose(logFile);
  }
  
  Attribute *attr = *attribute;
  for (int i = 0; i < arraysize(attributeReplacers); ++i) {
    char *name = attributeReplacers[i].name;
    u32 id = *attributeReplacers[i].id;
    r32 value = *attributeReplacers[i].value;
    bool enabled = *attributeReplacers[i].enabled;
    
    if (attr->id == id) {
      if (enabled && (value >= 0)) {
        attr->value = value;
        Log(LogInfo, "Name: %s,\tID: %d,\tChanged to: %f", name, attr->id, value);
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
    if (!WriteDetour(detour_disattributesentry_init, DisAttributesEntry_Init_Hook, 1)) {
      return false;
    }
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
  
  Log(LogInfo, "Health: bHealthRegen = %s", patchSettings.bHealthRegen ? "true" : "false");
  Log(LogInfo, "Health: fMax = %f", patchSettings.health.fMax);
  Log(LogInfo, "Health: fRegenAmount = %f", patchSettings.health.fRegenAmount);
  Log(LogInfo, "Health: fRegenInitialDelay = %f", patchSettings.health.fRegenInitialDelay);
  Log(LogInfo, "Health: fRegenRate = %f", patchSettings.health.fRegenRate);
  
  Log(LogInfo, "Mana: bManaRegen = %s", patchSettings.bManaRegen ? "true" : "false");
  Log(LogInfo, "Mana: fMax = %f", patchSettings.mana.fMax);
  Log(LogInfo, "Mana: fRegenAmount = %f", patchSettings.mana.fRegenAmount);
  Log(LogInfo, "Mana: fRegenInitialDelay = %f", patchSettings.mana.fRegenInitialDelay);
  Log(LogInfo, "Mana: fRegenStepTime = %f", patchSettings.mana.fRegenStepTime);
}

#endif
