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

#include "Arduino.h"
#include "Newton.h"

_Newton::_Newton(){
  #ifndef __NO_LEDS
  // Configure all LED connections as outputs.
  pinMode(LED_STATUS,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  #endif

  #ifndef __NO_SWITCHES
  // Configure all switches as inputs using the internal pull-up resistors.
  pinMode(SW1,INPUT_PULLUP);
  pinMode(SW2,INPUT_PULLUP);
  #endif

  #if !defined(__NO_RTC) || !defined(_NO_EEPROM)
  Wire.begin();
  #endif
  
  // Use the compile time and date to intitalize the Real-Time clock.
  //Initialize_Time_Date();

  // If the RTC is not running, set the time to start it up. The system time is used to do this.
  //if (! RTC.isrunning()) {
  //  Serial.println("*** WARNING: Real-time clock is not running.");
  //  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  }
  }

#ifndef __NO_LEDS
ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = TIMER1_CTR;   // preload timer
  Newton.Flash_Status_LED();
}

void _Newton::Flash_Status_LED(){
  Newton.Status_Indicator(ON); delay(400);
  Newton.Status_Indicator(OFF);
}
#endif

/*****************************************************************************
 * void Set_Alarm_Time()
 * 
 * This function will initialize a hardware timer to interrupt on
 * overflow.  When this happens, the interrupt service routine will call
 * a specified function routinely.  WORK IN PROGRESS.
 */
void _Newton::Set_Alarm_Time(){
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  TCNT1 = TIMER1_CTR;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}


/*####################################################################### 
    _    _                                                        
   | |  | |                                                       
   | |  | |  ___    ___   _ __                                    
   | |  | | / __|  / _ \ | '__|                                   
   | |__| | \__ \ |  __/ | |                                      
    \____/  |___/  \___| |_|             __                       
   |_   _|         | |                  / _|                      
     | |    _ __   | |_    ___   _ __  | |_    __ _    ___    ___ 
     | |   | '_ \  | __|  / _ \ | '__| |  _|  / _` |  / __|  / _ \
    _| |_  | | | | | |_  |  __/ | |    | |   | (_| | | (__  |  __/
   |_____| |_| |_|  \__|  \___| |_|    |_|    \__,_|  \___|  \___|
                                                                  
 #######################################################################*/

#ifndef __NO_LEDS
/*****************************************************************************
 * void _Newton::LED1_Indicator(bool Value)
 * 
 * This is a high level function call that allows a user to turn LED1 on or
 * off.
 */
void _Newton::LED1_Indicator(bool Value){
  if(Value==ON)
    digitalWrite(LED1,HIGH);
  else
    digitalWrite(LED1,LOW);
  }

/*****************************************************************************
 * void _Newton::LED2_Indicator(bool Value)
 * 
 * This is a high level function call that allows a user to turn LED2 on or
 * off.
 */
void _Newton::LED2_Indicator(bool Value){
  if(Value==ON)
    digitalWrite(LED2,HIGH);
  else
    digitalWrite(LED2,LOW);
  }

/*****************************************************************************
 * void _Newton::Status_Indicator(bool Value)
 * 
 * This is a high level function call that allows a user to turn the status 
 * LED on or off.
 */
void _Newton::Status_Indicator(bool Value){
  if(Value==ON)
    digitalWrite(LED_STATUS,HIGH);
  else
    digitalWrite(LED_STATUS,LOW);
  }
#endif

#ifndef __NO_SWITCHES
/*****************************************************************************
 * bool _Newton::SW1_Status()
 * 
 * Returns a value of true or false based on whether or not SW1 has been
 * pressed. The reading is done after a short period of time to allow for the 
 * mechanical parts to settle in a position (switch debouncing).
 */
bool _Newton::SW1_Status(){
  delay(DEBOUNCE_TIME);
  return(!digitalRead(SW1));
  }

/*****************************************************************************
 * bool _Newton::SW2_Status()
 * 
 * Returns a value of true or false based on whether or not SW2 has been
 * pressed. The reading is done after a short period of time to allow for the 
 * mechanical parts to settle in a position (switch debouncing).
 */
bool _Newton::SW2_Status(){
  delay(DEBOUNCE_TIME);
  return(!digitalRead(SW2));
  }
#endif

#ifndef __NO_SPEAKER
/*****************************************************************************
 * void _Newton::Beep()
 * 
 * Produces a tone at the speaker at a set frequency and and for a set period
 * beore returning to silence.
 */
void _Newton::Beep(){
  tone(SPKR,BEEP_FREQ,BEEP_TIME);
}

/*****************************************************************************
 * void _Newton::Beep(int Duration)
 * 
 * Produces a tone at the speaker at a set frequency(Hz) and for a specified
 * period (mS) beore returning to silence.
 */
void _Newton::Beep(int Duration){
  tone(SPKR,BEEP_FREQ,Duration);
}

/*****************************************************************************
 * void _Newton::Beep(int Frequency, int Duration)
 * 
 * Produces a tone at the speaker at a given frequency(Hz) and for a
 * specified period(mS) beore returning to silence.
 */
void _Newton::Beep(int Frequency, int Duration){
  tone(SPKR,Frequency,Duration);
}

/*****************************************************************************
 * void _Newton::Tone(int Frequency)
 * 
 * Produces a continuous tone at the speaker at a given frequency(Hz).
 */
void _Newton::Tone(int Frequency){
  tone(SPKR,Frequency);
}

/*****************************************************************************
 * void _Newton::Tone(int Frequency, int Duration))
 * 
 * Produces a continuous tone at the speaker at a given frequency(Hz) and for
 * a given duration(mS).
 */
void _Newton::Tone(int Frequency, int Duration){
  tone(SPKR,Frequency,Duration);
}

/*****************************************************************************
 * void _Newton::Silence()
 * 
 * Unconditionally stops any tones being produced at the speaker.
 */
void _Newton::Silence(){
  noTone(SPKR);
}

/*****************************************************************************
 * void _Newton::Sound_Effect(int Effect)
 * 
 * This function produces various auditory special effects at the speaker. 
 */
void _Newton::Sound_Effect(int Effect){
  switch(Effect)
    {
    case UP_SQUEAK:
      for(int f=100;f<=10000;f+=10)
        tone(SPKR,f,100);
      break;
    case DOWN_SQUEAK:
      for(int f=10000;f>=100;f-=10)
        tone(SPKR,f,100);
      break;
    case ALARM:
      for(int i=1;i<=10;i++)
        {
          for(int f=100;f<=10000;f+=100)
            tone(SPKR,f,20);
          for(int f=10000;f>=500;f-=50)
            tone(SPKR,f,30);
          delay(250);
        }
      break;
    };
}
#endif

/*#######################################################################  
    ______   ______   _____    _____     ____    __  __ 
   |  ____| |  ____| |  __ \  |  __ \   / __ \  |  \/  |
   | |__    | |__    | |__) | | |__) | | |  | | | \  / |
   |  __|   |  __|   |  ___/  |  _  /  | |  | | | |\/| |
   | |____  | |____  | |      | | \ \  | |__| | | |  | |
   |______| |______| |_|      |_|  \_\  \____/  |_|  |_|
   |  \/  |                                             
   | \  / |   ___   _ __ ___     ___    _ __   _   _    
   | |\/| |  / _ \ | '_ ` _ \   / _ \  | '__| | | | |   
   | |  | | |  __/ | | | | | | | (_) | | |    | |_| |   
   |_|  |_|  \___| |_| |_| |_|  \___/  |_|     \__, |   
                                                __/ |   
                                               |___/    
  #######################################################################*/
#ifndef __NO_EEPROM
/*****************************************************************************
 * void _Newton::Memory_Write(byte Data, unsigned long Address)
 * 
 * An abstract interface to the EEPROM via the I2C bus.  This function will
 * copy the given data to the specified location in the EEPROM memory.  The
 * memory unit is organized as 32k * 8.  Address validtion is not performed by
 * this function.
 */
void _Newton::Memory_Write(byte Data, unsigned long Address)
{
  Wire.beginTransmission(EEPROM_ADDRESS);   // Initiate I2C communication to device number 0x50 (hard-wired address).
  Wire.write(Address>>8);                  // Upper 8-bits of address.
  Wire.write(Address&0xFF);                // Lower 8-bits of address.
  Wire.write(Data);                        // 8-bits of data.
  Wire.endTransmission();                  // Terminate I2C communication.
  delay(5);
}

/*****************************************************************************
 * void _Newton::Memory_Read(unsigned long Address)
 * 
 * An abstract interface to the EEPROM via the I2C bus.  This function will
 * return the data at specified location in the EEPROM memory.  The memory
 * unit is organized as 32k * 8.  Address validtion is not performed by this
 * function.
 */
byte _Newton::Memory_Read(unsigned long Address)
{
  byte Data;
  
  Wire.beginTransmission(EEPROM_ADDRESS);   // Initiate I2C communication to device number 0x50 (hard-wired address).
  Wire.write(Address>>8);                  // Upper 8-bits of address.
  Wire.write(Address&0xFF);                // Lower 8-bits of address.
  Wire.endTransmission();                  // Terminate I2C communication.
  
  Wire.requestFrom(EEPROM_ADDRESS,1);       // Request data from the EEPROM.
  if(Wire.available())                     // If there is data available, then retrieve it.
    Data = Wire.read();  
  return(Data);
}
#endif
/*#######################################################################  
    _____                   _            _______   _                    
   |  __ \                 | |          |__   __| (_)                   
   | |__) |   ___    __ _  | |  ______     | |     _   _ __ ___     ___ 
   |  _  /   / _ \  / _` | | | |______|    | |    | | | '_ ` _ \   / _ \
   | | \ \  |  __/ | (_| | | |             | |    | | | | | | | | |  __/
   |_|__\_\  \___|  \__,_| |_|  _          |_|    |_| |_| |_| |_|  \___|
    / ____| | |                | |                                      
   | |      | |   ___     ___  | | __                                   
   | |      | |  / _ \   / __| | |/ /                                   
   | |____  | | | (_) | | (__  |   <                                    
    \_____| |_|  \___/   \___| |_|\_\                                   
                                                                        
  #######################################################################*/
#ifndef __NO_RTC
/*****************************************************************************
 * byte _Newton::Decimal_To_BCD(byte Decimal_Value)
 * 
 * A support function used to interpret decimal values for the real-time clock.
 * This function will convert a BCD value from 0 to 99 (unvalidated) and 
 * convert it to its corresponding decimal value.  Both values are byte encoded.
 */
 byte _Newton::Decimal_To_BCD(byte Decimal_Value)
{
  byte BCD_Value = (Decimal_Value/10*16) + (Decimal_Value%10);
  return(BCD_Value);
}

/*****************************************************************************
 * byte _Newton::BCD_To_Decimal(byte BCD_Value)
 * 
 * A support function used to interpret BCD values from the real-time clock.
 * This function will convert a decimal value from 0 to 99 (unvalidated) and
 * convert it to its corresponding BCD value.  Both values are byte encoded.
 */
byte _Newton::BCD_To_Decimal(byte BCD_Value)
{
  byte Decimal_Value = (BCD_Value/16*10) + (BCD_Value%16);
  return(Decimal_Value);
}



void _Newton::Set_Current_Time_Values(  byte second, byte minute, byte hour, byte dayOfWeek,
                byte dayOfMonth, byte month, byte year)
{
   Wire.beginTransmission(RTC_ADDRESS);
   Wire.write(0);
   Wire.write(Decimal_To_BCD(second));    // 0 to bit 7 starts the clock
   Wire.write(Decimal_To_BCD(minute));
   Wire.write(Decimal_To_BCD(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(Decimal_To_BCD(dayOfWeek));
   Wire.write(Decimal_To_BCD(dayOfMonth));
   Wire.write(Decimal_To_BCD(month));
   Wire.write(Decimal_To_BCD(year));
   Wire.endTransmission();
}

void _Newton::Get_Current_Time_Values(  byte *second, byte *minute, byte *hour, byte *dayOfWeek,
                byte *dayOfMonth, byte *month, byte *year)
{
// Reset the register pointer
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(RTC_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
//  *second     = Wire.read() & 0x7f;
//  *minute     = Wire.read();
//  *hour       = Wire.read() & 0x3f;  // Need to change this if 12 hour am/pm
//  *dayOfWeek  = Wire.read();
//  *dayOfMonth = Wire.read();
//  *month      = Wire.read();
//  *year       = Wire.read();

  *second     = BCD_To_Decimal(Wire.read() & 0x7f);
  *minute     = BCD_To_Decimal(Wire.read());
  *hour       = BCD_To_Decimal(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = BCD_To_Decimal(Wire.read());
  *dayOfMonth = BCD_To_Decimal(Wire.read());
  *month      = BCD_To_Decimal(Wire.read());
  *year       = BCD_To_Decimal(Wire.read());
}

void _Newton::Initialize_Time_Date()
{
  /* These functions use up memory and should be replaced by precompiler directives and macros.
   * This function does not check for errors in the __DATE__ and __TIME__ macros. If there were
   * errors, these would return all '?' characters.
   */
  int Compile_Hour = ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0');
  int Compile_Minute = ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0');
  int Compile_Second = ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0');

  int Compile_Month;
  String Date = String(__DATE__);
  if(Date.substring(0,3)=="Jan") Compile_Month = 1;
  if(Date.substring(0,3)=="Feb") Compile_Month = 2;
  if(Date.substring(0,3)=="Mar") Compile_Month = 3;
  if(Date.substring(0,3)=="Apr") Compile_Month = 4;
  if(Date.substring(0,3)=="May") Compile_Month = 5;
  if(Date.substring(0,3)=="Jun") Compile_Month = 6;
  if(Date.substring(0,3)=="Jul") Compile_Month = 7;
  if(Date.substring(0,3)=="Aug") Compile_Month = 8;
  if(Date.substring(0,3)=="Sep") Compile_Month = 9;
  if(Date.substring(0,3)=="Oct") Compile_Month = 10;
  if(Date.substring(0,3)=="Nov") Compile_Month = 11;
  if(Date.substring(0,3)=="Dec") Compile_Month = 12;

  int Compile_Day  =  ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + (__DATE__[5] - '0');
  int Compile_Year =  (__DATE__[ 7] - '0') * 1000 + (__DATE__[ 8] - '0') *  100 + (__DATE__[ 9] - '0') *   10 + (__DATE__[10] - '0') ;

  // The day of the week is not calculated by the DS1307.  It is arbitrarily set to 0 but will increment on 24h cycles.
  Set_Current_Time_Values(Compile_Second, Compile_Minute, Compile_Hour, 0 , Compile_Day, Compile_Month, Compile_Year - 2000);
  }

void _Newton::Set_Time(char *Time_String){
  // Time string format:
  // MM/DD/YY HH:MM:SS
  int Month, Day, Year;
  int Hours, Minutes, Seconds;
  
  sscanf(Time_String,"%02d/%02d/%02d %02d:%02d:%02d",&Month, &Day, &Year, &Hours, &Minutes, &Seconds);
  Set_Current_Time_Values((byte)Seconds, (byte)Minutes, (byte)Hours, 0 , (byte)Day, (byte)Month, (byte)Year);
  Serial.print("Current Time Set: ");
  Serial.print(Month);    Serial.print("/");
  Serial.print(Day);      Serial.print("/");
  Serial.print(Year);     Serial.print(" ");
  Serial.print(Hours);    Serial.print(":");
  Serial.print(Minutes);  Serial.print(":");
  Serial.println(Seconds);
}

void _Newton::Get_Time(char *Time_String){
  // Time string format:
  // MM/DD/YY HH:MM:SS
  byte Month, Day, Year, DayOfWeek;
  byte Hours, Minutes, Seconds;
  
  Get_Current_Time_Values(&Seconds, &Minutes, &Hours, &DayOfWeek, &Day, &Month, &Year);
  sprintf(Time_String,"%02d/%02d/%02d %02d:%02d:%02d",Month, Day, Year, Hours, Minutes, Seconds);
//  Serial.print("System Time: ");
//  Serial.println(Time_String);
//  Serial.print(Month);    Serial.print("/");
//  Serial.print(Day);      Serial.print("/");
//  Serial.print(Year);     Serial.print(" ");
//  Serial.print(Hours);    Serial.print(":");
//  Serial.print(Minutes);  Serial.print(":");
//  Serial.println(Seconds);
}
#endif

_Newton Newton = _Newton();

