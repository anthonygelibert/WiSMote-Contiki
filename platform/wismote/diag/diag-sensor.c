#include "diag.h"

static char * getSensor(void)
{
  return "NOT IMPLEMENTED SENSOR";
}

COMMAND(sensor_command, "sensor", getSensor);
