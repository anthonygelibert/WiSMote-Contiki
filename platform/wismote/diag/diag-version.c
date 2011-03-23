#include "diag.h"

/**
 * Return the list of versions.
 *
 * @return The list
 */
static char * getVersion(void)
{
  return "NOT IMPLEMENTED VERSION";
}

/** Define the "version" command. */
COMMAND(version_command, "version", getVersion);
