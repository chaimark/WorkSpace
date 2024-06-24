#ifndef __FLASH_H
#define __FLASH_H

#define  FL_FLASH_PAGE_OFFSET   9
#define  PAGE_SIZE   512

int flash_write_page(uint32_t addr, uint32_t *buf);
void flash_read_page(uint32_t addr, uint32_t *buf);

#endif 

