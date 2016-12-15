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
#define RADIO_COMMAND_PINMODE 2
#define RADIO_COMMAND_DIGITAL_WRITE 3
#define RADIO_COMMAND_ANALOG_WRITE 4

#define RADIO_MESSAGE_ID_SENDER 0
#define RADIO_MESSAGE_ID_RECIVER 1
#define RADIO_MESSAGE_ID_COMMAND 2
#define RADIO_MESSAGE_COMMAND 3
#define RADIO_MESSAGE_ARG_1 4
#define RADIO_MESSAGE_ARG_2 5
#define RADIO_MESSAGE_LEN 6

#define RADIO_FLAGS_SEND_NEED 0
#define RADIO_FLAGS_SEND_SUCC 1

#define RADIO_VALUES_ID_COMMANDS 0
#define RADIO_VALUES_SEND_TIME 1
#define RADIO_VALUES_SEND_TRY 2

RF24 radio(9,10); // Определяем рабочие ножки RF24;
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL,0xE8E8F0F0E1LL };
int radio_output_message[RADIO_MESSAGE_LEN];
int radio_input_message[RADIO_MESSAGE_LEN];
int radio_response_message[RADIO_MESSAGE_LEN];

int radio_flags[2] = {0,0};
int radio_values[3]; 
long counter = 0;
int try_max = 0;
int all_try = 0;
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
setupTimer(1000);
EEPROM_readSettings();
}

void loop() {
  radio_work();
//  if (point_ID == 1){
      counter++;
      if (counter == 10000){
        if (radio_values[RADIO_VALUES_ID_COMMANDS] < 200) radio_send_message(1,3,4,5);
        counter = 0;
      }
//  }
}
