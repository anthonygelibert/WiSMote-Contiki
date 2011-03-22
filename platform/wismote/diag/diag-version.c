#include "diag.h"

static char * getVersion(void)
{
  return "NOT IMPLEMENTED VERSION";
}

COMMAND(version_command, "version", getVersion);
