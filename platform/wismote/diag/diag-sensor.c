#include "diag.h"

/**
 * Return the list of available sensors, their states...
 *
 * @return The list
 */
static char * getSensor(void)
{
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
#warning "ASCII output not implemented for the sensor diagnostic"
  return "NOT YET IMPLEMENTED";
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
#warning "XML output not implemented for the sensor diagnostic"
  return "NOT YET IMPLEMENTED";
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
#warning "JSON output not implemented for the sensor diagnostic"
  return "NOT YET IMPLEMENTED";
#endif
}

/** Define the "sensor" command. */
COMMAND(sensor_command, "sensor", getSensor);
