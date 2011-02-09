#include "sensors/hwconf.h"
#include "lib/sensors.h"
#include "dev/button-sensor.h"
#include <signal.h>

const struct sensors_sensor button_sensor;

static struct timer debouncetimer;
static int status(int type);

HWCONF_PIN(BUTTON, 1, 4);
HWCONF_IRQ(BUTTON, 1, 4);

/** NOT_YET_DOCUMENTED_PTV */
interrupt(PORT1_VECTOR)
irq_p2(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  if (BUTTON_CHECK_IRQ()) {
    if (timer_expired(&debouncetimer)) {
      timer_set(&debouncetimer, CLOCK_SECOND / 4);
      sensors_changed(&button_sensor);
      LPM4_EXIT;
    }
  }
  P1IFG = 0x00;
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/** NOT_YET_DOCUMENTED_PTV */
static int
value(int type)
{
  return BUTTON_READ() || !timer_expired(&debouncetimer);
}

/** NOT_YET_DOCUMENTED_PTV */
static int
configure(int type, int c)
{
  switch (type) {
    case SENSORS_ACTIVE:
      if (c) {
        if (!status(SENSORS_ACTIVE)) {
          timer_set(&debouncetimer, 0);
          BUTTON_IRQ_EDGE_SELECTD();

          BUTTON_SELECT();
          BUTTON_MAKE_INPUT();

          BUTTON_ENABLE_IRQ();
        }
      } else {
        BUTTON_DISABLE_IRQ();
      }
      return 1;
  }
  return 0;
}

/** NOT_YET_DOCUMENTED_PTV */
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

/** NOT_YET_DOCUMENTED_PTV */
SENSORS_SENSOR(button_sensor, BUTTON_SENSOR,
    value, configure, status);
