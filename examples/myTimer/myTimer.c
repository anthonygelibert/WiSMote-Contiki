/******************************************************************************
 * Copyright 2010, Anthony Gelibert <anthony.gelibert@me.com>                 *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/

/**
 * This simple example increment a timer all the seconds.
 */
#include <stdio.h>
#include "contiki.h"

#define INTERVAL CLOCK_CONF_SECOND

PROCESS(myTimer_process, "Timer process");
AUTOSTART_PROCESSES(&myTimer_process);

static struct etimer et;
static int current_second = 0;
static int current_min = 0;
static int current_hour = 0;

/**
 * Increment the counter of one second and display the current time.
 */
static void timeout_handler(void)
{
    if ((++current_second)>=60)
    {
        current_second = 0;
        if ((++current_min)>=60)
        {
            current_min = 0;
            current_hour++;
        }
    }
    printf("%02d:%02d:%02d\n",current_hour,current_min,current_second);
}

/**
 * Set an interval of 1s. on the etimer and wait until the event.
 */
PROCESS_THREAD(myTimer_process, ev, data)
{
    PROCESS_BEGIN();
    printf("Timer process started\n");

    printf("I start the timer (1 s.)\n");
    while(1) 
    {
        etimer_set(&et, INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        timeout_handler();
    }
    PROCESS_END();
}
