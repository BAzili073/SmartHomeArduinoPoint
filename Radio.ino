void setup_radio(){
  radio.begin(); // Старт работы;
  radio.setAutoAck(false);
  radio.setRetries(15,15);
  radio.setPayloadSize(12);
  radio.enableAckPayload();  // Разрешение отправки нетипового ответа передатчику;
  radio.openWritingPipe(pipes[2]);
  radio.openReadingPipe(1,pipes[2]);
  radio.startListening();  //начинаем слушать;
  radio.printDetails();
  radio_flags[RADIO_FLAGS_SEND_SUCC] = 1;
}

void radio_send(){
  radio.stopListening();
//#ifdef DEBUG
//  Log.Info("Send message = [%d,%d,%d,%d,%d,%d]"CR,
//  radio_output_message[RADIO_MESSAGE_ID_SENDER],
//  radio_output_message[RADIO_MESSAGE_ID_RECIVER],
//  radio_output_message[RADIO_MESSAGE_ID_COMMAND],
//  radio_output_message[RADIO_MESSAGE_COMMAND],
//  radio_output_message[RADIO_MESSAGE_ARG_1],
//  radio_output_message[RADIO_MESSAGE_ARG_2]);
//#endif
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
  Log.Info("SEND MESSAGE! REC: %d COMMAND: %d"CR,ID_receiver,command);
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
    bool done = false;
    while (!done){
      done = radio.read( &radio_input_message, sizeof(radio_input_message) );
    }
//        radio.read( &radio_input_message, sizeof(radio_input_message) );
#ifdef DEBUG
        Log.Info ("Input message - [%d,%d,%d,%d,%d,%d] size %d"CR,radio_input_message[0],radio_input_message[1],radio_input_message[2],radio_input_message[3],radio_input_message[4],radio_input_message[5],sizeof(radio_input_message));
#endif
       if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] == radio_values.point_id) ||  (radio_input_message[RADIO_MESSAGE_ID_RECIVER] == RADIO_MESSAGE_FOR_ALL)){
          if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] != RADIO_MESSAGE_FOR_ALL) && (radio_input_message[RADIO_MESSAGE_COMMAND] != RADIO_COMMAND_RESPONSE_OK)){
            radio_send_response();
          }
          if (radio_values.last_id_command != radio_input_message[RADIO_MESSAGE_ID_COMMAND]){    
            analysis_Message();
          }     
          
      }
      radio_clean_radio_input_message();
  }
}

void radio_send_point_on(){
  if (radio_values.master_id != radio_values.point_id){
     if (!radio_values.state){
      radio_send_message(radio_values.master_id,RADIO_COMMAND_POINT_ON,0,0);
    }else{
      radio_send_message(radio_values.master_id,RADIO_COMMAND_PONG,0,0); 
    }
  }
}



void radio_work(){
      if ((radio_flags[RADIO_FLAGS_SEND_NEED]) || (!radio_flags[RADIO_FLAGS_SEND_SUCC] && !radio_values.send_time)){
        radio_send ();
        radio_values.send_try++;
        if (radio_values.send_try >= 15){
#ifdef DEBUG
  Log.Info ("Radio send message ERROR"CR);
#endif
          radio_flags[RADIO_FLAGS_SEND_SUCC] = 1 ;
          radio_values.send_try = 0;
        }      
        all_try++;
        radio_values.send_time = 3;
      }else{
        radio_rec();        
    }
    if (time_values.first_start == 0){
      radio_send_point_on();
      time_values.first_start = 60;
    }
}

bool radio_send_try_try(){
  radio_values.send_try = 1;
  while(radio_values.send_try){
            radio_send ();
        radio_values.send_try++;
        if (radio_values.send_try >= 15){
#ifdef DEBUG
  Log.Info ("Radio send message ERROR"CR);
#endif
          radio_flags[RADIO_FLAGS_SEND_SUCC] = 1 ;
          radio_values.send_try = 0;
        }
  }
}

void radio_clean_radio_input_message(){
  int i;
  for (i = 0; i < RADIO_MESSAGE_LEN; i++)  radio_input_message[i] = 0;
}
