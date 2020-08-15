/*
 * KT Controller LCD overrides
 * Romelec 2020
 *
 * - Displays the motor temperature on the LCD (set C8 parameter to 1)
 * - Deactivates regen if the battery is fully charged (TODO)
 * - Lowers the assistence level if the motor is hot (TODO)
 */

#include <Arduino.h>
#include "AltSoftSerial.h"
#include "KtCtrl.h"

// Comment it if you do not need logs on the USB port
//#define DEBUG

#ifdef DEBUG
    #include <stdio.h>
    static FILE uartout = {0} ;
    static int uart_putchar (char c, FILE *stream)
    {
        Serial.write(c) ;
        return 0 ;
    }
    #define DBG_PRINTF(f_, ...) printf((f_), __VA_ARGS__)
    #define DBG_PRINT(f_) printf((f_))
#else
    #define DBG_PRINTF(f_, ...)
    #define DBG_PRINT(f_)
#endif //DEBUG

#define ADCREF 4280    //ADC reference, mV, default ADC ref is power supply voltage. Controller's voltage at red wires is ~4.28V
#define SENSOR_COEFF 10 //Sensor coefficient mV/C for LM35 sensor

AltSoftSerial altSerial;
KtCtrl ktCtrl;

static uint8_t _get_motorTemp();
//static uint8_t _get_BatteryVoltage();

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);     // USB debug
    fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartout ;

    while(!Serial && millis()<5000) {
    //wait for serial monitor to connect
    }
#endif //DEBUG

    Serial1.begin(9600);    // HW serial, from controller to display
    //altSerial.begin(9600);  // Software serial, from display to controller

    ktCtrl.init();
}

void loop()
{
    uint8_t c;

    // Get data from the controller
    if (Serial1.available())
    {
        c = Serial1.read();
        if (ktCtrl.push(c))
        {
            DBG_PRINT("\n ktCtrl OK ");
            // Data from controller available
            if (!ktCtrl.setMotorTemp(_get_motorTemp()))
            {
                DBG_PRINT("Set KO !\n");
            }

            // Send the updated data
            ktCtrl.seek(0);
            while(ktCtrl.pop(&c))
            {
                Serial1.write(c);
                DBG_PRINTF("0x%.2x ", c);
            }
        }
    }

    // Get data from the LCD
    // TODO: altSerial Tx doesn't work
    /*if (altSerial.available())
    {
        c = altSerial.read();
        DBG_PRINTF("read 0x%x\n", c);
        if (KtLcd8.push(c))
        {
            // Data from LCD available
            if (_get_BatteryVoltage >=52)
            {
                KtLcd8.setRegen(0)
            }
        }
        altSerial.write(c);
    }*/
}

static uint8_t _get_motorTemp()
{
    uint16_t tempADC=analogRead(A0);  //read temp sensor and calculate temperature
    uint32_t motorTemp=ADCREF/100*tempADC;
    motorTemp=motorTemp*100/SENSOR_COEFF;
    motorTemp=motorTemp/1024;

    DBG_PRINTF("\nmotorTemp %u - %u C\n", tempADC, (uint8_t)motorTemp);

    return (uint8_t)motorTemp;
}

/*static uint8_t _get_BatteryVoltage()
{
    return 0;
}*/
