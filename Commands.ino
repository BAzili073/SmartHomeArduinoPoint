//INPUT:0 
//OUTPUT:1 
//Pins:
//2 - 13
//A0 - A7 : 14 - 21

void react_to_change(){
  int i;
  for (i = 0; i < PIN_NUMBERS; i++){
    if ((pin_values[i][PIN_VALUES_MODE] == INPUT) && (pin_values[i][PIN_VALUES_NOTIFY_CHANGE])){
      int a = digitalRead(i);
      Log.Info ("a[%d] = %d"CR,i,a);
        if ( a != pin_values[i][PIN_VALUES_STATE]){
            pin_values[i][PIN_VALUES_STATE] = a;
            radio_send_message(radio_values.master_id,RADIO_COMMAND_PIN_NOTIFY_CHANGE,i,a);
        }
    }
  }
}

void analysis_Message(){
    radio_values.last_id_command = radio_input_message[RADIO_MESSAGE_ID_COMMAND]; 
    
    switch (radio_input_message[RADIO_MESSAGE_COMMAND]){

      
    case RADIO_COMMAND_RESPONSE_OK:                   //ответ о принятой команде!
        if (radio_values.id_commands == radio_input_message[RADIO_MESSAGE_ID_COMMAND]){
          radio_flags[RADIO_FLAGS_SEND_SUCC] = 1;
          radio_values.send_time = 0;
#ifdef DEBUG
static int try_max = 0;

if (radio_input_message[RADIO_MESSAGE_ID_COMMAND] == 200){
     Log.Info ("Response OK! ID command: %d"CR,radio_input_message[RADIO_MESSAGE_ID_COMMAND]);
     int mean_try = all_try/radio_input_message[RADIO_MESSAGE_ID_COMMAND];
     Log.Info ("ALL/MAX/MEAN = %d/%d/%d]"CR,all_try,try_max,mean_try);
     if (try_max < radio_values.send_try) try_max = radio_values.send_try;
}    
     radio_values.send_try = 0;
#endif
        }
    break;
    
    case RADIO_COMMAND_PINMODE:
      pinMode(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
      pin_change_setting(radio_input_message[RADIO_MESSAGE_ARG_1],PIN_VALUES_MODE,radio_input_message[RADIO_MESSAGE_ARG_2]);
#ifdef DEBUG
     Log.Info ("pinMode(%d,%d);"CR,radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif      
    break;
    case RADIO_COMMAND_DIGITAL_WRITE:
      digitalWrite(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#ifdef DEBUG
     Log.Info ("digitalWrite(%d,%d);"CR,radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif 
    break;
    case RADIO_COMMAND_ANALOG_WRITE:
      digitalWrite(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#ifdef DEBUG
     Log.Info ("analogWrite(%d,%d);"CR,radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif 
    break;
    case RADIO_COMMAND_PIN_NOTIFY_CHANGE:
#ifdef DEBUG
     Log.Info ("Notify change! Point ID:%d, Pin:%d, State:%d;"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER],radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif 
    break;
    case RADIO_COMMAND_PIN_SET_SETTING:
        pin_change_setting(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2],1);
    break;
    case RADIO_COMMAND_PIN_RESET_SETTING:
         pin_change_setting(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2],0);
    break;
    case RADIO_COMMAND_DIGITALREAD:
         radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_DIGITALREAD_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],digitalRead(radio_input_message[RADIO_MESSAGE_ARG_1]));
    break;
    case RADIO_COMMAND_ANALOGREAD:
         radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_ANALOGREAD_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],analogRead(radio_input_message[RADIO_MESSAGE_ARG_1]));
    break;
    case RADIO_COMMAND_DHT_TEMP_GET:
       if (get_dht_id_of_pin(radio_input_message[RADIO_MESSAGE_ARG_1]) != -1)
         radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_DHT_TEMP_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],dht[get_dht_id_of_pin(radio_input_message[RADIO_MESSAGE_ARG_1])]->readTemperature());
       else radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_DHT_TEMP_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],-255);
    break;
    case RADIO_COMMAND_DHT_TEMP_RESP:
#ifdef DEBUG
     Log.Info ("Get temp! Point ID:%d, Pin:%d, Temp:%d;"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER],radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif 
    break;
    case RADIO_COMMAND_DHT_HUMI_RESP:
#ifdef DEBUG
     Log.Info ("Get humi! Point ID:%d, Pin:%d, Humi:%d;"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER],radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
#endif
    break;
    case RADIO_COMMAND_DHT_HUMI_GET:
      if (get_dht_id_of_pin(radio_input_message[RADIO_MESSAGE_ARG_1]) != -1)
         radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_DHT_HUMI_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],dht[get_dht_id_of_pin(radio_input_message[RADIO_MESSAGE_ARG_1])]->readHumidity());
      else radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_DHT_TEMP_RESP,radio_input_message[RADIO_MESSAGE_ARG_1],-255);
    break;
    case RADIO_COMMAND_DHT_ADD:
          add_dht_sensor(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
    break;
    case RADIO_COMMAND_FULL_RESET:
#ifdef DEBUG
  Log.Info ("FULL RESET BY RADIO COMMAND!"CR);
#endif
      time_values.radio_break = 0;
    break;
       
    case RADIO_COMMAND_CHANGE_POINT_ID:
      EEPROM.put(0,radio_input_message[RADIO_MESSAGE_ARG_1]);
      radio_values.point_id = radio_input_message[RADIO_MESSAGE_ARG_1];
#ifdef DEBUG
  Log.Info ("Change point ID! New Point ID: %d!"CR,radio_input_message[RADIO_MESSAGE_ARG_1]);
#endif
    break;
    case RADIO_COMMAND_CHANGE_MASTER_ID:
      EEPROM.put(2,radio_input_message[RADIO_MESSAGE_ARG_1]);
      radio_values.master_id = radio_input_message[RADIO_MESSAGE_ARG_1];
#ifdef DEBUG
  Log.Info ("Change master ID! New master ID: %d!"CR,radio_input_message[RADIO_MESSAGE_ARG_1]);
#endif
    break;
    case RADIO_COMMAND_PING:
#ifdef DEBUG
  Log.Info ("Get Ping from Point ID: %d!"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER]);
#endif
        radio_values.state = POINT_STATE_ONLINE;
        time_values.first_start = -1;
         time_values.radio_break = TIME_VALUES_RADIO_BREAK;
        radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_PONG,0,0);     
    break;
    case RADIO_COMMAND_PING_FOR_ALL:
          time_values.first_start = radio_values.point_id * 5;
    break;
    case RADIO_COMMAND_PONG:
#ifdef DEBUG
  Log.Info ("Get PONG from Point ID: %d!"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER]);
#endif
    break;
    case RADIO_COMMAND_POINT_ON:
#ifdef DEBUG
  Log.Info ("Found a new point! Id: %d!"CR,radio_input_message[RADIO_MESSAGE_ID_SENDER]);
#endif
         radio_send_message(radio_input_message[RADIO_MESSAGE_ID_SENDER],RADIO_COMMAND_PING,0,0);

    break;
    }
  radio_clean_radio_input_message();
}

void pin_change_setting(int pin, int setting,int value){
  pin_values[pin][setting] = value;
    Log.Info ("Pin %d   Setiing %d   Value %d"CR,pin,setting,value);
}

void add_dht_sensor(int pin,int type){
  static int dht_counter = 0;
  DHT_sensors[dht_counter] = pin;
  dht[dht_counter] = new DHT(pin, type);
  dht[dht_counter]->begin();
#ifdef DEBUG
  Log.Info ("Add DHT%d on pin %d"CR,radio_input_message[RADIO_MESSAGE_ARG_2],radio_input_message[RADIO_MESSAGE_ARG_1]);
#endif
}

int get_dht_id_of_pin(int pin){
  int i;
  for (i = 0; i < DHT_NUMBERS;i++){
    if (DHT_sensors[i] == pin) return i; 
  }
  return -1;
}





