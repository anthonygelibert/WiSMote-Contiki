/* From this platform */
#include "diag.h"

/** Size of the version buffer. */
#define PROCESS_SIZE 500
/** Buffer for the answer. */
static char process[PROCESS_SIZE];

/**
 * Get the list of processes and auto-processes.
 *
 * @return The list
 */
static char * getProcess(void)
{
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
#warning "ASCII output not implemented for the process diagnostic"
  return "NOT YET IMPLEMENTED";
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
#warning "XML output not implemented for the process diagnostic"
  return "NOT YET IMPLEMENTED";
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
#warning "JSON output not implemented for the process diagnostic"
  return "NOT YET IMPLEMENTED";
#endif
}

/** Define the "process" command. */
COMMAND(process_command, "process", getProcess);

static void
print_processes(struct process * const processes[])
{
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  sprintf(process,"List of process:\n");
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
  sprintf(process,"<wismote><process>");
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT

#endif

  while(*processes != NULL) {
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
    sprintf(process,"%s - %s\n",PROCESS_NAME_STRING(*processes));
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
#endif
    processes++;
  }

#if DIAGNOSTIC_OUTPUT == XML_OUTPUT

#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
#endif
}
