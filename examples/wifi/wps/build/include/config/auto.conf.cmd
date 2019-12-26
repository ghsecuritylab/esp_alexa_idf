deps_config := \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/app_trace/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/aws_iot/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/bt/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/driver/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esp32/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esp_adc_cal/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esp_event/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esp_http_client/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esp_http_server/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/ethernet/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/fatfs/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/freemodbus/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/freertos/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/heap/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/libsodium/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/log/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/lwip/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/mbedtls/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/mdns/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/mqtt/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/nvs_flash/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/openssl/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/pthread/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/spi_flash/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/spiffs/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/tcpip_adapter/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/vfs/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/wear_levelling/Kconfig \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/bootloader/Kconfig.projbuild \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/esptool_py/Kconfig.projbuild \
	/home/lin/esp/alexa/back/esp-alexa-idf/examples/wifi/wps/main/Kconfig.projbuild \
	/home/lin/esp/alexa/back/esp-alexa-idf/components/partition_table/Kconfig.projbuild \
	/home/lin/esp/alexa/back/esp-alexa-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
