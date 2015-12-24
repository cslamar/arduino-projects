#include <DS3231.h>
#include <Wire.h>

/*
    // To set Clock use the following in the setup method:
    Clock.setSecond(00);//Set the second 
    Clock.setMinute(10);//Set the minute 
    Clock.setHour(12);  //Set the hour 
    Clock.setDoW(5);    //Set the day of the week
    Clock.setDate(28);  //Set the date of the month
    Clock.setMonth(8);  //Set the month of the year
    Clock.setYear(15);  //Set the year (Last two digits of the year)
 */

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

byte year, month, date, DoW, hour, minute, second;

void setup() {
  // Start the I2C interface
  Wire.begin();
  Serial.begin(115200);
}

void ReadDS3231() {
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}

void loop() {
  // put your main code here, to run repeatedly:
  ReadDS3231();
  delay(1000);
}
