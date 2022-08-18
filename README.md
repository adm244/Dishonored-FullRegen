# Dishonored FullRegen
A native mod for **Dishonored** that changes health/mana regeneration limits to allow full regeneration.

You can also customize regeneration parameters such as:
* Maximum value (how much health/mana points player have in total)
* Regeneration amount (how much points being regenerated on each step)
* Regeneration delay (a delay before regeneration begins in seconds)
* Regeneration step (how much time passes between steps in seconds)

Besides these, you can optionally turn health/mana regeneration on and off.
Note that turning regeneration off means that it will have a default (vanilla) behavior, it won't turn it off completely.

## Compilation
Compilation is done as a **single unit**, just compile **main.cpp** as 32-bit dll and it should work.
For more details see `tools/build.bat` file.

## Installation
Copy "dinput8.dll" and "NativeMods" folder into "Dishonored\Binaries\Win32".

## Supported versions
Works on the latest version that is prepared for DLC **The Knife of Dunwall** and DLC **The Brigmore Witches**.
These DLC's are **NOT** required though.

## Notes
* If you're using GOG version do not launch game through Galaxy client (it's a known issue)
* DLL injector is **NOT** included in this repository
* No fool-proof checks, make sure you know what you're doing
* No dynamic memory allocations
* Game assets are untouched, all changes are made in memory at runtime

## License
This is free and unencumbered software released into the **public domain**.

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
