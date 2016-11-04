
/*********************************************

LIBRARY DEVELOPMENT
Newton v1.3

*********************************************/
#define __NO_RTC
#define __NO_EEPROM
#define __NO_SWITCHES
#define __NO_LEDS
#define __NO_SPEAKER
#include "Newton.h"

void setup()
{
  Newton.Set_Alarm_Time();
//  Newton.Sound_Effect(UP_SQUEAK);
//
//  // EEPROM[0] contains the number of system restarts (autoincrementing).
//  byte Count = Newton.Memory_Read(0);
//  Newton.Memory_Write(Count+1,0);
//
//  //Newton.Initialize_Time_Date();
//  //Newton.Set_Alarm_Time();
//  Serial.begin(9600);
//  Serial.print("Number of system restarts: ");
//  Serial.println(Count);
//
//  //char Time_String[18]="03/27/72 10:30:00";
//  //Newton.Set_Time(Time_String);
//  //Newton.Set_Time("05/16/16 13:08:00");
}

//void Alarm(){
//  Newton.Status_Indicator(ON); delay(400);
//  Newton.Status_Indicator(OFF);
//}

void loop()
{
//  if(Newton.SW2_Status()==ON)
//    {
//      Newton.LED1_Indicator(ON);
//      Newton.LED2_Indicator(ON);
//    }
//  else
//    {
//      Newton.LED1_Indicator(OFF);
//      Newton.LED2_Indicator(OFF);
//    }
//
//  if(Newton.SW1_Status()==ON)
//    {
//      Newton.Beep();
//      byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
//      Newton.Get_Current_Time_Values(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
//      char Time_Date[26];
//      sprintf(Time_Date,"%02d:%02d:%02d - %02d/%02d/%04d", hour, minute, second, month, dayOfMonth, year);
//      Serial.println(Time_Date);
//      delay(150);
//    }
//  char Time_String[18];
//  Newton.Get_Time(Time_String);
//  if(strcmp(Time_String,"05/16/16 13:15:00")==0) Newton.Sound_Effect(ALARM);
//  if(strcmp(Time_String,"05/16/16 13:30:00")==0) {Newton.Sound_Effect(DOWN_SQUEAK); Newton.LED1_Indicator(ON);}
//  if(strcmp(Time_String,"05/16/16 13:45:00")==0) {Newton.Sound_Effect(DOWN_SQUEAK); Newton.LED2_Indicator(ON);}
//  if(strcmp(Time_String,"05/16/16 14:00:00")==0) {Newton.Sound_Effect(ALARM); Newton.Status_Indicator(ON);}
//  Serial.print("System Time: ");
//  Serial.println(Time_String);
//  delay(500);
}


