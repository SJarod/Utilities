#pragma once

#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * Initialize the leaks detection.
 * Call this function int the main() after including this file.
 */
void initMemleaksDebug();



