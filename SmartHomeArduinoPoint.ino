#include <MsTimer2.h>
#include <TimerOne.h>
#include <EEPROM.h>
//////////////   RADIO /////////////////////////
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define ROLE 0 //sender
//#define ROLE 1 //reciver
#define RADIO_MESSAGE_FOR_ALL 999

#define RADIO_COMMAND_RESPONSE_OK 1


#define RADIO_MESSAGE_ID_SENDER 0
#define RADIO_MESSAGE_ID_RECIVER 1
#define RADIO_MESSAGE_ID_COMMAND 2
#define RADIO_MESSAGE_COMMAND 3
#define RADIO_MESSAGE_ARG_1 4
#define RADIO_MESSAGE_ARG_2 5
#define RADIO_MESSAGE_LEN 6
RF24 radio(9,10); // Определяем рабочие ножки RF24;
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL,0xE8E8F0F0E1LL };
int radio_output_message[RADIO_MESSAGE_LEN];
int radio_input_message[RADIO_MESSAGE_LEN];

int radio_send_need = 0;
int radio_ID_command = 0;
////////////////////////////////
#define DEBUG

#ifdef DEBUG
  #include <Logging.h>
#endif


#define LED 3
byte point_ID;

int rec_send_pack = 0;
int send_pack = 0;
int succ_pack = 0;
int fail_pack = 0;

void setup() {
pinMode (LED,OUTPUT);
setup_radio();
Log.Init(LOGLEVEL, 9600L);
setupTimer(100000);
//EEPROM_readSettings();
}

void loop() {
  radio_work();
}
