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

#ifndef _LOGFILE_H_
#define _LOGFILE_H_

enum LogType_e {
  LogInfo = 0,
  LogWarning,
  LogError,
};

struct LogFile_t {
  FILE *handle;
  char filepath[MAX_PATH];
};

internal LogFile_t logFile;

internal const char *logTypeName[] = {
  {"INFO"},
  {"WARNING"},
  {"ERROR"}
};

internal bool Log(LogType_e type, char *format, ...)
{
  assert(format != 0);
  
  if (!logFile.handle) {
    return false;
  }
  
  va_list list;
  va_start(list, format);
  
  char buffer[KB(16)];
  buffer[0] = '\0';
  
  sprintf_s(buffer, sizeof(buffer), "[%s]: %s\n", logTypeName[type], format);
  vfprintf(logFile.handle, buffer, list);
  fflush(logFile.handle);
  
  va_end(list);
  
  return true;
}

internal bool GetLogFilePath(HMODULE module, char *buffer, size_t size)
{
  assert(module != 0);
  assert(buffer != 0);
  assert(size > 0);
  
  buffer[0] = '\0';
  
  size_t length = GetModuleFileNameA(module, buffer, size);
  if (length == 0) {
    return false;
  }
  
  //HACK(adm244): replace dll extension
  buffer[length - 3] = 'l';
  buffer[length - 2] = 'o';
  buffer[length - 1] = 'g';
  
  return true;
}

internal bool InitLogFile(HMODULE module, bool append)
{
  assert(module != 0);
  
  if (!GetLogFilePath(module, logFile.filepath, sizeof(logFile.filepath))) {
    OutputDebugStringA("InitLogFile: Could not get a log filepath.");
    return false;
  }
  
  logFile.handle = fopen(logFile.filepath, append ? "a" : "w");
  if (!logFile.handle) {
    OutputDebugStringA("InitLogFile: Could not open a log file.");
    return false;
  }
  
  return true;
}

internal void DeinitLogFile()
{
  if (logFile.handle) {
    fclose(logFile.handle);
  }
}

#endif
