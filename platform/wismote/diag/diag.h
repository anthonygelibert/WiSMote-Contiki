#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

/* From CONTIKI */
#include "contiki.h"

/* Export the name of the process. */
PROCESS_NAME(diagnostic_process);

/** Define the type "command handler". */
typedef char * (*cmdHandler)(void);

/** A command. */
struct command {
    /** Name. */
    char name[10];
    /** Handler. */
    cmdHandler handler;
};

/** Export the command. */
#define COMMAND_NAME(sym) extern struct command sym

/** Define a new command. */
#define COMMAND(sym, name,handler)  struct command sym = { name , handler }

#endif
