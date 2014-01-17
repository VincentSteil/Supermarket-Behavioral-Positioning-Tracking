#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

#define DBG_GLOBAL_ENABLED
#define DBG_BUFF_SIZE 128

#include <stdint.h>

/**
 * Module enabling easy-way for printf-like debugging. Typical usage:
 *
 * During initialization:
 *  dbg_setDebugFct(debugFct);
 *
 * From now on, if global DBG_ENABLED is set, dbg_debugGlobal() called like printf will result in
 * invocation of debugFct with formatted string as argument.
 * When both DBG_ENABLED and DBG_MODULE_ENABLED are set, dbg_debugModule() will do the same.
 * @note: DBG_MODULE_ENABLED has to be set before this file header inclusion, as below:
 *  #define DBG_MODULE_ENABLED
 *  #include "debug_helper.h"
 *
 */

typedef void (*DBG_Fct)(const char *dbgString);

/**
 * @brief dbg_setDebugFct - set function used for debugging
 * @param dbgFunct
 */
void dbg_setDebugFct(DBG_Fct dbgFunct);

/**
 * @brief dbg_dbgFunction - variable holding poitner to the debugging function.
 * Modifable with @sa dbg_setDebugFct();
 */
extern DBG_Fct _dbg_dbgFunction;

#ifdef DBG_GLOBAL_ENABLED
#include <stdio.h>
extern char _dbg_debugBuffer[DBG_BUFF_SIZE];
#define dbg_debugGlobal(...) do {               \
    sprintf(_dbg_debugBuffer, __VA_ARGS__);     \
    _dbg_dbgFunction(_dbg_debugBuffer);         \
    } while (0);
#else
#define dbg_debugGlobal(...)
#endif //DBG_GLOBAL_ENABLED

#ifdef DBG_MODULE_ENABLED
#define dbg_debugModule(...) dbg_debugGlobal(__VA_ARGS__)
#else
#define dbg_debugModule(...)
#endif //DBG_GLOBAL_ENABLED

#endif // DEBUG_HELPER_H
