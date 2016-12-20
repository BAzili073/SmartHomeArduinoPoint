void EEPROM_readSettings() {
//  EEPROM.write(0, 2);

time_values.radio_break = TIME_VALUES_RADIO_BREAK;

#ifdef POINT_ID_PROG
  radio_values.point_id = POINT_ID_PROG;
  radio_values.master_id = 1;
  #ifdef POINT_MASTER
    radio_values.master_id = POINT_ID_PROG;
  #endif
  
  EEPROM.put(0,radio_values.point_id);
  EEPROM.put(2,radio_values.master_id);
#endif


  EEPROM.get(0,radio_values.point_id);
  EEPROM.get(2,radio_values.master_id);
  time_values.first_start = radio_values.point_id * 5;
  radio_values.state = POINT_STATE_OFFLINE;
//  Log.Info("0:%d    1:%d "CR,EEPROM.read(0),EEPROM.read(1));
  
  radio_output_message[RADIO_MESSAGE_ID_SENDER] = radio_values.point_id;
  radio_response_message[RADIO_MESSAGE_ID_SENDER] = radio_values.point_id;

    
#ifdef DEBUG
  Log.Info("START!\r\n RADIO POINT ID:%d \r\n MASTER ID: %d "CR,radio_values.point_id,radio_values.master_id);
#endif
}
//EEPROM.write(addr, val);
