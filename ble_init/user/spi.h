/*
	spi.h
*/
#ifndef SPI_H__
#define SPI_H__

#include "nrf_drv_spi.h"
#include "string.h"

#define SPI_INSTANCE                 0
#define SPI_TRANSFER_BUF_LEN         64

void user_spi_init(void);
uint8_t Spi_ReadByte(void);
void Spi_WriteByte(uint8_t data);
void Spi_WriteBuf( uint32_t addr, uint8_t *buffer, uint8_t size );
void Spi_ReadBuf( uint32_t addr, uint8_t *buffer, uint8_t size );


#endif
