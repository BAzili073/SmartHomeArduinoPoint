void setupTimer(long msec) {
  Timer1.initialize(msec);
  Timer1.attachInterrupt( timerIsr );
}

void timerIsr()
{ 
  if (role == 0){
        if (rec_send_pack == send_pack) succ_pack++;
        else fail_pack ++;
        send_pack++;
        send_need = 1;
        int proc = (100*fail_pack/send_pack);
        Log.Info ("Send/Succ/Fail/     %d/%d/%d  = %d]"CR,send_pack,succ_pack,fail_pack,proc);
  }
}
