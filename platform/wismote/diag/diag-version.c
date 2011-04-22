/* From MSP430-GCC */
#include <stdio.h>
#include <string.h>

/* From this platform */
#include "diag.h"

/* From CONTIKI */
#include "contiki-version.h"

/** Size of the version buffer. */
#define VERSION_SIZE 500
/** Buffer for the answer. */
static char version[VERSION_SIZE];

/**
 * Return the list of versions.
 *
 * @return The list
 */
static char * getVersion(void)
{
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  snprintf(version,VERSION_SIZE,"List of versions:\n - CONTIKI core: %s\n",CONTIKI_VERSION_STRING);
  version[VERSION_SIZE - 1] = '\0';
  return version;
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
  snprintf(version,VERSION_SIZE,"<wismote><versions><module name=\"CONTIKI core\" version=\"%s\"/>", CONTIKI_VERSION_STRING);
  strncat(version,"</versions></wismote>",VERSION_SIZE);
  version[VERSION_SIZE - 1] = '\0';
  return version;
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
  snprintf(version, VERSION_SIZE,"{\"wismote\":{\"version\":{\"module\":[{\"name\":\"CONTIKI core\",\"version\":\"%s\"}]}}}", CONTIKI_VERSION_STRING);
  version[VERSION_SIZE - 1] = '\0';
  return version;
#endif
}

/** Define the "version" command. */
COMMAND(version_command, "version", getVersion);
