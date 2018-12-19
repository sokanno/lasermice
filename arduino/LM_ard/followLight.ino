// following Light for collect robots for charging
float direction[] = {0.0, 0.0}; // -1.0 ~ 1.0 (L ~ R)

void followLight() {
  // "followLight" only works when followLightFlag is true
  // considering charging at night, this function make collecting robots easier.
  // basically it just following light.
  // but need to figure out how to stop it...
  int brghtnsL = analogRead(CDS_L);
  int brghtnsR = analogRead(CDS_R);
  // float brDif = map(brghtnsL - brghtnsR, -1024.0, 1024.0, -1.0, 1.0);
  float brDif = (brghtnsL - brghtnsR) / 1000.0;

  if (brDif > 0) { // means light is on the LEFT. so left wheel should be turn less.
    direction[0] = brDif;  // less power for LEFT
    direction[1] = 1;      // full power for RIGHT
  }
  else if (brDif < 0) {
    direction[0] = 1;          // full power for LEFT
    direction[1] = abs(brDif); // less power for RIGHT, minus change to plus.
  }

  digitalWrite(MTR_A_F, HIGH);
  digitalWrite(MTR_A_B, LOW);
  analogWrite(MTR_A_PWM, motorSpeed * direction[0]);
  digitalWrite(MTR_B_F, HIGH);
  digitalWrite(MTR_B_B, LOW);
  analogWrite(MTR_B_PWM, motorSpeed * direction[1]);

   Serial.print("left value is ");
   Serial.print(brghtnsL);
   Serial.print(" right value is ");
   Serial.print(brghtnsR);
   Serial.print(" brDif is ");
   Serial.println(brDif);
   delay(10);

}
