#include <signal.h>

#include "dev/button-sensor.h"
#include "dev/watchdog.h"
#include "lib/sensors.h"
#include "iohandlers.h"

const struct sensors_sensor button_sensor;
static struct timer debouncetimer;

static int
status(int type);

void
myHandler(void)
{
  if (timer_expired(&debouncetimer)) {
    timer_set(&debouncetimer, CLOCK_SECOND / 4);
    sensors_changed(&button_sensor);
    LPM4_EXIT;
  }
}

HWCONF_PIN(BUTTON, 1, 4)
HWCONF_IRQ(BUTTON, 1, 4, myHandler)

static int
value(int type)
{
  return BUTTON_READ() || !timer_expired(&debouncetimer);
}

static int
configure(int type, int c)
{
  switch (type) {
    case SENSORS_HW_INIT:
      timer_set(&debouncetimer, 0);
      BUTTON_IRQ_EDGE_SELECTD();
      BUTTON_SELECT();
      BUTTON_MAKE_INPUT();
      return 1;
    case SENSORS_ACTIVE:
      if (c) {
        BUTTON_SET_HANDLER();
        BUTTON_ENABLE_IRQ();
      } else {
        BUTTON_RESET_HANDLER();
        BUTTON_DISABLE_IRQ();
      }
      return 1;
  }
  return 0;
}

static int
status(int type)
{
  switch (type) {
    case SENSORS_ACTIVE:
    case SENSORS_READY:
      return BUTTON_IRQ_ENABLED();
  }
  return 0;
}
SENSORS_SENSOR(button_sensor, BUTTON_SENSOR, value, configure, status);
