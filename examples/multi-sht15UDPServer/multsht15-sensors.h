#ifndef __MULTSHT15_SENSORS_H__
#define __MULTSHT15_SENSORS_H__

#include "sht1x-sensor.h"

EXPORT_SHT1X(upper_sensor);
EXPORT_SHT1X(middle_upper_sensor);
EXPORT_SHT1X(middle_lower_sensor);
EXPORT_SHT1X(lower_sensor);

/**
 * Get a pointer on the specified sensor.
 *
 * \note 1 = upper, 2 = middle upper, 3 = middle lower and 4 = lower.
 * \param num Sensor ID.
 * \return Pointer on the sensor.
 */
struct sensors_sensor * getSensorByNum(const int num);

#endif /* __MULTSHT15_SENSORS_H__ */
