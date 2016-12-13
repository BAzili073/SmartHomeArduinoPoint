void serialEvent() {
    while (Serial.available()) {
        int magic = Serial.read();
                radio_send(magic);
    }
}

