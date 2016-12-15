void setup_radio(){
  radio.begin(); // Старт работы;
  radio.setRetries(15,15);
  radio.setPayloadSize(16);
  radio.enableAckPayload();  // Разрешение отправки нетипового ответа передатчику;
  radio.openWritingPipe(pipes[2]);
  radio.openReadingPipe(1,pipes[2]);
  radio.startListening();  //начинаем слушать;
  radio.printDetails();


  radio_flags[RADIO_FLAGS_SEND_SUCC] = 1;
}

void radio_send(){
  radio.stopListening();
#ifdef DEBUG
  Log.Info("Send message = [%d,%d,%d,%d,%d]"CR,radio_output_message[0],radio_output_message[1],radio_output_message[2],radio_output_message[3],radio_output_message[4]);
#endif
  radio.write (&radio_output_message, sizeof(radio_output_message));
  radio.startListening();
  radio_flags[RADIO_FLAGS_SEND_NEED] = 0;
}

bool radio_send_message(int ID_receiver,int command, int arg1,int arg2){
  if (!radio_flags[RADIO_FLAGS_SEND_SUCC]) return false;
  radio_values.id_commands++;
  if (radio_values.id_commands == 254) radio_values.id_commands = 0;
  radio_output_message[RADIO_MESSAGE_ID_RECIVER] = ID_receiver;
  radio_output_message[RADIO_MESSAGE_ID_COMMAND] = radio_values.id_commands;
  radio_output_message[RADIO_MESSAGE_COMMAND] = command;
  radio_output_message[RADIO_MESSAGE_ARG_1] = arg1;
  radio_output_message[RADIO_MESSAGE_ARG_2] = arg2;
  radio_flags[RADIO_FLAGS_SEND_NEED] = 1;
  radio_flags[RADIO_FLAGS_SEND_SUCC] = 0;
#ifdef DEBUG
  Log.Info("RADIO SEND MESSAGE!  ID COMMAND: %d"CR,radio_values.id_commands);
#endif
  return true;
}

void radio_send_response(){
  radio_response_message[RADIO_MESSAGE_ID_RECIVER] = radio_input_message[RADIO_MESSAGE_ID_SENDER];
  radio_response_message[RADIO_MESSAGE_ID_COMMAND] = radio_input_message[RADIO_MESSAGE_ID_COMMAND];
  radio_response_message[RADIO_MESSAGE_COMMAND] = RADIO_COMMAND_RESPONSE_OK;
  radio_response_message[RADIO_MESSAGE_ARG_1] = 0;
  radio_response_message[RADIO_MESSAGE_ARG_2] = 0;
  radio.stopListening();
  radio.write (&radio_response_message, sizeof(radio_response_message));
  radio.startListening();
}



void radio_rec(){
  while (radio.available()){
//    bool done = false;
//    while (!done){
//      done = radio.read( &radio_input_message, sizeof(radio_input_message) );
//    }
        radio.read( &radio_input_message, sizeof(radio_input_message) );
#ifdef DEBUG
//        Log.Info ("Input message - [%d,%d,%d,%d,%d]"CR,radio_input_message[0],radio_input_message[1],radio_input_message[2],radio_input_message[3],radio_input_message[4]);
#endif
       if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] == radio_values.point_id) ||  (radio_input_message[RADIO_MESSAGE_ID_RECIVER] == RADIO_MESSAGE_FOR_ALL)){
          if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] != RADIO_MESSAGE_FOR_ALL) && (radio_input_message[RADIO_MESSAGE_COMMAND] != RADIO_COMMAND_RESPONSE_OK)){
            radio_send_response();
          }
         analysis_Message();
      }
  }
}


void analysis_Message(){
    switch (radio_input_message[RADIO_MESSAGE_COMMAND]){

      
    case RADIO_COMMAND_RESPONSE_OK:                   //ответ о принятой команде!
        if (radio_values.id_commands == radio_input_message[RADIO_MESSAGE_ID_COMMAND]){
          radio_flags[RADIO_FLAGS_SEND_SUCC] = 1;
          radio_values.send_time = 0;
#ifdef DEBUG
if (radio_input_message[RADIO_MESSAGE_ID_COMMAND] == 200){
     Log.Info ("Response OK! ID command: %d"CR,radio_input_message[RADIO_MESSAGE_ID_COMMAND]);
     int mean_try = all_try/radio_input_message[RADIO_MESSAGE_ID_COMMAND];
     Log.Info ("ALL/MAX/MEAN = %d/%d/%d]"CR,all_try,try_max,mean_try);
}
     if (try_max < radio_values.send_try) try_max = radio_values.send_try;
     radio_values.send_try = 0;
#endif
        }
    break;
    
    case RADIO_COMMAND_PINMODE:
      pinMode(radio_input_message[RADIO_MESSAGE_ARG_1],radio_input_message[RADIO_MESSAGE_ARG_2]);
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
    break;
    }
  radio_clean_radio_input_message();
}


void radio_work(){
      if ((radio_flags[RADIO_FLAGS_SEND_NEED]) || (!radio_flags[RADIO_FLAGS_SEND_SUCC] && !radio_values.send_time)){
        radio_send ();
        radio_values.send_try++;
        if (radio_values.send_try == 15){
          radio_flags[RADIO_FLAGS_SEND_SUCC] = 1 ;
          radio_values.send_try = 0;
        }      
        all_try++;
        radio_values.send_time = 3;
      }else{
        radio_rec();        
    }
}


void radio_clean_radio_input_message(){
  int i;
  for (i = 0; i < RADIO_MESSAGE_LEN; i++)  radio_input_message[i] = 0;
}
