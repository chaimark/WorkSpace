#ifndef __UPDATA_H
#define __UPDATA_H


//#define UPDATA_PAGE_SIGN   4224
//#define UPDATA_PAGE_BEGIN  4096  
//#define UPDATA_PAGE_END    4223
#define UPDATA_PAGE_SIGN   0x1F000 
#define UPDATA_PAGE_BEGIN  0x11800 
#define UPDATA_PAGE_END    0x1F000 

#define UPDATA_SIGN   0xaa55aa55

// 地址偏移
#define UPDATA_MCU_BASE    0
#define UPDATA_MCU_OFFSET  0x4000

typedef struct updata_param_s
{
	uint32_t sign;    // 标志位
	uint32_t pageNum; // 大小
	uint8_t sum[128]; // 校验

} updata_param_t;

void updata_init(void);
int updata_check(void);
int updata_programming(void);

#endif 

