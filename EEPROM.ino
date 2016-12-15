void EEPROM_readSettings() {
//  EEPROM.write(0, 2);
  point_ID = EEPROM.read(0);
  radio_output_message[RADIO_MESSAGE_ID_SENDER] = point_ID;
  radio_response_message[RADIO_MESSAGE_ID_SENDER] = point_ID;
#ifdef DEBUG
  Log.Info("RADIO POINT ID:%d "CR,point_ID);
    Log.Info("RADIO POINT ID:%d "CR,point_ID);
      Log.Info("INPUT:%d "CR,INPUT);
      Log.Info("OUTPUT:%d "CR,OUTPUT);
      Log.Info("A1:%d "CR,A1);
      Log.Info("A7:%d "CR,A7);
      
#endif
}
//EEPROM.write(addr, val);
