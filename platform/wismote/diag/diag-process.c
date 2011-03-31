/* From this platform */
#include "diag.h"

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
//  printf("Starting");
  while(*processes != NULL) {
//    printf(" '%s'", PROCESS_NAME_STRING(*processes));
    processes++;
  }
//  putchar('\n');
}
