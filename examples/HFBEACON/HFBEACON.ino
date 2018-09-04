/*
 * F4GOH Anthony JN07DV
 * F4GOJ Christophe IN97IJ
 * Arduino HF Beacon PSK31 PSK63 PSK125 RTTY HELL WSPR with RSID for AD9850
 * AD9850 datasheet at http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf
 * Use freely
 * hellschreiber from http://brainwagon.org/2012/01/11/hellduino-sending-hellschreiber-from-an-arduino/
 */

#include <AD9850SPI.h>
#include <HFBEACON.h>
#include <SPI.h>
#include <avr/pgmspace.h>

char txString[] = "  f4goj f4goj beacon test 0123456789 f4goj f4goj";
char txCall[] = "f4goj";
char txLoc[] = "in97";
char txPow[] = "00";

long freq=10140600;

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup(){
 Serial.begin(57600);
 Serial.print(F("Free RAM : "));
 Serial.println(freeRam());
 DDS.begin(13,8,9);
 DDS.calibrate(124998250);
}

void loop(){
   
   Beacon.cwTx(freq, txString, 30);
   delay(2000);
   
   //Beacon.pskTx(freq, txString, 'B', 31);
   //delay(2000);
   
   //Beacon.rttyTx(freq, txString);
   //delay(2000);
   
   //Beacon.hellTx(freq, txString);
   //delay(2000);
   
   //Beacon.wsprTx(freq, txCall, txLoc, txPow);
   //delay(10000);
   
   Serial.print(F("Free RAM : "));
   Serial.println(freeRam());
}
