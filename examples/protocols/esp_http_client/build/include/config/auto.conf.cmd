deps_config := \
	/home/lin/2/esp-idf/components/app_trace/Kconfig \
	/home/lin/2/esp-idf/components/aws_iot/Kconfig \
	/home/lin/2/esp-idf/components/bt/Kconfig \
	/home/lin/2/esp-idf/components/driver/Kconfig \
	/home/lin/2/esp-idf/components/efuse/Kconfig \
	/home/lin/2/esp-idf/components/esp32/Kconfig \
	/home/lin/2/esp-idf/components/esp_adc_cal/Kconfig \
	/home/lin/2/esp-idf/components/esp_event/Kconfig \
	/home/lin/2/esp-idf/components/esp_http_client/Kconfig \
	/home/lin/2/esp-idf/components/esp_http_server/Kconfig \
	/home/lin/2/esp-idf/components/esp_https_ota/Kconfig \
	/home/lin/2/esp-idf/components/espcoredump/Kconfig \
	/home/lin/2/esp-idf/components/ethernet/Kconfig \
	/home/lin/2/esp-idf/components/fatfs/Kconfig \
	/home/lin/2/esp-idf/components/freemodbus/Kconfig \
	/home/lin/2/esp-idf/components/freertos/Kconfig \
	/home/lin/2/esp-idf/components/heap/Kconfig \
	/home/lin/2/esp-idf/components/libsodium/Kconfig \
	/home/lin/2/esp-idf/components/log/Kconfig \
	/home/lin/2/esp-idf/components/lwip/Kconfig \
	/home/lin/2/esp-idf/components/mbedtls/Kconfig \
	/home/lin/2/esp-idf/components/mdns/Kconfig \
	/home/lin/2/esp-idf/components/mqtt/Kconfig \
	/home/lin/2/esp-idf/components/nvs_flash/Kconfig \
	/home/lin/2/esp-idf/components/openssl/Kconfig \
	/home/lin/2/esp-idf/components/pthread/Kconfig \
	/home/lin/2/esp-idf/components/spi_flash/Kconfig \
	/home/lin/2/esp-idf/components/spiffs/Kconfig \
	/home/lin/2/esp-idf/components/tcpip_adapter/Kconfig \
	/home/lin/2/esp-idf/components/unity/Kconfig \
	/home/lin/2/esp-idf/components/vfs/Kconfig \
	/home/lin/2/esp-idf/components/wear_levelling/Kconfig \
	/home/lin/2/esp-idf/components/wifi_provisioning/Kconfig \
	/home/lin/2/esp-idf/components/app_update/Kconfig.projbuild \
	/home/lin/2/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/lin/2/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/lin/esp/alexa/esp-alexa-idf/examples/protocols/esp_http_client/main/Kconfig.projbuild \
	/home/lin/2/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/lin/2/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
