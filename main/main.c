#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rc522.h"

void app_main(void)
{
    rc522_config_t config = {
        .spi_host_id = SPI2_HOST,  // SPI2_HOST = HSPI, SPI3_HOST = VSPI
        .spi_miso_io = 19,
        .spi_mosi_io = 23,
        .spi_sck_io  = 18,
        .spi_sda_io  = 5,
        .poll_interval_ms = 100,  // Polling interval
    };

    rc522_handle_t rc522;
    esp_err_t err = rc522_create(&config, &rc522);
    if (err != ESP_OK) {
        printf("Failed to create RC522: %d\n", err);
        return;
    }

    err = rc522_start(rc522);
    if (err != ESP_OK) {
        printf("Failed to start RC522: %d\n", err);
        return;
    }

    printf("RC522 started, scanning for tags...\n");
    while (1) {
        rc522_picc_t picc;
        err = rc522_request(rc522, &picc);
        if (err == ESP_OK) {
            printf("Tag detected! UID: ");
            for (size_t i = 0; i < picc.uid.size; i++) {
                printf("%02X ", picc.uid.bytes[i]);
            }
            printf("\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
