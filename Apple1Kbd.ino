//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

#include <SPI.h>
#include <SD.h>

// Code should work with Arduino Mega, Adafruit SD library, and Data Logger Shield that doesn't have SPI bus header.

//For Arduino Mega, follow these instructions: https://learn.adafruit.com/adafruit-data-logger-shield/for-the-mega-and-leonardo
//(reverted to original SD libaray)
const int SSPIN = 53;
const int spi1 = 10;  // pins needed for SD.begin()
const int spi2 = 11;
const int spi3 = 12;
const int spi4 = 13;
int SDconnected = 0; //will get set to 1 if successful connection

const int maxfblob = 1000;
const int maxfiles = 100;
char fnameblob[maxfblob];
int foffset[maxfiles];
int currfblob = 0;
int curroffset = 1;
int numfiles = 0;
int currfilenum = 0;
char currfnam[13];
 
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//Pin numbers on Arduino Mega connected to Replica 1+ keyboard socket
#define strobe  23
#define d5      25
#define d4      27
#define d6      29
#define d2      31
#define d3      33
#define d0      35
#define d1      37

byte asciinum = 0;
char asciival = 0;
byte maxascii= 127;
byte minascii= 32;
 
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close

 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}



void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print(".");   // send an initial string
    delay(300);
  }
  //Serial.readStringUntil('\n');
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 // Serial.println("Send one line of text to begin");
 // establishContact();  // send a byte to establish contact until receiver responds

 // Serial.println("Connecting to SD card");

  pinMode(SSPIN, OUTPUT); // required for MEGA
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);  //LED will be lit if any error occurs
    
  if (!SD.begin(spi1, spi2, spi3, spi4)) {
    Serial.println("initialization failed!  Check wiring, if card inserted, and pin to use for your SD shield");
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);
    // don't do anything more:
    while (1) ;
  }
  SDconnected = 1;
  //Serial.println("initialization done");


 pinMode (d0,OUTPUT);
 pinMode (d1,OUTPUT);
 pinMode (d2,OUTPUT);
 pinMode (d3,OUTPUT);
 pinMode (d4,OUTPUT);
 pinMode (d5,OUTPUT);
 pinMode (d6,OUTPUT);
 pinMode (strobe,OUTPUT);
 digitalWrite (strobe,LOW);
 digitalWrite (d0,LOW);
 digitalWrite (d1,LOW);
 digitalWrite (d2,LOW);
 digitalWrite (d3,LOW);
 digitalWrite (d4,LOW);
 digitalWrite (d5,LOW);
 digitalWrite (d6,LOW);
 
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Select file:"); // print a simple message
 asciinum = 48;
 asciival = (char)asciinum;

  foffset[0] = 0;
  File fDir = SD.open("/apple1/",FILE_READ);

  //Serial.println("Files in apple1 directory of SD card");
  while (true) {

    File entry =  fDir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    String sEntry = entry.name();

    int entryLen = sEntry.length();

    char * ename = entry.name();
    if (!(ename[0] == '_'))
    {
      
      //lcd.setCursor(0,1);
      //lcd.println("                ");
      //lcd.setCursor(0,1);
      //lcd.println(entry.name());

      //TODO: ADD TO ARRAY!

      for (int i=0; i< entryLen; i++)
      {
        fnameblob[currfblob] = ename[i];
        currfblob++;
        if (currfblob == maxfblob)
        {
          currfblob--;
        }
      }
      foffset[numfiles] = currfblob;
      numfiles++;
    }
    entry.close();

//    Serial.print("numfiles=");
//    Serial.println(numfiles);
    
//    Serial.print("foffset=");
//    Serial.println(currfblob);
  }
  fDir.close();
  
  lcd.setCursor(0,1);

  int fstart = foffset[currfilenum];
  int fend = foffset[currfilenum+1];
  
  int chptr = 0;
  for (int k=0; k<12; k++)
  {
    currfnam[k] = ' ';
  }
  for (int j=fstart; j<fend; j++)
  {
    lcd.print(fnameblob[j]);
    Serial.print(fnameblob[j]);
    currfnam[chptr] = fnameblob[j];
    chptr++;
  }
  Serial.println();
  lcd.print("        ");
  
  //lcd.println("DISASS~1.TXT    ");
}

void loop() {
  // put your main code here, to run repeatedly:

 //lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 //lcd.print(millis()/1000);      // display seconds elapsed since power-up

 
 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons
 
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
       SendAscii();
       break;
     }
   case btnLEFT:
     {
     //lcd.setCursor(0,1);
     //lcd.print("_");
     asciinum = 0x5F;
     SendAscii();
     asciinum = 48;
     break;
     }
   case btnUP:
     {
      //lcd.setCursor(0,1);
      asciinum++;
      if (asciinum > maxascii)
      {
        asciinum = minascii;
      }
      asciival = (char)asciinum;
      //lcd.print(asciival);
      //Serial.println(asciival);

      currfilenum--;
      if (currfilenum < 0 )
      {
        currfilenum = 0;
      }
      
      lcd.setCursor(0,1);

      int fstart = foffset[currfilenum];
      int fend = foffset[currfilenum+1];
      int chptr = 0;
      for (int k=0; k<12; k++)
      {
        currfnam[k] = ' ';
      }
      for (int j=fstart; j<fend; j++)
      {
        lcd.print(fnameblob[j]);
        currfnam[chptr] = fnameblob[j];
        chptr++;
      }
      currfnam[chptr] = '\0';
      lcd.print("        ");
      Serial.println(currfnam);
      
     break;
     }
   case btnDOWN:
     {
      //lcd.setCursor(0,1);
      asciinum--;
      if (asciinum <= minascii)
      {
        asciinum = maxascii;
      }
      asciival = (char)asciinum;

      //lcd.print(asciival);
      //Serial.println(asciival);

      currfilenum++;
      if (currfilenum >= (numfiles-1))
      {
        currfilenum--;
      }
      
      lcd.setCursor(0,1);

      int fstart = foffset[currfilenum];
      int fend = foffset[currfilenum+1];
      int chptr = 0;
      for (int k=0; k<12; k++)
      {
        currfnam[k] = ' ';
      }
      for (int j=fstart; j<fend; j++)
      {
        lcd.print(fnameblob[j]);
        currfnam[chptr] = fnameblob[j];
        chptr++;
      }
      currfnam[chptr] = '\0';

      lcd.print("        ");
      Serial.println(currfnam);
      
     break;
     }
   case btnSELECT:
     {
      String fileLine;
      char * fpath = "/apple1/";
      char * ffullpath = strcat(fpath, currfnam);
      File fFile = SD.open(ffullpath, FILE_READ);

      if (fFile) {
        while (fFile.available()) {
          fileLine = fFile.readStringUntil('\n');
          //Serial.println(fileLine);
          for (int i=0; i < fileLine.length() ; i++)
          {
             asciinum = (int)fileLine[i];
             SendAscii();
          }
          asciinum = 0x0D;
          SendAscii();
          Serial.println();
          long tgttime = millis() + 250;  //delay between lines
          while (millis() < tgttime)
          {
          }
        }
      }
      else {
        Serial.println("ERROR opening DISASS~1.TXT");
      }
      fFile.close();

        
      //Test: E000R
      //asciinum = 0x45;
      //SendAscii();
      //asciinum = 0x30;
      //SendAscii();
      //SendAscii();
      //SendAscii();
      //asciinum = 0x52;
      //SendAscii();   
               
      asciinum = 48;
      break;
     }
     case btnNONE:
     {
     break;
     }
 }
 
 while (lcd_key == read_LCD_buttons())
 {
 } 
}

void SendAscii()
{
       //lcd.setCursor(0,1);
       asciival = (char)asciinum;
       //lcd.print(asciival);

       //lcd.setCursor(3,1);
       int testval = asciival;
       
       int bit8 = (testval >= 128);
       //lcd.print(bit8);
       testval = testval - 128 * bit8;
       
       int bit7 = (testval >= 64);
       testval = testval - 64 * bit7;
       //lcd.print(bit7);
       digitalWrite(d6, bit7);

       int bit6 = (testval >= 32);
       testval = testval - 32 * bit6;
       //lcd.print(bit6);
       digitalWrite(d5, bit6);
       
       int bit5 = (testval >= 16);
       testval = testval - 16 * bit5;
       //lcd.print(bit5);
       digitalWrite(d4, bit5);

       int bit4 = (testval >= 8);
       testval = testval - 8 * bit4;
       //lcd.print(bit4);
       digitalWrite(d3, bit4);

       int bit3 = (testval >= 4);
       testval = testval - 4 * bit3;
       //lcd.print(bit3);
       digitalWrite(d2, bit3);

       int bit2 = (testval >= 2);
       testval = testval - 2 * bit2;
       //lcd.print(bit2);
       digitalWrite(d1, bit2);

       int bit1 = (testval >= 1);
       //lcd.print(bit1);
       digitalWrite(d0, bit1);

       Serial.print(asciival);

       digitalWrite(strobe,HIGH);
       long tgttime = micros() + 10;
       while (micros() < tgttime)
       {
       }
       digitalWrite(strobe,LOW);
       
       tgttime = millis() + 13; // delay between characters
       while (millis() < tgttime)
       {
       }
      
}

