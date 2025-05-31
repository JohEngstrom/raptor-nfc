idf_component_register(
  SRCS
    "main.c"
    "i2c_slave_unlock.c"
  PRIV_REQUIRES spi_flash
  REQUIRES esp-idf-rc522
  INCLUDE_DIRS "."
)
