#include "main.h"

// FL_FLASH_PAGE_SIZE_BYTE   512 页大小


#define FLASH_PAGE_NUM   255
//#define FLASH_PAGE_NUM   155
//#define FLASH_SECTOR_NUM   63

#define FLASH_DMA_SIZE   256

// 0 ~ 63  块 
// 0 ~ 255 页 

int test = 0;

uint32_t flash32[FL_FLASH_PAGE_SIZE_BYTE >> 2];

static int flash_check_page(uint32_t addr, uint32_t *buf)
{
	for (int i = 0; i < FL_FLASH_PAGE_SIZE_BYTE >> 2; i++) 
	{
		if (buf[i] != *(uint32_t *)(addr + (i << 2)))
		{
			return -1;
		}
	}

	return 0;
}

// 写一页。把 buf 开始的数据，写入到 addr 开始的地址中, 并读出校验
int flash_write_page(uint32_t addr, uint32_t *buf)
{
	for (int i = 0; i < 3; i++) 
	{
		if (FL_FLASH_PageErase(FLASH, addr) == FL_FAIL ) 
			break;

		if ( FL_FLASH_Program_Page(FLASH, addr >> FL_FLASH_PAGE_OFFSET, buf) == FL_FAIL ) 
			break;

		// 校验
		if (flash_check_page(addr, buf) < 0) 
			break;

		return 0;
	}

	return -1;
}

void flash_read_page(uint32_t addr, uint32_t *buf)
{
	for (int i = 0; i < PAGE_SIZE >> 2; i++) 
		buf[i] = *(uint32_t *)(addr + (i << 2));
}

// 
void flash_test(void)
{
	// 初始化要写入的值
	uint8_t *p = (uint8_t *)flash32;
	for (int i = 0; i < FL_FLASH_PAGE_SIZE_BYTE; i++) 
		p[i] = i;

	// 写入一页
	if (flash_write_page(FLASH_PAGE_NUM * FL_FLASH_PAGE_SIZE_BYTE, flash32) < 0) 
	{
		test = 1;
	}
}

