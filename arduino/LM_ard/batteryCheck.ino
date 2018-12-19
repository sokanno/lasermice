void batteryCheck() {
  if (analogRead(BAT_CHK) < 240) {
    if (millis() % 500 < 250) digitalWrite(LED_G, HIGH);
    else digitalWrite(LED_G, LOW);
  }
  else if(tailLED) digitalWrite(LED_G, HIGH);
  else digitalWrite(LED_G, LOW);
}

