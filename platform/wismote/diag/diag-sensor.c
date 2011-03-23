#include "diag.h"

/**
 * Return the list of available sensors, their states...
 *
 * @return The list
 */
static char * getSensor(void)
{
  return "NOT IMPLEMENTED SENSOR";
}

/** Define the "sensor" command. */
COMMAND(sensor_command, "sensor", getSensor);
