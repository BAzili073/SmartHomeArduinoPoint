
void setupTimer(long mcsec) {
  Timer1.initialize(mcsec);
  Timer1.attachInterrupt( timerIsr );
  time_values.start = 1;
}

void timerIsr()
{ 
  time_values.msec++;
  if (time_values.msec >= 1000){
    if (!time_values.radio_break){
#ifdef DEBUG
  Log.Info ("FULL RESET TIME RADIO BREAK!"CR);
#endif      
      resetFunc();
    }
    time_values.sec++;
    time_values.msec = 0;
    if (time_values.sec>=60){   
        time_values.sec = 0;
        if (time_values.radio_break) time_values.radio_break --;
          
    }
  }

  
  if (radio_values.send_time) radio_values.send_time--;
  
}
