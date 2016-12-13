void setupTimer(long msec) {
  Timer1.initialize(msec);
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{
  
}
