#include "ramdisk.h"

uint8_t RamDisk[STORAGE_BLK_NBR * STORAGE_BLK_SIZ];

void ramdisk_write(int32_t blk_addr, uint8_t *buf, uint16_t  size)
{
	memcpy((void *)(RamDisk +  blk_addr), buf,  size);
}

void ramdisk_read(int32_t blk_addr, uint8_t *buf, uint16_t  size)
{
	memcpy(buf, (void *)(RamDisk +  blk_addr), size);
}
