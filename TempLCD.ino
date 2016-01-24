/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <TinyDHT.h>
#include <avr/power.h> 


#define DHTTYPE DHT22
#define TEMPTYPE 0


// Trinket GPIO #1 would be better but LED prevents digital talk with DHT sensor
#define DHTPIN 9         // Sensor connected to GPIO #1 (use a
                          //  1K ohm pullup to 5V to make it work!)
DHT dht(DHTPIN, DHTTYPE); // Define Temp Sensor


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 6, 5, 4, 3);
byte degree[8] = { 
  0b01110,
  0b11111,
  0b11011,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte up[8] = { 
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000
};
byte down[8] = { 
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};
byte eq[8] = { 
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte status1[8] = { 
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte status2[8] = { 
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b11111
};
byte status3[8] = { 
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111
};
byte status4[8] = { 
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111
};
byte blank[8] = { 
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

  int16_t lasttemp = 0;
  int8_t lasthum =0;
  int statuscnt = 4;
  
void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1); // 5V Trinket: run at 16 MHz
  dht.begin();  // Initialize DHT Teperature Sensor
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  lcd.createChar(0, degree);
  lcd.createChar(1, up);
  lcd.createChar(2, down);
  lcd.createChar(3, eq);
  //lcd.createChar(4, blank);
  lcd.createChar(4, status1);
  lcd.createChar(5, status2);
  lcd.createChar(6, status3);
  lcd.createChar(7, status4);
  
}

void loop() {
  int8_t h = dht.readHumidity();
  int16_t t = dht.readTemperature(TEMPTYPE);
  
  for (int i=0; i<600; i++) {
    int8_t h = dht.readHumidity();
    int16_t t = dht.readTemperature(TEMPTYPE);
   
    
    lcd.setCursor(0, 0);
    if ( t == BAD_TEMP || h == BAD_HUM ) { // if error conditions (see TinyDHT.h)
       lcd.print("Bad read on DHT");       //   print error message
    } else {
       lcd.print("Humidity: ");            // write to LCD
       lcd.setCursor(11,0); 
       lcd.print(h);
       lcd.setCursor(13, 0); 
       lcd.print(" % ");
       if ( h < lasthum) {
          lcd.setCursor(15, 0);
          lcd.print((char)2);
          
        } if ( h > lasthum) {
          lcd.setCursor(15, 0);
          lcd.print((char)1);
          
        } if ( h == lasthum) {
          lcd.setCursor(15, 0);
          lcd.print((char)3);
          
        } 
       lcd.setCursor(0, 1); 
       lcd.print("Temp:"); 
       lcd.setCursor(7, 1); 
       lcd.print(t);
       lcd.setCursor(10, 1); 
       lcd.print((char)0);
       lcd.print("C");
  
        if ( t < lasttemp) {
          lcd.setCursor(12, 1);
          lcd.print((char)2);
          lcd.print(" ");
          //lcd.print(lasttemp);
        } if ( t > lasttemp) {
          lcd.setCursor(12, 1);
          lcd.print((char)1);
          lcd.print(" ");
          //lcd.print(lasttemp);
        } if ( t == lasttemp) {
          lcd.setCursor(12, 1);
          lcd.print((char)3);
          lcd.print(" ");
          //lcd.print(lasttemp);
        } 
        lcd.setCursor(15,1);
        lcd.print((char)statuscnt);
        //lcd.print(lasttemp);
        /*else {
          lcd.setCursor(15, 1);
          lcd.print((char)3);
        }*/
        
        //lcd.setCursor(14,1);
        //lcd.print(lasttemp);
    }
    delay(200);
    lasttemp = (lasttemp + t) / 2;
    lasthum = (lasthum + h) / 2;
    statuscnt++;
    if (statuscnt > 7) {
      statuscnt = 4;
    }
  }
    lasttemp = t;
    lasthum = h;
  
  delay(2000);  // Read temp every second (2000 ms) (DHT sensor max rate)
}

