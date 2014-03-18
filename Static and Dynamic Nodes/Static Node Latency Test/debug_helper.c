#include "debug_helper.h"

#ifdef DBG_GLOBAL_ENABLED
char _dbg_debugBuffer[DBG_BUFF_SIZE];
#endif //DBG_ENABLED

void dbg_defaultFct(const char *dbgString)
{
    //simple_uart_putstring(dbgString);
}

DBG_Fct _dbg_dbgFunction = dbg_defaultFct;
void dbg_setDebugFct(DBG_Fct dbgFunct)
{
    _dbg_dbgFunction = dbgFunct;
}
