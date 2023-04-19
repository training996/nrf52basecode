/**
 * spi.c
 */
#include "spi.h"

static uint8_t       spi_tx_buf[ SPI_TRANSFER_BUF_LEN + 1 ];  /**<Spi TX buffer. */
static uint8_t       spi_rx_buf[ SPI_TRANSFER_BUF_LEN + 1 ];  /**<Spi RX buffer. */

static volatile bool spi_xfer_done;  /**<SPI data transfer complete flag. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */

/**
 * @brief SPI user event handler.
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
}

/**
 * @brief Spi initialization.
 */
void user_spi_init(void)
{
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
}

/**
 * @brief Spi read one byte.
 */
uint8_t Spi_ReadByte(void) 
{
    spi_tx_buf[0] = 0xFF;
    spi_xfer_done = false;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 1, spi_rx_buf, 1));
    while(!spi_xfer_done)
		 ;
    return (spi_rx_buf[0]);
}

/**
 * @brief Spi write one byte.
 */
void Spi_WriteByte(uint8_t data)
{
    spi_tx_buf[0] = data;
    spi_xfer_done = false;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, 1, spi_rx_buf, 1));
    while(!spi_xfer_done)
		 ;
}

/**
 * @brief Spi write buffer.
 */
void Spi_WriteBuf( uint32_t addr, uint8_t *buffer, uint8_t size )
{ 
    if( ( size > SPI_TRANSFER_BUF_LEN ) || ( size == 0 ) )
        return;
    memset( spi_tx_buf, 0, sizeof( spi_tx_buf ) );
    ASSERT( buffer );
    spi_tx_buf[0] = addr | 0x80;
    memcpy( &spi_tx_buf[1], buffer, size );
    nrf_drv_spi_transfer( &spi, spi_tx_buf, size + 1, NULL, 0 );
}   

/**
 * @brief Spi read buffer.
 */
void Spi_ReadBuf( uint32_t addr, uint8_t *buffer, uint8_t size )
{
    if( ( size > SPI_TRANSFER_BUF_LEN ) || ( size == 0 ) )
        return;
    memset(spi_tx_buf, 0, sizeof(spi_tx_buf));
    memset(spi_rx_buf, 0, sizeof(spi_rx_buf));    
    ASSERT( buffer );
    spi_tx_buf[0] = addr & 0x7F;
    nrf_drv_spi_transfer( &spi, spi_tx_buf, size + 1, spi_rx_buf, size + 1 );
    memcpy(buffer, &spi_rx_buf[1], size);
}

/**
 * @brief Spi uninitialization.
 */
void user_spi_uninit(void)
{
    nrf_drv_spi_uninit(&spi);
}
