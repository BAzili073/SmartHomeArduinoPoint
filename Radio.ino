void setup_radio(){
  radio.begin(); // Старт работы;
  radio.setRetries(15,15);
  radio.setPayloadSize(16);
  radio.enableAckPayload();  // Разрешение отправки нетипового ответа передатчику;
  radio.openWritingPipe(pipes[2]);
  radio.openReadingPipe(1,pipes[2]);
  radio.startListening();  //начинаем слушать;
  radio.printDetails();
}

void radio_send(){
  radio.stopListening();
//  Log.Info("Send message = [%d,%d,%d,%d]"CR,output_message[0],output_message[1],output_message[2],output_message[3]);
  radio.write (&radio_output_message, sizeof(radio_output_message));
  radio.startListening();
}

void radio_send_message(int ID_receiver,int command, int arg1,int arg2){
  radio_output_message[RADIO_MESSAGE_ID_RECIVER] = ID_receiver;
  radio_output_message[RADIO_MESSAGE_ID_COMMAND] = radio_ID_command;
  radio_output_message[RADIO_MESSAGE_COMMAND] = command;
  radio_output_message[RADIO_MESSAGE_ARG_1] = arg1;
  radio_output_message[RADIO_MESSAGE_ARG_2] = arg2;
  radio_send();
}

void radio_send_response(){
  radio_output_message[RADIO_MESSAGE_ID_RECIVER] = radio_input_message[RADIO_MESSAGE_ID_SENDER];
  radio_output_message[RADIO_MESSAGE_ID_COMMAND] = radio_input_message[RADIO_MESSAGE_ID_COMMAND];
  radio_output_message[RADIO_MESSAGE_COMMAND] = RADIO_COMMAND_RESPONSE_OK;
  radio_output_message[RADIO_MESSAGE_ARG_1] = 0;
  radio_output_message[RADIO_MESSAGE_ARG_2] = 0;
  radio_send();
}



void radio_rec(){
  while (radio.available()){
    bool done = false;
    while (!done){
      done = radio.read( &radio_input_message, sizeof(radio_input_message) );
    }
//        radio.read( &radio_input_message, sizeof(radio_input_message) );
#ifdef DEBUG
        Log.Info ("Input message - [%d,%d,%d,%d,%d]"CR,radio_input_message[0],radio_input_message[1],radio_input_message[2],radio_input_message[3],radio_input_message[4]);
#endif
       if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] == point_ID) ||  (radio_input_message[RADIO_MESSAGE_ID_RECIVER] == RADIO_MESSAGE_FOR_ALL)){
          if ((radio_input_message[RADIO_MESSAGE_ID_RECIVER] != RADIO_MESSAGE_FOR_ALL) && (radio_input_message[RADIO_MESSAGE_COMMAND] != RADIO_COMMAND_RESPONSE_OK)){
            radio_send_response();
          }
         analysis_Message();
      }
  }
}


void analysis_Message(){
    switch (radio_input_message[RADIO_MESSAGE_COMMAND]){
    case 0:                                      //Если это инфосообщение

      break;
      /////////////////
    case 1:                                  //Если это сообщение управления

    break;
      /////////////////////////
    case 2:                                             //Если это сообщение запрос

    break;
    }
  radio_clean_radio_input_message();
}


void radio_work(){
      if (!radio_send_need){
        radio_rec();
      }else{
        radio_send ();
        radio_send_need = 0;
    }
}


void radio_clean_radio_input_message(){
  int i;
  for (i = 0; i < RADIO_MESSAGE_LEN; i++)  radio_input_message[i] = 0;
}
