/*
 * Copyright (c) 2016, relayr http://relayr.io/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
/**
 * \file
 *         hello-world
 * \author
 *         Nenad Ilic <nenad@relayr.io>
 */
/*---------------------------------------------------------------------------*/
#include "contiki-conf.h"
#include "contiki.h"
#include "dev/leds.h"
#include "rest-engine.h"
#include "hih6130.h"

#include <stdio.h>
/*---------------------------------------------------------------------------*/
#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(M, ...) printf("DEBUG %s:%d: " M, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
#define LOOP_INTERVAL       (CLOCK_SECOND * 1)
static struct etimer et;
static struct etimer et_wait_measurmenet;
/*---------------------------------------------------------------------------*/
extern resource_t res_hello;
extern resource_t res_temp;
extern resource_t res_hum;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  hih6130.configure(HIH6130_ACTIVE, 1);
  /* Initialize the REST engine. */
  rest_init_engine();

  rest_activate_resource(&res_hello, "hello");
  rest_activate_resource(&res_temp, "temp");
  rest_activate_resource(&res_hum, "hum");

  etimer_set(&et, LOOP_INTERVAL);
  leds_on(STATUS_LED);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    printf("Hello, world!\n");
    leds_toggle(STATUS_LED);

    if(hih6130.configure(HIH6130_MEASUREMENT_REQUEST, 0) >= 0) {
      etimer_set(&et_wait_measurmenet, CLOCK_SECOND / 10); /* set timeout */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_wait_measurmenet));
      if(hih6130.configure(HIH6130_SENSOR_READ, 0) >= 0) {
        res_temp.trigger();
        res_hum.trigger();
      }
    }

    etimer_set(&et, LOOP_INTERVAL);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
