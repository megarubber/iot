#include "driver/spi_slave.h"

#define PIN_SCLK 18
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CS   5
#define SPI_HOST VSPI_HOST

byte data;

void setup() {
    Serial.begin(9600);
    spi_bus_config_t busConfig = {};
    busConfig.mosi_io_num = PIN_MOSI;
    busConfig.miso_io_num = PIN_MISO;
    busConfig.sclk_io_num = PIN_SCLK;
    spi_slave_interface_config_t slaveConfig = {};
    slaveConfig.spics_io_num = PIN_CS;
    slaveConfig.mode = 0;
    slaveConfig.queue_size = 1;
    esp_err_t result = spi_slave_initialize(SPI_HOST, &busConfig, &slaveConfig, SPI_DMA_DISABLED);
}

void loop() {
    data = 0;
    spi_slave_transaction_t transaction = {};
    transaction.length = 8;
    transaction.rx_buffer = &data;
    esp_err_t result = spi_slave_transmit(SPI_HOST, &transaction, portMAX_DELAY);
    if (result == ESP_OK) Serial.printf("Received: %c\n", (char)data);
}