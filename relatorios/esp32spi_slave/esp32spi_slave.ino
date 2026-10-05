#include "driver/spi_slave.h"

#define PIN_SCLK 18
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CS   5

#define SPI_HOST VSPI_HOST

uint8_t rxData;

void setup() {
    Serial.begin(9600);

    // 1. Configure the SPI Bus pins
    spi_bus_config_t busConfig = {};
    busConfig.mosi_io_num = PIN_MOSI;
    busConfig.miso_io_num = PIN_MISO;
    busConfig.sclk_io_num = PIN_SCLK;
    //busConfig.quadwp_io_num = -1;
    //busConfig.quadhd_io_num = -1;

    // 2. Configure the SPI Slave Interface settings
    spi_slave_interface_config_t slaveConfig = {};
    slaveConfig.spics_io_num = PIN_CS;           // Define Chip Select pin here
    slaveConfig.mode = 0;                        // SPI_MODE0 equivalent
    //slaveConfig.queue_size = 1;                  // Queue depth for transactions
    //slaveConfig.flags = 0;

    // 3. Initialize with fixed argument order
    esp_err_t result = spi_slave_initialize(
        SPI_HOST,
        &busConfig,
        &slaveConfig,        // 3rd: Pointer to the slave interface config
        SPI_DMA_DISABLED     // 4th: DMA channel (or disabled)
    );

    if (result != ESP_OK) {
        Serial.printf(
            "SPI initialization failed: %s\n",
            esp_err_to_name(result)
        );

        while (true) {
            delay(1000);
        }
    }

    Serial.println("SPI Slave ready");
}

void loop() {
    rxData = 0;

    spi_slave_transaction_t transaction = {};
    transaction.length = 8;
    transaction.rx_buffer = &rxData;

    esp_err_t result = spi_slave_transmit(
        SPI_HOST,
        &transaction,
        portMAX_DELAY
    );

    if (result == ESP_OK) {
        Serial.printf("Received: %c\n", (char)rxData);
    }
}