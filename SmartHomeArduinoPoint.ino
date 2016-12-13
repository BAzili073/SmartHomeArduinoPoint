#include <TimerOne.h>
#include <EEPROMex.h>
//////////////   RADIO /////////////////////////
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10); // Определяем рабочие ножки RF24;
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL,0xE8E8F0F0E1LL };
int output_message[4] = {0,0,0,0};
int input_message[4] = {0,0,0,0};
////////////////////////////////
#define DEBUG

#ifdef DEBUG
  #include <Logging.h>
#endif


#define LED 3
byte point_ID;

void setup() {
pinMode (LED,OUTPUT);
setup_radio();
Log.Init(LOGLEVEL, 9600L);
//setupTimer(1000);
//EEPROM_readSettings();
}

void loop() {
radio_rec();
}
