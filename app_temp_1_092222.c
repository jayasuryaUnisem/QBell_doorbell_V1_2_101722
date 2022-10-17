#include "app.h"

extern int_flags intF;

extern struct define_flag Mflag;
//
extern _BC3602_device_ BC3602_T;

void app_pair_status_check()
{
    uart_printf("Device Address: ");

	eepromDeviceAddrRead();
	delay(10);
	eepromPstatusCheck();
	delay(10);

	u8 j;
	uart_printf("Device ID: ");

	for(j = 0; j<4; j++)
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
		
		for(i = 0; i<4; i++)
			uart_send(intF.p1DeviceID[i]);
		uart_printf("\n");
		break;
	
	case P2_DEVICE_CONN:
		eepromP2Read();
		uart_printf("P2 Device Paired: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p2DeviceID[i]);
		uart_printf("\n");
		break;
	
	case BOTH_DEVICE_CONN:
		eepromP1Read();
		eepromP2Read();
		uart_printf("Both the Device are Paired\n");
		uart_printf("P1 Device ID: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p1DeviceID[i]);
		uart_printf("\nP2 Device ID: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p2DeviceID[i]);
		uart_printf("\n");
		break;	
	
	default:
		break;
	}

	uart_printf("\n");

}

void app_pair_status_update()
{
    uart_printf("Pairing Ack Received\n");
    intF.pairingAck = 0;
    eepromPstatusCheck();
    delay(10);
    u8 i = 0;
    u8 tempArr[5];
    for(i = 0;i<4;i++)
        tempArr[i] = intF.rxPayload[i+1];
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
        for(i = 0;i<4;i++)
            uart_send(intF.p1DeviceID[i]);
        uart_printf("\n");
        break;
    
    case P1_DEVICE_CONN:
        eepromP1Read();
        delay(10);
        if(memcmp(tempArr, intF.p1DeviceID, sizeof(intF.p1DeviceID)))
        {
            uart_printf("Writing the ID to the P2 Address: ");
            eepromP2Write();
            delay(10);
            eepromP2Read();
            delay(10);
            eepromPstatusWrite(BOTH_DEVICE_CONN);
            delay(10);
            for(i = 0;i<4;i++)
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
        if(memcmp(tempArr, intF.p2DeviceID, sizeof(intF.p2DeviceID)))
        {
            uart_printf("Writing trhe ID to the P1 Address: ");
            eepromP1Write();
            delay(10);
            eepromP1Read();
            delay(10);
            eepromPstatusWrite(BOTH_DEVICE_CONN);
            delay(10);
            for(i = 0;i<4;i++)
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
    
    if(!_pb4)
    {
        uart_printf("Button Pressed\n");
    }
    #endif
    #if 1
    if(intF.waitingFlag)
    { 
        if(Mflag.fs > 124)
        {
            uart_printf("Waiting for Ack\n");
            Mflag.fm++;
            if(Mflag.fm > 5)
            {
                uart_printf("Timeout\n");
                Mflag.fm = 0;
                Mflag.fs = 0;
                intF.waitingFlag = 0;
                Mflag.fs = 0;
                System_deepsleep();
            }
            Mflag.fs = 0;
        }
    }
    if(!intF.pairStatus || 1 == intF.waitingFlag)
    {
        if(1 == intF.pairingAck)
        {
            //app_pair_status_update();
            intF.pairingAck = 0;
            eepromPstatusCheck();
            if(BOTH_DEVICE_CONN == intF.pairStatus)
                uart_printf("2 Devices already Paired\n");
            else
            {
                //app_pair_status_update();
                u8 tempValue = intF.rxPayload[0];
                u8 tempArr[5];
                u8 i = 0;

                app_pair_status_check();

                for(i = 0; i<4; i++)
                    tempArr[i] = intF.rxPayload[i+1];

                switch (tempValue)
                {
                case NOTIFI_REQ:
                    uart_printf("Notification Ack Received\n");
                    break;

                case PAIR_REQ:
                    if(!memcmp(tempArr, intF.p1DeviceID, sizeof(intF.p1DeviceID)))
                        uart_printf("This Device is Already Paired at P1\n");
                    
                    else if(!memcmp(tempArr, intF.p2DeviceID, sizeof(intF.p2DeviceID)))
                        uart_printf("This Device is Already Paired at P2\n");

                    else
                    {
                        uart_printf("Pairing Request Received\n");
                        KEY_Process(PAIR_CONF_REQ, 0x00);
                    }
                    break;
                
                case PAIR_CONF_REQ:
                    LED(CLEAR_MODE);
                    LED(PAIR_MODE);
                    delay(2000);
                    LED(CLEAR_MODE);
                    uart_printf("Pairing Confirmation Request Received\n");
                    app_pair_status_update();
                    eepromPstatusCheck();
                    break;
                
                case UNPAIR_REQ:
                    uart_printf("Unpairing Request Received\n");

                    break;
                
                case UNPAIR_CONF_REQ:
                    intF.waitingFlag = 0;
                    Mflag.fs = 0;
                    Mflag.fm = 0;
                    uart_printf("Unpaired Confirmation Request Received\n");
                    break;
                
                default:
                    uart_printf("Not a valid data\n");
                    break;
                }
            }
        }
    }	
    else
    {
        if(!WAKEUP_PIN)
        {
            uart_printf("Button Pressed\n");
            while(!WAKEUP_PIN);
            delay(1000);
            KEY_Process(NOTIFI_REQ, BATT_FULL);
            delay(20);
            intF.waitingFlag = 1;
            

        }

        if(!UNPAIR_BUTTON)
        {
            LED(CLEAR_MODE);
            LED(UNPAIR_MODE);
            delay(2000);
            LED(CLEAR_MODE);
            uart_printf("Unpairing the All Device\n");
            //intF.waitingFlag = 1;
            delay(200);
            KEY_Process(UNPAIR_REQ, BATT_FULL);
            eepromUnpairAll();
            delay(10);
            app_pair_status_check();
            delay(10);
            eepromPstatusCheck();
            delay(10);
        }

        if(!intF.waitingFlag)
        {
            System_deepsleep();
            _halt();
        }
        
    }
    #if RF_Payload_Length>64
    if(BC3602_T.mode == RF_TX)													// while RF mode flag = TX
        ExtendFIFO_TX_Process(&BC3602_T);												// RF Extend FIFO TX porcess
        
    if(BC3602_T.mode == RF_RX)															// while RF mode flag = RX
        ExtendFIFO_RX_Process(&BC3602_T);												// RF Extend FIFO RX porcess	
    
    #endif
    RF_Finished_Process(&BC3602_T);	
    #endif
}

void unpairSpNode()
{
    u8 tempAddr[5];
    u8 i = 0;
    for(i = 0;i<4; i++)
        tempAddr[i] = intF.rxPayload[i+1];
    uart_printf("Unprint the specfice Node: ");
    uart_printf(tempAddr);
    uart_printf("\n");
    
}

void rgb_blink(u8 r, u8 g, u8 b)
{
    RGB_R = r;
    RGB_G = g;
    RGB_B = b;
}

void LED(u8 mode)
{
    switch (mode)
    {
    case CLEAR_MODE:
        rgb_blink(1, 1, 1);
        break;

    case START_MODE:
        rgb_blink(0, 1, 1);
        break;
    
    case PAIR_MODE:
        rgb_blink(1, 0, 1);
        break;
    
    case UNPAIR_MODE:
        rgb_blink(1, 1, 0);
        break;

    default:
        break;
    }
}