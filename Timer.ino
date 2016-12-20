
void setupTimer(long mcsec) {
  Timer1.initialize(mcsec);
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{ 
  if (radio_values.send_time) radio_values.send_time--;
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
    if ((time_values.first_start > 0) && (radio_values.master_id != radio_values.point_id)) time_values.first_start--;  
    if (time_values.sec>=60){    
        time_values.sec = 0;
        if (time_values.radio_break && (radio_values.master_id != radio_values.point_id)) time_values.radio_break --;
          
    }
  } 
}
