int get_int()
{
    while (!(Serial.available())); 
        return Serial.read();
}


void serialEvent() {
    while (Serial.available()) {
      byte uart_mes[7];
      int i;
      for (i = 0; i<7;i++){
        uart_mes[i] = Serial.read();
        delay(10);
      }
//      if (uart_mes[0] == '1'){
//          if (uart_mes[2] == '1'){
      if (uart_mes[0] == 0xA9){
          if (uart_mes[2] == radio_values.point_id){
            for (i = 0;i<6;i++){
              radio_input_message[i] = uart_mes[i+1];
            }
            analysis_Message();
            radio_clean_radio_input_message();
          }else{
            radio_send_message(uart_mes[2],uart_mes[4],uart_mes[5],uart_mes[6]);
          }
      }
    }




    
}

