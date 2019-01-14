/*
 * Code that taked in data from 2 thermocouples and a DNT Humidity/Temperature Sensor and outputs them to a Nextion Display 
 * and saves the data in a file in an SD card. 
 */
#include "max6675.h"
#include "SoftwareSerial.h"
#include "TimeLib.h"
#include "Nextion.h"
#include "DHT.h"
#include "DHT_U.h"
#include "Wire.h"
#include "RTClib.h"
#include "SPI.h"
#include "SD.h"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int chipSelect = 10;
#define DHTPIN 6
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

//Set variables for humidity and temperature and pins for both devices
int thermo1SO = 46;
int thermo1CS = 44;
int thermo1SCK = 42;
int thermo2SO = 32;
int thermo2CS = 30;
int thermo2SCK = 28;
int DNTgnd = 5;
int DNTout = 6;
int DNTpos = 7;
MAX6675 thermocouple1(thermo1SCK, thermo1CS, thermo1SO);
MAX6675 thermocouple2(thermo2SCK, thermo2CS, thermo2SO);
int vccPin1 = 48;
int vccPin2 = 34;
int gndThermo2 = 36;
//Sets misc variables used in code, BT for Barrel Temperature (dummy variable for testing) and currentpage to track the current page
int BT = 100;
int currentpage = 0;

//sets new scaled values for which page they are displayed on
int newBT0 = 0;
int newNT0 = 0;
int newhum0 = 0;
int newRT0 = 0;
int newBT1 = 0;
int newNT1 = 0;
int newhum2 = 0;
int newRT3 = 0;
int newBT5 = 0;
int newNT4 = 0;
int oldmaxH = 0;
int oldmax2 = 0;
int oldmaxRT = 0;
int oldmaxNT = 0;
int oldmaxBT = 0;
//sets min and max values for each page
int setmaxy0 = 0;
int setminy0 = 0;
int setmaxy1 = 0;
int setminy1 = 0;
int setmaxy2 = 0;
int setminy2 = 0;
int setmaxy3 = 0;
int setminy3 = 0;
int setmaxy4 = 0;
int setminy4 = 0;
int setmaxy5 = 0;
int setminy5 = 0;
int cmaxRT = 0; //current maximum and minimum for Room Temperature
int cminRT = 0;
int cmaxH = 0;//current maximum and minimum for Humidity
int cminH = 0;
int cmaxNT = 0; //current maximum and minimum for Nozzle Temp
int cminNT = 0;
int cmaxBT = 0; //current maximum and minimum for Barrel Temp 
int cminBT = 0;
int cmax2 = 0;
int cmin2 = 0;
float RTdata[183];
float Hdata[183];
float NTdata[183];
float BTdata[183];
int place = 0;
int hum = 0;
int RT = 0;
int NT = 0;
int delaymin = 0;
int delaysec = 0;
int UShours0 = 0;
int UShours1 = 0;
int counter = 0;

//refresh variables 
boolean refresh0 = false;
boolean refresh1 = false;
boolean refresh2 = false;
boolean refresh3 = false;
boolean refresh4 = false;
boolean refresh5 = false;

//Declare objects from Nextion Display
NexNumber maxy0 = NexNumber(6, 15, "maxy0");
NexNumber miny0 = NexNumber(6, 20, "miny0");
NexButton minusmaxy0 = NexButton(6, 11, "minusmaxy0");
NexButton minusminy0 = NexButton(6, 25, "minusminy0");
NexButton plusmaxy0 = NexButton(6, 10, "plusmaxy0");
NexButton plusminy0 = NexButton(6, 4, "plusminy0");
NexPage page0 = NexPage(0, 0, "page0");
NexNumber maxy1 = NexNumber(6, 16, "maxy1");
NexNumber miny1 = NexNumber(6, 21, "miny1");
NexButton minusmaxy1 = NexButton(6, 9, "minusmaxy1");
NexButton minusminy1 = NexButton(6, 5, "minusminy1");
NexButton plusmaxy1 = NexButton(6, 8, "plusmaxy1");
NexButton plusminy1 = NexButton(6, 26, "plusminy1");
NexPage page1 = NexPage(1, 0, "page1");
NexNumber maxy2 = NexNumber(6, 17, "maxy2");
NexNumber miny2 = NexNumber(6, 22, "miny2");
NexButton minusmaxy2 = NexButton(6, 7, "minusmaxy2");
NexButton minusminy2 = NexButton(6, 24, "minusminy2");
NexButton plusmaxy2 = NexButton(6, 6, "plusmaxy2");
NexButton plusminy2 = NexButton(6, 23, "plusminy2");
NexPage page2 = NexPage(2, 0, "page2");
NexNumber maxy3 = NexNumber(7, 15, "maxy3");
NexNumber miny3 = NexNumber(7, 20, "miny3");
NexButton minusmaxy3 = NexButton(7, 11, "minusmaxy3");
NexButton minusminy3 = NexButton(7, 25, "minusminy3");
NexButton plusmaxy3 = NexButton(7, 10, "plusmaxy3");
NexButton plusminy3 = NexButton(7, 4, "plusminy3");
NexPage page3 = NexPage(3, 0, "page3");
NexNumber maxy4 = NexNumber(7, 16, "maxy4");
NexNumber miny4 = NexNumber(7, 21, "miny4");
NexButton minusmaxy4 = NexButton(7, 9, "minusmaxy4");
NexButton minusminy4 = NexButton(7, 5, "minusminy4");
NexButton plusmaxy4 = NexButton(7, 8, "plusmaxy4");
NexButton plusminy4 = NexButton(7, 26, "plusminy4");
NexPage page4 = NexPage(4, 0, "page4");
NexNumber maxy5 = NexNumber(7, 17, "maxy5");
NexNumber miny5 = NexNumber(7, 22, "miny5");
NexButton minusmaxy5 = NexButton(7, 7, "minusmaxy5");
NexButton minusminy5 = NexButton(7, 24, "minusminy5");
NexButton plusmaxy5 = NexButton(7, 6, "plusmaxy5");
NexButton plusminy5 = NexButton(7, 23, "plusminy5");
NexPage page5 = NexPage(5, 0, "page5");
NexPage page6 = NexPage(6, 0, "page6");
NexPage page7 = NexPage(7, 0, "page7");

char buffer[100] = {0};

//Declare list for the loop function to listen to see if any of these are active
NexTouch *nex_listen_list[] = {
  &maxy0,
  &miny0,
  &minusmaxy0,
  &minusminy0,
  &plusmaxy0,
  &plusminy0,
  &page0,
  &maxy1,
  &miny1,
  &minusmaxy1,
  &minusminy1,
  &plusmaxy1,
  &plusminy1,
  &page1,
  &maxy2,
  &miny2,
  &minusmaxy2,
  &minusminy2,
  &plusmaxy2,
  &plusminy2,
  &page2,
  &maxy3,
  &miny3,
  &minusmaxy3,
  &minusminy3,
  &plusmaxy3,
  &plusminy3,
  &page3,
  &maxy4,
  &miny4,
  &minusmaxy4,
  &minusminy4,
  &plusmaxy4,
  &plusminy4,
  &page4,
  &maxy5,
  &miny5,
  &minusmaxy5,
  &minusminy5,
  &plusmaxy5,
  &plusminy5,
  &page5,
  &page6,
  &page7,
  NULL
};

//Create functions for each of the touch events
void maxy0PopCallback(void *ptr)
{
  dbSerialPrintln("maxy0PopCallback");
}
void miny0PopCallback(void *ptr)
{
  dbSerialPrintln("miny0PopCallback");
  //miny0.setValue(0);
}
void plusmaxy0PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("plusmaxy0PopCallback");

 maxy0.getValue(&number);
 
 Serial.print("maxy0.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy0 = number;
 refresh0 = true;
}
void minusmaxy0PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("minusmaxy0PopCallback");

 maxy0.getValue(&number);
    
 Serial.print("maxy0.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy0 = number;
 refresh0 = true;
}
void plusminy0PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy0PopCallback");

  miny0.getValue(&number);

  Serial.print("miny0.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy0 = number;
  refresh0 = true;
}
void minusminy0PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy0PopCallback");

  miny0.getValue(&number);

  Serial.print("miny0.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy0 = number;
  refresh0 = true;
}
void page0PushCallback(void *ptr)
{
  dbSerialPrintln("page0PushCallback");
  //page0.show();
  currentpage = 0;
}
void maxy1PopCallback(void *ptr)
{
  dbSerialPrintln("maxy1PopCallback");
  //maxy0.setValue(0);
}
void miny1PopCallback(void *ptr)
{
  dbSerialPrintln("miny1PopCallback");
  //miny0.setValue(0);
}
void plusmaxy1PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("plusmaxy1PopCallback");

 maxy1.getValue(&number);

 Serial.print("maxy1.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy1 = number;
 refresh1 = true;
}
void minusmaxy1PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("minusmaxy1PopCallback");

 maxy1.getValue(&number);

 Serial.print("maxy1.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy1 = number;
 refresh1 = true;
}
void plusminy1PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy1PopCallback");

  miny1.getValue(&number);

  Serial.print("miny1.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy1 = number;
  refresh1 = true;
  
}
void minusminy1PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy1PopCallback");

  miny1.getValue(&number);

  Serial.print("miny1.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy1 = number;
  refresh1 = true;
}
void page1PushCallback(void *ptr)
{
  dbSerialPrintln("page1PushCallback");
  currentpage = 1;
}
void maxy2PopCallback(void *ptr)
{
  dbSerialPrintln("maxy2PopCallback");
  maxy2.setValue(0);
}
void miny2PopCallback(void *ptr)
{
  dbSerialPrintln("miny2PopCallback");
  miny2.setValue(0);
}
void plusmaxy2PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusmaxy2PopCallback");

  maxy2.getValue(&number);

  Serial.print("maxy2.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setmaxy2 = number;
  refresh2 = true;
  
}
void minusmaxy2PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusmaxy2PopCallback");

  maxy2.getValue(&number);

  Serial.print("maxy2.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setmaxy2 = number;
}
void plusminy2PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy2PopCallback");

  miny2.getValue(&number);

  Serial.print("miny2.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy2 = number;
  refresh2 = true;
}
void minusminy2PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy0PopCallback");

  miny2.getValue(&number);

  Serial.print("miny2.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy2 = number;
  refresh2 = true;
}
void page2PushCallback(void *ptr)
{
  dbSerialPrintln("page0PushCallback");
  //page2.show();
  currentpage = 2;
}
void maxy3PopCallback(void *ptr)
{
  dbSerialPrintln("maxy3PopCallback");
  maxy3.setValue(0);
}
void miny3PopCallback(void *ptr)
{
  dbSerialPrintln("miny3PopCallback");
  miny3.setValue(0);
}
void plusmaxy3PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("plusmaxy3PopCallback");

 maxy3.getValue(&number);

 Serial.print("maxy3.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy3 = number;
 refresh3 = true;
}
void minusmaxy3PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("minusmaxy3PopCallback");

 maxy3.getValue(&number);

 Serial.print("maxy3.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy3 = number;
 refresh3 = true;
}
void plusminy3PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy3PopCallback");

  miny3.getValue(&number);

  Serial.print("miny3.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy3 = number;
  refresh3 = true;
}
void minusminy3PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy3PopCallback");

  miny3.getValue(&number);

  Serial.print("miny3.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy3 = number;
  refresh3 = true;
}
void page3PushCallback(void *ptr)
{
  dbSerialPrintln("page3PushCallback");
  //page3.show();
  currentpage = 3;
}
void maxy4PopCallback(void *ptr)
{
  dbSerialPrintln("maxy4PopCallback");
  maxy4.setValue(0);
}
void miny4PopCallback(void *ptr)
{
  dbSerialPrintln("miny4PopCallback");
  miny4.setValue(0);
}
void plusmaxy4PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("plusmaxy4PopCallback");

 maxy4.getValue(&number);

 Serial.print("maxy4.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy4 = number;
 refresh4 = true;
}
void minusmaxy4PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("minusmaxy4PopCallback");

 maxy4.getValue(&number);

 Serial.print("maxy4.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy4 = number;
 refresh4 = true;
}
void plusminy4PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy4PopCallback");

  miny4.getValue(&number);

  Serial.print("miny4.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy4 = number;
  refresh4 = true;
}
void minusminy4PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy4PopCallback");

  miny4.getValue(&number);

  Serial.print("miny4.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy4 = number;
  refresh4 = true;
}
void page4PushCallback(void *ptr)
{
  dbSerialPrintln("page4PushCallback");
  //page4.show();
  currentpage = 4;
}
void maxy5PopCallback(void *ptr)
{
  dbSerialPrintln("maxy5PopCallback");
  maxy5.setValue(0);
}
void miny5PopCallback(void *ptr)
{
  dbSerialPrintln("miny5PopCallback");
  miny5.setValue(0);
}
void plusmaxy5PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("plusmaxy5PopCallback");

 maxy5.getValue(&number);

 Serial.print("maxy5.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy5 = number;
 refresh5 = true;
}
void minusmaxy5PopCallback(void *ptr)
{
 uint32_t number;
    
 dbSerialPrintln("minusmaxy5PopCallback");

 maxy5.getValue(&number);

 Serial.print("maxy5.val=");
 Serial.print(number);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);

 setmaxy5 = number;
 refresh5 = true;
}
void plusminy5PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("plusminy5PopCallback");

  miny5.getValue(&number);

  Serial.print("miny5.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy5 = number;
  refresh5 = true;
}
void minusminy5PopCallback(void *ptr)
{
  uint32_t number;
    
  dbSerialPrintln("minusminy5PopCallback");

  miny5.getValue(&number);

  Serial.print("miny5.val=");
  Serial.print(number);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  setminy5 = number;
  refresh5 = true;
}
void page5PushCallback(void *ptr)
{
  dbSerialPrintln("page5PushCallback");
  //page5.show();
  currentpage = 5;
}
void page6PopCallback(void *ptr)
{
  dbSerialPrintln("page6PushCallback");
  //page6.show();
  currentpage = 6;
}
void page7PopCallback(void *ptr)
{
  dbSerialPrintln("page7PushCallback");
  currentpage = 7;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
  //sets baud rate between devices
  nexInit();
  if (! rtc.begin()) 
  {
    while(1);
  }
  if (! rtc.initialized()) 
  {
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  
  //Serial.println("card initialized.");
  dbSerialPrintln("setup begin");
  //declares functions that will be used as touch events
  plusmaxy0.attachPop(plusmaxy0PopCallback);
  minusmaxy0.attachPop(minusmaxy0PopCallback);
  maxy0.attachPop(maxy0PopCallback);
  miny0.attachPop(miny0PopCallback);
  page0.attachPush(page0PushCallback);
  plusminy0.attachPop(plusminy0PopCallback);
  minusminy0.attachPop(minusminy0PopCallback);
  maxy1.attachPop(maxy1PopCallback);
  miny1.attachPop(miny1PopCallback);
  plusmaxy1.attachPop(plusmaxy1PopCallback);
  minusmaxy1.attachPop(minusmaxy1PopCallback);
  plusminy1.attachPop(plusminy1PopCallback);
  minusminy1.attachPop(minusminy1PopCallback);
  page1.attachPush(page1PushCallback);
  maxy2.attachPop(maxy2PopCallback);
  miny2.attachPop(miny2PopCallback);
  plusmaxy2.attachPop(plusmaxy2PopCallback);
  minusmaxy2.attachPop(minusmaxy2PopCallback);
  plusminy2.attachPop(plusminy2PopCallback);
  minusminy2.attachPop(minusminy2PopCallback);
  page2.attachPush(page2PushCallback);
  maxy3.attachPop(maxy3PopCallback);
  miny3.attachPop(miny3PopCallback);
  plusmaxy3.attachPop(plusmaxy3PopCallback);
  minusminy3.attachPop(minusminy3PopCallback);
  plusminy3.attachPop(plusminy3PopCallback);
  minusminy3.attachPop(minusminy3PopCallback);
  page3.attachPush(page3PushCallback);
  maxy4.attachPop(maxy4PopCallback);
  miny4.attachPop(miny4PopCallback);
  plusmaxy4.attachPop(plusmaxy4PopCallback);
  minusmaxy4.attachPop(minusmaxy4PopCallback);
  plusminy4.attachPop(plusminy4PopCallback);
  minusminy4.attachPop(minusminy4PopCallback);
  page4.attachPush(page4PushCallback);
  maxy5.attachPop(maxy5PopCallback);
  miny5.attachPop(miny5PopCallback);
  plusmaxy5.attachPop(plusmaxy5PopCallback);
  minusmaxy5.attachPop(minusmaxy5PopCallback);
  plusminy5.attachPop(plusminy5PopCallback);
  minusminy5.attachPop(minusminy5PopCallback);
  page5.attachPush(page5PushCallback);
  dbSerialPrintln("setup done");

  //function that allows the DHT sensor to communicate
  dht.begin();
  
  // used Arduino pins 
  pinMode(vccPin1, OUTPUT); digitalWrite(vccPin1, HIGH);
  pinMode(vccPin2, OUTPUT); digitalWrite(vccPin2, HIGH);
  pinMode(DNTgnd, OUTPUT); digitalWrite(DNTgnd, LOW);
  pinMode(DNTpos, OUTPUT); digitalWrite(DNTpos,HIGH);
  pinMode(DNTout, INPUT); 
  pinMode(gndThermo2, OUTPUT); digitalWrite(gndThermo2, LOW);
  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
 if (currentpage == 6 or currentpage == 7);
 {
  nexLoop(nex_listen_list);
 }
 DateTime now = rtc.now();
 
 if(now.hour() > 12)
 {
  UShours1 = now.hour() - 12;
  UShours0 = UShours1;
  if(now.minute() < 2)
 {
  UShours0 = now.hour() - 1;
  UShours1 = now.hour();
 }
 if(now.second() > 19)
 {
  delaysec = now.second() - 19;
  if(now.minute() > 1)
  {
   delaymin = now.minute() - 2;
  }
  if(now.minute()==0)
  {
   delaymin = 58;
   UShours0 = now.hour() - 1;
  }
  if(now.minute() == 1)
  {
   delaymin = 59;
   UShours0 = now.hour() - 1;
  }
  }
  if(now.second() >= 0 and now.second() <= 19)
  {
   delaysec = now.second() + 41; 
   if(now.minute() > 1)
   {
    delaymin = now.minute() - 2;
   }
   if(now.minute()==0)
   {
    delaymin = 58;
   UShours0 = now.hour() - 1;
   }
   if(now.minute() == 1)
   {
   delaymin = 59;
   UShours0 = now.hour() - 1;
   }
  }
  if(now.second() == 19)
  {
   delaysec = 0;
   delaymin = now.minute() - 2;
   UShours0 = now.hour() - 12;
  }
 }
 
 else
 {
  UShours1 = now.hour();
  UShours0 = UShours1;
  if(now.minute() < 2)
  {
   UShours0 = now.hour() - 1;
   UShours1 = now.hour();
  }
 if(now.second() > 19)
 {
  delaysec = now.second() - 19;
  if(now.minute() > 1)
  {
   delaymin = now.minute() - 2;
  }
  if(now.minute()==0)
  {
   delaymin = 58;
   UShours0 = now.hour() - 1;
  }
  if(now.minute() == 1)
  {
   delaymin = 59;
   UShours0 = now.hour() - 1;
  }
 }
 if(now.second() >= 0 and now.second() <= 18)
 {
  delaysec = now.second() + 41; 
  if(now.minute() > 2)
  {
   delaymin = now.minute() - 3;
   UShours0 = now.hour();
  }
  if(now.minute()==0)
  {
   delaymin = 57;
   UShours0 = now.hour() - 1;
  }
 if(now.minute() == 1)
  {
   delaymin = 58;
   UShours0 = now.hour() - 1;
  }
  if(now.minute() == 2)
  {
   delaymin = 59;
   UShours0 = now.hour() - 1;
  }
 }
 if(now.second() == 19)
 {
  delaysec = 0;
  delaymin = now.minute() - 2;
  UShours0 = now.hour();
 }
 }
 Serial.print("maxx.txt=");
 Serial.print("\"");
 Serial.print(UShours1, DEC);
 Serial.print(':');
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 Serial.print(now.second(), DEC);
 Serial.print("\"");
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.print("minx.txt=");
 Serial.print("\"");
 Serial.print(UShours0, DEC);
 Serial.print(':');
 Serial.print(delaymin, DEC);
 Serial.print(':');
 Serial.print(delaysec, DEC);
 Serial.print("\"");
 Serial.write(0xff);
 Serial.write(0xff);
 Serial.write(0xff);
 
   delay(50);

   
   //sets variables that need to be reset to zero before the loop starts again
   int NTzeros = 0;
   int RTzeros = 0;
   int Hzeros = 0;
   int BTzeros = 0;
   //Sets each of the four variables plotted and needed for display
   BT = thermocouple2.readCelsius();;
   hum = dht.readHumidity();
   RT = dht.readTemperature();
   NT = thermocouple1.readCelsius();
   
   //Updating the four arrays to fit the new data set, data goes in until the array is full, then it moves each data point down one in the array
   //to make room for new data
   //Counts the number of zeros in the array
   
   for(int i = 0; i <= 182; i++)
   {
    if(NTdata[i] == 0)
    {
     NTzeros += 1;
    }
    if(RTdata[i] == 0)
    {
     RTzeros += 1;
    }
    if(Hdata[i] == 0)
    {
     Hzeros += 1;
    }
    if(BTdata[i] == 0)
    {
     BTzeros += 1;
    }
   }
   //if the number of zeros is 0, then slide each datapoint down one and have the last datapoint in the array equal the new one
   if(NTzeros == 0)
   {
     for(int i = 0; i <= 181; i++)
     {
       NTdata[i] = NTdata[i+1];
      }
    NTdata[182] = NT;
   }  
   //if the number of zeros doesnt equal zero, update each value to equal the data
   if(!NTzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     if(NTdata[i] == 0)
     {
      NTdata[i] = NT;
      break;
     }
    }
   }
   //Repeat process for other 3 data values
   if(RTzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     for(i = 0; i <= 181; i++)
     {
      RTdata[i] = RTdata[i+1];
     }
    }
    RTdata[182] = RT;
   }
   if(!RTzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     if(RTdata[i] == 0)
     {
      RTdata[i] = RT;
      break;
     }
    }
   }
   if(BTzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     for(i = 0; i <= 181; i++)
     {
      BTdata[i] = BTdata[i+1];
     }
    }
    BTdata[182] = BT;
   }
   if(!BTzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     if(BTdata[i] == 0)
     {
      BTdata[i] = BT;
      break;
     }
    }
   }
   if(Hzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     for(i = 0; i <= 181; i++)
     {
      Hdata[i] = Hdata[i+1];
     }
    }
    Hdata[182] = hum;
   }
   if(!Hzeros == 0)
   {
    for(int i = 0; i <= 182; i++)
    {
     if(Hdata[i] == 0)
     {
      Hdata[i] = hum;
      break;
     }
    }
   }
   
   //sets scales to coincide with minimum and maximum values for each variable on each graph when maxes or mins are reset 
  
   if(currentpage == 0)
   {
    if(refresh0 == true)
    {
     if(oldmaxH < setmaxy0)
     {
      Serial.print("cle 18,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
      for(int i = 0; i <= 182; i++)
      {
       int variable1 = Hdata[i];
       int variable0 = map(variable1, setminy0, setmaxy0, 0, 210);
       if(Hdata[i+1] == 0)
       {
        break;
       }
       else
       {
        Serial.print("add 18,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("add 18,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);      
       }
      }
      oldmaxH = setmaxy0;
     }
     newhum0 = map(hum, setminy0, setmaxy0, 0, 210);
     if(newhum0 > 210)
     {
      if(newhum0 > 210)
      {
       newhum0 = 211;
      }
      else
      {
        newhum0 = -1;
      }
      Serial.print("graph0.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newhum0 <=210 and newhum0 >= 0)
     {
      Serial.print("graph0.pco0=64495");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    
     Serial.print("maxy00.val=");
     Serial.print(setmaxy0);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     Serial.print("miny00.val=");
     Serial.print(setminy0);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("testH0.val=");
     Serial.print(newhum0);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    }
    if(refresh0 == false)
    {
     if(oldmaxH < cmaxH)
     {
      Serial.print("cle 18,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    
     for(int i = 0; i <= 182; i++)
     {
      int variable1 = Hdata[i];
      int variable0 = map(variable1, cminH, cmaxH, 0, 210);
      if(Hdata[i+1] == 0)
      {
        break;
      }
      else
      {
       Serial.print("add 18,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);

       Serial.print("add 18,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);
      }
     }
     oldmaxH = cmaxH;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(Hdata[i] > cmaxH)
      {
       cmaxH = Hdata[i];
       cmaxH += 10;
       }
      if(Hdata[i] < cminH)
      {
       cminH = Hdata[i];
       cminH -= 10;
      }
     }
     
     newhum0 = map(hum, cminH, cmaxH, 0, 210);

     if(newhum0 > 210 or newhum0 < 0)
     {
      if(newhum0 > 0)
      {
       newhum0 = 211;
      }
      else
      {
       newhum0 = -1;
      }
      Serial.print("graph0.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newhum0<=210 and newhum0>=0)
     {
      Serial.print("graph0.pco0=64495");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     
     Serial.print("maxy00.val=");
     Serial.print(cmaxH);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny00.val=");
     Serial.print(cminH);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("testH0.val=");
     Serial.print(newhum0);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
    }
    
    Serial.print("rh.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(hum);  // This is the value you want to send to that object and atribute mention before.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
   
    Serial.print("RT.val=");
    Serial.print(RT);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
   
    Serial.print("NT.val=");
    Serial.print(NT);
    Serial.write(0xff); //three lines always sent after each command is sent to nextion display
    Serial.write(0xff);
    Serial.write(0xff);
   
    Serial.print("BT.val=");
    Serial.print(BT);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    
   //Prints the humidity value on the waveform of the first page
    //Notation is "add *id*, *channel (0-4)*, variable"
    Serial.print("add 18,0,");
    Serial.print(newhum0);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    
    Serial.print("add 18,0,");
    Serial.print(newhum0);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
   }   
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////
   
   if(currentpage == 1)
   {
    if(refresh1 == true)
    {
     if(oldmax2 < setmaxy1)
     {
      Serial.print("cle 3,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     for(int i = 0; i <= 182; i++)
     {
      int variableNT1 = NTdata[i];
      int variableNT0 = map(variableNT1, setminy1, setmaxy1, 0, 210);
      int variableBT1 = BTdata[i];
      int variableBT0 = map(variableBT1, setminy1, setmaxy1, 0, 210);
      if(NTdata[i+1] == 0 or BTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 3,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 3,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);      

      Serial.print("add 3,1,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 3,1,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);   
      }
     }
     oldmax2 = setmaxy1;
     }
     newNT1 = map(NT,setminy1, setmaxy1, 0, 210);
     newBT1 = map(BT,setminy1, setmaxy1, 0, 210);
     
     Serial.print("maxy11.val=");
     Serial.print(setmaxy1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny11.val=");
     Serial.print(setminy1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     if(newNT1 > 210 or newNT1 < 0)
     {
      if(newNT1>0)
      {
       newNT1 = 211;
      }
      else
      {
        newNT1 = -1;
      }
      Serial.print("graph1.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newNT1<=210 and newNT1 >= 0)
     {
      Serial.print("graph1.pco0=65504");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1 > 210 or newBT1 < 0)
     {
      if(newBT1 > 0)
      {
       newBT1 = 211;
      }
      else
      {
       newBT1 = 0;
      }
      Serial.print("graph1.pco1=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1<=210 and newBT1 >= 0)
     {
      Serial.print("graph1.pco1=2047");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
    if(refresh1 == false)
    {
     if(oldmax2 < cmax2)
     {
      Serial.print("cle 3,255");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    for(int i = 0; i <= 182; i++)
     {
      int variableNT1 = NTdata[i];
      int variableNT0 = map(variableNT1, cmin2, cmax2, 0, 210);
      int variableBT1 = BTdata[i];
      int variableBT0 = map(variableBT1, cmin2, cmax2, 0, 210);
      if(NTdata[i+1] == 0 and BTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 3,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 3,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);      

      Serial.print("add 3,1,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 3,1,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);   
      }
     }
     oldmax2 = cmax2;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(BTdata[i] > cmax2 or NTdata[i] > cmax2)
      {
       if(BTdata[i] > NTdata[i])
       {
        cmax2 = BTdata[i];
        cmax2 += 10;
       }
       else
       {
        cmax2 = NTdata[i];
        cmax2 += 10;
       }
      if(BTdata[i] < NTdata[i])
      {
       cmin2 = BTdata[i];
       cmin2 -= 10;
      }
      else
      {
        cmin2 = NTdata[i];
        cmin2 -= 10;
      }
     }
     }
     Serial.print("maxy11.val=");
     Serial.print(cmax2);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny11.val=");
     Serial.print(cmin2);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     newNT1 = map(NT,cmin2, cmax2, 0, 210);
     newBT1 = map(BT,cmin2, cmax2, 0, 210);

     if(newNT1 > 210 or newNT1 < 0)
     {
      if(newNT1 > 0)
      {
       newNT1 = 211;
      }
      else
      {
       newNT1 = -1;
      }
      Serial.print("graph1.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1 > 210 or newBT1 < 0)
     {
      if(newBT1 > 210)
      {
       newBT1 = 211;
      }
      else
      {
        newBT1 = -1;
      }
      Serial.print("graph1.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newNT1 <= 210 or newNT1 >= 0)
     {
      Serial.print("graph1.pco0=65504");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1 <= 210 or newBT1 >= 0)
     {
      Serial.print("graph1.pco1=2047");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
   //Prints the second and third temperatures on the waveform on the second page

     Serial.print("add 3,0,");
     Serial.print(newNT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    
     Serial.print("add 3,0,");
     Serial.print(newNT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("add 3,1,");
     Serial.print(newBT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    
     Serial.print("add 3,1,");
     Serial.print(newBT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
}

/////////////////////////////////////////////////////////////////////////////////////////////

   if(currentpage == 2)
   {
    if(refresh2 == true)
    {
     if(oldmaxH < setmaxy2)
     {
      Serial.print("cle 8,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
      for(int i = 0; i <= 182; i++)
      {
       int variable1 = Hdata[i];
       int variable0 = map(variable1, setminy2, setmaxy2, 0, 180);
       if(Hdata[i+1] == 0)
       {
        break;
       }
       else
       {
        Serial.print("add 8,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("add 8,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);      
       }
      }
      oldmaxH = setmaxy2;
     }
     newhum0 = map(hum, setminy2, setmaxy2, 0, 180);
     if(newhum0 > 180 or newhum0 < 0)
     {
      if(newhum0 > 180)
      {
       newhum0 = 181;
      }
      else
      {
        newhum0 = -1;
      }
      Serial.print("graph2.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newhum0 <=180 and newhum0 >= 0)
     {
      Serial.print("graph2.pco0=64495");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    
     Serial.print("maxy22.val=");
     Serial.print(setmaxy2);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     Serial.print("miny22.val=");
     Serial.print(setminy2);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

    }
    if(refresh2 == false)
    {
     if(oldmaxH < cmaxH)
     {
      Serial.print("cle 8,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    
     for(int i = 0; i <= 182; i++)
     {
      int variable1 = Hdata[i];
      int variable0 = map(variable1, cminH, cmaxH, 0, 180);
      if(Hdata[i+1] == 0)
      {
        break;
      }
      else
      {
       Serial.print("add 8,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);

       Serial.print("add 8,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);
      }
     }
     oldmaxH = cmaxH;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(Hdata[i] > cmaxH)
      {
       cmaxH = Hdata[i];
       cmaxH += 10;
       }
      if(Hdata[i] < cminH)
      {
       cminH = Hdata[i];
       cminH -= 10;
      }
     }
     
     newhum0 = map(hum, cminH, cmaxH, 0, 180);

     if(newhum0 > 180 or newhum0 < 0)
     {
      if(newhum0 > 180)
      {
        newhum0 = 181;
      }
      else
      {
        newhum0 = -1;
      }
      Serial.print("graph2.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newhum0<=180 and newhum0>=0)
     {
      Serial.print("graph2.pco0=64495");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     
     Serial.print("maxy22.val=");
     Serial.print(cmaxH);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny22.val=");
     Serial.print(cminH);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    }
    
    Serial.print("rh.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(hum);  // This is the value you want to send to that object and atribute mention before.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    
   //Prints the humidity value on the waveform of the first page
    //Notation is "add *id*, *channel (0-4)*, variable"
    Serial.print("add 8,0,");
    Serial.print(newhum0);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    
    Serial.print("add 8,0,");
    Serial.print(newhum0);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
   }   
   

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
   if(currentpage == 3)
   {
   if(refresh3 == true)
    {
     if(oldmaxRT < setmaxy3)
     {
      Serial.print("cle 8,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
      for(int i = 0; i <= 182; i++)
      {
       int variable1 = RTdata[i];
       int variable0 = map(variable1, setminy3, setmaxy3, 0, 180);
       if(RTdata[i+1] == 0)
       {
        break;
       }
       else
       {
        Serial.print("add 8,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("add 8,0,");
        Serial.print(variable0);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);      
       }
      }
      oldmaxRT = setmaxy3;
     }
     newRT3 = map(RT, setminy3, setmaxy3, 0, 180);
     if(newRT3 > 180 or newRT3 < 0)
     {
      if(newRT3 > 180)
      {
       newRT3 = 181;
      }
      else
      {
        newRT3 = -1;
      }
      Serial.print("graph3.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newRT3 <=180 and newRT3 >= 0)
     {
      Serial.print("graph3.pco0=64495");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    
     Serial.print("maxy33.val=");
     Serial.print(setmaxy3);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     Serial.print("miny33.val=");
     Serial.print(setminy3);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

    }
   if(refresh3 == false)
    {
     if(oldmaxRT < cmaxRT)
     {
      Serial.print("cle 8,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    
     for(int i = 0; i <= 182; i++)
     {
      int variable1 = RTdata[i];
      int variable0 = map(variable1, cminRT, cmaxRT, 0, 180);
      if(RTdata[i+1] == 0)
      {
        break;
      }
      else
      {
        counter += 1;
        if(counter >= 182)
        {
          break;
        }
       Serial.print("add 8,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);

       Serial.print("add 8,0,");
       Serial.print(variable0);
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);
      }
     }
     oldmaxRT = cmaxRT;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(RTdata[i] > cmaxRT)
      {
       cmaxRT = RTdata[i];
       cmaxRT += 10;
       }
      if(RTdata[i] < cminRT)
      {
       cminRT = RTdata[i];
       cminRT -= 10;
      }
     }
     
     newRT3 = map(RT, cminRT, cmaxRT, 0, 180);

     if(newRT3 > 180 or newRT3 < 0)
     {
      if(newRT3 > 180)
      {
       newRT3 = 181;
      }
      else
      {
        newRT3 = -1;
      }
      Serial.print("graph3.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newRT3<=180 and newRT3>=0)
     {
      Serial.print("graph3.pco0=48631");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     
     Serial.print("maxy33.val=");
     Serial.print(cmaxRT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny33.val=");
     Serial.print(cminRT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    }
    
    Serial.print("RT.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(RT);  // This is the value you want to send to that object and atribute mention before.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    
    //Notation is "add *id*, *channel (0-4)*, variable"
    Serial.print("add 8,0,");
    Serial.print(newRT3);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    
    Serial.print("add 8,0,");
    Serial.print(newRT3);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if(currentpage == 4)
   {
    if(refresh4 == true)
    {
     if(oldmaxNT < setmaxy4)
     {
      Serial.print("cle 6,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     for(int i = 0; i <= 182; i++)
     {
      int variableNT1 = NTdata[i];
      int variableNT0 = map(variableNT1, setminy4, setmaxy4, 0, 180);
      if(NTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 6,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 6,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);         
      }
     }
     oldmaxNT = setmaxy4;
     }
     newNT1 = map(NT,setminy4, setmaxy4, 0, 180);
     
     Serial.print("maxy44.val=");
     Serial.print(setmaxy4);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny44.val=");
     Serial.print(setminy4);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     if(newNT1 > 180 or newNT1 < 0)
     {
      if(newNT1>0)
      {
       newNT1 = 181;
      }
      else
      {
        newNT1 = -1;
      }
      Serial.print("graph4.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newNT1<=180 and newNT1 >= 0)
     {
      Serial.print("graph4.pco0=65504");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
    if(refresh4 == false)
    {
     if(oldmaxNT < cmaxNT)
     {
      Serial.print("cle 6,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    for(int i = 0; i <= 182; i++)
     {
      int variableNT1 = NTdata[i];
      int variableNT0 = map(variableNT1, cminNT, cmaxNT, 0, 180);
      if(NTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 6,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 6,0,");
      Serial.print(variableNT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);      
 
      }
     }     
     oldmaxNT = cmaxNT;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(NTdata[i] > cmaxNT)
      {
       cmaxNT = NTdata[i];
       cmaxNT += 10;
      }
      if(NTdata[i] < cminNT)
      {
        cminNT = NTdata[i];
        cminNT -= 10;
      }
     }
     Serial.print("maxy44.val=");
     Serial.print(cmaxNT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny44.val=");
     Serial.print(cminNT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     newNT1 = map(NT,cminNT, cmaxNT, 0, 180);

     if(newNT1 > 180 or newNT1 < 0)
     {
      if(newNT1 > 180)
      {
       newNT1 = 181;
      }
      else
      {
        newNT1 = -1;
      }
      Serial.print("graph4.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newNT1 >= 0 and newNT1 <= 180)
     {
      Serial.print("graph4.pco0=65504");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
   //Prints the second and third temperatures on the waveform on the second page
     Serial.print("NT.val=");
     Serial.print(NT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
      
     Serial.print("add 6,0,");
     Serial.print(newNT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    
     Serial.print("add 6,0,");
     Serial.print(newNT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
}
   
   
  /////////////////////////////////////////////////////////////////////////////////////////
  
  if(currentpage == 5)
   {
    if(refresh5 == true)
    {
     if(oldmaxBT < setmaxy5)
     {
      Serial.print("cle 7,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     for(int i = 0; i <= 182; i++)
     {
      int variableBT1 = BTdata[i];
      int variableBT0 = map(variableBT1, setminy5, setmaxy5, 0, 180);
      if(BTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 7,0,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 7,0,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);         
      }
     }
     oldmaxBT = setmaxy5;
     }
     newBT1 = map(BT,setminy5, setmaxy5, 0, 180);
     
     Serial.print("maxy55.val=");
     Serial.print(setmaxy5);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny55.val=");
     Serial.print(setminy5);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     if(newBT1 > 180 or newBT1 < 0)
     {
      if(newBT1>0)
      {
       newBT1 = 181;
      }
      else
      {
        newBT1 = -1;
      }
      Serial.print("graph5.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1<=180 and newBT1 >= 0)
     {
      Serial.print("graph5.pco0=65504");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
    if(refresh5 == false)
    {
     if(oldmaxBT < cmaxBT)
     {
      Serial.print("cle 7,0");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    for(int i = 0; i <= 182; i++)
     {
      int variableBT1 = BTdata[i];
      int variableBT0 = map(variableBT1, cminBT, cmaxBT, 0, 180);
      if(BTdata[i+1] == 0)
      {
        break;
      }
      else
      {
      Serial.print("add 7,0,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("add 7,0,");
      Serial.print(variableBT0);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);      
 
      }
     }     
     oldmaxBT = cmaxBT;
     }
     for(int i = 0; i <= 182; i++)
     {
      if(BTdata[i] > cmaxBT)
      {
       cmaxBT = BTdata[i];
       cmaxBT += 10;
      }
      if(BTdata[i] < cminBT)
      {
        cminBT = BTdata[i];
        cminBT -= 10;
      }
     }
     Serial.print("maxy55.val=");
     Serial.print(cmaxBT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);

     Serial.print("miny55.val=");
     Serial.print(cminBT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
     
     newBT1 = map(BT,cminBT, cmaxBT, 0, 180);

     if(newBT1 > 180 or newBT1 < 0)
     {
      if(newBT1 > 180)
      {
       newBT1 = 181;
      }
      else
      {
        newBT1 = -1;
      }
      Serial.print("graph5.pco0=63488");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
     if(newBT1 >= 0  and newBT1 <= 180)
     {
      Serial.print("graph5.pco0=2047");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
     }
    }
   //Prints the second and third temperatures on the waveform on the second page
     Serial.print("BT.val=");
     Serial.print(BT);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
      
     Serial.print("add 7,0,");
     Serial.print(newBT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
    
     Serial.print("add 7,0,");
     Serial.print(newBT1);
     Serial.write(0xff);
     Serial.write(0xff);
     Serial.write(0xff);
}   
    delay(50);
    
    String data = "";
     data += String(now.month());
     data += "/";
     data += String(now.day());
     data += "/";
     data += String(now.year()); 
     data += "\t";
     data += String(UShours1);
     data += ":";
     data += String(now.minute());
     data += ":";
     data += String(now.second());
     data += "\t";
     data += "\t";
     data += String(hum);
     data += "\t";
     data += "\t";
     data += String(RT);
     data += "\t";
     data += "\t";
     data += String(NT);
     data += "\t";
     data += "\t";
     data += String(BT);
    
    File dataFile = SD.open("TempData.txt", FILE_WRITE);

    if(SD.exists("TempData.txt"))
    {
      dataFile.println(data);
      dataFile.close();
    }
    
}
