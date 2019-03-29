
#ifndef COAP_H
#define COAP_H

#ifdef _cplusplus
extern "C"{
#endif

int gcoap_cli_send(char* path, char* data, char *addr_str, char *port_str);

#ifdef _cplusplus
}
#endif

#endif /* COAP_H */
