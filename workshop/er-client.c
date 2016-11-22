/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Erbium (Er) CoAP client.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-engine.h"
#include "dev/hih6130.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

/* Define the border router settings. */
#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 0x1)
#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(8181)
/* Interval for setting data to the server. */
#define TOGGLE_INTERVAL 5
/* Path info and cloud credentials. */
#define URL_PATH "/target"
#define DEVICE_ID "42beac85-ec65-4e16-9b7a-df4ffa85b17d"
#define USER_TOKEN "Bearer nlm5mMJNTu6NayCtB7cwU3IGxbOcQI28Iw8k9V7mm6Q42lKnS5QNf11WrrNhgRku"

PROCESS(er_example_client, "Erbium Example Client");
AUTOSTART_PROCESSES(&er_example_client);

uip_ipaddr_t server_ipaddr;
static struct etimer et; 

/* Global variables for reading the sensor. */
uint16_t rh = 0;
uint16_t temperature = 0;


/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;

  int len = coap_get_payload(response, &chunk);

  printf("|%.*s", len, (char *)chunk);
}


PROCESS_THREAD(er_example_client, ev, data)
{
  PROCESS_BEGIN();
  /* ACTIVATE the HIH6130. */
  SENSORS_ACTIVATE(hih6130);

  static coap_packet_t request[1];      /* This way the packet can be treated as pointer as usual. */

  SERVER_NODE(&server_ipaddr);

  /* receives all CoAP messages. */
  coap_init_engine();
  etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      printf("--Sending readings...--\n");
        
      /*********   Get the sensor info.  *************/

      if(hih6130.configure(HIH6130_MEASUREMENT_REQUEST, 0) >= 0) {   
        if(hih6130.configure(HIH6130_SENSOR_READ, 0) >= 0) {
          rh = hih6130.value(HIH6130_VAL_HUMIDITY) /1000 ;
          temperature = hih6130.value(HIH6130_VAL_TEMP) /1000 ;
        }
      } 

      /**********************************************/

      printf("%u,%u", rh,temperature); //Debug to console

        /*************** First message with temperature info. ************************/
        
        /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URL_PATH);
        
        char msg[148];
        
        snprintf((char *)msg, REST_MAX_CHUNK_SIZE, "%s,%s,%s,%u", DEVICE_ID, USER_TOKEN,"temperature",temperature);

        coap_set_payload(request, (uint8_t *)msg, sizeof(DEVICE_ID)+sizeof(USER_TOKEN)+sizeof("temperature")+2);
        
        PRINT6ADDR(&server_ipaddr);
        PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
        
        COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                              client_chunk_handler);
        /***********************************************************************/

        /*************** Second message with humidity info. ************************/
        
        /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URL_PATH);
        
        char msg2[148];
        
        snprintf((char *)msg2, REST_MAX_CHUNK_SIZE, "%s,%s,%s,%u", DEVICE_ID, USER_TOKEN,"humidity",rh);

        coap_set_payload(request, (uint8_t *)msg2, sizeof(DEVICE_ID)+sizeof(USER_TOKEN)+sizeof("humidity")+2);
        
        PRINT6ADDR(&server_ipaddr);
        PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
        
        COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                              client_chunk_handler);
        /***********************************************************************/

                
      printf("\n--Done--\n");
      
      etimer_reset(&et);
    }
  }
    

  PROCESS_END();
}
