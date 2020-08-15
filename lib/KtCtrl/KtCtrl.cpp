/*  KT Controller parser
    This library gets the data from the controller
    and patches it with various informations (currently only temperature)

    Author: Romelec, inspired from M.Sharonov aka obelix662000
    https://endless-sphere.com/forums/viewtopic.php?t=73475
 */

#include <Arduino.h>
#include "KtCtrl.h"

void KtCtrl::init()
{
    push_idx = 0;
    pop_idx = 0;
    push_last = millis();
}

/** Add data to the buffer
 *  Returns true if the buffer is complete
 */
int KtCtrl::push(uint8_t data)
{
    unsigned long cur_time = millis();

    // Reset if no data for more than 50ms
    if (cur_time - push_last > 50)
    {
        push_idx = 0;
    }

    if (push_idx == 0 && data != 0x41)
    {
        // Invalid 1st byte
        return false;
    }
    if (push_idx >= KtCtrlBufferLength)
    {
        // Never supposed to be here
        return crc_check();
    }
    buffer[push_idx++] = data;
    push_last = cur_time;

    if (push_idx == KtCtrlBufferLength)
    {
        return crc_check();
    }
    return false;
}

/** Update the motor temperature - CRC is updated */
int KtCtrl::setMotorTemp(uint8_t temperature)
{
    if (temperature <= 15)
    {
        // 15 is the minimum
        return true;
    }
    // Remove offset
    temperature -= 15;

    // Value is signed, do not allow negative
    if (temperature > 0x7F)
    {
        temperature = 0x7F;
    }
    buffer[9] = temperature;
    crc_update();
    return crc_check();
}

/** Set the buffer to be read from a specific index */
int KtCtrl::seek(uint8_t buffer_idx)
{
    if (buffer_idx > KtCtrlBufferLength) return false;

    pop_idx = buffer_idx;
    return true;
}

/** Get a character from the buffer */
int KtCtrl::pop(uint8_t* data)
{
    *data = buffer[pop_idx];
    pop_idx++;

    if (pop_idx > KtCtrlBufferLength)
    {
        pop_idx = 0;
        return false;
    }
    return true;
}

/** Compute and check the CRC in the buffer */
int KtCtrl::crc_check(void)
{
    uint8_t crc=0;
    for(int k=1;k<12;k++)
    {
        if (k!=6) crc^=buffer[k];
    };

    return (buffer[6] == crc);
}

/** Update the CRC in the buffer */
void KtCtrl::crc_update(void)
{
    uint8_t crc=0;
    for(int k=1;k<12;k++)
    {
        if (k!=6) crc^=buffer[k];
    };
    buffer[6]=crc;
}
