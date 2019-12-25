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

#ifndef _GAME_PATCHES_H_
#define _GAME_PATCHES_H_

#define INI_STORAGE_SIZE 1024
internal char ini_storage[INI_STORAGE_SIZE];

struct PatchHealth {
  r32 fMax;
  r32 fRegenAmount;
  r32 fRegenInitialDelay;
  r32 fRegenRate;
};

struct PatchMana {
  r32 fMax;
  r32 fRegenAmount;
  r32 fRegenInitialDelay;
  r32 fRegenStepTime;
};

struct PatchSettings {
  bool bHealthRegen;
  bool bManaRegen;
  
  PatchHealth health;
  PatchMana mana;
};

#define SETTINGS_HEALTH "Health"
#define SETTINGS_MANA "Mana"

internal PatchSettings patchSettings = {0};

#include "patches\FullRegen.cpp"

internal bool GetConfigFilePath(char *buffer, size_t size)
{
  size_t length = GetModuleFileNameA(thisModule, buffer, size);
  if (length == 0) {
    return false;
  }
  
  //HACK(adm244): replace dll extension
  buffer[length - 3] = 'i';
  buffer[length - 2] = 'n';
  buffer[length - 1] = 'i';
  
  return true;
}

internal bool ReadConfig()
{
  char filepath[MAX_PATH];
  if (!GetConfigFilePath(filepath, MAX_PATH)) {
    OutputDebugStringA("ReadConfig: Cannot get filepath for a config.");
    filepath[0] = '\0';
  }
  
  ini_init(filepath, ini_storage, INI_STORAGE_SIZE);
  bool result = ini_parse();
  
  InitFullRegenConfig();
  
  return result;
}

internal bool InitPatches()
{
  if (!ReadConfig()) {
    OutputDebugStringA("InitPatches: Some issues reading config file.");
  }
  
  if (!InitFullRegen()) {
    OutputDebugStringA("InitPatches: Couldn't initialize FullRegen patch.");
    return false;
  }
  
  return true;
}

#endif
