#include "mgos.h"
#include "mgos_spi.h"
#include "mgos_timers.h"

/*
 * This example demonstrates how to use the C SPI API.
 *
 */

void timer_cb(void *arg) {
  struct mgos_spi *spi;
#if 1
  /* Global SPI instance is configured by the `spi` config section. */
  spi = mgos_spi_get_global();
  if (spi == NULL) {
    LOG(LL_ERROR, ("SPI is not configured, make sure spi.enable is true"));
    return;
  }
#else
  /* Alternatively, you can set up the bus manually. */
  struct sys_config_spi bus_cfg = {
      .unit_no = 3,
      .miso_gpio = 19,
      .mosi_gpio = 23,
      .sclk_gpio = 18,
      .cs0_gpio = 5,
      .debug = true,
  };
  spi = mgos_spi_create(&bus_cfg);
  if (spi == NULL) {
    LOG(LL_ERROR, ("Bus init failed"));
    return MGOS_APP_INIT_ERROR;
  }
#endif

  uint8_t tx_data[1] = {0x9f /* Read JEDEC ID */};
  uint8_t rx_data[3] = {0, 0, 0};

  struct mgos_spi_txn txn = {
      .cs = 0, /* Use CS0 line as configured by cs0_gpio */
      .mode = 0,
      .freq = 10000000,
  };
  /* Half-duplex, command/response transaction setup */
  /* Transmit 1 byte from tx_data. */
  txn.hd.tx_len = 1;
  txn.hd.tx_data = tx_data;
  /* No dummy bytes necessary. */
  txn.hd.dummy_len = 0;
  /* Receive 3 bytes into rx_data. */
  txn.hd.rx_len = 3;
  txn.hd.rx_data = rx_data;
  if (!mgos_spi_run_txn(spi, false /* full_duplex */, &txn)) {
    LOG(LL_ERROR, ("SPI transaction failed"));
    return;
  }

  LOG(LL_INFO,
      ("JEDEC ID: %02x %02x %02x", rx_data[0], rx_data[1], rx_data[2]));
  (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
  mgos_set_timer(1000, true, timer_cb, NULL);
  return MGOS_APP_INIT_SUCCESS;
}
