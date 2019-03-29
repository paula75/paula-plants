
/*
 * Copyright (C)
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */


#include "od.h"
#include "net/gcoap.h"
#include "coap.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote)
{
    (void)remote;       /* not interested in the source currently */

    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
        return;
    }
    else if (req_state == GCOAP_MEMO_ERR) {
        printf("gcoap: error in response\n");
        return;
    }

    char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS)
                            ? "Success" : "Error";
    printf("gcoap: response %s, code %1u.%02u", class_str,
                                                coap_get_code_class(pdu),
                                                coap_get_code_detail(pdu));
    if (pdu->payload_len) {
        unsigned content_type = coap_get_content_type(pdu);
        if (content_type == COAP_FORMAT_TEXT
                || content_type == COAP_FORMAT_LINK
                || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE
                || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE) {
            /* Expecting diagnostic payload in failure cases */
            printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
                                                          (char *)pdu->payload);
        }
        else {
            printf(", %u bytes\n", pdu->payload_len);
            od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
        }
    }
    else {
        printf(", empty payload\n");
    }
}

/* Send message to server */
static size_t _send(uint8_t *buf, size_t len, char *addr_str, char *port_str)
{
  ipv6_addr_t addr;
  sock_udp_ep_t remote;
  size_t bytes_sent;

  remote.family = AF_INET6;
  // remote.netif = SOCK_ADDR_ANY_NETIF;
  /* parse for interface */
  int iface = ipv6_addr_split_iface(addr_str);
  if (iface == -1) {
      if (gnrc_netif_numof() == 1) {
          /* assign the single interface found in gnrc_netif_numof() */
          remote.netif = (uint16_t)gnrc_netif_iter(NULL)->pid;
      }
      else {
          remote.netif = SOCK_ADDR_ANY_NETIF;
      }
  }
  else {
      if (gnrc_netif_get_by_pid(iface) == NULL) {
          puts("gcoap_cli: interface not valid");
          return 0;
      }
      remote.netif = iface;
  }

  /* parse destination address */
  if (ipv6_addr_from_str(&addr, addr_str) == NULL) {
    puts("gcoap_cli: unable to parse destination address");
    return 0;
  }
  if ((remote.netif == SOCK_ADDR_ANY_NETIF) && ipv6_addr_is_link_local(&addr)) {
    puts("gcoap_cli: must specify interface for link local target");
    return 0;
  }
  memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

  /* CoAP port */
  remote.port = atoi(port_str);
  if (remote.port == 0) {
    puts("gcoap_cli: unable to parse destination port");
    return 0;
  }

  bytes_sent = gcoap_req_send2(buf, len, &remote, _resp_handler);
  return bytes_sent;
}

/* CoAP post  */
int gcoap_cli_send(char* path, char* data, char *addr_str, char *port_str)
{
  /* Allocate a buffer and a coap_pkt_t for the request */
  coap_pkt_t pdu;
  size_t len;
  uint8_t buf[GCOAP_PDU_BUF_SIZE];

  /* Call gcoap_req_init to initialize the request */
  gcoap_req_init(&pdu, &buf[0], GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, path);

  /* You can mark the request as confirmable by calling coap_hdr_set_type COAP_TYPE_CON */
  // coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

  /* Write the request payload */
  memcpy(pdu.payload, data, strlen(data));
  /* TODO: check if buffer size is ok */

  /* Call gcoap_finish to update the packet with the payload */
  len = gcoap_finish(&pdu, strlen(data), COAP_FORMAT_TEXT);

  DEBUG("[CoAP] coap post: sending msg \n");

  if(!_send(buf, len, addr_str, port_str)){
    puts("gcoap: message send failed");
    return -1;
  }
  else{
    printf("gcoap: message send %s\n", data);
  }
  return len;
}
