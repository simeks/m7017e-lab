#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#undef min // Removing annoying macros from windows.h

#define vsnprintf _vsnprintf
#endif

#include "debug.h"

#endif // __COMMON_H__
