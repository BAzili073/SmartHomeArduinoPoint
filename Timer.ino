void setupTimer(long mcsec) {
  Timer1.initialize(mcsec);
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{ 
  if (radio_values.send_time) radio_values.send_time--;
}
