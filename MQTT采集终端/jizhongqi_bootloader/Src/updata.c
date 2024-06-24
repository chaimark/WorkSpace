#include "main.h"


int ascii2hex(uint8_t *asc, uint8_t *hex, uint8_t len)
{
	uint8_t value;

	for (int i = 0; i < len; i++) 
	{
		if (asc[i << 1] >= '0' && asc[i << 1]  <= '9')      value = asc[i << 1] - '0';
		else if (asc[i << 1] >= 'a' && asc[i << 1]  <= 'f') value = asc[i << 1] - 'a' + 10;
		else if (asc[i << 1] >= 'A' && asc[i << 1]  <= 'F') value = asc[i << 1] - 'A' + 10;
		else												return -1;

		if (asc[(i << 1) + 1] >= '0' && asc[(i << 1) + 1]  <= '9')		value = (value << 4) | (asc[(i << 1) + 1] - '0');
		else if (asc[(i << 1) + 1] >= 'a' && asc[(i << 1) + 1]  <= 'f') value = (value << 4) | (asc[(i << 1) + 1] - 'a' + 10);
		else if (asc[(i << 1) + 1] >= 'A' && asc[(i << 1) + 1]  <= 'F') value = (value << 4) | (asc[(i << 1) + 1] - 'A' + 10);
		else												            return -1;

		hex[i] = value;
	}
	return 0;
}

// 八位和校验
// static uint8_t updata_checksum(uint8_t *buf, int len)
// {
// 	uint8_t sum = 0;
// 	for (int i = 0; i < len; i++) 
// 		sum += buf[i];
// 
// 	sum = 0x100 - sum;
// 	return sum;
// }

// ***************************************************************
// ***************************************************************
updata_param_t updata_param;

#define UPDATA_LINE_MAX  32
//static uint8_t updata_line[UPDATA_LINE_MAX];

static uint32_t updata_page32[PAGE_SIZE >> 2];  // 整页存储
static uint8_t *updata_page = (uint8_t *)updata_page32;
//static uint16_t updata_page_num;

//static uint8_t updata_buf[PAGE_SIZE];   // 临时存储
//static uint16_t updata_buf_len;

// static uint32_t updata_addrBase = 0xaa; // 基地址
// static uint16_t updata_addrNow = 0xaa;  // 当前地址


// 读标志扇区
static void updata_read_sign(void)
{
	uint8_t *p = (uint8_t *)(&updata_param);
	for (int i = 0; i < sizeof(updata_param_t); i++) 
		p[i] = *(uint8_t *)(UPDATA_PAGE_SIGN + i);
}

// 写标志扇区
static uint8_t updata_page_buf[PAGE_SIZE];
static void updata_write_sign(void)
{
	memset(updata_page_buf, 0xff, PAGE_SIZE);
	memcpy(updata_page_buf, &updata_param, sizeof(updata_param_t));
	flash_write_page(UPDATA_PAGE_SIGN, (uint32_t *)updata_page_buf);
}

// 清除标志扇区
static void updata_clean_sign(void)
{
	memset(&updata_param, 0, sizeof(updata_param));
	updata_write_sign();

	// updata_page_num = UPDATA_PAGE_BEGIN;
	// updata_page_len = 0;
	// updata_buf_len = 0;
	// updata_addrBase = UPDATA_MCU_BASE;
	// updata_addrNow = UPDATA_MCU_OFFSET;
}

void updata_init(void)
{
	updata_clean_sign();
}

int updata_check(void)
{
	updata_read_sign();
	uint8_t sum;

	if (updata_param.sign == UPDATA_SIGN && updata_param.pageNum > 10) 
	{
		printf("updata find page = %d\r\n", updata_param.pageNum);
		for (int i = 0; i < updata_param.pageNum; i++) 
		{
			flash_read_page(UPDATA_PAGE_BEGIN +  (i << FL_FLASH_PAGE_OFFSET), updata_page32);

			sum = 0;
			for (int j = 0; j < PAGE_SIZE; j++) 
				sum += updata_page[j];

			if (sum != updata_param.sum[i])
			{
				printf("updata check err\r\n");
				return -1;
			}
		}

		return 0;
	}

	return -1;
}

int updata_programming(void)
{
	uint8_t sum;
	uint8_t ok = 1;
	uint32_t addr;

	while(1)
	{
		for (int i = 0; i < updata_param.pageNum; i++) 
		{
			// 读
			flash_read_page(UPDATA_PAGE_BEGIN +  (i << FL_FLASH_PAGE_OFFSET), updata_page32);

			// 写
			addr = UPDATA_MCU_OFFSET + (i << FL_FLASH_PAGE_OFFSET);
			if (flash_write_page(addr, (uint32_t *)updata_page32) < 0) 
			{
				printf("updata page %04x program fail\r\n", addr);
				ok = 0;
				break;
			}

			// 校验
			sum = 0;
			for (int j = 0; j < PAGE_SIZE; j++) 
				sum += *(uint8_t *)(addr + j);

			if (sum != updata_param.sum[i])
			{
				printf("updata page %04x check fail\r\n", addr);
				ok = 0;
				break;
			}

			printf("updata addr %04x sucess\r\n", addr);
		}

		if (ok == 1) 
			break;
	}

	updata_clean_sign();
	return 0;
}



