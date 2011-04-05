/* From MSP430-GCC */
#include <stdio.h>
#include <string.h>

/* From platform */
#include "diag.h"

/* From CONTIKI */
#include "contiki.h"
#include "watchdog.h"
#include "lib/sensors.h"


/** Size of the version buffer. */
#define SENSORS_SIZE 500
/** Buffer for the answer. */
static char sensors[SENSORS_SIZE];

/**
 * Return the list of available sensors, their states...
 *
 * @return The list
 */
static char * getSensor(void)
{
  struct sensors_sensor * sensor = sensors_first();
#define BUFFER_SIZE 100
  char buffer[BUFFER_SIZE];
  void * previous = NULL;

#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  snprintf(sensors,SENSORS_SIZE,"List of sensors:\n");
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
  snprintf(sensors,SENSORS_SIZE, "<wismote><sensors>");
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
  snprintf(sensors,SENSORS_SIZE,"{\"wismote\":{\"sensors\":{\"sensor\":[");
#endif

  watchdog_periodic();

  while (sensor != NULL) {
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
  snprintf(buffer, BUFFER_SIZE," - %s\n",sensor->type);
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
  snprintf(buffer,BUFFER_SIZE,"<sensor type=\"%s\"/>", sensor->type);
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
  snprintf(buffer,BUFFER_SIZE,"{\"type\":\"%s\"},",sensor->type);
#endif
    strncat(sensors, buffer, BUFFER_SIZE);
    sensor = sensors_next(sensor);
    if (sensor != previous)
    {
      watchdog_periodic();
      previous = sensor;
    }
  }

#if DIAGNOSTIC_OUTPUT == XML_OUTPUT
  strncat(sensors,"</sensors></wismote>",21);
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
  strncat(sensors,"]}}}",5);
#endif
  return sensors;
}

/** Define the "sensor" command. */
COMMAND(sensor_command, "sensor", getSensor);
