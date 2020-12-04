#ifndef __IAP_H
#define __IAP_H

#include "common.h"
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
void iap_load_app(uint32_t appxaddr);

#endif
