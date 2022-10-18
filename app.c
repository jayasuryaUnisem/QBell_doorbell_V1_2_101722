#include "app.h"

extern int_flags intF;

extern struct define_flag Mflag;
//
extern _BC3602_device_ BC3602_T;

void app_pair_status_check()
{
    #if 0
    uart_printf("Device Address: ");

    eepromDeviceAddrRead();
    delay(10);
    eepromPstatusCheck();
    delay(10);

    u8 j;
    for (j = 0; j < 4; j++)
        uart_send(intF.deviceID[j]);

    uart_printf("\n");
    delay(10);

    switch (intF.pairStatus)
    {
        u8 i = 0;
    case NO_DEVICE_FOUND:
        uart_printf("No Device Paired Yet\n");
        break;

    case P1_DEVICE_CONN:
        eepromP1Read();
        uart_printf("P1 Device Paired: ");

        for (i = 0; i < 4; i++)
            uart_send(intF.p1DeviceID[i]);
        uart_printf("\n");
        break;

    case P2_DEVICE_CONN:
        eepromP2Read();
        uart_printf("P2 Device Paired: ");

        for (i = 0; i < 4; i++)
            uart_send(intF.p2DeviceID[i]);
        uart_printf("\n");
        break;

    case BOTH_DEVICE_CONN:
        eepromP1Read();
        eepromP2Read();
        uart_printf("Both the Device are Paired\n");
        uart_printf("P1 Device ID: ");

        for (i = 0; i < 4; i++)
            uart_send(intF.p1DeviceID[i]);
        uart_printf("\nP2 Device ID: ");

        for (i = 0; i < 4; i++)
            uart_send(intF.p2DeviceID[i]);
        uart_printf("\n");
        break;

    default:
        break;
    }
    uart_printf("\n");
    #endif

    #if 1
    uart_printf("Checking Device Address: \n");
    eepromDeviceAddrRead();
    // delay(1);
    // eepromDeviceAddrCpyCheck();

    uart_printf("Device Addr Value: ");
    uart_printf(intF.deviceID);
    // uart_printf("\nDevice Addr Cpy Value: ");
    // uart_printf(intF.deviceAddrCpy);
    #endif
}

void app_pair_status_update()
{
    uart_printf("Updating the EEPROM\n");
    intF.pairingAck = 0;
    eepromPstatusCheck();
    delay(10);
    u8 i = 0;
    u8 tempArr[5];
    for (i = 0; i < 4; i++)
        tempArr[i] = intF.rxPayload[i + 1];
    switch (intF.pairStatus)
    {
    case NO_DEVICE_FOUND:
        uart_printf("Writing the ID to the P1 Address: ");
        eepromP1Write();
        delay(10);
        eepromP1Read();
        delay(10);
        eepromPstatusWrite(P1_DEVICE_CONN);
        delay(10);
        for (i = 0; i < 4; i++)
            uart_send(intF.p1DeviceID[i]);
        uart_printf("\n");
        break;

    case P1_DEVICE_CONN:
        eepromP1Read();
        delay(10);
        if (memcmp(tempArr, intF.p1DeviceID, sizeof(intF.p1DeviceID)))
        {
            uart_printf("Writing the ID to the P2 Address: ");
            eepromP2Write();
            delay(10);
            eepromP2Read();
            delay(10);
            eepromPstatusWrite(BOTH_DEVICE_CONN);
            delay(10);
            for (i = 0; i < 4; i++)
                uart_send(intF.p2DeviceID[i]);
            uart_printf("\n");
        }

        else
        {
            uart_printf("This device is already paired\n");
        }

        break;

    case P2_DEVICE_CONN:
        eepromP2Read();
        if (memcmp(tempArr, intF.p2DeviceID, sizeof(intF.p2DeviceID)))
        {
            uart_printf("Writing trhe ID to the P1 Address: ");
            eepromP1Write();
            delay(10);
            eepromP1Read();
            delay(10);
            eepromPstatusWrite(BOTH_DEVICE_CONN);
            delay(10);
            for (i = 0; i < 4; i++)
                uart_send(intF.p1DeviceID[i]);
            uart_printf("\n");
        }
        else
        {
            uart_printf("This is already Paired with this device\n");
        }
        break;

    case BOTH_DEVICE_CONN:
        uart_printf("P1 and P2 both the address are busy\n");
        break;

    default:
        break;
    }
}


void app()
{
#if 0
    
    rgb_blink(1, 0, 0);
#endif
#if 1
    if(intF.waitingFlag)
    {
        //uart_printf("Im here dood 1\n");
        if(Mflag.fs > 124)
        {
            Mflag.fs = 0;
            Mflag.fm++;
            uart_printf("Im here dood\n");
            if(Mflag.fm>2)
            {
                Mflag.fm = 0;
                Mflag.fs = 0;
                intF.waitingFlag = 0;
            }
        }
    }

    if (intF.sleepFlag || !WAKEUP_PIN)
    {
        _emi = 1;
        intF.sleepFlag = 0;
        LED(NOTIFI_MODE);
        u16 adcBattValue = adc_getValue();
        u8 value[10];
        itoa(adcBattValue, value, 10);
        uart_printf("Button Pressed\n");
        uart_printf("ADC Value: ");
        uart_printf((char *)value);
        uart_printf("\n");
        // lvd_read();
        // if(1 == intF.lvdF)
        // {
        //     intF.battStatus = BATT_LOW;
        //     uart_printf("Voltage is less than 3v3\n");
        // }

        lvd_mode_select(MODE_3V3);
        delay(1);
        lvd_read_select(MODE_3V3);
        delay(1);
        lvd_mode_select(MODE_3V0);
        delay(1);
        lvd_read_select(MODE_3V0);
        delay(1);

        if(1 == intF.mode3V3F)
        {
            if(1 == intF.mode3V0F)
            {
                intF.battStatus = BATT_LOW;
                uart_printf("Low Battery Voltage\n");
            }   
            else
            {
                intF.battStatus = BATT_AVG;
                uart_printf("Average Battery Voltage\n");
            }         
        }

        else
        {
            if (adcBattValue > ADC_BATT_LEVEL_2)
            {
                intF.battStatus = BATT_FULL;
                uart_printf("Full Battery\n");
                // intF.deviceID[5] = BATT_FULL;
            }

            else if (adcBattValue > ADC_BATT_LEVEL_3 && adcBattValue < ADC_BATT_LEVEL_2)
            {
                intF.battStatus = BATT_AVG;
                // intF.deviceID[5] = BATT_AVG;
                uart_printf("Average 1 Battery \n");
            }

            else if (adcBattValue < ADC_BATT_LEVEL_3)
            {
                intF.battStatus = BATT_LOW;
                // intF.deviceID[5] = BATT_LOW;
                uart_printf("Low Battery\n");
            }

            else
            {
                intF.battStatus = BATT_AVG;
                // intF.deviceID[5] = BATT_AVG;
                uart_printf("undefined Battery Value\n");
            }
        }
        // while(!WAKEUP_PIN);
        KEY_Process(NOTIFI_REQ, BATT_FULL);
        delay(20);
        intF.waitingFlag = 1;
        LED(CLEAR_MODE);
    }
#if 1
    if (!intF.waitingFlag)
    {
        uart_printf("Going to Sleep Mode\n");
        intF.sleepFlag = 1;
        System_deepsleep();
        // delay(10);
       // initSys();
        //_halt();
       
    }
#endif
#if RF_Payload_Length > 64
    if (BC3602_T.mode == RF_TX)           // while RF mode flag = TX
        ExtendFIFO_TX_Process(&BC3602_T); // RF Extend FIFO TX porcess

    if (BC3602_T.mode == RF_RX)           // while RF mode flag = RX
        ExtendFIFO_RX_Process(&BC3602_T); // RF Extend FIFO RX porcess

#endif
    RF_Finished_Process(&BC3602_T);
#endif
}

void unpairSpNode()
{
    u8 tempAddr[5];
    u8 i = 0;
    for (i = 0; i < 4; i++)
        tempAddr[i] = intF.rxPayload[i + 1];
    uart_printf("Unprint the specfice Node: ");
    uart_printf((char *)tempAddr);
    uart_printf("\n");
}

void rgb_blink(u8 color)
{
    switch (color)
    {
    case RED:
        RGB_R = 0;
        RGB_B = 1;
        RGB_G = 1;
        break;

    case GREEN:
        RGB_R = 1;
        RGB_B = 1;
        RGB_G = 0;
        break;

    case BLUE:
        RGB_R = 1;
        RGB_B = 0;
        RGB_G = 1;
        break;

    default:
        RGB_R = 1;
        RGB_B = 1;
        RGB_G = 1;
        break;
    }
}

void LED(u8 mode)
{
    switch (mode)
    {
    case CLEAR_MODE:
        rgb_blink(CLEAR_MODE);
        break;

    case START_MODE:
        rgb_blink(GREEN);
        break;

    case PAIR_MODE:
        rgb_blink(BLUE);
        break;

    case UNPAIR_MODE:
        rgb_blink(RED);
        break;

    case NOTIFI_MODE:
        rgb_blink(RED);
        break;
    default:
        break;
    }
}

void initSys(void)
{
    //WDTC();
	Sys_init();
   	sysInit();

	RF_Init();																				// RF Initialization
	RF_Parameter_loading();
	BC3602_WriteRegister(IO2_REGS,0xcd);													// debug use
	BC3602_T.mode = RF_RX;	

	//GCC_CLRWDT(); 
	//app_pair_status_check();
    _pb4 = 0;
	_pbc4 = 1;
    _emi = 1;
    GCC_CLRWDT();
}