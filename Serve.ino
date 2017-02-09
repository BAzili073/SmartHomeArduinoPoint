void setup_point_3(){
  add_point(3);
  radio_send_message(3,2,3,1);
  radio_send_try_try();
  radio_send_message(3,2,4,0);
  radio_send_try_try();
  radio_send_message(3,6,4,1);
  radio_send_try_try();
  radio_send_message(3,18,0,0);
  radio_send_try_try(); 
   Log.Info ("Setup point 3 OK!"CR);
}

void setup_point_4(){
   add_point(4);
   radio_send_message(4,2,17,1);
   radio_send_try_try();
   radio_send_message(4,18,0,0);
   radio_send_try_try();
   Log.Info ("Setup point 4 OK!"CR);
}

void ping_points(){
  int i;
  for (i = 0; i < 5; i++){
    if (points[i]){
      radio_send_message(points[i],18,0,0);
      radio_send_try_try();
    }
  }
}

void add_point(int point){
  int i;
    for (i = 0; i < 5; i++){
    if (!points[i]){
      points[i] = point;
      break;
    }
  }
}

