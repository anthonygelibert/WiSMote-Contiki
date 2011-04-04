/* From MSP430-GCC */
#include <stdio.h>
#include <string.h>

/* From CONTIKI */
#include "contiki.h"
#include "watchdog.h"

/* From this platform */
#include "diag.h"

/** Size of the version buffer. */
#define PROCESS_SIZE 500
/** Buffer for the answer. */
static char process[PROCESS_SIZE];
/* From process.c :
#define PROCESS_STATE_NONE        0
#define PROCESS_STATE_RUNNING     1
#define PROCESS_STATE_CALLED      2
*/
/** ASCII version of the process state */
static char *process_state[4] = {"NONE","RUNNING","CALLED","UNKNOWN"};

/**
 * Translate the byte 'state' of a process into an ascii string.
 *
 * \param state raw value
 * \return ASCII string
 */
static CC_INLINE char * getASCIIState(char state)
{
  return process_state[(state < 0 || state > 3 ? 3 : state)];
}

/**
 * Translate the byte 'poll' of a process into an ascii string.
 *
 * \param poll raw value
 * \retval "!" if polling is required
 * \retval "" if polling isn't required.
 */
static CC_INLINE char * getASCIIPoll(char poll)
{
  return (poll ? "!" : "");
}

/**
 * List all the process and autoprocess
 */
static void listAllProcesses()
{
  struct process *  list = PROCESS_LIST();
  struct process ** autolist = (struct process **)autostart_processes;
  void * previous = NULL;
#define BUFFER_SIZE 50
  char buffer[BUFFER_SIZE];

#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  snprintf(process,PROCESS_SIZE,"List of process:\n");
#endif
  while(list) {
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
    snprintf(buffer, BUFFER_SIZE, " - %s [%s%s]\n",
        PROCESS_NAME_STRING(list),
        getASCIIState(list->state),
        getASCIIPoll(list->needspoll));
#endif
    strncat(process, buffer, BUFFER_SIZE);

    if (list != previous)
    {
      watchdog_periodic();
      previous = list;
    }
    list = list->next;
  }

  watchdog_periodic();

#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  strncat(process,"\nList of autoprocess:\n",22);
#endif
  while (*autolist)
  {
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
    snprintf(buffer, BUFFER_SIZE, " - %s [%s%s]\n",
        PROCESS_NAME_STRING(*autolist),
        getASCIIState((*autolist)->state),
        getASCIIPoll(list->needspoll));
#endif
    strncat(process, buffer, BUFFER_SIZE);
    if (autolist != previous)
    {
      watchdog_periodic();
      previous = autolist;
    }
    autolist++;
  }

  watchdog_periodic();
}

/**
 * Get the list of processes and auto-processes.
 *
 * @return The list
 */
static char * getProcess(void)
{
  listAllProcesses();
  process[PROCESS_SIZE - 1] = '\0';
  return process;
}

/** Define the "process" command. */
COMMAND(process_command, "process", getProcess);
