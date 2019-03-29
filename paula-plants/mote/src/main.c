/*
 * Copyright (C)
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "xtimer.h"
#include "timex.h"
#include "coap.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#define COAP_URL        "/count"
#define DEST_ADDRESS    "fe80::3050:69ff:febd:6d5d"
#define DEST_PORT       "5683"
/* Use default CoAP port */

#define PERIOD            (1U * US_PER_SEC)


int main(void)
{
  xtimer_ticks32_t last_wakeup = xtimer_now();

  printf("Running internet of plants application\n");

  char payload[64] = "";
  char data[64] = "\"light\" : 2";
  sprintf(payload, "{%s}", data);

  while(1) {
      printf("Sending sensor value\n");
      gcoap_cli_send(COAP_URL, payload, DEST_ADDRESS, DEST_PORT);

      xtimer_periodic_wakeup(&last_wakeup, PERIOD);
  }

  return 0;
}
