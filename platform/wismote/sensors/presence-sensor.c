#include <signal.h>

#include "presence-sensor.h"
#include "lib/sensors.h"
#include "iohandlers.h"
#include "leds.h"

const struct sensors_sensor presence_sensor;

static void myHandler(void);

HWCONF_PIN(PRESENCE, 1, 2)
HWCONF_IRQ(PRESENCE, 1, 2, myHandler)

/*---------------------------------------------------------------------------*/

static void
myHandler(void)
{
  PRESENCE_RESISTOR_PULL_SWAP();
  PRESENCE_IRQ_EDGE_SWAP();
  sensors_changed(&presence_sensor);
}

/*---------------------------------------------------------------------------*/

static int
value(int type)
{
  return PRESENCE_READ() ;
}

/*---------------------------------------------------------------------------*/

static int
configure(int type, int c)
{
  switch (type) {
    case SENSORS_ACTIVE:
      if (c) {
        PRESENCE_RESISTOR_ENABLE();
        PRESENCE_RESISTOR_PULL_DOWN();
        PRESENCE_IRQ_EDGE_SELECTU();
        PRESENCE_SELECT_IO();
        PRESENCE_MAKE_INPUT();
        PRESENCE_SET_HANDLER();
        PRESENCE_ENABLE_IRQ();

      } else {
        PRESENCE_DISABLE_IRQ();
        PRESENCE_RESET_HANDLER();
        PRESENCE_RESISTOR_ENABLE();
        PRESENCE_RESISTOR_PULL_DOWN();
      }
      return 1;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

static int
status(int type)
{
  switch (type) {
    case SENSORS_ACTIVE:
    case SENSORS_READY:
      return PRESENCE_IRQ_ENABLED();
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

SENSORS_SENSOR(presence_sensor, PRESENCE_SENSOR, value, configure, status);

/** @} */
