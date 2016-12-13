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

void radio_send(int out_adress){
  output_message[3] = out_adress;
  radio.stopListening();
  Log.Info("Send message = [%d,%d,%d,%d]"CR,output_message[0],output_message[1],output_message[2],output_message[3]);
  radio.write (&output_message, sizeof(output_message));
  radio.startListening();   
}

void radio_rec(){
  if (radio.available()){
//    bool done = false;
//    while (!done){
//      done = radio.read( &input_message, sizeof(input_message) );
//    }
//    if (input_message[3]){
        radio.read( &input_message, sizeof(input_message) );  
        Log.Info ("Input message - [%d,%d,%d,%d]"CR,input_message[0],input_message[1],input_message[2],input_message[3]);
        analysis_Message();
  }
}


void analysis_Message(){
  if ((input_message[3] == 1)){
      digitalWrite(LED,HIGH);
    }else{
      digitalWrite(LED,LOW);
    }
  if ((input_message[0] == point_ID)){
    switch (input_message[1]){ 
    case 0:                                      //Если это инфосообщение 

      break;  
      /////////////////      
    case 1:                                  //Если это сообщение управления

    break; 
      /////////////////////////    
    case 2:                                             //Если это сообщение запрос        
          
    break;         
    }
    /////////////////////       
  }
  clean_Message();
}

void clean_Message(){
  input_message[0] = 0;
  input_message[1] = 0;
  input_message[2] = 0;
  input_message[3] = 0;
  input_message[4] = 0;
}

