void setupTimer(long mcsec) {
  Timer1.initialize(mcsec);
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{ 
  if (radio_values[RADIO_VALUES_SEND_TIME]) radio_values[RADIO_VALUES_SEND_TIME]--;
}
