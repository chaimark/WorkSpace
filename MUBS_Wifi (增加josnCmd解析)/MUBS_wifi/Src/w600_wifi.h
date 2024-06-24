#ifndef __W600_WIFI_H
#define __W600_WIFI_H

#include "main.h"

void wifi_init(void);
void wifi_timer_add(void);
void wifi_timer_clean(void);
void wifi_task(void);

// 
void wifi_set_param(uint8_t *host_confirm);
uint8_t wifi_is_connect(void);



#endif


