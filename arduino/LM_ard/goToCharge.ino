// following Light for collect robots for charging
float coefficient[] = {0.0, 0.0}; // -1.0 ~ 1.0 (L ~ R)

void goToCharge() {
  // "followLight" only works when followLightFlag is true
  // considering charging at night, this function make collecting robots easier.
  // basically it just following light.
  // but need to figure out how to stop it...
  int brghtnsL = analogRead(CDS_L);
  int brghtnsR = analogRead(CDS_R);
  // float brDif = map(brghtnsL - brghtnsR, -1024.0, 1024.0, -1.0, 1.0);
  float brDif = (brghtnsL - brghtnsR) / 1000.0;

  if (brDif > 0) { // means light is on the LEFT. so left wheel should be turn less.
    coefficient[0] = brDif;  // less power for LEFT
    coefficient[1] = 1;      // full power for RIGHT
  }
  else if (brDif < 0) {
    coefficient[0] = 1;          // full power for LEFT
    coefficient[1] = abs(brDif); // less power for RIGHT, minus change to plus.
  }

  digitalWrite(MTR_A_F, HIGH);
  digitalWrite(MTR_A_B, LOW);
  analogWrite(MTR_A_PWM, motorSpeed * coefficient[0]);
  digitalWrite(MTR_B_F, HIGH);
  digitalWrite(MTR_B_B, LOW);
  analogWrite(MTR_B_PWM, motorSpeed * coefficient[1]);

   Serial.print("left value is ");
   Serial.print(brghtnsL);
   Serial.print(" right value is ");
   Serial.print(brghtnsR);
   Serial.print(" brDif is ");
   Serial.println(brDif);
   delay(10);

}
