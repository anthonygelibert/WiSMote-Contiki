#include <signal.h>

#include "contiki-conf.h"
#include "lib/sensors.h"

#include "multsht15-sensors.h"
#include "sensors/sht1x-sensor.h"

NEW_SHT1X(upper_sensor, "SHT15 UPPER",       6,4,6,5, 6,1);
NEW_SHT1X(middle_upper_sensor, "SHT15 MIDDLE UPPER",6,4,6,5, 6,0);
NEW_SHT1X(middle_lower_sensor, "SHT15 MIDDLE LOWER",6,4,6,5, 6,2);
NEW_SHT1X(lower_sensor, "SHT15 LOWER",       6,4,6,5, 6,3);

