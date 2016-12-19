
#include "DHT.h"
#include <TimerOne.h>
#include <EEPROM.h>
//////////////   RADIO /////////////////////////
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define DEBUG
#ifdef DEBUG
#include <Logging.h>
#endif

//#define POINT_ID_PROG 3

struct Radio_a {
  int point_id;
  int id_commands;
  int send_time;
  int send_try;
  int master_id;
  int last_id_command;
};
Radio_a radio_values;
#define PIN_NUMBERS 21
#define PIN_VALUES_MODE 0
#define PIN_VALUES_NOTIFY_CHANGE 1
#define PIN_VALUES_STATE 2

int pin_values[PIN_NUMBERS][3];


void(* resetFunc) (void) = 0; // объявляем функцию reset
//resetFunc(); //вызываем reset



#define RADIO_MESSAGE_FOR_ALL 999

#define RADIO_COMMAND_RESPONSE_OK 1
#define RADIO_COMMAND_PINMODE 2
#define RADIO_COMMAND_DIGITAL_WRITE 3
#define RADIO_COMMAND_ANALOG_WRITE 4
#define RADIO_COMMAND_PIN_NOTIFY_CHANGE 5
#define RADIO_COMMAND_PIN_SET_SETTING 6
#define RADIO_COMMAND_PIN_RESET_SETTING 7
#define RADIO_COMMAND_DIGITALREAD 8
#define RADIO_COMMAND_ANALOGREAD 9
#define RADIO_COMMAND_DHT_TEMP_GET 10
#define RADIO_COMMAND_DHT_TEMP_RESP 11
#define RADIO_COMMAND_DHT_HUMI_GET 12
#define RADIO_COMMAND_DHT_HUMI_RESP 13
#define RADIO_COMMAND_DHT_ADD 14

#define RADIO_COMMAND_DIGITALREAD_RESP 108
#define RADIO_COMMAND_ANALOGREAD_RESP 109


#define RADIO_MESSAGE_ID_SENDER 0
#define RADIO_MESSAGE_ID_RECIVER 1
#define RADIO_MESSAGE_ID_COMMAND 2
#define RADIO_MESSAGE_COMMAND 3
#define RADIO_MESSAGE_ARG_1 4
#define RADIO_MESSAGE_ARG_2 5

#define RADIO_MESSAGE_LEN 6

#define RADIO_FLAGS_SEND_NEED 0
#define RADIO_FLAGS_SEND_SUCC 1

#define DHT_NUMBERS 3
DHT * dht[DHT_NUMBERS];
int DHT_sensors[DHT_NUMBERS];

RF24 radio(9,10); // Определяем рабочие ножки RF24;
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL,0xE8E8F0F0E1LL };
int radio_output_message[RADIO_MESSAGE_LEN];
int radio_input_message[RADIO_MESSAGE_LEN];
int radio_response_message[RADIO_MESSAGE_LEN];

int radio_flags[2] = {0,0};
long counter = 0;

int all_try=0;
////////////////////////////////





#define LED 3

int rec_send_pack = 0;
int send_pack = 0;
int succ_pack = 0;
int fail_pack = 0;


void setup() {
Serial.begin(9600);
pinMode (LED,OUTPUT);
setup_radio();
#ifdef DEBUG
Log.Init(LOGLEVEL, 9600L);
#endif
setupTimer(1000);
EEPROM_readSettings();
}

void loop() {
  radio_work();
  react_to_change();
//  if (point_ID == 1){
//      counter++;
//      if (counter == 10000){
//        if (radio_values.id_commands < 200) radio_send_message(1,3,4,5);
//        counter = 0;
//      }
//  }
}
