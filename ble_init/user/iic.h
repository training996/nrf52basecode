/**
 * iic.h
 */
#ifndef IIC_H__
#define IIC_H__

#include "nrf_drv_twi.h"

#define TWI_INSTANCE_ID     0

void twi_init (void);
void twi_uninit (void);


#endif
