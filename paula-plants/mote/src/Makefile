APPLICATION = internet-of-plants

BOARD ?= iotlab-m3

RIOTBASE ?= $(CURDIR)/../RIOT

# Modules
# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules
USEMODULE += gcoap
USEMODULE += gnrc_ipv6_default

USEMODULE += xtimer
USEMODULE += od
USEMODULE += fmt



# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
