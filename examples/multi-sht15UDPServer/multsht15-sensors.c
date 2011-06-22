#include <signal.h>

#include "contiki-conf.h"
#include "lib/sensors.h"

#include "multsht15-sensors.h"
#include "sensors/sht1x-sensor.h"

NEW_SHT1X(upper_sensor, "SHT15 UPPER",       6,4,6,5, 6,1);
NEW_SHT1X(middle_upper_sensor, "SHT15 MIDDLE UPPER",6,4,6,5, 6,0);
NEW_SHT1X(middle_lower_sensor, "SHT15 MIDDLE LOWER",6,4,6,5, 6,2);
NEW_SHT1X(lower_sensor, "SHT15 LOWER",       6,4,6,5, 6,3);

/**
 * Get a pointer on the specified sensor.
 *
 * \note 1 = upper, 2 = middle upper, 3 = middle lower and 4 = lower.
 * \param num Sensor ID.
 * \return Pointer on the sensor.
 */
struct sensors_sensor * getSensorByNum(const int num) {
  switch (num) {
    case 1  : return &upper_sensor;
    case 2  : return &middle_upper_sensor;
    case 3  : return &middle_lower_sensor;
    case 4  :
    default : return &lower_sensor;
  }
}
