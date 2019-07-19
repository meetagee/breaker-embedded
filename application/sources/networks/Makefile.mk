ifeq ($(IF_NETWORK_NRF24_OPTION),-DIF_NETWORK_NRF24_EN)
include sources/networks/rf_protocols/Makefile.mk
endif

ifeq ($(IF_LINK_OPTION),-DIF_LINK_UART_EN)
include sources/networks/net/link/Makefile.mk
endif

include sources/networks/ArduinoZigBee/Makefile.mk
