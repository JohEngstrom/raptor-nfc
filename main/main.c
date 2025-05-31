#include <esp_log.h>
#include "rc522.h"
#include "driver/rc522_spi.h"
#include "rc522_picc.h"
#include "driver/i2c.h"
#include "unlock_types.h"
#include "i2c_slave_unlock.h"

static const char *TAG = "rc522-basic-example";

// Assign your pins here!
#define RC522_SPI_BUS_GPIO_MISO    (19)
#define RC522_SPI_BUS_GPIO_MOSI    (23)
#define RC522_SPI_BUS_GPIO_SCLK    (18)
#define RC522_SPI_SCANNER_GPIO_SDA (5)    // CS/SS
#define RC522_SCANNER_GPIO_RST     (-1)   // Soft-reset

#define I2C_SLAVE_SCL_IO   22    // Your board's SCL pin
#define I2C_SLAVE_SDA_IO   21    // Your board's SDA pin
#define I2C_SLAVE_NUM      I2C_NUM_0
#define I2C_DISPLAY_ADDR   0x28

#define I2C_BUF_LEN        128

static rc522_spi_config_t driver_config = {
    .host_id = SPI3_HOST, // VSPI
    .bus_config = &(spi_bus_config_t){
        .miso_io_num = RC522_SPI_BUS_GPIO_MISO,
        .mosi_io_num = RC522_SPI_BUS_GPIO_MOSI,
        .sclk_io_num = RC522_SPI_BUS_GPIO_SCLK,
        .quadwp_io_num = -1, // not used
        .quadhd_io_num = -1, // not used
        .max_transfer_sz = 0 // default
    },
    .dev_config = {
        .spics_io_num = RC522_SPI_SCANNER_GPIO_SDA,
        .clock_speed_hz = 1*1000*1000, // 1 MHz (default, can increase later)
        .queue_size = 1, // default
    },
    .rst_io_num = RC522_SCANNER_GPIO_RST,
};

static rc522_driver_handle_t driver;
static rc522_handle_t scanner;

static void on_picc_state_changed(void *arg, esp_event_base_t base, int32_t event_id, void *data)
{
    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *)data;
    rc522_picc_t *picc = event->picc;

    if (picc->state == RC522_PICC_STATE_ACTIVE) {
      rc522_picc_print(picc);

      // Add your BASIC tag UID check here (for now, allow any)
      i2c_slave_send_unlock_basic(picc->uid.value, picc->uid.length);

      ESP_LOGI("NFC", "Sending unlock BASIC over I2C");
    }
    else if (picc->state == RC522_PICC_STATE_IDLE && event->old_state >= RC522_PICC_STATE_ACTIVE) {
      ESP_LOGI(TAG, "Card has been removed");
    }
}

void app_main()
{
    i2c_slave_init();

    rc522_spi_create(&driver_config, &driver);
    rc522_driver_install(driver);

    rc522_config_t scanner_config = {
        .driver = driver,
    };

    rc522_create(&scanner_config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, NULL);
    rc522_start(scanner);
}
