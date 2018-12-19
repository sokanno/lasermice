

void bumperFunction() {

  //update reaction time
  reactionLength_BMP = BMPconstant / motorSpeed;
  retreatLength = retreatConstant / motorSpeed;

  bumpLstate = digitalRead(L_BUMPER);
  bumpRstate = digitalRead(R_BUMPER);

  if (!bumpLstate && lastBumpLstate && !bumpLreactFlag && !bumpRreactFlag) {
    bumpLreactFlag = true;
    timeStamp_BMP = millis();
  }
  if (!bumpRstate && lastBumpRstate && !bumpLreactFlag && !bumpRreactFlag) {
    bumpRreactFlag = true;
    timeStamp_BMP = millis();
  }

  if (bumpLreactFlag && playFlag) {
    if (millis() < timeStamp_BMP + retreatLength) {
      digitalWrite(MTR_B_F, LOW);
      digitalWrite(MTR_B_B, HIGH);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(MTR_A_F, LOW);
      digitalWrite(MTR_A_B, HIGH);
      analogWrite(MTR_A_PWM, motorSpeed);
    }
    else if (millis() < timeStamp_BMP + reactionLength_BMP + retreatLength) {
      digitalWrite(MTR_B_F, LOW);
      digitalWrite(MTR_B_B, HIGH);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      // digitalWrite(LED_R, HIGH);
    }
    else {
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      // analogWrite(MTR_B_PWM, 0);
      // analogWrite(MTR_A_PWM, 0);
      // digitalWrite(LED_R, LOW);
      bumpLreactFlag = false;
    }
  }
  if (bumpRreactFlag && playFlag) {
    if (millis() < timeStamp_BMP + retreatLength) {
      digitalWrite(MTR_B_F, LOW);
      digitalWrite(MTR_B_B, HIGH);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(MTR_A_F, LOW);
      digitalWrite(MTR_A_B, HIGH);
      analogWrite(MTR_A_PWM, motorSpeed);
    }
    else if (millis() < timeStamp_BMP + reactionLength_BMP + retreatLength) {
      digitalWrite(MTR_A_F, LOW);
      digitalWrite(MTR_A_B, HIGH);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
      // digitalWrite(LED_G, HIGH);
    }
    else {
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      // analogWrite(MTR_A_PWM, 0);
      // analogWrite(MTR_B_PWM, 0);
      // digitalWrite(LED_G, LOW);
      bumpRreactFlag = false;
    }
  }
  lastBumpLstate = bumpLstate;
  lastBumpRstate = bumpRstate;
}
