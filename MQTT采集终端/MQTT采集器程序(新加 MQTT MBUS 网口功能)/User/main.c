#include "main.h"
#include "NumberBaseLic.h"

void Read_GATEWAY_Working_Mode_From_AT256_To_ARM(void) {
    unsigned char Temp_Buffer;
    Temp_Buffer = EEprom_Byte1Read(EEPROM_WOKE_MODE_ADDR);
    current_GW_model = Temp_Buffer;
    UP_Mode_EC20_ON = (Temp_Buffer & 0x80) >> 7;
    UP_Mode_NET_ON = (Temp_Buffer & 0x40) >> 6;
    UP_Mode_MBUSTOUSB_ON = (Temp_Buffer & 0x20) >> 5;
    // 预留
    DOWN_Mode_MBUS_ON = (Temp_Buffer & 0x08) >> 3;
    DOWN_Mode_RS4851_ON = (Temp_Buffer & 0x04) >> 2;
    DOWN_Mode_RS4852_ON = (Temp_Buffer & 0x02) >> 1;
    DOWN_Mode_WIR_ON = (Temp_Buffer & 0x01);
    DEFINE_Mode_SETUSB_ON = 1;
}

void Set_GATEWAY_Working_Mode_From_Buffer_To_AT256(unsigned char data) {
    EEprom_Byte1Write(EEPROM_WOKE_MODE_ADDR, data);
}

// LED测试
void Test_LED_Twinkle() {
    for (unsigned char i = 0; i < 4; i++) {
        Heart_LED_H;
        EC20_LED_H;
        NET_LED_H;
        CH340_To_MBUS_LED_H;
        WIR_LED_H;
        RS485_LED_H;
        ERR_LED_H;
        FL_DelayMs(800);
        Heart_LED_L;
        EC20_LED_L;
        NET_LED_L;
        CH340_To_MBUS_LED_L;
        WIR_LED_L;
        RS485_LED_L;
        ERR_LED_L;
    }
}
// 工作模式
void Set_GetWay_WorkingMode(void) {
    Read_GATEWAY_Working_Mode_From_AT256_To_ARM();
    if (current_GW_model == 0xFF) {
        Set_GATEWAY_Working_Mode_From_Buffer_To_AT256(Default_Working_Mode);
        Read_GATEWAY_Working_Mode_From_AT256_To_ARM();
    }
    Select_OFF_EC20_To_UART0;
    Select_OFF_NET_To_UART0;

    Select_OFF_MBUS_To_UART1;
    Select_OFF_RS4851_To_UART4;
    Select_OFF_RS4852_To_LPUART0;
    Select_OFF_WIR_To_UART1;

    Select_OFF_CH340_To_LPUART1;
    Select_OFF_CH340_To_MBUS;
    Select_OFF_CH340_To_RS4851;
    Select_OFF_CH340_To_RS4852;

    Heart_LED_L;
    EC20_LED_L;
    NET_LED_L;
    CH340_To_MBUS_LED_L;
    WIR_LED_L;
    RS485_LED_L;
    ERR_LED_L;

    if (UP_Mode_EC20_ON == 1) {
        EC20_LED_H;
        MF_EC20_To_UART0_Init(); // 4G
        MF_EC20_To_UART0_Interrupt_Init();
        Select_ON_EC20_To_UART0;
    }
    if (UP_Mode_NET_ON == 1) {
        NET_LED_H;
        MF_NET_To_UART0_Init(); // 以太网
        MF_NET_To_UART0_Interrupt_Init();
        Select_ON_NET_To_UART0;
    }

    if (DOWN_Mode_MBUS_ON == 1) {
        MF_MBUS_To_UART1_Init(); // MBUS/WIR电路对接串口
        MF_MBUS_To_UART1_Interrupt_Init();
        Select_ON_MBUS_To_UART1;
    }
    if (DOWN_Mode_RS4851_ON == 1) {
        RS485_LED_H;
        MF_RS4851_To_LPUART0_Init(); // RS4851电路对接串口
        MF_RS4851_To_LPUART0_Interrupt_Init();
        Select_ON_RS4851_To_UART4;
    }
    if (DOWN_Mode_RS4852_ON == 1) {
        RS485_LED_H;
        MF_RS4852_To_LPUART1_Init(); // RS4852电路对接串口
        MF_RS4852_To_LPUART1_Interrupt_Init();
        Select_ON_RS4852_To_LPUART0;
    }
    if (DOWN_Mode_WIR_ON == 1) {
        WIR_LED_H;
        // USART1_Used_WIR_Initial();  //WIR电路对接串口
        Select_ON_WIR_To_UART1;
    }

    if (UP_Mode_MBUSTOUSB_ON != 1) {
        DEFINE_Mode_SETUSB_ON = 1;
        MF_CH340_To_LPUART1_Init(); // 配置端口初始化
        MF_CH340_To_LPUART1_Interrupt_Init();

        Select_OFF_CH340_To_MBUS;
        Select_OFF_CH340_To_RS4851;
        Select_OFF_CH340_To_RS4852;

        Select_ON_CH340_To_LPUART1;
    } else {
        Select_OFF_EC20_To_UART0;

        Select_OFF_NET_To_UART0;
        Select_OFF_WIR_To_UART1;
        Select_OFF_RS4851_To_UART4;
        Select_OFF_RS4852_To_LPUART0;

        Select_OFF_CH340_To_LPUART1;
        Select_OFF_CH340_To_RS4851;
        Select_OFF_CH340_To_RS4852;

        Select_ON_CH340_To_MBUS;
    }
    EEprom_MQTT_Parameter_Init();
    Read_MBUS_Buad_Local_From_AT256_To_ARM();
    Read_RS4851_Buad_Local_From_AT256_To_ARM();
    Read_RS4852_Buad_Local_From_AT256_To_ARM();
}

void MBUS_Uart1_Init(void) {
    Select_ON_MBUS_To_UART1;
    MBUS_UART1_BaudRate = EEprom_Byte4Read(EEPROM_MBUS_UART1_BAUDRATE_ADDR); // 波特率存储
    if ((MBUS_UART1_BaudRate == 1200) || (MBUS_UART1_BaudRate == 2400)) {
        MBUS_UART1_Parity = EEprom_Byte1Read(EEPROM_MBUS_UART1_PARITY_ADDR);
    } else {
        EEprom_Byte4Write(EEPROM_MBUS_UART1_BAUDRATE_ADDR, 2400); // 2400偶校验
        EEprom_Byte1Write(EEPROM_MBUS_UART1_PARITY_ADDR, 2);
        MBUS_UART1_BaudRate = 2400;
        MBUS_UART1_Parity = 2;
    }

    if (UP_Mode_MBUSTOUSB_ON == 1) { // USB转MBUS
        Select_OFF_MBUS_To_UART1;
        Select_OFF_CH340_To_LPUART1;
        Select_OFF_CH340_To_RS4851;
        Select_OFF_CH340_To_RS4852;
        Select_ON_CH340_To_MBUS;
    }
    MF_MBUS_To_UART1_Init();
    MF_MBUS_To_UART1_Interrupt_Init();
    MBUS_OUT_ON;
    ERR_LED_L;
}

void RS4851_Uart4_Init(void) {
    //    Select_ON_RS4851_To_UART4;
    //    RS4851_LPUART0_BaudRate=EEprom_Byte4Read(EEPROM_RS4851_UART4_BAUDRATE_ADDR);   //波特率存储
    //    if((RS4851_LPUART0_BaudRate==1200)||(RS4851_LPUART0_BaudRate==2400)||(RS4851_LPUART0_BaudRate==4800)||(RS4851_LPUART0_BaudRate==9600)){
    //      RS4851_LPUART0_Parity=EEprom_Byte1Read(EEPROM_RS4851_UART4_PARITY_ADDR);
    //    }else{
    //      EEprom_Byte4Write(EEPROM_RS4851_UART4_BAUDRATE_ADDR,9600);
    //      EEprom_Byte1Write(EEPROM_RS4851_UART4_PARITY_ADDR,0);
    //      RS4851_LPUART0_BaudRate=9600;
    //      RS4851_LPUART0_Parity=0;
    //    MF_RS4851_To_LPUART0_Init();             //RS4851电路对接串口
}

void RS4852_LPUart0_Init(void) {
    //		Select_ON_RS4852_To_LPUART0;
    //		RS4852_LPUART1_BaudRate=EEprom_Byte4Read(EEPROM_RS4852_LPUART0_BAUDRATE_ADDR);   //波特率存储
    //		if((RS4852_LPUART1_BaudRate==1200)||(RS4852_LPUART1_BaudRate==2400)||(RS4852_LPUART1_BaudRate==4800)||(RS4852_LPUART1_BaudRate==9600)){
    //			RS4852_LPUART1_Parity=EEprom_Byte1Read(EEPROM_RS4852_LPUART0_PARITY_ADDR);
    //		}else{
    //			EEprom_Byte4Write(EEPROM_RS4852_LPUART0_BAUDRATE_ADDR,9600);
    //			EEprom_Byte1Write(EEPROM_RS4852_LPUART0_PARITY_ADDR,0);
    //			RS4852_LPUART1_BaudRate=9600;
    //			RS4852_LPUART1_Parity=0;
    //		}
    //    MF_RS4852_To_LPUART1_Init();           //RS4852电路对接串口
}

/*Start LED配置调用****************************************************/
int main(void) {
    /* Configure the system clock */
    MF_Clock_Init();
    /* Initialize FL Driver Library */
    FL_Init();
    /* Initialize all configured peripherals */
    MF_Config_Init();
    BSTIM_10mS_Enable();
    FL_IWDT_ReloadCounter(IWDT); // 喂狗
    // 功能初始化
    //	Gpio_Init();
    //	console_init();
    //	RTC_Init_By_RX8025T(); // 用外部时钟，刷新内部时钟
    Read_EEprom_Or_RX8025T_To_Set_RTC();
    MBUS_OUT_OFF;
    Test_LED_Twinkle();
    // Set_GATEWAY_Working_Mode_From_Buffer_To_AT256(Default_Working_Mode);
    Set_GetWay_WorkingMode(); // 必须初始化设备
    if (DOWN_Mode_MBUS_ON == 1) {
        MBUS_Uart1_Init();
        MF_NVIC_UART1_Init();
    }
    if (DOWN_Mode_RS4851_ON == 1) {
        RS4851_Uart4_Init();
        MF_NVIC_UART4_Init();
    }
    if (DOWN_Mode_RS4852_ON == 1) {
        RS4852_LPUart0_Init();
        MF_NVIC_LPUART0_Init();
    }
    // 读取eepro （4G）
    if (UP_Mode_EC20_ON == 1) {
        EEprom_MQTT_Parameter_Init();
        Select_OFF_NET_To_UART0;
        Select_ON_EC20_To_UART0;
        MF_NVIC_UART0_Init();
        //    for(int i=0;i<10;i++){
        //      Save_EC20_Local_IMEI[i] = EEprom_Byte1Read(i);
        //    }
        //
        //    if((Save_EC20_Local_IMEI[0]==0xFF) ||(Save_EC20_Local_IMEI[1]==0xFF)||(Save_EC20_Local_IMEI[2]==0xFF)||(Save_EC20_Local_IMEI[3]==0xFF)||(Save_EC20_Local_IMEI[4]==0xFF)||(Save_EC20_Local_IMEI[5]==0xFF)||(Save_EC20_Local_IMEI[6]==0xFF)||(Save_EC20_Local_IMEI[7]==0xFF)||(Save_EC20_Local_IMEI[8]==0xFF)||(Save_EC20_Local_IMEI[9]==0xFF)){
        //      Set_EC20_Used_Programme_From_Buffer_To_AT256(EC20_Used_Programme);
        //      Set_EC20_Local_IMEI_From_Buffer_To_AT256(EC20_Local_IMEI);
        //      Set_EC20_Remote_QIOPEN_From_Buffer_To_AT256(EC20_Remote_QIOPEN);
        //    }
        //    Read_EC20_Used_Programme_From_AT256_To_ARM();
        //    Read_EC20_Local_IMEI_From_AT256_To_ARM();
        //    Read_EC20_Remote_QIOPEN_From_AT256_To_ARM();
    }
    // 初始化 NET
    if (UP_Mode_NET_ON == 1) {
        EEprom_MQTT_Parameter_Init();
        Select_OFF_EC20_To_UART0;
        Select_ON_NET_To_UART0;
        MF_NVIC_UART0_Init();
        MQTT_NetProt_Init();
        //    for(int i=0;i<11;i++){
        //      Save_NET_Local_IMEI[i] = EEprom_Byte1Read(100+i);
        //    }
        //
        //    if((Save_NET_Local_IMEI[0]==0xFF) ||(Save_NET_Local_IMEI[1]==0xFF)||(Save_NET_Local_IMEI[2]==0xFF)||(Save_NET_Local_IMEI[3]==0xFF)||(Save_NET_Local_IMEI[4]==0xFF)||(Save_NET_Local_IMEI[5]==0xFF)||(Save_NET_Local_IMEI[6]==0xFF)||(Save_NET_Local_IMEI[7]==0xFF)||(Save_NET_Local_IMEI[8]==0xFF)||(Save_NET_Local_IMEI[9]==0xFF)){
        //      Set_NET_Local_IMEI_From_Buffer_To_AT256(Define_Local_IMEI);
        //      Set_NET_Local_IP_From_Buffer_To_AT256(Define_Local_IP);
        //      Set_NET_Local_MASK_From_Buffer_To_AT256(Define_Local_MASK);
        //      Set_NET_Local_GATEWAY_From_Buffer_To_AT256(Define_Local_GETWAY);
        //      Set_NET_Local_MAC_From_Buffer_To_AT256(Define_Used_Local_MAC,Define_Local_MAC);
        //      Set_NET_Remote_IP_From_Buffer_To_AT256(Define_Remote_IP);
        //      Set_NET_Remote_Port_From_Buffer_To_AT256(Define_Remote_Port);
        //      Set_NET_Used_Programme_From_Buffer_To_AT256(Define_Used_Programme);
        //    }
        //    Read_NET_Local_IMEI_From_AT256_To_ARM();
        //    Read_NET_Local_IP_From_AT256_To_ARM();
        //    Read_NET_Local_MASK_From_AT256_To_ARM();
        //    Read_NET_Local_GATEWAY_From_AT256_To_ARM();
        //    Read_NET_Local_MAC_From_AT256_To_ARM();
        //    Read_NET_Remote_IP_From_AT256_To_ARM();
        //    Read_NET_Remote_Port_From_AT256_To_ARM();
        //    Read_NET_Used_Programme_From_AT256_To_ARM();
    }

    if (UP_Mode_MBUSTOUSB_ON != 1)
        MF_NVIC_LPUART1_Init();
    // USB显示功能块
    if (DEFINE_Mode_SETUSB_ON == 1) {
        LPUart1Send(SHOW_VER, strlen((char *)SHOW_VER)); // 串口回数显示版本
        if (UP_Mode_EC20_ON == 1)
            LPUart1Send(SHOW_UP_MODE_EC20, strlen((char *)SHOW_UP_MODE_EC20));
        if (UP_Mode_NET_ON == 1)
            LPUart1Send(SHOW_UP_MODE_NET, strlen((char *)SHOW_UP_MODE_NET));
        if (DOWN_Mode_MBUS_ON == 1)
            LPUart1Send(SHOW_DOWN_MODE_MBUS, strlen((char *)SHOW_DOWN_MODE_MBUS));
        if (DOWN_Mode_RS4851_ON == 1)
            LPUart1Send(SHOW_DOWN_MODE_RS4851, strlen((char *)SHOW_DOWN_MODE_RS4851));
        if (DOWN_Mode_RS4852_ON == 1)
            LPUart1Send(SHOW_DOWN_MODE_RS4852, strlen((char *)SHOW_DOWN_MODE_RS4852));
        if (DOWN_Mode_WIR_ON == 1)
            LPUart1Send(SHOW_DOWN_MODE_WIR_ON, strlen((char *)SHOW_DOWN_MODE_WIR_ON));
        LPUart1Send(SHOW_DEFINE_MODE_SETUSB, strlen((char *)SHOW_DEFINE_MODE_SETUSB));
    }
    // 初始化 4G
    if (UP_Mode_EC20_ON == 1) {
        VBAT_29302_CTL_ON;
        FL_DelayMs(1000);
        FL_IWDT_ReloadCounter(IWDT);
        V33_PWR_CTL_ON;
        EC20_WAKEUP_IN_L;
        EC20_PWR_KEY_L;
        for (int i = 0; i < 3; i++) {
            char j = 30;
            FL_DelayMs(500);
            FL_IWDT_ReloadCounter(IWDT);
            EC20_PWR_KEY_H;
            FL_DelayMs(1000);
            FL_IWDT_ReloadCounter(IWDT);
            EC20_PWR_KEY_L;
            FL_DelayMs(1000);
            FL_IWDT_ReloadCounter(IWDT);
            EC20_PWR_KEY_H;
            do {
                FL_DelayMs(500);
                if (EC20_STATUS_VALUE == 0) {
                    j = 0;
                    FL_DelayMs(2);
                    break;
                } else
                    goto NEXTEC20;
            } while (j--);
        }
    NEXTEC20:
        MQTT_4G_Init(); // 初始化 通信
    }
    while (1) {
        FL_IWDT_ReloadCounter(IWDT); // 喂狗
        if (UP_Mode_EC20_ON == 1) {
            MQTT_4G_Task();
        }
        // 以太网链接网络
        if (UP_Mode_NET_ON == 1) {
            MOTT_Net_Task();
        }

        if (Set_RX8025T_By_MQTT_CCLK_In_Main_Flage != 0) {
            Set_RX8025T_By_MQTT_CCLK_In_Main_Flage = 0;
            Write_RX8025T_AndRTC_By_UTC8();
        }

        if (Adjust_Meter_In_Main_Flage != 0) {
            Adjust_Meter_In_Main_Flage = 0;
            if (DOWN_Mode_MBUS_ON == 1) {
                Adjust_Meter_By_MBUS_Uart1();
            }
        }

        if (MBUS_UART1_Reboot_In_Main_Flage != 0) {
            MBUS_UART1_Reboot_In_Main_Flage = 0;
            if (DOWN_Mode_MBUS_ON == 1) {
                MBUS_Uart1_Init();
            }
        }

        if (read_Main_Meter_In_Timer_In_Main_Flage != 0) { // 主表
            if (imm_Read.immediately_Exec_Flage == 0) {
                execute_Main_Reading_In_Timer();
            }
        }
        if (read_Copy_Meter_In_Timer_In_Main_Flage != 0) { // 辅表
            if ((read_Main_Meter_In_Timer_In_Main_Flage == 0) && (imm_Read.immediately_Exec_Flage == 0)) {
                execute_Copy_Reading_In_Timer();
            }
        }
        if (imm_Read.immediately_Exec_Flage != 0) { // 透传操作
            execute_Transparent_Reading_In_Timer();
        }

        if (up_Immediately_History_In_Timer_In_Main_Flage != 0) {
            up_Immediately_History_In_Timer_In_Main_Flage = 0;
        }
        FL_IWDT_ReloadCounter(IWDT); // 喂狗
    }
}
