void EEPROM_readSettings() {
//  EEPROM.write(0, 2);


#ifdef POINT_ID_PROG
  radio_values.point_id = POINT_ID_PROG;
  radio_values.master_id = 0;
  EEPROM.put(0,radio_values.point_id);
  EEPROM.put(2,radio_values.master_id);
#endif


  EEPROM.get(0,radio_values.point_id);
  EEPROM.get(2,radio_values.master_id);
  
//  Log.Info("0:%d    1:%d "CR,EEPROM.read(0),EEPROM.read(1));
  
  radio_output_message[RADIO_MESSAGE_ID_SENDER] = radio_values.point_id;
  radio_response_message[RADIO_MESSAGE_ID_SENDER] = radio_values.point_id;

    
#ifdef DEBUG
  Log.Info("RADIO POINT ID:%d "CR,radio_values.point_id);
#endif
}
//EEPROM.write(addr, val);
