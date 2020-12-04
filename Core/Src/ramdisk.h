#ifndef  __RAMDISK_H_
#define  __RAMDISK_H_

#include "common.h"

#define STORAGE_CAPPACITY                1024*100
#define STORAGE_BLK_SIZ                  512
#define STORAGE_BLK_NBR                  STORAGE_CAPPACITY / STORAGE_BLK_SIZ
#define MEDIA_DESCRIPTOR                 0xf8

void ramdisk_write(int32_t blk_addr, uint8_t *buf, uint16_t  size);
void ramdisk_read(int32_t blk_addr, uint8_t *buf, uint16_t  size);
#endif