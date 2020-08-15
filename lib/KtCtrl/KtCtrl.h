/*  KT Controller parser
    This library gets the data from the controller
    and patches it with various informations (currently only temperature)

    Author: Romelec, inspired from M.Sharonov aka obelix662000
    https://endless-sphere.com/forums/viewtopic.php?t=73475
 */

#ifndef KtCtrl_h
#define KtCtrl_h

#include <inttypes.h>

#define KtCtrlBufferLength 12

class KtCtrl
{
public:
    KtCtrl() { }
    ~KtCtrl() { }
    void init();

    int push(uint8_t data);
    void flush() { init(); };

    int setMotorTemp(uint8_t temperature); // In °C

    int seek(uint8_t buffer_idx);
    int pop(uint8_t* data);

private:
    int crc_check(void);
    void crc_update(void);
    uint8_t buffer[KtCtrlBufferLength];
    uint8_t push_idx;
    unsigned long push_last;
    uint8_t pop_idx;
};

#endif
