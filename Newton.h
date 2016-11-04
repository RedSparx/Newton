/*
  _   _                  _                        __     ____  
 | \ | |                | |                      /_ |   |___ \ 
 |  \| |  ___ __      __| |_  ___   _ __   __   __| |     __) |
 | . ` | / _ \\ \ /\ / /| __|/ _ \ | '_ \  \ \ / /| |    |__ < 
 | |\  ||  __/ \ V  V / | |_| (_) || | | |  \ V / | | _  ___) |
 |_| \_| \___|  \_/\_/   \__|\___/ |_| |_|   \_/  |_|(_)|____/ 
  ___          _  ___                       
 | _ \ ___  __| |/ __| _ __  __ _  _ _ __ __
 |   // -_)/ _` |\__ \| '_ \/ _` || '_|\ \ /
 |_|_\\___|\__,_||___/| .__/\__,_||_|  /_\_\
                      |_|                   
 A simplified interface to the RedSparx* Newton v1.3 rapid-prototyping
 system. It is intended for rapid-development of applications.  Code
 is deliberately unoptimized so that developers can understand how the
 hardware is used and be able to develop their specific application with
 confidence.

 This C++ interface implements the "Newton" object with member functions
 that can be used to operate Newton's hardware functions.  Specifically, 
 This module provides an interface to the:

    - two (2) hardware switches
    - three (3) LEDs
    - speaker
    - battery-backed RTC
    - 32k EEPROM

  No warranty is provided with this software and users should refer to
  the schematic provided with the board before running any code.

*/

#ifndef NEWTON_H
#define NEWTON_H

#if !defined(__NO_RTC) || !defined(_NO_EEPROM)
#include <Wire.h>
#endif


/************************************
 * Conditional Compilation
 * -----------------------
 * 
 * To reduce the codespace used, not all of this module is needed.  When 
 * these tokens are defined, specific hardware interfaces will not be used.
 * Users may uncomment those tokens here, or explicitly define them in their
 * code (recommended).  If defined, they should appear before "Newton.h" is  
 * included. Bear in mind that devices that are not being used still draw
 * power as they are still connected.  
 */
//#define __NO_RTC
//#define __NO_EEPROM
//#define __NO_SWITCHES
//#define __NO_LEDS
//#define __NO_SPEAKER

// Definition of pin numbers.
#define SW1          4
#define SW2          8
#define LED1         6
#define LED2         7
#define LED_STATUS   13
#define RF_TX        9
#define RF_RX        12
#define SPKR         2

// Other Definitions
#define ON            HIGH
#define OFF           LOW
#define BEEP_FREQ     1500
#define BEEP_TIME     100
#define TIMER1_CTR    34286
#define DEBOUNCE_TIME 25

// Sound Effect Definitions
#define UP_SQUEAK    0
#define DOWN_SQUEAK  1
#define ALARM        2

// I2C Device numbers.
#define EEPROM_ADDRESS   0x50
#define RTC_ADDRESS      0x68

// Macro definitions.
#define _SW1_ACTIVE      !digitalRead(SW1)
#define _SW2_ACTIVE      !digitalRead(SW2)
#define _QUICK_BEEP      tone(SPKR,2000,20)
#define _LED1_ON         digitalWrite(LED1,HIGH)
#define _LED1_OFF        digitalWrite(LED1,LOW)
#define _LED2_ON         digitalWrite(LED2,HIGH)
#define _LED2_OFF        digitalWrite(LED2,LOW)
#define _STATUS_ON       digitalWrite(LED_STATUS,HIGH)
#define _STATUS_OFF      digitalWrite(LED_STATUS,LOW)

class _Newton{
  private:
    #ifndef __NO_RTC
    byte Decimal_To_BCD(byte Value);
    byte BCD_To_Decimal(byte Value);
    #endif
  public:
    _Newton();
  
    #ifndef __NO_LEDS
    // Indicators 
    void LED1_Indicator(bool Value);
    void LED2_Indicator(bool Value);
    void Status_Indicator(bool Value);
    void Flash_Status_LED();
    #endif

    #ifndef __NO_SWITCHES
    // Switches
    bool SW1_Status();
    bool SW2_Status();
    #endif

    #ifndef __NO_SPEAKER
    // Speaker
    void Beep();
    void Beep(int Duration);
    void Beep(int Frequency,int Duration);
    void Sound_Effect(int Effect);
    
    void Tone(int Frequency);
    void Tone(int Frequency,int Duration);
    void Silence();
    #endif

    #ifndef __NO_RTC
    // Real-Time Clock Commands
    void Set_Current_Time_Values( byte second,   // 0-59
                   byte minute,           // 0-59
                   byte hour,             // 1-23
                   byte dayOfWeek,        // 1-7
                   byte dayOfMonth,       // 1-28/29/30/31
                   byte month,            // 1-12
                   byte year);            // 0-99
    void Get_Current_Time_Values( byte *second,  // 0-59
                   byte *minute,          // 0-59
                   byte *hour,            // 1-23
                   byte *dayOfWeek,       // 1-7
                   byte *dayOfMonth,      // 1-28/29/30/31
                   byte *month,           // 1-12
                   byte *year);           // 0-99
    void Set_Time(char *Time_String);
    void Get_Time(char *Time_String);
    void Initialize_Time_Date();
    void Set_Alarm_Time();
    void *Alarm_Function;
    #endif

    #ifndef __NO_EEPROM
    // EEPROM Memory Commands
    byte Memory_Read(unsigned long Address);
    void Memory_Write(byte Data, unsigned long Address);
    #endif
    };

extern _Newton Newton;
#endif
