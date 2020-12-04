#ifndef __HDL_DRIVER_MANAGER_H_
#define __HDL_DRIVER_MANAGER_H_

#include "common.h"

void driver_com_init(void);
void driver_com_handle(void);
void driver_com_exit(void);

void driver_com_regist_reccallback(uint32_t USARTx,void (*drv_com_m_handle)(unsigned char data));

void drv_com1_write(uint8_t data);
void drv_com2_write(uint8_t data);
void drv_com3_write(uint8_t data);
void drv_com4_write(uint8_t data);


void drv_com1_printf(char *fmt, ...);
void drv_com2_printf(char *fmt, ...);
void drv_com3_printf(char *fmt, ...);
void drv_com4_printf(char *fmt, ...);

#endif
