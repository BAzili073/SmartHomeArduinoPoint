int get_int()
{
    while (!(Serial.available())); 
        return Serial.read();
  
}


void serialEvent() {
    while (Serial.available()) {
        int temp1 = Serial.read();
        delay(10);
        int temp2 = Serial.read();
        delay(10);
        int temp3 = Serial.read();
        delay(10);
        int temp4 = Serial.read();
        delay(10);
        int temp5 = Serial.read();
#ifdef DEBUG
        Log.Info ("%d,%d,%d,%d,%d"CR,temp1,temp2,temp3,temp4,temp5);
#endif

        radio_send_message(temp2,temp3,temp4,temp5);
    }




    
}

