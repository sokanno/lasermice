
void play() {
  if (playFlag) { // "play" only works when playFlag is true
    if (sequenceMode) {
      // if((millis() % cycleLength) > (shiftAmount % cycleLength) &&
      //    (millis() % cycleLength) < (shiftAmount % cycleLength) + debounce && !bangFlag){
      if ((millis() % (cycleLength/4)) > (shiftAmount % (cycleLength/4)) &&
          (millis() % (cycleLength/4)) < (shiftAmount % (cycleLength/4)) + debounce && !bangFlag) {
        if (beat[seqCount]) {
          bangFlag_LSR = true;
          bangFlag_SLND = true;
        }
        timeStamp_LSR = millis();
        seqCount ++;
        bangFlag = true;
        turnFlag = true;
        // Serial.print(seqCount-1);
        // Serial.print(' ');
        // Serial.println(beat[seqCount]);
        if (seqCount >= loopDigit) seqCount = 0;
      }
    }
    else if (!sequenceMode) {

      //blinking and hitting
      // if(((millis() + shiftAmount) % cycleLength) < reactionLength_LSR && !bangFlag_LSR){
      // if((millis() % cycleLength) == shiftAmount &&  !bangFlag_LSR){
      // if((millis() % cycleLength) > (shiftAmount % cycleLength) &&
      //    (millis() % cycleLength) < (shiftAmount % cycleLength) + debounce && !bangFlag){
      if ((millis() % cycleLength) > shiftAmount &&
          (millis() % cycleLength) < shiftAmount + debounce && !bangFlag) {
        bangFlag_LSR = true;
        bangFlag_SLND = true;
        timeStamp_LSR = millis();
        bangFlag = true;
        turnFlag = true;
        // Serial.print("blinks");
        // Serial.print(' ');
        // Serial.print(cycleLength);
        // Serial.print(' ');
        // Serial.println(shiftAmount);
        strikeCount++;
      }
    }

    // something common in Seq mode and non-seq mode
    if (millis() - timeStamp_LSR > debounce) {
      bangFlag = false;
    }

    // moving forward 
    if (!bumpLreactFlag && !bumpRreactFlag) {
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
      // Serial.println("motorB should work");
    }

    // truning around while laser is on
    if (bangFlag_LSR) {
      // turn
      if (turnDirection) {
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
      }
    }
    // finish turning when laser blink has finished
    if (millis() > timeStamp_LSR + reactionLength_LSR && turnFlag == true) {
      // digitalWrite(LASER, LOW);
      if (turnDirection) {
        digitalWrite(MTR_B_F, HIGH);
        digitalWrite(MTR_B_B, LOW);
        turnFlag = false;
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, HIGH);
        digitalWrite(MTR_A_B, LOW);
        turnFlag = false;
      }
      if (int(random(50)) % 2 == 1) turnDirection = true;
      else turnDirection = false;
      // Serial.println(turnDirection);
    }
  }
  else if (!playFlag && lastPlayFlag){
    analogWrite(MTR_A_PWM, 0);
    analogWrite(MTR_B_PWM, 0);
    // Serial.println("motor off");
  }

  // else if (!followLightFlag || !rotateReactFlag) {
  //   analogWrite(MTR_A_PWM, 0);
  //   analogWrite(MTR_B_PWM, 0);
  // }

  lastPlayFlag = playFlag;
}
