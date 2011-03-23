#include "diag.h"

/**
 * Get the list of processes and auto-processes.
 *
 * @return The list
 */
static char * getProcess(void)
{
  return "NOT IMPLEMENTED PROCESS";
}

/** Define the "process" command. */
COMMAND(process_command, "process", getProcess);
